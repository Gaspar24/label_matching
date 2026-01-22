#include "Company.hpp"


// function to get and store the labels from the file
vector<string> getLabels(string labels_path){
	fstream labels_taxonomy;
	labels_taxonomy.open(labels_path,ios::in);
	if(!labels_taxonomy.is_open()){
		cerr << "Can t open labels file\n";
        cerr << "Check if the file exists or the if file name given as argument is correct!\n";
		exit(1);
	}
	string line_labels;
	getline(labels_taxonomy,line_labels);
	vector<string>labels_rows;
	while(getline(labels_taxonomy,line_labels)){
		//checks if the last caracter of the line is a carriage return .back()
		if (!line_labels.empty() && line_labels.back() == '\r')
			line_labels.pop_back();
		labels_rows.push_back(line_labels);
	}
	labels_taxonomy.close();
	return labels_rows;
}



// simple function to assign the atributes to the company
// the index i in the vector will be used to assign the atributes
// the enum declared in the header file will match with the index, that s why i use switch case here

void assignAtributes(Company &company,vector<string> &columns, int id){
	vector<string>tags = parseTags(columns[BUSINESS_TAGS]);
	for(int i = 0; i < columns.size(); i++){
		switch (i)
		{
		case DESCRIPTION:
			company.setDescription(columns[DESCRIPTION]);
			break;
		case BUSINESS_TAGS:
			company.setBussines_tag(tags);
			break;
		case SECTOR:
			company.setSector(columns[SECTOR]);
			break;
		case CATEGORY:
			company.setCategory(columns[CATEGORY]);
			break;
		case NICHE:
			company.setNiche(columns[NICHE]);
			break;
		default:
			cerr << "Error assign values to class, input file has different format.\n";
			exit(1);
		}
	}
	company.setId(id);
}

// function to read the file line by line, parse the line in to the columns and assign the atributes to the compnay
// the main function that creates and initiate the companies
vector<Company> getAllCompanies(string companies_path) {
    fstream companiesFile(companies_path, ios::in);
    if (!companiesFile.is_open()) {
        cerr << "Can t open companies file\n";
        cerr << "Check if the file exists or if the file name given as argument is correct!\n";
		exit(1);
    }

    vector<Company> companies;
    string line;

    // skip header
    getline(companiesFile, line);
	int i = 0;
    while (getline(companiesFile, line)) {
        vector<string> columns = parse_csv(line);

        if (columns.size() != 5) 
            continue; // basic safety in case empty line or missing data    

        Company c;
        assignAtributes(c, columns, i);
        companies.push_back(c);
		i++;
    }
    companiesFile.close();
    return companies;
}


// this is the search engine builder, it stores the company id and the field where that word has been found
// more info about it on the readme file. (short reason for using an inverted index is the time complexity wit, i traded space complexity for time)
InvertedIndex buildInvertedIndex(std::vector<Company>& companies) {
    InvertedIndex index;

    for (auto& comp : companies) {
        int cid = comp.getId();

        // 1. DESCRIPTION
        string desc = removePunct(toLowerString(comp.getDescription()));
        for (const auto& word : tokenize(desc)) {
            index[word].insert({cid, DESCRIPTION});
        }

        // 2. BUSINESS_TAGS
        for (const auto& tag : comp.getBusiness_tag()) {
            string t = removePunct(toLowerString(tag));
            for (const auto& word : tokenize(t)) {
                index[word].insert({cid, BUSINESS_TAGS});
            }
        }

        // 3. SECTOR
        string sector = removePunct(toLowerString(comp.getSector()));
        for (const auto& word : tokenize(sector)) {
            index[word].insert({cid, SECTOR});
        }

        // 4. CATEGORY
        string category = removePunct(toLowerString(comp.getCatrgory()));
        for (const auto& word : tokenize(category)) {
            index[word].insert({cid, CATEGORY});
        }

        // 5. NICHE
        string niche = removePunct(toLowerString(comp.getNiche()));
        for (const auto& word : tokenize(niche)) {
            index[word].insert({cid, NICHE});
        }
    }

    return index;
}

// gets the keyword from the label and looks for matches in the inverted index map, assign labels bassed on the score(wheight)
void assignLabels(vector<Company>& companies,  vector<string>& labels, InvertedIndex& index) {
    // Define points per field
    unordered_map<Field, int> fieldPoints = {
        {DESCRIPTION, 1},
        {BUSINESS_TAGS, 2},
        {SECTOR, 2},
        {CATEGORY, 3},
        {NICHE, 3}
    };

    for (auto& label : labels) {
        string labelLower = toLowerString(label);
        vector<string> keywords = tokenize(removePunct(labelLower));
	
        unordered_map<int, int> companyScores; // company id -> score
        // so we iterate trough our keywords from the label and check if exists in our inverted index,
        //  if so we give it some points bassed on which column(field) is found
        for (auto& kw : keywords) {
            if (index.find(kw) != index.end()) {
                for (auto& [cid, field] : index.at(kw)) {
                    companyScores[cid] += fieldPoints[field];
                }
            }
        }

        //assign label to companies with positive score
        for (auto& [cid, score] : companyScores) {
            if (score >= 5) { // score to assign label
                companies[cid].addLabel(label);
            }
        }
    }
}
//function to write back the file with the new column
// the function writes everything again in to a new file
void writeCategorizedCSV(string output_path, vector<Company>& companies) {
    ofstream outFile(output_path);
    if (!outFile.is_open()) {
        cerr << "Error , Could not open " << output_path << " for writing." << endl;
        return;
    }
    //write column titles 
    outFile << "description,business_tags,sector,category,niche,insurance_label" << endl;
    //adding \ for the "" (quotes)
    //go trough each row
    for (auto& c : companies) {
        outFile << "\"" << c.getDescription() << "\",";
        outFile << "\"[";
        vector<string> tags = c.getBusiness_tag();
        for (size_t i = 0; i < tags.size(); ++i) {
            outFile << "'" << tags[i] << "'";
            if (i != tags.size() - 1)
                outFile << ", ";
        }
        outFile << "\"]";
        outFile << "\",";
        outFile << c.getSector() << ",";
        outFile << c.getCatrgory() << ",";
        outFile << c.getNiche() << ",";

        //adding new column
        outFile << "\"[";
        vector<string> labs = c.getLabels();
        for (size_t i = 0; i < labs.size(); ++i) {
            outFile << labs[i];
            // if this is not the last element add a comma
            if (i < labs.size() - 1)
                outFile << ",";
        }
        outFile << "]\"" << endl; 
    }

    outFile.close();
}

 
int main(int argc, char *argv[]){

    if(argc != 3){
        cerr<< "Wrong number of arguments!" << endl;
        return 1;
    }
    string folder_path = "insurances/";
    string companies_path = folder_path + argv[1];
    string labels_path = folder_path +  argv[2];
	Company company;
	vector<Company>companies = getAllCompanies(companies_path);
	vector<string>labels = getLabels(labels_path);

    // Assign labels
	InvertedIndex index = buildInvertedIndex(companies);
    assignLabels(companies, labels, index);
    
    string output_path = "classified_" + string(argv[1]);
    writeCategorizedCSV(output_path,companies);
	return 0;

}


/// try to match every keword from the label in every section for the company
// conver everything to lowercase if use keyword
// inverted index approach
// took out some words that apear to foten (service business )
// Option 2 — Domain expansion (recommended, still rule-based)
// Add controlled semantic bridges:unordered_map<string, vector<string>> DOMAIN_EXPANSION = {
//     {"farm", {"food", "agriculture"}},
//     {"vegetable", {"food"}},
//     {"farming", {"food"}},
//     {"cafe", {"food"}}
// };
// Food Safety Services
// → food + safety
// had to check for \r and \n 

// i write to a new file so i don t corrupt the source in case of failure
//use back slash to write double quotes
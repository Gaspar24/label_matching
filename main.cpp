#include "Company.hpp"


// works
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



// works
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
            continue; // basic safety

        Company c;
        assignAtributes(c, columns, i);
        companies.push_back(c);
		i++;
    }
    companiesFile.close();
    return companies;
}


using InvertedIndex = unordered_map<string,set<pair<int,Field>>>;

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
		// cout << label << endl;
		// cout << "---------------------" << endl;
        string labelLower = toLowerString(label);
        vector<string> keywords = tokenize(removePunct(labelLower));
	
        unordered_map<int, int> companyScores; // company id -> score

        for (auto& kw : keywords) {
            if (index.find(kw) != index.end()) {
                for (auto& [cid, field] : index.at(kw)) {
                    companyScores[cid] += fieldPoints[field];
                }
            }
        }

        // Assign label to companies with positive score
        for (auto& [cid, score] : companyScores) {
            if (score >= 5) { // threshold to assign label
                companies[cid].addLabel(label);
            }
        }
    }
}

// use with arguments later
// 
int main(int argc, char *argv[]){

    if(argc != 3){
        // type to  error 
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
  
	// vector<string> labels_test =companies[0].getLabels();
	// cout << companies[0].getDescription()<< endl;
	// cout << "-------------------"<< endl;
	// for(int i = 0; i < labels_test.size();i++){
	// 	cout << labels_test[i] << endl;
	// }
	for(auto &c : companies){
		vector<string> lab = c.getLabels();
		cout << "Company ID " << c.getId() << ": ";
		for(int i = 0; i < lab.size(); i++){
			cout << lab[i] << " | ";
		}
		cout << endl;
		cout << "---------------------" << endl;

	}


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
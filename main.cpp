
#include "classes.hpp"
#include <cctype>


//helper function to parse the cs
vector<string> parse_csv(string line){
	string currentToken;
	bool quoteFlag = false;
	vector<string>columns;

	for(int i = 0; i <line.length(); i++){
		if(line[i] == '"'){
			quoteFlag =!quoteFlag;

		}
		else if(line[i] == ',' && quoteFlag == false){
			columns.push_back(currentToken);
			currentToken = "";
		}
		else if(line[i] != '\n' && line[i] != '\r')
			currentToken += line[i];
	}
	//add final column
	columns.push_back(currentToken);
	return columns;
}


// use with arguments later
// works
vector<string> getLabels(void){
	fstream labels_taxonomy;
	labels_taxonomy.open("insurance_taxonomy.csv",ios::in);
	if(!labels_taxonomy.is_open()){
		cout << "can t open file!\n";
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
//works
vector<string> parseTags(string str) {
    vector<string> tags;
    string tag = "";
    
    for (int i = 0; i < str.length(); i++) {
        // 1. If comma, save the tag and reset
        if (str[i] == ',') {
            if (!tag.empty()){
				tags.push_back(tag);
				tag = "";
			}
        }
        // 2. If it's a valid character (alphanumeric or space), add to tag
        // We exclude spaces at the very beginning of a tag to prevent " Leading Spaces"
        else if (isalnum(static_cast<unsigned char>(str[i])) || (str[i] == ' ' && !tag.empty())) {
            tag += str[i];
        }
        // 3. Ignore everything else (brackets, quotes)
    }
    // 4. Push the last tag that wasn't followed by a comma
    if (!tag.empty()) {
        tags.push_back(tag);
    }
    return tags;
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

vector<Company> getAllCompanies() {
    fstream companiesFile("ml_insurance_challenge1.csv", ios::in);
    if (!companiesFile.is_open()) {
        cout << "can't open company file\n";
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


// make a string to lower function

string toLowerString(string str){
	int i = 0;
	while(i < str.length()){
		str[i] = tolower(static_cast<unsigned char>(str[i]));
		i++;
	}
	return str;
}

vector<string> tokenize( string text) {
	set<string> STOPWORDS = {
		"and","or","the","a","an","of","in","on","for","with",
		"services","service","solutions","solution",
		"company","companies","business","industry","manufacturing"
	};
    vector<string> tokens;
    string current;

    for (int i = 0; i < text.length();i++) {
		char c = text[i];
        if (isalnum(static_cast<unsigned char>(c))) {
            current += tolower(c);
        } else {
            if (!current.empty()) {
				if(STOPWORDS.find(current) == STOPWORDS.end())
                	tokens.push_back(current);
                current.clear();
            }
        }
    }
    if (!current.empty() && STOPWORDS.find(current) == STOPWORDS.end())
        tokens.push_back(current);

    return tokens;
}

// Helper: remove punctuation
std::string removePunct(const std::string& str) {
    std::string result;
    for (char c : str) {
        if (!ispunct(static_cast<unsigned char>(c)))
            result += c;
        else
            result += ' '; // replace punctuation with space to split words
    }
    return result;
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
// 
int main(void){
	Company company;
	vector<Company>companies = getAllCompanies();
	vector<string>labels = getLabels();

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

// int main() {
//     // Create a few sample companies
//     vector<Company> companies(2);

//     companies[0].setId(0);
//     companies[0].setDescription("We provide civil engineering and construction services");
//     companies[0].setBussines_tag({"Construction Services", "Multi-utilities"});
//     companies[0].setSector("Services");
//     companies[0].setCategory("Civil Engineering Services");
//     companies[0].setNiche("Other Heavy and Civil Engineering Construction");

//     companies[1].setId(1);
//     companies[1].setDescription("A vegetable farm with a farm shop and cafe");
//     companies[1].setBussines_tag({"Farm Cafe", "Fresh Vegetables"});
//     companies[1].setSector("Manufacturing");
//     companies[1].setCategory("Farms & Agriculture Production");
//     companies[1].setNiche("All Other Miscellaneous Crop Farming");

//     // Sample labels
//     vector<string> labels = {
//         "Construction Services",
//         "Community Engagement Services",
// 		"Food Safety Services",
//         "Environmental Health Services",
// "Quality Assurance Services",
// "Compliance Services",
// "Environmental Health Services",
// "Risk Assessment Services",
// "Crisis Management Services",
// "Community Engagement Services",
// "Stakeholder Services",
// "Corporate Responsibility Services",
// "Fundraising Services",
// "Volunteer Services",
// "Non-Profit Management",
// "Arts Services",
// "Sports Management Services",
// "Fitness Coaching",
// "Health Promotion Services",
// "Physical Therapy Services"
//     };

//     InvertedIndex index = buildInvertedIndex(companies);
//     assignLabels(companies, labels, index);

//     for (auto& c : companies) {
//         cout << "Company ID " << c.getId() << " assigned labels: ";
//         for (auto& l : c.getLabels()) cout << l << " | ";
//         cout << endl;
//     }

//     return 0;
// }

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
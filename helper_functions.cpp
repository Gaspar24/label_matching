#include "Company.hpp"

//helper function to parse the csv, it will basicly separate the row in to columns (the description, bussines_tag...etc)
// store them in a vector so we can go trough it later to assign the atributes
vector<string> parse_csv(string line){
	string currentToken;
	bool quoteFlag = false;
	vector<string>columns;

	for(int i = 0; i <line.length(); i++){
		if(line[i] == '"'){
			quoteFlag =!quoteFlag;

		} 
        // check if comma found and not between quotes
		else if(line[i] == ',' && quoteFlag == false){
			columns.push_back(currentToken);
			currentToken = "";
		}
        //safety check for end of line and carriage return 
		else if(line[i] != '\n' && line[i] != '\r')
			currentToken += line[i];
	}
	//add final column
	columns.push_back(currentToken);
	return columns;
}

//function to parse the tags and add them to the vector
vector<string> parseTags(string str) {
    vector<string> tags;
    string tag = "";
    
    for (int i = 0; i < str.length(); i++) {
        // if find comma push the tag and reset
        if (str[i] == ',') {
            if (!tag.empty()){
				tags.push_back(tag);
				tag = "";
			}
        }
         // check if alphanumeric and ignore leading spaces
        else if (isalnum(static_cast<unsigned char>(str[i])) || (str[i] == ' ' && !tag.empty())) {
            tag += str[i];
        }
    }
    // push the last tag 
    if (!tag.empty()) {
        tags.push_back(tag);
    }
    return tags;
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
// function to break the string in to individual words 
vector<string> tokenize( string text) {
    // just defined some stopWords (to common  appera to often) in a set (set  because allows O(log n) lookup)
    // feels a bit hardcoded , i didn t know if i should keep it.
	set<string> STOPWORDS = {
		"and","or","the","a","an","of","in","on","for","with",
		"services","service","solutions","solution",
		"company","companies","business","industry","manufacturing"
	};
    vector<string> tokens;
    string current;

    for (int i = 0; i < text.length();i++) {
		char c = text[i];
        // if the char is alphanumeric convert to lower and build the token
        if (isalnum(static_cast<unsigned char>(c))) {
            current += tolower(c);
        }
        // if we hit something else we check if  the world
        //  is build and if it s not on the stopWord list we add it and reset the token
        else {
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

//  remove punctuation
std::string removePunct(const std::string& str) {
    std::string result;
    for (char c : str) {
        if (!ispunct(static_cast<unsigned char>(c)))
            result += c;
        else
            result += ' '; // replace punctuation with space to split the word
    }
    return result;
}
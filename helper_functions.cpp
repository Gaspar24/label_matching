#include "Company.hpp"

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
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <cctype>
#include <unistd.h> // test for now

using namespace std;


enum  Field {
    DESCRIPTION,
    BUSINESS_TAGS,
    SECTOR,
    CATEGORY,
    NICHE
};

class Company
{
private:
	string _description;
	vector<string> _business_tag;
	string _sector;
    string _category;
    string _niche;
	int		_score;
	int 	_id;
	vector<string> _labels;

public:
	Company();
	~Company();

	//getters
	string getDescription();
	vector<string> getBusiness_tag();
	string getSector();
	string getCatrgory();
	string getNiche();
	vector<string> getLabels();

	int	   getScore();
	int		getId();

	//setters
	void setId(int nb);
	void setDescription(string description);
	void setBussines_tag(vector<string>busines_tag);
	void setSector(string sector);
	void setCategory(string category);
	void setNiche(string niche);


	void updateScore(int nb);
	void addLabel(string label);
	
};
using InvertedIndex = unordered_map<string,set<pair<int,Field>>>;

		// helper functions
vector<string> parse_csv(string line);
vector<string> parseTags(string str);
std::string removePunct(const std::string& str);
vector<string> tokenize( string text);
string toLowerString(string str);
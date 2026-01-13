
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <set>



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
	vector<string> _labels; // assigned labels

public:
	// Company(string description,vector<string> business_tag,string sector, string category, string niche);
	void display();
	Company();
	~Company();
	//getters
	string getDescription();
	vector<string> getBusiness_tag();
	string getSector();
	string getCatrgory();
	string getNiche();
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
	//test
	void addLabel(string label);
	vector<string> getLabels(){return _labels;};
	
};



void Company::addLabel(string label) {
    if (find(_labels.begin(), _labels.end(), label) == _labels.end())
        _labels.push_back(label);
}
// ---- getters--



int Company::getId(){
	return _id;
}

int	Company::getScore(){
	return _score;
}

string Company::getDescription() {
    return _description;
}

vector<string> Company::getBusiness_tag() {
    return _business_tag;
}

string Company::getSector() {
    return _sector;
}

string Company::getCatrgory() { // Kept your spelling 'Catrgory' from the header
    return _category;
}

string Company::getNiche() {
    return _niche;
}

// --- Setters ---

void Company::setId(int nb){
	_id = nb;
}

void Company::updateScore(int nb){
	_score+=nb;
}

void Company::setDescription(string description) {
    _description = description;
}

void Company::setBussines_tag(vector<string> busines_tag) {
    _business_tag = busines_tag;
}

void Company::setSector(string sector) {
    _sector = sector;
}

void Company::setCategory(string category) {
    _category = category;
}

void Company::setNiche(string niche) {
    _niche = niche;
}

void Company::display(){
	cout<<this->_description << endl;
}
// Company::Company(string description,vector<string> business_tag,string sector, string category, string niche)
// {
// 	this->_description = description;
// 	this->_business_tag = business_tag;
// 	this->_sector = sector;
// 	this->_category = category;
// 	this->_niche = niche;
// }

Company::Company(){}

Company::~Company(){}

#include "Company.hpp"

Company::Company() : _score(0), _id(0) {}\
Company::~Company() {}

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

string Company::getCatrgory() { 
    return _category;
}

string Company::getNiche() {
    return _niche;
}

vector<string> Company::getLabels()
{return _labels;};

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

// void Company::display(){
// 	cout<<this->_description << endl;
// }

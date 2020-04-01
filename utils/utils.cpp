#include <iostream>
#include "utils.h"

using namespace std;

string Utils::strToUpper(string s)
{
    string saida;
    for(unsigned i=0;i<s.size();i++){
        saida+=toupper(s[i]);
    }
    return saida;
}


vector<string> Utils::splitString(string str, char delimitador)
{
    vector<string> tokens;
    string token = "";
    for(unsigned  i = 0; i < str.size(); i++){
        if(str.at(i) == delimitador){
            tokens.push_back(token);
            token = "";
        }else{
            token.push_back(str.at(i));
        }
        if(i == str.size()-1){
            tokens.push_back(token);
        }
    }
    return tokens;
}

bool Utils::replace(string& str, string from, string to) {
    size_t start_pos = str.find(from);
    if(start_pos == string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

#define EPSILON 1E-6
bool Utils::doubleEqual(double a, double b) {
    return fabs(a - b) < EPSILON;
}

bool Utils::isBinary(double a) {
    return Utils::doubleEqual(a, 1.0) || Utils::doubleEqual(a, 0.0);
}

//Max é exclusivo
unordered_set<unsigned> Utils::chooseRandomly(unsigned max, unsigned quantity)
{
    unordered_set<unsigned> values;
    unordered_set<unsigned>::iterator nav;

    while(values.size() != quantity){
        unsigned element = rand()%max;// tamanho maximo
        nav = values.find(element);
        if(nav == values.end()){
            values.insert(element);
        }
    }

    return values;
}

unordered_set<unsigned> Utils::chooseRandomlyRestricted(unsigned max, unsigned quantity, unordered_set<unsigned> restricted)
{
    unordered_set<unsigned> values;
    unordered_set<unsigned>::iterator nav;
    unordered_set<unsigned>::iterator restrictedNav;

    while(values.size() != quantity){
        unsigned element = rand()%max;// tamanho maximo
        nav = values.find(element);
        restrictedNav = restricted.find(element);
        if(nav == values.end() && restrictedNav == restricted.end()){ //Se não está presente em nenhum dos conjuntos
            values.insert(element);
        }
    }

    return values;
}

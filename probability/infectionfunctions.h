#ifndef INFECTIONFUNCTIONS_H
#define INFECTIONFUNCTIONS_H
#include <string>

//namespace probability {
using namespace std;

class InfectionFunctions
{
private:
    string  functionType;
    double a;
    double b;
    double c;
    double k;

public:
    InfectionFunctions(string,double,double,double,double);
    double calculate(double time);
};
//}

#include "infectionfunctions.cpp"
#endif // INFECTIONFUNCTIONS_H

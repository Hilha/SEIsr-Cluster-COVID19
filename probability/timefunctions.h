#ifndef TIMEFUNCTIONS_H
#define TIMEFUNCTIONS_H
#include <string>

//namespace probability {
using namespace std;

class TimeFunctions
{
private:
    string functionType;
    double k1;
    double k2;
    double b;
    double E;
    double c;

public:
    TimeFunctions(string,double,double,double,double,double);
    double calculate(double time);
};
//}
#include "timefunctions.cpp"

#endif // TIMEFUNCTIONS_H

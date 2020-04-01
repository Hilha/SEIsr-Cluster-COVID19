#include "timefunctions.h"
#include <math.h>

//using namespace probability;

TimeFunctions::TimeFunctions(string functionType, double k1, double k2, double b, double E, double c)
{
    this->functionType = functionType;
    this->k1 = k1;
    this->k2 = k2;
    this->b = b;
    this->E = E;
    this->c = c;

}


double TimeFunctions::calculate(double time){
    if(this->functionType == "poly"){
         return ((this->k1*pow(time,E))+c);
    }else if(this->functionType  == "exp"){
         return this->k1*pow(b,k2*time)+c;
    }else if(this->functionType  == "log"){
        return this->k1*log(time)/log(b)+c; //t e o b
    }else if(this->functionType  == "expexp"){
        return this->k1*pow(time,k2*time)+c;
    }else if(this->functionType  == "const"){
        return this->k1;
    }
    return 0;
}

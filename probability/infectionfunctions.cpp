#include "infectionfunctions.h"
#include <boost/math/distributions/normal.hpp>
//using namespace probability;

InfectionFunctions::InfectionFunctions(string functionType, double a, double b, double c, double k)
{
    this->functionType = functionType;
    this->a = a;
    this->b = b;
    this->c = c;
    this->k = k;
}

double InfectionFunctions::calculate(double time){

    if (this->functionType == "const"){
        return this->a * this->k;
    }

    if (this->functionType == "normal"){
        boost::math::normal_distribution<double> dist(this->a, this->b);
        return boost::math::pdf(dist, time)*this->k;
    }
    /*
    if(this->functionType == "uniform"){
        boost::math::uniform_distribution<double> dist(this->a, this->b);
        return boost::math::pdf(dist, time)*this->k;
    }
    if(this->functionType == "triangular"){
        boost::math::triangular_distribution<double> dist(this->a, this->b, this->c);
        return boost::math::pdf(dist, time)*this->k;
    }
    if(this->functionType == "beta"){
        boost::math::beta_distribution(this->a, this->b);
        return boost::math::pdf(dist, time)*this->k;
    }
    if(this->functionType == "poisson"){
        boost::math::poisson_distribution<double> dist(this->a);
        return boost::math::pdf(dist, time)*this->k;
    }
    */

    return 0;
}


#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <math.h>
#include <vector>
#include <unordered_set>
#include <utility>
#include <functional>
#include "../probability/infectionfunctions.h"

using namespace std;

struct Comunidade {
    unordered_set<unsigned> nodos;
    InfectionFunctions* probabilidade;
};

class Utils
{
public:
    static string strToUpper(string s);
    static vector<string> splitString(string str, char delimitador);
    static bool replace(string& str, string from, string to);
    static bool doubleEqual(double a, double b);
    static bool isBinary(double a);
    static unordered_set<unsigned> chooseRandomly(unsigned max, unsigned quantity);
    static unordered_set<unsigned> chooseRandomlyRestricted(unsigned max, unsigned quantity, unordered_set<unsigned> restricted);

    template <class Container>
    static Container takeFirst(const Container& data, unsigned count = 1) {
        Container c;

        for (typename Container::iterator it = data.begin(); it != data.end(); ++it) {
            c.insert(*it);
            count--;

            if (count == 0) break;
        }

        return c;
    }

    template <class Destination, class Source, class Mapper>
    static Destination mapContents(const Source& source, Mapper mapper) {
        Destination dest;

        for (typename Source::iterator it = source.begin(); it != source.end(); ++it) {
            dest.insert(mapper(*it));
        }

        return dest;
    }

    template <class Container>
    static void printContainer(const Container& container, ostream& out) {
        out << "{";
        bool first = true;
        for (typename Container::const_reference value : container) {
            if (!first) {
                out << ", ";
            }

            out << value;
            first = false;
        }
        out << "}\n";
    }
};

#include "utils.cpp"

#endif // UTILS_H

#ifndef LEITOR_H
#define LEITOR_H
#include "../graph/largegraph.h"
#include "../probability/infectionfunctions.h"
#include "../probability/timefunctions.h"
#include "utils.h"
#include <list>
#include <vector>
#include <unordered_map>
using namespace std;

class Leitor
{
public:
    struct Dados {
        string path;
        vector<Comunidade> comunidades;
        TimeFunctions* exposition;
        TimeFunctions* infection;
        InfectionFunctions* outside;
        
        //SANTIAGO:
        LargeGraph lg;
        NodeClasses  CDI;
        double initially_infected_rate;
        double removed_rate;
        unsigned periods;
        unsigned replications;
        double adjacents_to_visit;
        double k_edges;
        vector< pair<unsigned, unsigned> > edges_removed; //listas de edges a serem removidos. nao foi implementado ainda  (santiago: 20200321:15h19m)
        vector< unsigned > communityByNode; //id da comunidade de cada nodo
        string method;
        unordered_map <string, double> method_parameters;
    };

    static string extrairTipo(string informacao);
    static double *extrairNumeros(string informacao, int quantidade);
    static InfectionFunctions *lerInfection(string informacao);
    static TimeFunctions *lerTime(string informacao);
    static Leitor::Dados lerArquivo(string caminho);
};

#include "leitor.cpp"

#endif // LEITOR_H

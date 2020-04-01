#ifndef MINSEISCLUSTER_H
#define MINSEISCLUSTER_H

#include "graph/largegraph.h"
#include "probability/infectionfunctions.h"
#include "probability/timefunctions.h"
#include "utils/utils.h"
#include <functional>
#include <map>
#include <vector>
#include <list>
using std::vector;
using namespace std;
class MinSEISCluster
{
public:
    using UnsignedSet = unordered_set<unsigned int>;
    using SolutionGenerator = std::function<UnsignedSet(vector<LargeGraphEdge>&)>;
    using SolutionFunction = function<void(pair<double, UnsignedSet>)>;
    using NodeStatMap = unordered_map<double, pair<double, double>>;

private:

    
    enum NodeState{SUSCEPTIBLE, EXPOSED, INFECTED, 
		//SANTIAGO: adicionando a possibilidade de remoção
		REMOVED};

    struct Node { // adicionar contadores de infectado e exposed
        NodeState state;
        unsigned timeToChange;
        unsigned lastChange;
        unsigned comm;
        unsigned infecTimes=0;
        unsigned propagateTimes=0; //numero de vezes que propagou a infeccao
        Node(NodeState state_p, unsigned timeToChange_p, unsigned lastChange_p, unsigned comm_p):
            state(state_p), timeToChange(timeToChange_p), lastChange(lastChange_p), comm(comm_p){}
        //santiago:
        unsigned id;
        Node(NodeState state_p, unsigned timeToChange_p, unsigned lastChange_p, unsigned comm_p, unsigned id):
            state(state_p), timeToChange(timeToChange_p), lastChange(lastChange_p), comm(comm_p), id(id){}
    };

    LargeGraph graph;
    vector<Comunidade>& communities;
    unsigned k;
    unsigned totalTime;
    double initialFactor;
    InfectionFunctions* outsideFunction;
    TimeFunctions* expositionFunction;
    TimeFunctions* infectionFunction;
    UnsignedSet immuneVertices;
    SolutionGenerator generator; //Função responsável por gerar solução e remover arestas.
    NodeStatMap statMap; //Mapa nodo(propagou, recebeu)
    vector<pair<unsigned, unsigned>> infectionStats;
	//SANTIAGO
    vector< pair<unsigned, unsigned> > edgesRemoved;
    double removedRate;
    vector<unsigned> & communityByNode;    
    NodeClasses & CDI;

    void repairGraph(vector<LargeGraphEdge> backupOfEdges);
    vector<MinSEISCluster::Node> initializeNodes(const UnsignedSet &infectedVertices);
    unsigned simulateInfection(const UnsignedSet& infectedVertices, double adjacentsToInfect);

public:
	//SANTIAGO: cache de registro por unidade de tempo
    vector< list<unsigned> > dsByTime;
	bool removedStatus = true;

    MinSEISCluster(LargeGraph& graph, vector<Comunidade>& communities, unsigned k, unsigned totalTime, double initiallyInfected,
                   InfectionFunctions* outsideFunction, TimeFunctions* expositionFunction, TimeFunctions* infectionFunction,
                   //SANTIAGO:
				   double removedRate, vector< pair<unsigned, unsigned> > &edgesRemoved, vector<unsigned> &communityByNode,
				   NodeClasses & CDI
                   );


    std::vector<LargeGraphEdge> removeEdgeAndReturn(const UnsignedSet &chosenEdges);
    pair<double, unordered_set<unsigned>> solve(const UnsignedSet& infectedVertices, unsigned replications, double adjacentsToInfect, SolutionFunction func = nullptr);
    void immunize(UnsignedSet vertices);
    void setSolutionGenerator(SolutionGenerator generator);
    MinSEISCluster::SolutionGenerator getDefaultSolutionGenerator();

    NodeStatMap getStatMap() const;
    LargeGraph& getLargeGraph();
    vector<pair<unsigned, unsigned>> getInfectionStats() const;
};
#include "minseiscluster_mod.cpp"
#endif // MINSEISCLUSTER_H

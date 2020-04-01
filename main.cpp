#include <iostream>
#include <iomanip>
#include "nodeclasses.h"
#include "minseiscluster_mod.h"
#include "graph/largegraph.h"
#include <list>
#include <vector>
#include <unordered_set>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <ctime>
#include <random>
#include <chrono>
#include <numeric>
#include <utility>
#include "utils/leitor.h"

using namespace std;
string suffix;
string instance;

using Solution = std::vector<unsigned int>;
using UnsignedSet = std::unordered_set<unsigned int>;
using QualityPair = std::pair<std::vector<double>, std::vector<double>>;






template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& c) {
    out << "{";
    for (unsigned int i = 0; i < c.size(); i++) {
        if (i > 0) {
            out << ", ";
        }

        out << c[i];
    }

    out << "}\n";
    return out;
}








QualityPair testClassic(LargeGraph lg, Leitor::Dados dados) {
    
    QualityPair pair;

	//santiago:
	vector< list<unsigned> > dsByTimeBest;
	
	
    double best = std::numeric_limits<double>::max();
    cout<<"Selecionando nodos infectados..."<<endl;
	UnsignedSet infectedVertices = Utils::chooseRandomly(lg.numberOfNodes, std::ceil(lg.numberOfNodes * dados.initially_infected_rate));
    cout<<"Nodos infectados selecionados"<<endl;
	
    unsigned k = std::ceil(lg.numberOfEdges * dados.k_edges);
        
        
    
    MinSEISCluster msc(lg, dados.comunidades, k, dados.periods, dados.initially_infected_rate, dados.outside, dados.exposition, dados.infection,
            //SANTIAGO:
            dados.removed_rate, dados.edges_removed, dados.communityByNode, dados.CDI);

    

    std::pair<double, UnsignedSet> result = msc.solve(infectedVertices, dados.replications, dados.adjacents_to_visit);
    double sum = result.first;
	dsByTimeBest = msc.dsByTime;
	cout<<"finalizando a evaluation..."<<endl;
    best = sum;
    dsByTimeBest = msc.dsByTime;
    
    
    //SANTIAGO: teste
    int i=0;
    ofstream simp(instance+"_"+suffix+".resSimple2");
    //ofstream comp(instance+"_"+suffix+".resComplex2");
    for (list<unsigned> l: dsByTimeBest){
		simp<<"time; "<<i<<"; "<<l.size()<<"; "<< dados.CDI.effectedByTime[i] <<"\n";
		/*comp<<"time; "<<i<<"; "<<l.size()<<"; "<< dados.CDI.effectedByTime[i];
		for (unsigned v : l){
			comp<<v<<", ";
		}
		comp<<endl<<"\n";*/
		i++;
	}
	simp.close();
	//comp.close();
	
    std::cout << "Best: " << best << "\n";
    
    for (int i=0; i<dados.CDI.counterClasses.size();i++){
		cout<<dados.CDI.classes[i]<<": "<<dados.CDI.counterClassesEffected[i]<<" / "<<dados.CDI.counterClasses[i]<<endl;
	}
    return pair;
}





int main(int argc, char** argv) {
    
	//cout<<std::time(nullptr)<<endl; exit(0);
    std::srand(std::time(nullptr));


	suffix = "";
	if (argc > 2 ){
		suffix = argv[2];
	}
    
	

    instance  = argv[1];
    string path = "instances/MINSEISC/" + instance + ".msc2";
    cout<<"Lendo arquivo de entrada..."<<endl;
    Leitor::Dados dados = Leitor::lerArquivo(path);
    cout<<"Arquivo de entrada lido."<<endl;

	
    
    cout<<"Iniciando simulação..."<<endl;
	QualityPair classic = testClassic(dados.lg, dados) ;
    cout<<"Terminando simulação..."<<endl;
	

    
}

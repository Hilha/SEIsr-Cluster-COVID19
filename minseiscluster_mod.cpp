#include "minseiscluster_mod.h"
#include <utility>
#include <functional>
#include <cmath>
#include <algorithm>

using uint = unsigned int;
using UnsignedSet = unordered_set<unsigned>;

unsigned findEdge(const LargeGraph& lg, uint source, uint destination) {
    for (uint i = 0; i < lg.edges.size(); i++) {
        const SimpleEdgeLG& edge = lg.edges[i];

        if (edge.v1 == source && edge.v2 == destination) {
            return i;
        }
    }

    return -1;
}

unsigned findEdgeUnidirectional(const LargeGraph& lg, uint source, uint destination) {
    for (uint i = 0; i < lg.edges.size(); i++) {
        const SimpleEdgeLG& edge = lg.edges[i];

        if ((edge.v1 == source && edge.v2 == destination) ||
            (edge.v2 == source && edge.v1 == destination)) {
            return i;
        }
    }

    return -1;
}

MinSEISCluster::MinSEISCluster(LargeGraph &graph, vector<Comunidade> &communities, unsigned k,
                               unsigned totalTime, double initialFactor, InfectionFunctions *outsideFunction,
                               TimeFunctions *expositionFunction, TimeFunctions *infectionFunction,

							   //SANTIAGO:
							   double removedRate, vector< pair<unsigned, unsigned> > &edgesRemoved,
							   vector<unsigned> &communityByNode, NodeClasses & CDI
                                           
                               
                               
                               )
    : graph(graph), communities(communities), k(k), totalTime(totalTime), initialFactor(initialFactor),
        outsideFunction(outsideFunction), expositionFunction(expositionFunction), infectionFunction(infectionFunction),
        removedRate(removedRate), communityByNode(communityByNode), CDI (CDI)
{
    generator = getDefaultSolutionGenerator();
	this->edgesRemoved = edgesRemoved; //nao implementado ainda  (20200321)
    for (uint i = 0; i < graph.numberOfNodes; i++) {
        statMap[i] = make_pair(0, 0);
    }
}

MinSEISCluster::NodeStatMap MinSEISCluster::getStatMap() const
{
    return statMap;
}

vector<LargeGraphEdge> MinSEISCluster::removeEdgeAndReturn(const UnsignedSet& edges){
    vector<LargeGraphEdge> removed;

    for(unsigned edgeNumber : edges){
        SimpleEdgeLG edge = graph.edges[edgeNumber];

        //std::cout << "Removing edge " << edgeNumber << ". Vertices are " << edge.v1 << " and " << edge.v2 << ".\n";

        unordered_map<unsigned, LargeGraphEdge>* adjacents = &graph.adjNodes[edge.v1];
        unordered_map<unsigned, LargeGraphEdge>::iterator edgeToRemove = adjacents->find(edge.v2);

        if(adjacents->end() != edgeToRemove){
            removed.push_back(edgeToRemove->second);
            adjacents->erase(edgeToRemove);
        }

        adjacents = &graph.adjNodes[edge.v2];
        edgeToRemove = adjacents->find(edge.v1);

        if(adjacents->end() != edgeToRemove){
            removed.push_back(edgeToRemove->second);
            adjacents->erase(edgeToRemove);
        }

    }

    return removed;
}

MinSEISCluster::SolutionGenerator MinSEISCluster::getDefaultSolutionGenerator() {
    return [&](vector<LargeGraphEdge>& removed) {
        UnsignedSet solution = Utils::chooseRandomly(graph.numberOfEdges, k);

        removed = removeEdgeAndReturn(solution);
        return solution;
    };
}

LargeGraph &MinSEISCluster::getLargeGraph()
{
    return graph;
}

vector<pair<unsigned, unsigned>> MinSEISCluster::getInfectionStats() const
{
    return infectionStats;
}

void MinSEISCluster::repairGraph(vector<LargeGraphEdge> backupOfEdges){
    for(LargeGraphEdge edge : backupOfEdges){
        graph.adjNodes[edge.sourceNode].insert(std::make_pair(edge.targetNode, edge));
    }
}

vector<MinSEISCluster::Node> MinSEISCluster::initializeNodes(const UnsignedSet& infectedVertices) {
    vector<Node> nodes;
	
    UnsignedSet::const_iterator itInfected;
    //agora para inicialização tera de achar onde o nodo v se localiza
    for (unsigned v = 0; v < graph.numberOfNodes; v++){//inicializa os minseisnodes
        itInfected = infectedVertices.find(v);
        UnsignedSet::iterator communityNumber;
        unsigned x = 0;
        //SANTIAGO: REMOVI ESSA PARTE, POIS É MUITO CUSTOS PARA UM GRAFO GRANDE.
        /*for (; x < communities.size(); x++){
            communityNumber = communities[x].nodos.find(v);
            if (communityNumber != communities[x].nodos.end()){
                break;
            }
        }*/
        x = communityByNode[v];
        //Passar essa informação (infectedVertices) para o arquivo .cgc
        if (itInfected == infectedVertices.end()){
            nodes.push_back(Node(SUSCEPTIBLE, 0, 1, x, v) );
        } else {
            nodes.push_back(Node(INFECTED, (infectionFunction->calculate(1)+1), 1, x, v));
        }
    }

    return nodes;
}


unsigned MinSEISCluster::simulateInfection(const UnsignedSet& infectedVertices, double adjacentsToInfect) {
	cout<<"Iniciando simulateInfection "<<endl;
    UnsignedSet infecteds(infectedVertices);
    UnsignedSet exposeds;
    cout<<"Inicializando nodos... "<<endl;
    vector<Node> nodes = initializeNodes(infectedVertices);
	cout<<"Nodos inicializados... "<<endl;
    unsigned infectionAmount = 0;
    
    //SANTIAGO: registro por unidade de tempo
    cout<<"Preparando estrutura de dados para registro de infectados por ut... "<<endl;
    dsByTime.clear();
    for (unsigned it = 0; it < totalTime; it++){
		dsByTime.push_back(list<unsigned>());
	}

	
	cout<<"Elapsed time: "<<endl;
    for (unsigned it = 1; it <= totalTime; it++){ // tem que verificar a solução apos acabar este FOR
		cout<<"\t"<<it<<endl;
        bool removeElementAux;//auxiliar para remover os elementos das listas
		//gravar mortos
		CDI.effectedByTime.push_back(0);
        
        for(UnsignedSet::iterator navInfecteds = infecteds.begin(); navInfecteds != infecteds.end(); ){
            removeElementAux = false;
            Node& infectedNode = nodes[*navInfecteds];

            if (infectedNode.timeToChange <= it) {
                infectedNode.lastChange = it;
                infectedNode.timeToChange = 0;
                infectedNode.state = SUSCEPTIBLE;
                //SANTIAGO:
                
                double currentChance = rand() / static_cast<double>(RAND_MAX);
                //if(this->removedStatus){
                if(currentChance <= removedRate){
					infectedNode.state = REMOVED;
					//considerar como morto ou vivo
					if (CDI.effectedByNodeId(infectedNode.id)){
						/*if (CDI.classByNodeId[infectedNode.id] ==0 ){
							cout<<infectedNode.id<<endl;
							//exit(0);
						}*/
						CDI.effectedByTime[it-1]++;
					}
				}
                removeElementAux = true;
            }//ele remove no tempo porem ainda neste tempo ele esta
            //infectando
//cout<<"infectando..."<<endl;
            unsigned i = 0;
            std::unordered_map<unsigned, LargeGraphEdge>& adjacents = graph.adjNodes[*navInfecteds];
            //SANTIAGO: removi esse trecho e adicionei dentro do laço de infecção, assumindo que adjacentsToInfect% das adjancencias são selecionadas sem ordem, por isso a distribuição aleatória normal
            unsigned int visits = std::ceil(adjacents.size());// * adjacentsToInfect);
            //std::cout << "Node " << *navInfecteds << " has " << adjacents.size() << " adjacents, visiting " << visits << ".\n";

//            std::random_shuffle(ids.begin(), ids.end());
//cout<<"tentando infectar adjacentes..."<<endl;
            for (const std::pair<unsigned, LargeGraphEdge>& adj : adjacents) {
				
                const LargeGraphEdge& edge = adj.second;

                //Se o vértice está imune
//                if (immuneVertices.find(edge.targetNode) != immuneVertices.end()) {
//                    continue;
//                }
				//SANTIAGO
				/*
                if(i >= visits){
                    break;
                }*/
                double chanceToVisit = rand() / static_cast<double>(RAND_MAX);
                if (chanceToVisit > adjacentsToInfect){
					continue;
				}
                i++;

                Node& node = nodes[edge.targetNode];
                if(node.state != INFECTED && node.state != EXPOSED
					//SANTIAGO:
					 && node.state != REMOVED
                ) {
//cout<<"Infectar!:"<<node.comm<<endl;					
//cout<<"Node!:"<<node.id<<endl;			
//cout<<"infectedNode!:"<<infectedNode.id<<endl;			
                    double chance;
                    if (node.comm == infectedNode.comm) {
//cout<<"211"<<node.comm<<endl;					
						//SANTIAGO:
                        //ANTES: chance = communities[node.comm].probabilidade->calculate(it); //Dentro da comunidade
                        chance = communities[node.comm].probabilidade->calculate(it) * CDI.inMultiplierByNodeId(node.id); //Dentro da comunidade
                    } else {
//cout<<"214"<<node.comm<<endl;											
                        chance = outsideFunction->calculate(it)* CDI.outMultiplierByNodeId(node.id); //Fora
                    }
                    
//cout<<"#215"<<endl;					
					
                    double currentChance = rand() / static_cast<double>(RAND_MAX);
//cout<<"#217"<<endl;					                    
                    if(currentChance <= chance) {
						
//cout<<"#219"<<endl;											
                        infectionAmount++;
                        node.state = EXPOSED;
                        node.lastChange = it;
                        node.timeToChange = expositionFunction->calculate(it)+it;
//cout<<"#224"<<endl;					                        
//                        infectionStats.push_back({edge.sourceNode, edge.targetNode});

//                        statMap[edge.sourceNode].first++; //Origem propagou infecção
//                        statMap[edge.targetNode].second++; //Destino recebeu infecção

                        nodes[*navInfecteds].propagateTimes++; //CONTADOR DE PROPAGA INFECCAO
//cout<<"#230"<<endl;					                                                
                        exposeds.insert(edge.targetNode);
//cout<<"#232"<<endl;					                        
                    }
                }


            }

            UnsignedSet::iterator aux = navInfecteds;
            navInfecteds++;
            if (removeElementAux){
                infecteds.erase(*aux);
            }
        }

        removeElementAux = false;
        for(UnsignedSet::iterator navExposeds = exposeds.begin(); navExposeds != exposeds.end();) {
            Node& node = nodes[*navExposeds];
            if(node.timeToChange <= it){
                node.timeToChange = infectionFunction->calculate(it)+it;
                node.lastChange = it;
                node.state = INFECTED;
                node.infecTimes++;
                infecteds.insert(*navExposeds);
                removeElementAux = true;
            }

            UnsignedSet::iterator aux = navExposeds; // necessario para não perder posição na fila
            navExposeds++;
            if(removeElementAux){
                removeElementAux = false;
                exposeds.erase(*aux);
            }
        }
   
   				//SANTIAGO: adiciona infectados no tempo
                for(unsigned v : infecteds){
					dsByTime[it-1].push_back(v);
				}
    }

    return infectionAmount;
}

void MinSEISCluster::setSolutionGenerator(SolutionGenerator generator)
{
    this->generator = generator;
}

pair<double, unordered_set<unsigned>> MinSEISCluster::solve(const UnsignedSet& infectedVertices, unsigned replications, double adjacentsToInfect, SolutionFunction func)
{
    vector<LargeGraphEdge> removedEdges;
    double bestAttempt = numeric_limits<double>::max();
    vector<LargeGraphEdge> bestSolution;
    UnsignedSet selectedEdges;
    UnsignedSet bestSelectedEdges;  

    selectedEdges = generator(removedEdges);

    unsigned worstReplication = numeric_limits<unsigned>::min();

    for (unsigned rep = 0; rep < replications; rep++){
        unsigned infectionAmount = simulateInfection(infectedVertices, adjacentsToInfect);
        //cout << "Resultado: " << infectionAmount << "\n";

        //guarda solução para verificar
        if(infectionAmount > worstReplication){
            worstReplication=infectionAmount;//so guardar a maior de todas as replicações
        }

    }


    if(worstReplication < bestAttempt) {
        bestAttempt = worstReplication;
        bestSolution = removedEdges;
        bestSelectedEdges = selectedEdges;
    }

    if (func != nullptr) {
        func(make_pair(worstReplication, selectedEdges));
    }

//    repairGraph(removedEdges);

    //std::cout << "Melhor encontrada: " << bestAttempt << "\n";
    //std::cout << "Soluçao: ";
    //Utils::printContainer(bestSelectedEdges, std::cout);
    return make_pair(bestAttempt, bestSelectedEdges);
}

void MinSEISCluster::immunize(MinSEISCluster::UnsignedSet vertices)
{
    immuneVertices = vertices;
}


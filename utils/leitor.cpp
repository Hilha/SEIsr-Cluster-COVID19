#include "leitor.h"

#include <unordered_set>
#include <iostream>
#include <fstream>
using namespace std;

string Leitor::extrairTipo(string informacao) {
    return informacao.substr(0, informacao.find_first_of(","));
}

double* Leitor::extrairNumeros(string informacao, int quantidade) {
    double* numeros = new double[quantidade];
    string::size_type index = 0;
    for (int i = 0; i < quantidade; i++) {
        if (informacao.length() < index) {
            numeros[i] = 0;
        } else {
            informacao = informacao.substr(index);
            numeros[i] = stod(informacao, &index);
            index++;
        }
        
    }
    return numeros;
}

InfectionFunctions* Leitor::lerInfection(string informacao) {
    string tipo = extrairTipo(informacao);
    double* numeros = extrairNumeros(informacao.substr(tipo.length() + 1), 4);
    return new InfectionFunctions(tipo, numeros[0], numeros[1], numeros[2], numeros[3]);
}

TimeFunctions* Leitor::lerTime(string informacao) {
    string tipo = extrairTipo(informacao);
    double* numeros = extrairNumeros(informacao.substr(tipo.length() + 1), 5);
    return new TimeFunctions(tipo, numeros[0], numeros[1], numeros[2], numeros[3], numeros[4]);
}

Leitor::Dados Leitor::lerArquivo(string caminho){
    Dados read;
    
    string path;
    string line;
    string antes;
    string depois;
    ifstream myfile (caminho);
    
    
    
    

    if (myfile.is_open()) {
        while (getline(myfile,line)) {
			//SANTIAGO: comments
			if (line[0] == '#'){
				continue;
			}
			
            int pos = line.find_first_of(":") + 1;
            antes = line.substr(0, pos);
            depois = line.substr(pos);

            if(antes == "graph:") {
                read.path=depois;
                path = depois;
                //SANTIAGO
                cout<<"\tLendo grafo de entrada..."<<endl;
				read.lg.readFile("instances/"+path);
				cout<<"\tGrafo de entrada lido:"<<read.lg.numberOfNodes<<endl;
				cout<<"\tPreparando estrutura de dados auxiliar para comunidade..."<<endl;
				for (unsigned i=0; i<read.lg.numberOfNodes; i++){
					read.communityByNode.push_back(-1);
				}
				cout<<"\tEstrutura de dados auxiliar para comunidade preparada."<<endl;
				cout<<"\tDefinindo as classes de cada nodo..."<<endl;
				for (unsigned i=0; i<read.lg.numberOfNodes; i++){
					unsigned cid = read.CDI.getRandomClass();
					read.CDI.classByNodeId.push_back(cid);
					read.CDI.counterClasses[cid]++;
				}
				cout<<"\tClasses de cada nodo definidas."<<endl;
				/*teste de classes
				 for (int i=0; i<read.CDI.counterClasses.size();i++){
					cout<<read.CDI.classes[i]<<": "<<read.CDI.counterClasses[i]<<", "<<read.CDI.outFactor[i]<<", "<<read.CDI.inFactor[i]<<endl;
				}*/
				
            } else if(antes == "communities:") {
				cout<<"\tIniciando leitura das comunidades..."<<endl;
                Comunidade comunidade;
                unordered_set<unsigned> nodos;

                /*string::size_type index = 0;
                do {
                    depois = depois.substr(index);
                    nodos.insert(stoi(depois, &index));
                    if (depois[index] == ';') {
                        comunidade.nodos = nodos;
                        read.comunidades.push_back(comunidade);
                        nodos.clear();
                    }
                    index++;
                } while (depois.length() >= index);
                */
                string value="";
                for(int i=0; i<depois.size(); i++){
					char v = depois[i];
					if (v>='0' && v<='9'){
						value+=v;
					}else{
						nodos.insert(atoi(value.c_str()));
						read.communityByNode[atoi(value.c_str())] = read.comunidades.size();//grava id da comunidade
						value="";
						
						if (v == ';'){
							comunidade.nodos = nodos;	
							read.comunidades.push_back(comunidade);
							nodos.clear();
						}
					}
				}

				
                //Última comunidade
               comunidade.nodos = nodos;
               read.comunidades.push_back(comunidade);
               cout<<"\tComunidades lidas."<<endl;
            } else if(antes == "expositionTime:"){
                read.exposition = lerTime(depois);
            } else if(antes == "infectionTime:"){
                read.infection = lerTime(depois);
            } else if(antes == "probabilityOutside:"){
                read.outside = lerInfection(depois);
            } else if(antes == "probabilityInside:"){
				cout<<"\tIniciando leitura das probabilidades das comunidades..."<<endl;
				bool copy = false;
				string save = depois;
                for (unsigned i = 0; i < read.comunidades.size(); i++) {
					if (depois[0] == '*'){
						//cout<<"copy"<<endl;
						copy = true; 
						break;
					}
                    read.comunidades[i].probabilidade = lerInfection(depois);
                    depois = depois.substr(depois.find_first_of(";") + 1);
                }
                //cout<<save<<endl;
                if (copy){
					for (unsigned i = 1; i < read.comunidades.size(); i++) {
						read.comunidades[i].probabilidade = lerInfection(save);
					}
				}
				cout<<"\tLeitura das probabilidades das comunidades concluída."<<endl;
				//cout<<read.comunidades.size()<<endl;exit(0);
            } else if(antes == "method:"){
				read.method = depois;
			} else if(antes == "initially_infected_rate:"){
				read.initially_infected_rate =  atof(depois.c_str());
			} else if(antes == "removed_rate:"){
				read.removed_rate = atof(depois.c_str());
			} else if(antes == "replications:"){
				read.replications = atoi(depois.c_str());
			} else if(antes == "adjacents_to_visit:"){
				read.adjacents_to_visit = atof(depois.c_str());
			} else if(antes == "periods:"){
				read.periods = atof(depois.c_str());
			} else if(antes == "perc_edges_to_remove:"){
				read.k_edges = atof(depois.c_str());
			} else if(antes == "edges_to_remove:"){
				vector <string> values = Utils::splitString(depois, ';');
				for (string value : values){
					vector <string> edge = Utils::splitString(value, ',');
					read.edges_removed.push_back(pair<unsigned, unsigned> (atoi(edge[0].c_str()),atoi(edge[1].c_str())));
				}
			} else if(antes == "method_parameters:"){
				vector <string> values = Utils::splitString(depois, ';');
				for (string value: values){
					vector <string> parameter = Utils::splitString(value, ':');
					read.method_parameters[parameter[0]]=atof(parameter[1].c_str());
				}
			}
        }
        myfile.close();
        return read;
    }else{
        cout << "Unable to open file";
    }
}

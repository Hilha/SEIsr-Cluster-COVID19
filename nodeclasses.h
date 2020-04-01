#ifndef NODECLASSES_H
#define NODECLASSES_H

#include <iostream>
#include <vector>
#include <list>
using namespace std;

//SANTIAGO: classes de pessoas para o efeito MORTE
class NodeClasses{
public:
	vector<string> classes; //nome da classe
	vector<long double> chanceToBelong; //chance de pertencer a classe
	vector<long double> effectChance; 
	vector<long double> upperboundChance; //para facilitar o calculo
	//vector<string> effectLabel;
	vector<unsigned> counterClasses;
	vector<unsigned> counterClassesEffected;
	vector<unsigned >effectedByTime;
	vector<unsigned> classByNodeId;
	//multipliers
	vector<double> outFactor;
	vector<double> inFactor;
	
	NodeClasses(){
		double fora = 0.01;
		double dentro = 0.01;
		
		//80+ years old
		classes.push_back("80+");
		chanceToBelong.push_back(0.012412875);
		effectChance.push_back(0.148);
		//effectLabel("Death");
		counterClasses.push_back(0);
		counterClassesEffected.push_back(0);
		outFactor.push_back(fora);
		inFactor.push_back(dentro);
		
		//70-79 years old
		classes.push_back("70-79");
		chanceToBelong.push_back(0.0335117433);
		effectChance.push_back(0.08);
		//effectLabel("Death");
		counterClasses.push_back(0);
		counterClassesEffected.push_back(0);
		outFactor.push_back(fora);
		inFactor.push_back(dentro);		
				
		//60-69 years old
		classes.push_back("60-69");
		chanceToBelong.push_back(0.0653301357);
		effectChance.push_back(0.036);
		//effectLabel("Death");
		counterClasses.push_back(0);
		counterClassesEffected.push_back(0);
		outFactor.push_back(fora);
		inFactor.push_back(dentro);		
		
		//50-59 years old
		classes.push_back("50-59");
		chanceToBelong.push_back(0.1138703474);
		effectChance.push_back(0.013);
		//effectLabel("Death")
		counterClasses.push_back(0);
		outFactor.push_back(1);
		inFactor.push_back(1);		
		
		//40-49 years old
		classes.push_back("40-49");
		chanceToBelong.push_back(0.1156245979);
		effectChance.push_back(0.004);
		//effectLabel("Death");
		counterClasses.push_back(0);
		counterClassesEffected.push_back(0);
		outFactor.push_back(1);
		inFactor.push_back(1);		
		
		//30-39 years old
		classes.push_back("30-39");
		chanceToBelong.push_back(0.1685008586);
		effectChance.push_back(0.002);
		//effectLabel("Death");
		counterClasses.push_back(0);
		counterClassesEffected.push_back(0);
		outFactor.push_back(1);
		inFactor.push_back(1);
		
				
		//20-29 years old
		classes.push_back("20-29");
		chanceToBelong.push_back(0.1942166);
		effectChance.push_back(0.002);
		//effectLabel("Death");
		counterClasses.push_back(0);
		counterClassesEffected.push_back(0);
		outFactor.push_back(1);
		inFactor.push_back(1);		
				
		//10-19 years old
		classes.push_back("10-19");
		chanceToBelong.push_back(0.1826933733);
		effectChance.push_back(0.002);
		//effectLabel("Death");
		counterClasses.push_back(0);
		counterClassesEffected.push_back(0);
		outFactor.push_back(1);
		inFactor.push_back(1);		


		//0-9
		classes.push_back("0-9");
		chanceToBelong.push_back(0.1138394687);
		effectChance.push_back(0.002);
		//effectLabel("Death");
		counterClasses.push_back(0);	
		counterClassesEffected.push_back(0);	
		outFactor.push_back(1);
		inFactor.push_back(1);		
		
		//defining upperboundChance
		upperboundChance.push_back(chanceToBelong[0]);
		for (int i=1; i<classes.size()-1;i++){
			upperboundChance.push_back(upperboundChance[i-1]+chanceToBelong[i]);
		}
		upperboundChance.push_back(1);
	}
	//sofreu o efeito? COVID-19:morte
	bool effectedByClass(int clss){
		double chance = rand() / static_cast<double>(RAND_MAX);
		bool res = chance <= effectChance[clss];
		if (res){counterClassesEffected[clss]++;}
		return res;
	}
	bool effectedByNodeId(int id){
		int clss = classByNodeId[id];
		return effectedByClass(clss);
	}
	
	double inMultiplierByNodeId(int id){
		return inFactor[classByNodeId[id]];
	}
	
	double outMultiplierByNodeId(int id){
		return inFactor[classByNodeId[id]];
	}
	
	int getRandomClass(){
		double chance = rand() / static_cast<double>(RAND_MAX);
		int i;
		for (i=0; i< classes.size(); i++){
			if (chance <= upperboundChance[i]){
				return i;
			}
		} 
		return i-1;
	}
};

#endif

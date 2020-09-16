#include "varWeights.hh"
#include <math.h>
//#include "ReducedBase_slim.hh"
varWeights::varWeights(const string& label){
	m_label = label;
}

varWeights::~varWeights(){}

void varWeights::setMinMax(double minWeight, double maxWeight){
	m_minWeight = minWeight;
	m_maxWeight = maxWeight;
}

std::vector<double> varWeights::getMinMax(){
	std::vector<double> v = {m_minWeight, m_maxWeight};
	return v;
}
//for one event
float varWeights::lepPTweight(ReducedBase* base, float ptMean, Systematic& sys){
	float ptWeight = 0.;
	float avgPt = 0;
	for(int iLep = 0; iLep < base->Nlep; iLep++){
			avgPt += base->PT_lep->at(iLep);
		}	
		avgPt /= base->Nlep;
	if(sys.IsUp())
		ptWeight = avgPt/ptMean; //normalize by mean
	
	else ptWeight = ptMean/avgPt;

	return ptWeight;
}


//for one event
float varWeights::lepIsoweight(ReducedBase* base, float isoMean, Systematic& sys){
	float isoWeight = 0.;
	float avgIso = 0;
	for(int iLep = 0; iLep < base->Nlep; iLep++){
			avgIso += base->MiniIso_lep->at(iLep);
		}	
		avgIso /= base->Nlep;
	if(sys.IsUp())
		isoWeight = avgIso/isoMean; //normalize by mean
	
	else isoWeight = isoMean/avgIso;

	return isoWeight;
}


float varWeights::expWeight(ReducedBase* base, double mu, vector<double>* var, Systematic& sys){
	double weight = 0.;
	if(base->Nlep == 0){
		weight = 1.;
		return weight;
	}	
	double x = 0.;
	double c = exp(m_minWeight);
	double b = mu/(exp(1) - c);
	double alpha = mu/log(m_maxWeight);
	for(int iLep = 0; iLep < base->Nlep; iLep++){
		x += fabs(var->at(iLep));
		}	
		x /= base->Nlep;
	if(sys.IsUp())
		weight = log(x/b + c); //normalize by mean
	
	else weight = exp(-(x-mu)/alpha);

	return weight;
}

//for one event
float varWeights::basicWeight(ReducedBase* base, double mu, vector<double>* var, Systematic& sys){
	double weight = 0.;
	if(base->Nlep == 0){
		weight = 1.;
		return weight;
	}	
	double avg = 0.;
	//cout << "nLep: " << base->Nlep << endl;
	for(int iLep = 0; iLep < base->Nlep; iLep++){
		avg += fabs(var->at(iLep));
		//cout << "lepton Iso avg: " <<  avg << endl;
		//cout << "lepton Iso: " << var->at(iLep) << endl;
		}	
		avg /= base->Nlep;
	if(avg == 0){
		avg = mu;
	}
	if(sys.IsUp())
		weight = avg/mu; //normalize by mean
	
	else weight = mu/avg;
	if( isinf(weight)){ 
	std::cout << "down weight: " << weight << " avg for event: " << avg << " mean: " << mu << "NLep: " << base->Nlep << std::endl;
	for(int i = 0; i < base->Nlep; i++){
	cout << " iso: " << var->at(i) << endl;
}
}
	return weight/10;
}

#include "varWeights.hh"
//#include "ReducedBase_slim.hh"
varWeights::varWeights(const string& label){
	m_label = label;
}

varWeights::~varWeights(){}

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


//for one event
float varWeights::lepWeight(ReducedBase* base, vector<double> var, float mean, Systematic& sys){
	float weight = 0.;
	float avg = 0;
	for(int iLep = 0; iLep < base->Nlep; iLep++){
			avg += var->at(iLep);
		}	
		avg /= base->Nlep;
	if(sys.IsUp())
		weight = avg/mean; //normalize by mean
	
	else weight = mean/avg;

	return weight;
}
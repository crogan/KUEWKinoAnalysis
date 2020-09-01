#include "varWeights.hh"

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
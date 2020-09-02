#ifndef varWeights_HH
#define varWeights_HH


#include "ReducedBase_slim.hh"
#include "Systematics.hh"
class varWeights{
	public:
	varWeights(const string& label);
	virtual ~varWeights();
	float	lepPTweight(ReducedBase* base, float ptMean, Systematic& sys);
	float lepIsoweight(ReducedBase* base, float isoMean, Systematic& sys);
	float lepWeight(ReducedBase* base, vector<double> var, float mean, Systematic& sys);

	private:
	std::string m_label;
};

#endif

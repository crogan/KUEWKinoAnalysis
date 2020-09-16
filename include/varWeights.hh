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
	float basicWeight(ReducedBase* base, double mean, vector<double>* var, Systematic& sys);
	float expWeight(ReducedBase* base, double mean, vector<double>* var, Systematic& sys);
	void setMinMax(double minWeight, double maxWeight);
	std::vector<double> getMinMax();
	double m_minWeight;
	double m_maxWeight;

	private:
	std::string m_label;
};

#endif

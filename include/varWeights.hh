#ifndef varWeights_HH
#define varWeights_HH


#include "ReducedBase_slim.hh"
#include "Systematics.hh"
class varWeights{
	public:
	varWeights(const string& label);
	virtual ~varWeights();
	float	lepPTweight(ReducedBase* base, float ptMean, Systematic& sys);
	private:
	std::string m_label;
};

#endif

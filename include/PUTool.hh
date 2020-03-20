#ifndef PUTool_h
#define PUTool_h

#include "LumiReweightingStandAlone.h"

using namespace reweight;

class PUTool {
  
public:
  PUTool(){
    for(int i = 0; i < 3; i++){
      m_PU2016[i] = nullptr;
      m_PU2017[i] = nullptr;
      m_PU2018[i] = nullptr;
    }
  }
  
  virtual ~PUTool(){
    for(int i = 0; i < 3; i++){
      if(m_PU2016[i] != nullptr)
	delete m_PU2016[i];
      if(m_PU2017[i] != nullptr)
	delete m_PU2017[i];
      if(m_PU2018[i] != nullptr)
	delete m_PU2018[i];
    }
  }

  void BuildMap(const std::string& pufolder);

  double GetWeight(int NPV, int year, int updown = 0);

private:
  LumiReWeighting* m_PU2016[3];
  LumiReWeighting* m_PU2017[3];
  LumiReWeighting* m_PU2018[3];
};

#endif







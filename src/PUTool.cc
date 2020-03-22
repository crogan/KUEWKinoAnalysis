#include "PUTool.hh"

void PUTool::BuildMap(const std::string& pufolder){
  for(int i = 0; i < 3; i++){
    if(m_PU2016[i] != nullptr)
      delete m_PU2016[i];
    if(m_PU2017[i] != nullptr)
      delete m_PU2017[i];
    if(m_PU2018[i] != nullptr)
      delete m_PU2018[i];
  }

  m_PU2016[0] = (LumiReWeighting*) new LumiReWeighting(pufolder+"/mcPileup2016.root",
						       pufolder+"/PileupData_GoldenJSON_Full2016.root",
						       "pu_mc", "pileup_minus");
  m_PU2016[1] = (LumiReWeighting*) new LumiReWeighting(pufolder+"/mcPileup2016.root",
						       pufolder+"/PileupData_GoldenJSON_Full2016.root",
						       "pu_mc", "pileup");
  m_PU2016[2] = (LumiReWeighting*) new LumiReWeighting(pufolder+"/mcPileup2016.root",
						       pufolder+"/PileupData_GoldenJSON_Full2016.root",
						       "pu_mc", "pileup_plus");
  m_PU2017[0] = (LumiReWeighting*) new LumiReWeighting(pufolder+"/mcPileup2017.root",
						       pufolder+"/PileupData_GoldenJSON_Full2017.root",
						       "pu_mc", "pileup_minus");
  m_PU2017[1] = (LumiReWeighting*) new LumiReWeighting(pufolder+"/mcPileup2017.root",
						       pufolder+"/PileupData_GoldenJSON_Full2017.root",
						       "pu_mc", "pileup");
  m_PU2017[2] = (LumiReWeighting*) new LumiReWeighting(pufolder+"/mcPileup2017.root",
						       pufolder+"/PileupData_GoldenJSON_Full2017.root",
						       "pu_mc", "pileup_plus");
  m_PU2018[0] = (LumiReWeighting*) new LumiReWeighting(pufolder+"/mcPileup2018.root",
						       pufolder+"/PileupData_GoldenJSON_Full2018.root",
						       "pu_mc", "pileup_minus");
  m_PU2018[1] = (LumiReWeighting*) new LumiReWeighting(pufolder+"/mcPileup2018.root",
						       pufolder+"/PileupData_GoldenJSON_Full2018.root",
						       "pu_mc", "pileup");
  m_PU2018[2] = (LumiReWeighting*) new LumiReWeighting(pufolder+"/mcPileup2018.root",
						       pufolder+"/PileupData_GoldenJSON_Full2018.root",
						       "pu_mc", "pileup_plus");
}

double PUTool::GetWeight(int NPV, int year, int updown){
  if(abs(updown) > 1) return 1.;
  
  if(year == 2016)
    if(m_PU2016[updown+1] != nullptr)
      return m_PU2016[updown+1]->weight(NPV);
  if(year == 2017)
    if(m_PU2017[updown+1] != nullptr)
      return m_PU2017[updown+1]->weight(NPV);
  if(year == 2018)
    if(m_PU2018[updown+1] != nullptr)
      return m_PU2018[updown+1]->weight(NPV);
   
  return 1.;
}





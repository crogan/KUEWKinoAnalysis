#ifndef NeventTool_h
#define NeventTool_h

#include <iostream>
#include <string>
#include <map>

class NeventTool {

public:
  NeventTool();
  virtual ~NeventTool();

private:
  static std::map<std::string,double> m_Label2Nevent_BKG;
  static std::map<std::string,double> InitMap_Nevent_BKG();
  static std::map<std::string,double> m_Label2Nweight_BKG;
  static std::map<std::string,double> InitMap_Nweight_BKG();
  
};

#endif




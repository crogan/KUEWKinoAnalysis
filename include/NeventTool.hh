#ifndef NeventTool_h
#define NeventTool_h

#include <iostream>
#include <string>
#include <map>

class NeventTool {

public:
  NeventTool();
  virtual ~NeventTool();

  double GetNevent_BKG(const std::string& dataset) const;
  double GetNevent_SMS(const std::string& dataset, int MP, int MC) const;
  double GetNweight_BKG(const std::string& dataset) const;
  double GetNweight_SMS(const std::string& dataset, int MP, int MC) const;

private:
  static std::map<std::string,double> m_Label2Nevent_BKG;
  static std::map<std::string,double> InitMap_Nevent_BKG();
  static std::map<std::string,double> m_Label2Nweight_BKG;
  static std::map<std::string,double> InitMap_Nweight_BKG();
  static std::map<std::string,std::map<std::pair<int,int>,double> > m_Label2Nevent_SMS;
  static std::map<std::string,std::map<std::pair<int,int>,double> > InitMap_Nevent_SMS();
  static std::map<std::string,std::map<std::pair<int,int>,double> > m_Label2Nweight_SMS;
  static std::map<std::string,std::map<std::pair<int,int>,double> > InitMap_Nweight_SMS();
  
};

#endif




#ifndef XsecTool_h
#define XsecTool_h

#include <iostream>
#include <string>
#include <map>

class XsecTool {

public:
  XsecTool();
  virtual ~XsecTool();

private:
  static std::map<std::string,double> m_Label2Xsec_BKG;
  static std::map<std::string,double> InitMap_Xsec_BKG();
  
};

#endif




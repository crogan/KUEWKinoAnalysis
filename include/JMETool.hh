#ifndef JMETool_h
#define JMETool_h

#include "../include/FormulaBin.hh"

//#include "../src/FormulaBin.cc"

using std::string;

class JMETool {
  
public:
  JMETool(){
    
  }
  
  virtual ~JMETool();

  void BuildMap(const std::string& JMEfolder);

  void ParseJESUncertainty(const string& input, int year, const string& prefix = "");
  void ParseJEC(const string& input, int year, const string& prefix = "");
  
  double GetFactor(int year, const string& name, double pT, double Eta,
		   double A = 0., double rho = 0.) const;

private:
  mutable std::map<string, FormulaBinsBins*> m_Factors[3]; // [year]

  // void ParseCSV(const std::string& csvfile, int iyear);
  // std::string popcomma(std::string& line);

  double popdouble(std::string& line);
  std::string popstring(std::string& line);
};

#endif







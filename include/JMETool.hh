#ifndef JMETool_h
#define JMETool_h

#include "../include/FormulaBin.hh"

using std::string;

class JMETool {
  
public:
  JMETool(){
    
  }
  
  virtual ~JMETool();

  void BuildMap(const std::string& JMEfolder);

  void ParseJESUncertainty(const string& input, int year, const string& prefix = "");
  void ParseJEC(const string& input, int year, const string& prefix = "");
  
  double GetJESFactor(int year, const string& name, double pT, double Eta,
		   double A = 0., double rho = 0.) const;


  // JER based on https://github.com/cms-nanoAOD/nanoAOD-tools/blob/master/python/postprocessing/modules/jme/jetmetUncertainties.py
  // with values taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution 
  // and https://github.com/cms-jet/JRDatabase/tree/master/textFiles
  void BuildJERMap(const std::string& JMEfolder);

  void ParseJERSFUncertainty(const string& input, int year);
  void ParseJERSF(const string& input, int year, const string& prefix = "");
  void ParseJER(const string& input, int year);
  double GetJERFactor(int year, double pT, double Eta, double rho) const;
  double GetJERSFFactor(int year, double Eta, int updown = 0) const;
  int GetPairIndex(int year, double Eta) const;

private:
  mutable std::map<string, FormulaBinsBins*> m_Factors[3]; // [year]
  mutable std::map<int, FormulaBinsBins*> m_JERFactors[3]; // [year]
  mutable std::map<int, std::pair<double,double>> m_JERSFEtaBins[3]; // <etaIndex, <etaMin,etaMax>> [year]
  mutable std::map<int, std::pair<double,double>> m_JERSFFactors[3]; // <etaBin, <SF,unc>> [year]

  // void ParseCSV(const std::string& csvfile, int iyear);
  // std::string popcomma(std::string& line);

  double popdouble(std::string& line);
  std::string popstring(std::string& line);
};

#endif







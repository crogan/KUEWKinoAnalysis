#ifndef FormulaBin_h
#define FormulaBin_h

#include <RooRealVar.h>
#include <RooFormulaVar.h>

using std::vector;

class FormulaBin {

public:
  FormulaBin(double min, double max, const std::string& formula, int Nvar = 1, double const_val = 0.);
  FormulaBin(double min, double max, RooFormulaVar& formula,
  	     const std::vector<RooRealVar*>& X,
	     const std::vector<RooRealVar*>& param,
	     const std::vector<double>& vals);
  FormulaBin(double min, double max, double const_val = 0.);
  
  virtual ~FormulaBin();

  // rho and area for JEC
  double SF(double pT, double A = 0, double rho = 0) const;

  double Min() const { return m_Min; }
  double Max() const { return m_Max; }
  
private:
  bool m_MINE;
  
  double m_Min;
  double m_Max;
  
  RooFormulaVar* m_formula;
  int m_Nvar;
  mutable vector<RooRealVar*> m_X;
  int m_Nparam;
  mutable vector<RooRealVar*> m_param;
  vector<double>              m_val;
  
};


class FormulaBins {

public:
  FormulaBins();
  FormulaBins(RooFormulaVar& formula,
	      const std::vector<RooRealVar*>& X,
	      const std::vector<RooRealVar*>& param);

  virtual ~FormulaBins(){
    for(int i = 0; i < m_Nbins; i++)
      delete m_Bins[i];
  }

  double SF(double pT, double A = 0, double rho = 0) const;

  void AddBin(double min, double max, const std::string& formula, int Nvar = 1, double const_val = 0) const;
  void AddBin(double min, double max, const vector<double>& vals) const;
  void AddBin(double min, double max, double val) const;

private:
  mutable int m_Nbins;
  mutable vector<FormulaBin*> m_Bins;

  RooFormulaVar* m_formula;
  const vector<RooRealVar*>* m_X;
  const vector<RooRealVar*>* m_param;
};


class FormulaBinsBins {

public:
  FormulaBinsBins();
  FormulaBinsBins(const std::string& formula, int Nvar, int Nparam);
  
  virtual ~FormulaBinsBins();

  double SF(double Eta, double pT, double A = 0, double rho = 0) const;

  const FormulaBins& AddBin(double min, double max);
  
private:
  bool m_MINE;
  
  int m_Nbins;
  vector<FormulaBins*> m_Bins;
  vector<double>       m_Min;
  vector<double>       m_Max;

  RooFormulaVar* m_formula;
  int m_Nvar;
  vector<RooRealVar*> m_X;
  int m_Nparam;
  vector<RooRealVar*> m_param;
};

#endif

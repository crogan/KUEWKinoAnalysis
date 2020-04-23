#include "../include/FormulaBin.hh"

#include <RooArgList.h>

FormulaBin::FormulaBin(double min, double max, const std::string& formula, int Nvar, double const_val){
  m_MINE = true;

  m_Min = min;
  m_Max = max;
  m_Nvar = Nvar;
  m_Nparam = 0;
  
  if(m_Nvar < 1){
    m_val.push_back(const_val);
    m_Nparam++;
  } else if(m_Nvar ==  1){
    m_X.push_back(new RooRealVar("x", "x", 0., 1e8));
    m_formula = new RooFormulaVar("formula", formula.c_str(), RooArgList(*m_X[0]));
  } else if(m_Nvar ==  2){
    m_X.push_back(new RooRealVar("x", "x", 0., 1e8));
    m_X.push_back(new RooRealVar("y", "y", 0., 1e8));
    m_formula = new RooFormulaVar("formula", formula.c_str(), RooArgList(*m_X[0],*m_X[1]));
  } else if(m_Nvar ==  3){
    m_X.push_back(new RooRealVar("x", "x", 0., 1e8));
    m_X.push_back(new RooRealVar("y", "y", 0., 1e8));
    m_X.push_back(new RooRealVar("z", "z", 0., 1e8));
    m_formula = new RooFormulaVar("formula", formula.c_str(), RooArgList(*m_X[0],*m_X[1],*m_X[2]));
  } else
    m_formula = nullptr;
}

FormulaBin::FormulaBin(double min, double max, RooFormulaVar& formula,
		       const std::vector<RooRealVar*>& X,
		       const std::vector<RooRealVar*>& param,
		       const std::vector<double>& vals){
  m_MINE = false;
  m_Min = min;
  m_Max = max;

  m_formula = &formula;
  
  m_Nvar = X.size();
  m_Nparam = param.size();
  
  for(int i = 0; i < m_Nvar; i++)
    m_X.push_back(X[i]);

  for(int i = 0; i < m_Nparam; i++){
    m_param.push_back(param[i]);
    m_val.push_back(vals[i]);
  }
}

FormulaBin::FormulaBin(double min, double max, double const_val){
  m_MINE = false;
  m_Min = min;
  m_Max = max;
  m_formula = nullptr;
  m_Nvar = 0;
  m_Nparam = 0;
  m_val.push_back(const_val);
}

FormulaBin::~FormulaBin(){
  if(m_MINE){
    if(m_formula != nullptr)
      delete m_formula;
    for(int i = 0; i < m_Nvar; i++){
      delete m_X[i];
    }
  }
}

  // rho and area for JEC
double FormulaBin::SF(double pT, double A, double rho) const {
  if(m_Nvar < 1 || !m_formula){
    return m_val[0];
  }
  for(int i = 0; i < m_Nparam; i++){
    m_param[i]->setVal(m_val[i]);
  }

  m_X[0]->setVal(pT);
  if(m_Nvar > 1)
    m_X[1]->setVal(A);
  if(m_Nvar > 2)
    m_X[2]->setVal(rho);

  return m_formula->getVal();
}

FormulaBins::FormulaBins(){
  m_Nbins = 0;

  m_formula = nullptr;
  m_X = nullptr;
  m_param = nullptr;
}

FormulaBins::FormulaBins(RooFormulaVar& formula,
			 const std::vector<RooRealVar*>& X,
			 const std::vector<RooRealVar*>& param){
  m_Nbins = 0;

  m_formula = &formula;
  m_X = &X;
  m_param = &param;
}

double FormulaBins::SF(double pT, double A, double rho) const {
  for(int i = 0; i < m_Nbins; i++){
    if(pT < m_Bins[i]->Max()){
      return m_Bins[i]->SF(pT, A, rho);
    }
  }

  return 1;
}

void FormulaBins::AddBin(double min, double max, const std::string& formula, int Nvar, double const_var) const {
  FormulaBin* bin = new FormulaBin(min, max, formula, Nvar, const_var);

  if(m_Nbins == 0){
    m_Bins.push_back(bin);
    m_Nbins++;
    return;
  }

  m_Bins.push_back(bin);
  m_Nbins++;
  
  for(int b = m_Nbins-2; b >= 0; b--){
    if(bin->Min() < m_Bins[b]->Min()){
      m_Bins[b+1] = m_Bins[b];
      m_Bins[b] = bin;
    } else {
      break;
    }
  }

  return;
}

void FormulaBins::AddBin(double min, double max, const vector<double>& vals) const {
  FormulaBin* bin = new FormulaBin(min, max, *m_formula, *m_X, *m_param, vals);
  
  if(m_Nbins == 0){
    m_Bins.push_back(bin);
    m_Nbins++;
    return;
  }

  m_Bins.push_back(bin);
  m_Nbins++;
  
  for(int b = m_Nbins-2; b >= 0; b--){
    if(bin->Min() < m_Bins[b]->Min()){
      m_Bins[b+1] = m_Bins[b];
      m_Bins[b] = bin;
    } else {
      break;
    }
  }

  return;
}

void FormulaBins::AddBin(double min, double max, double val) const {
  FormulaBin* bin = new FormulaBin(min, max, val);

  if(m_Nbins == 0){
    m_Bins.push_back(bin);
    m_Nbins++;
    return;
  }

  m_Bins.push_back(bin);
  m_Nbins++;
  
  for(int b = m_Nbins-2; b >= 0; b--){
    if(bin->Min() < m_Bins[b]->Min()){
      m_Bins[b+1] = m_Bins[b];
      m_Bins[b] = bin;
    } else {
      break;
    }
  }

  return;
}

FormulaBinsBins::FormulaBinsBins(){
  m_Nbins = 0;
  m_MINE = false;
  m_formula = nullptr;
  m_Nvar = 0;
  m_Nparam = 0;
}

FormulaBinsBins::FormulaBinsBins(const std::string& formula, int Nvar, int Nparam){
  m_Nbins = 0;
  m_MINE = true;

  m_Nvar = Nvar;
  m_Nparam = Nparam;

  size_t found;

  RooArgList VAR;
  
  std::string inserts[m_Nparam];
  for(int i = 0; i < m_Nparam; i++){
    inserts[i] = "["+std::to_string(i)+"]";
    m_param.push_back(new RooRealVar(Form("param%d", i),Form("param%d", i), -1e8, 1e8));
    VAR.add(*m_param[i]);
  }

  std::string bin_formula = formula;
  for(int i = 0; i < m_Nparam; i++){
    while(bin_formula.find(inserts[i]) != std::string::npos){
      found = bin_formula.find(inserts[i]);
      bin_formula.replace(found,3,std::string("param")+std::to_string(i));
    }
  }

  if(m_Nvar > 0){
    m_X.push_back(new RooRealVar("x", "x", 0., 1e8));
    VAR.add(*m_X[0]);
  }
  if(m_Nvar > 1){
    m_X.push_back(new RooRealVar("y", "y", 0., 1e8));
    VAR.add(*m_X[1]);
  }
  if(m_Nvar > 2){
    m_X.push_back(new RooRealVar("z", "z", 0., 1e8));
    VAR.add(*m_X[2]);
  }

  m_formula = new RooFormulaVar("formula", bin_formula.c_str(), VAR);
}

FormulaBinsBins::~FormulaBinsBins(){
  for(int i = 0; i < m_Nbins; i++)
    delete m_Bins[i];

  if(m_formula)
    delete m_formula;

  for(int i = 0; i < m_Nvar; i++)
    delete m_X[i];

  for(int i = 0; i < m_Nparam; i++)
    delete m_param[i];
}

double FormulaBinsBins::SF(double Eta, double pT, double A, double rho) const {
  for(int i = 0; i < m_Nbins; i++){
    if(Eta < m_Max[i])
      return m_Bins[i]->SF(pT, A, rho);
  }

  return 1;
}

const FormulaBins& FormulaBinsBins::AddBin(double min, double max){
  FormulaBins* bin;
  if(m_formula)
    bin = new FormulaBins(*m_formula, m_X, m_param);
  else 
    bin = new FormulaBins();

  if(m_Nbins == 0){
    m_Bins.push_back(bin);
    m_Min.push_back(min);
    m_Max.push_back(max);
    m_Nbins++;
    return *bin;
  }

  m_Bins.push_back(bin);
  m_Min.push_back(min);
  m_Max.push_back(max);
  m_Nbins++;

  for(int b = m_Nbins-2; b >= 0; b--){
    if(min < m_Min[b]){
      m_Bins[b+1] = m_Bins[b];
      m_Min[b+1] = m_Min[b];
      m_Max[b+1] = m_Max[b];
      m_Bins[b] = bin;
      m_Min[b] = min;
      m_Max[b] = max;
    } else {
      break;
    }
  }
  
  return *bin;
}

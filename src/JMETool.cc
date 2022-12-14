#include "../include/JMETool.hh"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

using std::string;
using std::cout;
using std::endl;

JMETool::~JMETool(){
  for(int i = 0; i < 3; i++){
    std::map<string, FormulaBinsBins*>::iterator it = m_Factors[i].begin();
    while(it != m_Factors[i].end()){
      delete it->second;
      it++;
    }
  }
}

double JMETool::GetFactor(int year, const string& name, double pT, double Eta, double A, double rho) const {
  if(year < 2016 || year > 2018)
    return 0;
  
  if(m_Factors[year-2016].count(name) == 0)
    return 0;
  
  return m_Factors[year-2016][name]->SF(Eta, pT, A, rho);
}

void JMETool::BuildMap(const std::string& JMEfolder){
  string JME[3]; // [year]
  string JME_FS[3]; // [year];

  JME[0] = "Summer16_07Aug2017_V11_MC";
  JME[1] = "Fall17_17Nov2017_V32_MC";
  JME[2] = "Autumn18_V19_MC";
  
  JME_FS[0] = "Summer16_FastSimV1_MC";
  JME_FS[1] = "Fall17_FastSimV1_MC";
  JME_FS[2] = "Autumn18_FastSimV1_MC";

  string post = "AK4PFchs.txt";

  cout << "Building JME maps for all three years" << endl;
  
  for(int y = 0; y < 3; y++){
    // ParseJESUncertainty(JMEfolder+"/"+JME[y]+"/"+JME[y]+"_Uncertainty_"+post, 2016+y);
    ParseJESUncertainty(JMEfolder+"/"+JME[y]+"/"+JME[y]+"_UncertaintySources_"+post, 2016+y);
    ParseJESUncertainty(JMEfolder+"/"+JME_FS[y]+"/"+JME_FS[y]+"_Uncertainty_"+post, 2016+y, "FS");

    ParseJEC(JMEfolder+"/"+JME[y]+"/"+JME[y]+"_L1FastJet_"+post, 2016+y);
    ParseJEC(JMEfolder+"/"+JME_FS[y]+"/"+JME_FS[y]+"_L1FastJet_"+post, 2016+y, "FS");
  
    ParseJEC(JMEfolder+"/"+JME[y]+"/"+JME[y]+"_L2Relative_"+post, 2016+y);
    ParseJEC(JMEfolder+"/"+JME_FS[y]+"/"+JME_FS[y]+"_L2Relative_"+post, 2016+y, "FS");
    //cout << "...Done" << endl;
  }
}

void JMETool::ParseJESUncertainty(const string& input, int year, const string& prefix){
  if(year < 2016 || year > 2018)
    return;
  
  std::ifstream ifile(input.c_str());
   if(!ifile.is_open()){
    std::cout << "can't open JME file " << input << std::endl;
    return;
   }
   
   string line;
   size_t found;

   string label = "JESUncer_Total";
   if(prefix != "")
     label = prefix+"_"+label;
   bool start = false;
   FormulaBinsBins* Bins = nullptr;

   double ptmin, ptmax, factor;
   while(getline(ifile,line)){
     // discard comment line
     if(line.find("#") != string::npos)
       continue;

     // new name, new label
     if(line.find("[") != string::npos){
       found = line.find("[");
       line.erase(0,found+1);
       found = line.find("]");
       label = "JESUncer_"+line.substr(0,found);
       if(prefix != "")
	 label = prefix+"_"+label;
       
       continue;
     }
       
     // start new bin map
     if(line.find("{") != string::npos){
       Bins = new FormulaBinsBins();
       m_Factors[year-2016][label] = Bins;
       continue;
     }

     // new eta bin for existing bin map
     double etamin = popdouble(line);
     double etamax = popdouble(line);
     
     const FormulaBins& Bin = Bins->AddBin(etamin, etamax);

     int N = popdouble(line);

     ptmin  = popdouble(line);
     while(line.length() > 0){
       factor = popdouble(line);
       popdouble(line); // factor repeated for some reason
       if(line.length() > 0)
	 ptmax = popdouble(line);
       else
	 ptmax = ptmin + 1000000;

       Bin.AddBin(ptmin, ptmax, factor);
       
       ptmin = ptmax;
     }
   }

   ifile.close();
}

void JMETool::ParseJEC(const string& input, int year, const string& prefix){
  if(year < 2016 || year > 2018)
    return;
  
  std::ifstream ifile(input.c_str());
   if(!ifile.is_open()){
    std::cout << "can't open JME file " << input << std::endl;
    return;
   }
   
   string line;
   size_t found;

   // first line with variable and equation info
   getline(ifile,line);
   
   // remove bracket
   found = line.find("{");
   line.erase(0,found+1);

   // get how many bins specified per line
   int Nbinvar = popdouble(line);

   for(int i = 0; i < Nbinvar; i++)
     popstring(line);

   // get the number of variables in the formula
   int Nvar = popdouble(line);

   std::vector<std::string> svar;
   for(int i = 0; i < Nvar; i++)
     svar.push_back(popstring(line));
   
   string formula = popstring(line);
  
   if(Nvar == 3){
     std::vector<std::string> var;
     std::vector<std::string> sym;
     std::vector<std::string> rep;
     var.push_back("JetPt");
     var.push_back("JetA");
     var.push_back("Rho");
     sym.push_back("x");
     sym.push_back("y");
     sym.push_back("z");
     rep.push_back("q");
     rep.push_back("v");
     rep.push_back("f");

     if(formula.find("max") != string::npos)
       formula.replace(formula.find("max"), 3, "%%%");
     
     for(int i = 0; i < 3; i++){
       while(formula.find(sym[i]) != string::npos){
	 found = formula.find(sym[i]);
	 formula.replace(found, 1, rep[i]);
       }
     }

     for(int v = 0; v < 3; v++){
       for(int s = 0; s < 3; s++){
	 if(svar[s].find(var[v]) != string::npos){
	   while(formula.find(rep[s]) != string::npos){
	     found = formula.find(rep[s]);
	     formula.replace(found, 1, sym[v]);
	   }
	 }
       }
     }
   }

   if(formula.find("%%%") != string::npos)
       formula.replace(formula.find("%%%"), 3, "max");
   
   popstring(line);

   string label = popstring(line);
   found = label.find("}");
   label.erase(found,found+1);
   if(prefix != "")
     label = prefix+"_"+label;

   // get the first line to get parameter numbers
   double etamin, etamax;
   double emi, ema;
   double ptmin, ptmax;
   getline(ifile,line);
   etamin = popdouble(line);
   etamax = popdouble(line);
   if(Nbinvar == 2){
     ptmin = popdouble(line);
     ptmax = popdouble(line);
   } else {
     ptmin = 0.;
     ptmax = 1e10;
   }

   int Nparam = popdouble(line);
   
   for(int i = 0; i < Nvar*2; i++)
     popdouble(line);
   Nparam -= Nvar*2;

   std::vector<double> params;
   for(int i = 0; i < Nparam; i++)
     params.push_back(popdouble(line));
   
   FormulaBinsBins* Bins = new FormulaBinsBins(formula, Nvar, Nparam);
   m_Factors[year-2016][label] = Bins;

   const FormulaBins* Bin = &Bins->AddBin(etamin, etamax);
   Bin->AddBin(ptmin, ptmax, params);

   while(getline(ifile,line)){
     emi = popdouble(line);
     ema = popdouble(line);
     
     // new eta bin
     if(fabs(emi-etamin) + fabs(ema-etamax) > 1e-8){
       etamin = emi;
       etamax = ema;
       Bin = &Bins->AddBin(etamin, etamax);
     }
     
     if(Nbinvar == 2){
       ptmin = popdouble(line);
       ptmax = popdouble(line);
     } else {
       ptmin = 0.;
       ptmax = 1e10;
     }
     
     int N = popdouble(line);
     for(int i = 0; i < Nvar*2; i++)
       popdouble(line);
     N -= Nvar*2;

     params.clear();
     for(int i = 0; i < N; i++)
       params.push_back(popdouble(line));
     
     Bin->AddBin(ptmin, ptmax, params);
   }

   ifile.close();
}

double JMETool::popdouble(std::string& line){
  // remove leading whitespace
  while(line[0] == string(" ")[0])
    line.erase(0,1);
  
  string num;
  if(line.find(" ") != string::npos){
    size_t p = line.find(" ");
    num = line.substr(0,p);
    line.erase(0,p+1);
    while(line[0] == string(" ")[0])
      line.erase(0,1);
  } else
    num = line;

  return stod(num);
}

std::string JMETool::popstring(std::string& line){
  // remove leading whitespace
  while(line[0] == string(" ")[0])
    line.erase(0,1);
  
  string ret;
  if(line.find(" ") != string::npos){
    size_t p = line.find(" ");
    ret = line.substr(0,p);
    line.erase(0,p+1);
    while(line[0] == string(" ")[0])
      line.erase(0,1);
  } else
    ret = line;

  return ret;
}


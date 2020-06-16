#include "../include/BtagSFTool.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include <TFile.h>

using std::string;

double BtagSFTool::EFF(double pT, int year, int flavor){
  if(flavor < 0 || flavor > 2)
    return 0.;
  
  if(year == 2016)
    if(m_BtagEff2016[flavor] != nullptr)
      return m_BtagEff2016[flavor]->GetEfficiency(m_BtagEff2016[flavor]->FindFixBin(pT));
  if(year == 2017)
    if(m_BtagEff2017[flavor] != nullptr)
      return m_BtagEff2017[flavor]->GetEfficiency(m_BtagEff2016[flavor]->FindFixBin(pT));
  if(year == 2018)
    if(m_BtagEff2018[flavor] != nullptr)
      return m_BtagEff2018[flavor]->GetEfficiency(m_BtagEff2016[flavor]->FindFixBin(pT));

  return 0.;
}

double BtagSFTool::SF(double pT, int year, int flavor, int updown){
  if(abs(updown) > 1)
    return 1.;
  if(flavor < 0 || flavor > 2)
    return 1;

  if(year == 2016)
    if(m_SFs[0][flavor][updown+1] != nullptr)
      return m_SFs[0][flavor][updown+1]->SF(pT);
  if(year == 2017)
    if(m_SFs[1][flavor][updown+1] != nullptr)
      return m_SFs[1][flavor][updown+1]->SF(pT);
  if(year == 2018)
    if(m_SFs[2][flavor][updown+1] != nullptr)
      return m_SFs[2][flavor][updown+1]->SF(pT);

  return 1;
}

void BtagSFTool::BuildMap(const std::string& btagSFfolder){
  SetEfficiencies(btagSFfolder+"/BtagEff.root");
  SetSFs(btagSFfolder+"/DeepJet_2016.csv", 2016);
  SetSFs(btagSFfolder+"/DeepJet_2017.csv", 2017);
  SetSFs(btagSFfolder+"/DeepJet_2018.csv", 2018);
}

void BtagSFTool::SetEfficiencies(const std::string& rootfile){
  for(int i = 0; i < 3; i++){
    if(m_BtagEff2016[i] != nullptr)
      delete m_BtagEff2016[i];
    if(m_BtagEff2017[i] != nullptr)
      delete m_BtagEff2017[i];
    if(m_BtagEff2018[i] != nullptr)
      delete m_BtagEff2018[i];
  }

  TFile* input = new TFile(rootfile.c_str(),"READ");
  if(!input->IsOpen())
    return;

  for(int i = 0; i < 3; i++){
    m_BtagEff2016[i] = (TEfficiency*)((TEfficiency*)input->Get(Form("BtagEff_2016_flavor%d",i)))->Clone("BEff_2016_flavor%d");
    m_BtagEff2017[i] = (TEfficiency*)((TEfficiency*)input->Get(Form("BtagEff_2017_flavor%d",i)))->Clone("BEff_2017_flavor%d");
    m_BtagEff2018[i] = (TEfficiency*)((TEfficiency*)input->Get(Form("BtagEff_2018_flavor%d",i)))->Clone("BEff_2018_flavor%d");
  }
  
  input->Close();
}

void BtagSFTool::SetSFs(const std::string& csvfile, int year){
  if(year == 2016)
    ParseCSV(csvfile, 0);
  if(year == 2017)
    ParseCSV(csvfile, 1);
  if(year == 2018)
    ParseCSV(csvfile, 2);
}

void BtagSFTool::ParseCSV(const std::string& csvfile, int iyear){
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if(m_SFs[iyear][i][j] != nullptr)
	delete m_SFs[iyear][i][j];
      m_SFs[iyear][i][j] = new FormulaBins();
    }
  }

  std::ifstream ifile(csvfile.c_str());
  if(!ifile.is_open()){
    std::cout << "can't open csv file " << csvfile << std::endl;
    return;
  }

  string line, name;
  size_t found, end;

  //discard first line
  getline(ifile,line);
  while(getline(ifile,line)){
    //remove whitespace
    //line.erase(remove(line.begin(), line.end(), ' '), line.end());
    line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

    // first digit is WP - keeoing only medium (1) working point for now
    if(stoi(line.substr(0,1)) != 1)
      continue;

    // pop WP
    popcomma(line);

    // measurement type
    name = popcomma(line);
    if(name.find("comb") == string::npos &&
       name.find("incl") == string::npos)
      continue;

    // sys type
    int isys = -2;
    name = popcomma(line);
    if(name == "down")
      isys = -1;
    if(name == "central")
      isys = 0;
    if(name == "up")
      isys = 1;
    if(isys < -1)
      continue;

    // jet flavor
    int iflavor = -1;
    name = popcomma(line);
    if(name == "0")
      iflavor = 0;
    if(name == "1")
      iflavor = 1;
    if(name == "2")
      iflavor = 2;
    if(iflavor < 0)
      continue;

    // eta min/max
    popcomma(line);
    popcomma(line);

    // pT min
    name = popcomma(line);
    int pt_min = stoi(name);

    // pT max
    name = popcomma(line);
    int pt_max = stoi(name);

     // disc min/max
    popcomma(line);
    popcomma(line);

    // formula remains
    found = line.find("\"");
    line.erase(0,found+1);
    found = line.find("\"");
    name = line.substr(0,found);

    //cout << iyear << " " << iflavor << " " << isys << " " << name << endl;
    
    m_SFs[iyear][iflavor][isys+1]->AddBin(pt_min, pt_max, name);
  }
  
  ifile.close();
}

std::string BtagSFTool::popcomma(std::string& line){
  if(line.find(",") == string::npos)
    return "";
  
  size_t p = line.find(",");
  string ret = line.substr(0,p);
  line.erase(0,p+1);

  return ret;
}

// double BtagSFTool::GetWeight(int Njet, double* PT_jet, double* flavor_jet, bool* Btag_jet, int year, int updown){
//   if(abs(updown) > 1) return 1.;
  
//   // if(year == 2016)
//   //   if(m_PU2016[updown+1] != nullptr)
//   //     return m_PU2016[updown+1]->weight(NPV);
//   // if(year == 2017)
//   //   if(m_PU2017[updown+1] != nullptr)
//   //     return m_PU2017[updown+1]->weight(NPV);
//   // if(year == 2018)
//   //   if(m_PU2018[updown+1] != nullptr)
//   //     return m_PU2018[updown+1]->weight(NPV);

//   return 1.;
// }





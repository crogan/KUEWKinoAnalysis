#include "../include/METTriggerTool.hh"

METTriggerTool::METTriggerTool(){
  m_filename = "Parameters.csv";
}
    
METTriggerTool::~METTriggerTool(){

}

void METTriggerTool::BuildMap(const string& csv){
  SetCSV(csv);
  ParseCSV();
}

double METTriggerTool::Get_EFF(double MET, double HT, int year,
			       bool el, bool mu,
			       bool data, int updown){
  string name = Get_Name(HT, year, el, mu, data);
  return Get_EFF(name, MET, updown);
}

double METTriggerTool::Get_SF(double MET, double HT, int year,
			       bool el, bool mu,
			       bool data, int updown){
  string data_name, bkg_name;

  double SF;
  
  if(!el && !mu){
    data_name = Get_Name(HT, year, true, mu, true); 
    bkg_name = Get_Name(HT, year, true, mu, false);
    SF = Get_EFF(data_name, MET)/Get_EFF(bkg_name, MET);
    data_name = Get_Name(HT, year, el, mu, true);
    bkg_name = Get_Name(HT, year, el, mu, false);
  } else {
    data_name = Get_Name(HT, year, el, mu, true); 
    bkg_name = Get_Name(HT, year, el, mu, false);
    SF = Get_EFF(data_name, MET)/Get_EFF(bkg_name, MET);
  }
  
  if(updown == 0)
    return SF; 
  else if(updown > 0)
    return SF*(m_map[bkg_name][4] + (MET < m_map[bkg_name][2] ? m_map[bkg_name][0]*(MET-m_map[bkg_name][2])*(MET-m_map[bkg_name][2]) : 0.)); 
  else
    return SF*(m_map[bkg_name][5] + (MET < m_map[bkg_name][3] ? m_map[bkg_name][1]*(MET-m_map[bkg_name][3])*(MET-m_map[bkg_name][3]) : 0.));

  return 1.;  
}

void METTriggerTool::SetCSV(const string& csv){
  m_filename = csv;
}

void METTriggerTool::ParseCSV(){
  std::ifstream ifile(m_filename);
  if(!ifile.is_open()){
    std::cout << "can't open csv file " << std::endl;
    return;
  }
  std::string line;
  //discard first line
  getline(ifile,line);
  while(getline(ifile,line)){
    size_t comma = line.find(",");
    std::string name = "";
    std::vector<double> values;
    name = line.substr(0,comma);
    line.erase(0,(name+",").length());
    while(line.length() > 0) { 
      values.push_back(Get_Value(line)); 
    }
    m_map.insert(std::make_pair(name,values));
  }
}

string METTriggerTool::Get_Name(double HT, int year, bool el, bool mu, bool data){
  string name = "";
  
  if(HT <= 600.)
    name+="HT-Le600--"; 
  else if(HT > 600. && HT < 750.)
    name+="HT-G600--HT-L750--"; 
  else if(HT >= 750.)
    name+="HT-Ge750--";
  
  if(el)
    name+="SingleElectrontrigger-E1--Nele-E1_"; 
  else if(mu)
    name+="SingleMuontrigger-E1--Nmu-E1_"; 
  else
    name+="Nlep-E0_";
  
  if(data){
    if(el)
      name+="SingleElectron_"+std::to_string(year)+"_Electron"; 
    else if(mu)
      name+="SingleMuon_"+std::to_string(year)+"_Muon"; 
    else {
      name+="SingleElectron_"+std::to_string(year)+"_ZeroLepton";
    }
  } else {
    name+="Bkg_"+std::to_string(year)+"_";
    if(el)
      name+="Electron"; 
    else if(mu)
      name+="Muon"; 
    else
      name+="ZeroLepton"; 
  }

  return name;
}

int METTriggerTool::Get_Curve_Index(double HT, int year, bool el, bool mu, bool data){
  int index = 0;

  if(HT <= 600.)
    index += 1;
  else if(HT > 600. && HT < 750.)
    index += 2;
  else if(HT >= 750.)
    index += 3;
  
  if(el)
    index += 10;
  else if(mu)
    index += 20;
  else
    index += 30;
  
  if(data){
    if(el)
      index += 100;
    else if(mu)
      index += 200;
    else
      index += 300;
  } else {
    if(el)
      index += 400;
    else if(mu)
      index += 500;
    else
      index += 600;
  }

  return index;
}

double METTriggerTool::Get_EFF(string name, double MET, int updown){

  double EFF = 0.;
  if(m_map[name][9] == 0 && m_map[name][10] == 0){ 
    EFF = m_map[name][6]*ROOT::Math::normal_cdf(MET, m_map[name][8], m_map[name][7]); 
  } else {
    EFF = m_map[name][6]*(TMath::Cos(m_map[name][10])*TMath::Cos(m_map[name][10])*ROOT::Math::normal_cdf(MET, m_map[name][8], m_map[name][7])+TMath::Sin(m_map[name][10])*TMath::Sin(m_map[name][10])*ROOT::Math::normal_cdf(MET, m_map[name][8]*m_map[name][9], m_map[name][7]));
  }
  
  if(updown == 0)
    return EFF;
  else if(updown > 0)
    return std::min(1., EFF*(m_map[name][4] + (MET < m_map[name][2] ? m_map[name][0]*(MET-m_map[name][2])*(MET-m_map[name][2]) : 0.))); 
  else
    return std::min(1., EFF*(m_map[name][5] + (MET < m_map[name][3] ? m_map[name][1]*(MET-m_map[name][3])*(MET-m_map[name][3]) : 0.))); 

  return 1.;
}

double METTriggerTool::Get_Value(std::string& line){
  size_t comma_pos = line.find(",");
  std::string value = line.substr(0,comma_pos);
  line.erase(0,comma_pos+1);
  
  return std::stod(value);
}

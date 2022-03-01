#include "BRILTool.hh"

BRILTool::BRILTool(){
  m_filename = "brilcalc_2017.txt";
}
    
BRILTool::~BRILTool(){

}

void BRILTool::BuildMap(const string& csv){
  SetCSV(csv);
  ParseCSV();
}


void BRILTool::SetCSV(const string& csv){
  m_filename = csv;
}

void BRILTool::ParseCSV(){
  std::ifstream ifile(m_filename);
  if(!ifile.is_open()){
    std::cout << "can't open bril file " << std::endl;
    return;
  }
  std::string line;
  //discard first four lines
  getline(ifile,line);
  getline(ifile,line);
  getline(ifile,line);
  getline(ifile,line);
  int temp_run, temp_fill, temp_DeliveredLumiSection, temp_RecordedLumiSection;
  double temp_RecordedLumi, temp_Pileup;
  getline(ifile,line);
  line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
  ParseLine(line, temp_run, temp_fill, temp_DeliveredLumiSection, temp_RecordedLumiSection, temp_RecordedLumi, temp_Pileup);
  if(temp_fill == -1) std::cout << "ERROR: CHECK LUMI SECTIONS IN FIRST LINE OF INPUT FILE!" << std::endl;
  int current_fill = temp_fill;
  vector<int> current_runs;
  current_runs.push_back(temp_run);
  vector<int> current_LumiSections;
  current_LumiSections.push_back(temp_RecordedLumiSection);
  vector<double> current_RecordedLumis;
  current_RecordedLumis.push_back(temp_RecordedLumi);
  vector<double> current_Pileups;
  current_Pileups.push_back(temp_Pileup);
  while(getline(ifile,line)){
    if(line[0] == '+') break;
    line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
    ParseLine(line, temp_run, temp_fill, temp_DeliveredLumiSection, temp_RecordedLumiSection, temp_RecordedLumi, temp_Pileup);
    if(temp_fill == -1)
    {
     line.clear();
     continue;
    }
    if(current_fill != temp_fill) // new fill info
    {
     FillInfo temp_FillInfo(current_fill, current_runs, current_LumiSections, current_RecordedLumis, current_Pileups);
     m_mapFillInfo.insert(std::make_pair(current_fill,temp_FillInfo));
     current_fill = temp_fill;
     current_runs.clear();
     current_runs.push_back(temp_run);
     current_LumiSections.clear();
     current_LumiSections.push_back(temp_RecordedLumiSection);
     current_RecordedLumis.clear();
     current_RecordedLumis.push_back(temp_RecordedLumi);
     current_Pileups.clear();
     current_Pileups.push_back(temp_Pileup);
    }
    else
    {
     current_runs.push_back(temp_run);
     current_LumiSections.push_back(temp_RecordedLumiSection);
     current_RecordedLumis.push_back(temp_RecordedLumi);
     current_Pileups.push_back(temp_Pileup);
    }
  }
}

// probably want to return FillInfo Class at some point but void placeholder for now
void BRILTool::ParseLine(std::string& line, int& run, int& fill, int& DeliveredLumiSection, int& RecordedLumiSection, double& RecordedLumi, double& Pileup){
  line.erase(0,1); // Remove first |
  
  size_t colon_pos = line.find(":");
  std::string str_run = line.substr(0,colon_pos);
  line.erase(0,colon_pos+1);
  run = std::stod(str_run);
  size_t vert_pos = line.find("|");
  std::string str_fill = line.substr(0,vert_pos);
  line.erase(0,vert_pos+1);
  fill = std::stod(str_fill);
 
  colon_pos = line.find(":");
  std::string str_DeliveredLumiSection = line.substr(0,colon_pos);
  line.erase(0,colon_pos+1);
  DeliveredLumiSection = std::stod(str_DeliveredLumiSection);
  vert_pos = line.find("|");
  std::string str_RecordedLumiSection = line.substr(0,vert_pos);
  line.erase(0,vert_pos+1);
  RecordedLumiSection = std::stod(str_RecordedLumiSection);
  //only want to save recorded lumi sections 
  if(DeliveredLumiSection != RecordedLumiSection) { fill = -1; }

  //remove time info
  vert_pos = line.find("|");
  line.erase(0,vert_pos+1);

  //remove beam info
  vert_pos = line.find("|");
  line.erase(0,vert_pos+1);
  vert_pos = line.find("|");
  line.erase(0,vert_pos+1);

  //remove delivered lumi info
  vert_pos = line.find("|");
  line.erase(0,vert_pos+1);

  //get recorded lumi info
  vert_pos = line.find("|");
  std::string str_RecordedLumi = line.substr(0,vert_pos);
  line.erase(0,vert_pos+1);
  RecordedLumi = std::stod(str_RecordedLumi);

  //get recorded pu info
  vert_pos = line.find("|");
  std::string str_Pileup = line.substr(0,vert_pos);
  line.erase(0,vert_pos+1);
  Pileup = std::stod(str_Pileup);
  line.clear();
  
}

void BRILTool::AddEventToFill(int fill){
  m_mapFillInfo[fill].AddEvent();
}

int BRILTool::GetTotalRuns(int fill){
  return m_mapFillInfo[fill].GetTotalRuns();
}

int BRILTool::GetTotalLumiSections(int fill){
  return m_mapFillInfo[fill].GetTotalLumiSections();
}

double BRILTool::GetIntegratedLumi(int fill){
  return m_mapFillInfo[fill].GetIntegratedLumi();
}

double BRILTool::GetAveragePileup(int fill){
  return m_mapFillInfo[fill].GetAveragePileup();
}

int BRILTool::GetFillFromRun(int run){
  for(std::map<int,FillInfo>::iterator iter = m_mapFillInfo.begin(); iter != m_mapFillInfo.end(); ++iter)
  {
   int fill = iter->first;
   FillInfo temp_FillInfo = iter->second;
   if(temp_FillInfo.IsRunInFill(run))
     return fill;
  }
  return 0;
}

int BRILTool::GetTotalFills(){
  return m_mapFillInfo.size();
}

int BRILTool::GetEventsInFill(int fill){
  return m_mapFillInfo[fill].GetNEvents();
}

bool BRILTool::IsFillInJSON(int fill){
  if(m_mapFillInfo.count(fill)) return true;
  else return false;
}



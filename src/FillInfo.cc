#include "../include/FillInfo.hh"

FillInfo::FillInfo(){  
}

FillInfo::FillInfo(int fill, vector<int> runs, vector<int> LumiSections, vector<double> RecordedLumis, vector<double> Pileups){
 m_fill = fill;
 m_runs = runs;
 m_LumiSections = LumiSections;
 m_RecordedLumis = RecordedLumis;
 m_Pileups = Pileups;
}
    
FillInfo::~FillInfo(){
}

void FillInfo::SetFillNumber(int fill){
 m_fill = fill;
}

void FillInfo::SetRuns(vector<int> runs){
 m_runs = runs;
}

void FillInfo::SetLumiSections(vector<int> LumiSections){
 m_LumiSections = LumiSections;
}

void FillInfo::SetRecordedLumis(vector<double> RecordedLumis){
 m_RecordedLumis = RecordedLumis;
}

void FillInfo::SetPileups(vector<double> Pileups){
 m_Pileups = Pileups;
}

void FillInfo::AddEvent(){
 m_events++;
}

int FillInfo::GetFillNumber(){
 return m_fill;
}

vector<int> FillInfo::GetRuns(){
 return m_runs;
}

vector<int> FillInfo::GetLumiSections(){
 return m_LumiSections;
}

vector<double> FillInfo::GetRecordedLumis(){
 return m_RecordedLumis;
}

vector<double> FillInfo::GetPileups(){
 return m_Pileups;
}

int FillInfo::GetNEvents(){
 return m_events;
}

int FillInfo::GetTotalRuns(){
 return int(m_runs.size());
}

int FillInfo::GetTotalLumiSections(){
 return int(m_LumiSections.size());
}

double FillInfo::GetIntegratedLumi(){
 return std::accumulate(m_RecordedLumis.begin(), m_RecordedLumis.end(), 0.);
}

double FillInfo::GetAveragePileup(){
 return std::accumulate(m_Pileups.begin(), m_Pileups.end(), 0)/m_Pileups.size();
}

bool FillInfo::IsRunInFill(int run){
 for(int i = 0; i < int(m_runs.size()); i++)
 {
  if(m_runs[i] == run) return true;
 }
 return false;
}




#ifndef FillInfo_h
#define FillInfo_h

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <TMath.h>

using std::string;
using std::vector;

class FillInfo {
  
public:
  FillInfo();
  FillInfo(int fill, vector<int> runs, vector<int> LumiSections, vector<double> RecordedLumis, vector<double> Pileup);
    
  virtual ~FillInfo();
  virtual void SetFillNumber(int fill);
  virtual void SetRuns(vector<int> runs);
  virtual void SetLumiSections(vector<int> LumiSections);
  virtual void SetRecordedLumis(vector<double> RecordedLumis);
  virtual void SetPileups(vector<double> Pileups);

  virtual void AddEvent();
  virtual int GetFillNumber();
  virtual vector<int> GetRuns();
  virtual vector<int> GetLumiSections();
  virtual vector<double> GetRecordedLumis();
  virtual vector<double> GetPileups();
  virtual int GetNEvents();

  virtual int GetTotalRuns();
  virtual int GetTotalLumiSections();
  virtual double GetIntegratedLumi();
  virtual double GetAveragePileup();
  virtual bool IsRunInFill(int run);

private:
  
  int m_fill;
  vector<int> m_runs;
  vector<int> m_LumiSections;
  vector<double> m_RecordedLumis;
  vector<double> m_Pileups;

  int m_events = 0;
  
};

#endif







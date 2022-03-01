#ifndef BRILTool_h
#define BRILTool_h

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <TMath.h>
#include "FillInfo.hh"

using std::string;

class BRILTool {
  
public:
  BRILTool();
    
  virtual ~BRILTool();

  virtual void BuildMap(const string& csv);

  virtual void AddEventToFill(int fill);

  virtual int GetTotalRuns(int fill);
  virtual int GetTotalLumiSections(int fill);
  virtual double GetIntegratedLumi(int fill);
  virtual double GetAveragePileup(int fill);
  virtual int GetFillFromRun(int run); 
  virtual int GetTotalFills();
  virtual int GetEventsInFill(int fill);

  virtual bool IsFillInJSON(int fill);


private:

  string m_filename;
  std::map<int,FillInfo> m_mapFillInfo;
  
  virtual void SetCSV(const string& csv);
  virtual void ParseCSV();
  virtual void ParseLine(std::string& line, int& run, int& fill, int& DeliveredLumiSection, int& RecordedLumiSection, double& RecordedLumi, double& Pileup);

  
  
};

#endif







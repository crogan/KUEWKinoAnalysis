#include <iostream>
#include <fstream>
#include <string>

#include "../include/JSONTool.hh"
#include "../src/BtagSFTool.cc"
#include "../src/JMETool.cc"

using namespace std;

void JSON_parse(string file){

  // BtagSFTool* SF = new BtagSFTool();

  // SF->SetSFs(file, 2016);
  // cout << SF->SF(20, 2016, 0, 0) << endl;

  JMETool* JME = new JMETool();

  JME->BuildMap(file);

  cout << JME->GetFactor(2016, "L2Relative", 100., 0.2, 0.5, 15.) << endl;
  cout << JME->GetFactor(2016, "L2Relative", 100., 1., 0.5, 15.) << endl;
  cout << JME->GetFactor(2016, "L2Relative", 100., 3.2,  0.5, 15.) << endl;
  cout << JME->GetFactor(2017, "L2Relative", 100., 3.2,  0.5, 10.) << endl;
  cout << JME->GetFactor(2018, "L2Relative", 100., 3.2,  0.5, 5.) << endl;
  cout << JME->GetFactor(2016, "L2Relative", 100., 3.2,  0.4, 15.) << endl;
  cout << JME->GetFactor(2017, "L2Relative", 100., 3.2,  0.3, 15.) << endl;

  cout << endl << JME->GetFactor(2017, "JESUncer_Total", 100., 3.2) << endl;
  cout << endl << JME->GetFactor(2017, "JESUncer_Total", 80., 1.0) << endl;
  
  // SF->SetSFs(file, 2016);
  // cout << SF->SF(20, 2016, 0, 0) << endl;
  
  // JSONTool JSON;
  // JSON.BuildMap(file);

  // cout << "283934 875 1245 " << JSON.IsGood(283934, 876) << endl;
  // cout << "283934 874 " << JSON.IsGood(283934, 875) << endl;
  // cout << "283934 1246 " << JSON.IsGood(283934, 1245) << endl;
  
  /*
  ifstream ifile(file.c_str());
  string line;
  if(!ifile.is_open())
    return;

  size_t found, end;
  
  while(getline(ifile,line)){
    //  {getline(ifile,line);
    
    // remove any leading or trailing brackets
    while(line.find("{") != string::npos){
      found = line.find("{");
      line.erase(found,found+1);
    }
    while(line.find("}") != string::npos){
      found = line.find("}");
      line.erase(found,found+1);
    }

    //remove whitespace
    line.erase(remove(line.begin(), line.end(), ' '), line.end());

    while(line.find("\"") != string::npos){
      // parse run number
      found = line.find("\"");
      line.erase(0,found+1);
      found = line.find("\"");
      string srun = line.substr(0,found);
      int run = stoi(srun);
      line.erase(0,found+1);

      // parse lumi sections
      found  = line.find("[[");
      end = line.find("]]");
      string slumis = line.substr(found+1,end-1);
      line.erase(0,end+2);

      while(slumis.find("[") != string::npos){
	found = slumis.find("[");
	end   = slumis.find("]");
	string slumi = slumis.substr(found,end);
	slumis.erase(0, end+1);

	int bg, en;
	sscanf(slumi.c_str(),"[%d,%d]", &bg, &en);
	
	cout << run << " " << bg << " " << en << endl;
      }
    }
  }
  ifile.close();
  */
}

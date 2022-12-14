#include <string>
#include <map>
#include <set>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
// #include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
// #include "CombineHarvester/CombineTools/interface/Observation.h"
// #include "CombineHarvester/CombineTools/interface/Process.h"
// #include "CombineHarvester/CombineTools/interface/Utilities.h"
// #include "CombineHarvester/CombineTools/interface/Systematics.h"
// #include "CombineHarvester/CombineTools/interface/BinByBin.h"

#include "TSystem.h"

#include "FitConfiguration.hh"

using namespace std;

int main(int argc, char* argv[]) {
  string InputFile = "test/FitInput_test.root";
  string OutputFold = "BuildFit_output";

  bool addSig = false; // all signals
  bool addBkg = false; // all backgrounds
  VS   proc_to_add;
  VS   proc_to_rem;

  bool addChan = false; // all channels
  VS   chan_to_add;
  VS   chan_to_rem;

  bool addCat = false; // all categories
  VS   cat_to_add;
  VS   cat_to_rem;

  bool addSys = false; // all available shape systematics
  VS   sys_to_add;
  VS   sys_to_rem;

  bool bprint  = false;
  bool verbose = false;
  int  year    = 2017;

  double xsec_norm = -999.0;

  bool workspace = false;

  bool doMCstats = false;

  bool doSepChan = false;

  bool batch = false;
  bool connect = false;
    
  for(int i = 0; i < argc; i++){
    if(strncmp(argv[i],"--workspace", 11) == 0){
      workspace = true;
    }
    if(strncmp(argv[i],"-w", 2) == 0){
      workspace = true;
    }
    if(strncmp(argv[i],"--help", 6) == 0){
      bprint = true;
    }
    if(strncmp(argv[i],"-h", 2) == 0){
      bprint = true;
    }
    if(strncmp(argv[i],"--verbose", 9) == 0){
      verbose = true;
    }
    if(strncmp(argv[i],"-v", 2) == 0){
      verbose = true;
    }
    if(strncmp(argv[i],"-o", 2) == 0){
      i++;
      OutputFold = string(argv[i]);
    }
    if(strncmp(argv[i],"--output", 8) == 0){
      i++;
      OutputFold = string(argv[i]);
    }
    if(strncmp(argv[i],"-i", 2) == 0){
      i++;
      InputFile = string(argv[i]);
    }
    if(strncmp(argv[i],"--input", 7) == 0){
      i++;
      InputFile = string(argv[i]);
    }
    if(strncmp(argv[i],"-year", 5) == 0){
      i++;
      year = std::stoi(argv[i]);
    }
    if(strncmp(argv[i],"++bkg", 5) == 0){
      addBkg = true;
    }
    if(strncmp(argv[i],"++sig", 5) == 0){
      addSig = true;
    }
    if(strncmp(argv[i],"+proc", 5) == 0){
      i++;
      proc_to_add += string(argv[i]);
    }
    if(strncmp(argv[i],"-proc", 5) == 0){
      i++;
      proc_to_rem += string(argv[i]);
    }
    if(strncmp(argv[i],"++chan", 6) == 0){
      addChan = true;
    }
    if(strncmp(argv[i],"+chan", 5) == 0){
      i++;
      chan_to_add += string(argv[i]);
    }
    if(strncmp(argv[i],"-chan", 5) == 0){
      i++;
      chan_to_rem += string(argv[i]);
    }
    if(strncmp(argv[i],"++cat", 5) == 0){
      addCat = true;
    }
    if(strncmp(argv[i],"+cat", 4) == 0){
      i++;
      cat_to_add += string(argv[i]);
    }
    if(strncmp(argv[i],"-cat", 4) == 0){
      i++;
      cat_to_rem += string(argv[i]);
    }
    if(strncmp(argv[i],"++sys", 5) == 0){
      addSys = true;
    }
    if(strncmp(argv[i],"+sys", 4) == 0){
      i++;
      sys_to_add += string(argv[i]);
    }
    if(strncmp(argv[i],"-sys", 4) == 0){
      i++;
      sys_to_rem += string(argv[i]);
    }
    if(strncmp(argv[i],"+MCstats", 8) == 0){
      doMCstats = true;
    }
    if(strncmp(argv[i],"-sepchan", 8) == 0){
      doSepChan = true;
    }
    if(strncmp(argv[i],"--batch", 7) == 0){
      batch = true;
    }
    if(strncmp(argv[i],"--connect", 9) == 0){
      batch = true;
      connect = true;
    }
    if(strncmp(argv[i],"--setXsec", 9) == 0){
      i++;
      xsec_norm = std::stod(argv[i]);
    }
     
  }
    
  if(!addBkg && !addSig && (proc_to_add.size() == 0))
    bprint = true;

  if(!addChan && (chan_to_add.size() == 0))
    bprint = true;

  if(!addCat && (cat_to_add.size() == 0))
    bprint = true;
  
  if(bprint){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << "  options:" << endl;
    cout << "   --help(-h)          print options" << endl;
    cout << "   --verbose(-v)     increase verbosity" << endl;
    cout << "   --input(-i) [file]  input root file" << endl;
    cout << "   --output(-o) [fold] folder path for output" << endl;
    cout << "   -year [year]        year of inputs" << endl;
    cout << "   ++bkg               add all bkgs" << endl;
    cout << "   ++sig               add all signals" << endl;
    cout << "   +proc [label]       add processes matching label" << endl;
    cout << "   -proc [label]       remove processes matching label" << endl;
    cout << "   ++chan              add all channels" << endl;
    cout << "   +chan [label]       add channels matching label" << endl;
    cout << "   -chan [label]       removes channels matching label" << endl;
    cout << "   ++cat               add all categories" << endl;
    cout << "   +cat [label]        add categories matching label" << endl;
    cout << "   -cat [label]        removes categories matching label" << endl;
    cout << "   ++sys               add all shape systematics" << endl;
    cout << "   +sys [label]        add systematics matching label" << endl;
    cout << "   -sys [label]        removes systematics matching label" << endl;
    cout << "   +MCstats            adds autoMCStats uncertainties" << endl;
    cout << "   -sepchan            make datacards for each group of channels separately" << endl;
    cout << "   --workspace(-w)     also build workspaces (note: faster not to, and run message)" << endl;
    cout << "   --batch             for running inside a batch job" << endl;
    cout << "   --connect           for running inside a batch job on CMS connect" << endl;

    return 0;
  }

  ////////////////////////////////////////////////////////////////////                                  
  ////////////////////////////////////////////////////////////////////                                  
  ////////////////////////////////////////////////////////////////////
  
  string Era = std::to_string(year);
  string Ana = "KUEWKino";

  FitReader FIT(InputFile);
  if(verbose){
    cout << "Input file " << InputFile << " contains:" << endl;
    FIT.PrintCategories();
    FIT.PrintProcesses();
  }

  ProcessList processes = FIT.GetProcesses().FilterOR(proc_to_add); 
  if(addBkg)
    processes += FIT.GetProcesses().Filter(kBkg);
  if(addSig)
    processes += FIT.GetProcesses().Filter(kSig);
  processes = processes.RemoveOR(proc_to_rem);

  // keep only the process-by-process fakes
  ProcessList proc_fakes = processes.Filter("_Fakes_");
  processes.Remove("Fakes");
  processes += proc_fakes;
  
  VS channels;
  if(addChan)
    channels = FIT.GetChannels();
  else 
    channels = FIT.GetChannels().FilterOR(chan_to_add);
  channels = channels.RemoveOR(chan_to_rem);
  CategoryList categories;
  if(addCat)
    categories = FIT.GetCategories();
  else
    categories = FIT.GetCategories().FilterOR(cat_to_add);
  categories = categories.RemoveOR(cat_to_rem);

  Systematics systematics;
  if(addSys)
    systematics = FIT.GetSystematics();
  else 
    systematics = FIT.GetSystematics().FilterOR(sys_to_add);
  if(systematics.GetN() > 0)
    systematics = systematics.RemoveOR(sys_to_rem);
//cout << "systematics" << endl;
//  for(int s = 0; s < systematics.GetN(); s++)
//    cout << systematics[s].Label() << endl;

CategoryTree CT_Fakes1L;
vector<const CategoryTree*> catTrees;
CT_Fakes1L.GetListDepth(catTrees,1);
vector<string> catLabels;
map<string,VC> catBins;
//for(int i = 0; i < int(catTrees.size()); i++) catBins[catTrees[i]->GetSpecLabel()] = categories.Filter(*catTrees[i]).GetCategories();
//if(catBins.count("1Lel1J") > 0) cout << catBins["1Lel1J"].size() << endl;




  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  
  // prepare processes
  ProcessList signals = processes.Filter(kSig);

  ProcessList backgrounds = processes.Filter(kBkg); 

  // prepare channels/categories
  map<string,CategoryList> chanMap;
  for(auto c : channels)
    chanMap[c] = categories.Filter(c);
  categories.Clear();
  for(auto c : channels)
    categories += chanMap[c];

  ch::CombineHarvester cb;
  //if(verbose)
  //  cb.SetVerbosity(3);

  // Add all the channels/categories to CombineHarvester as observations
  for(auto c : channels){
    cout << "+ channel \"" << c << "\"";
    if(!verbose)
      cout << endl;
    else 
      cout << " with categories/bins:" << endl;
    
    VC cats = chanMap[c].GetCategories();
 //cout << "# categories for AddObservations: " << cats.size() << endl;
    
    if(verbose){
      cout << "  $BIN_ID   $BIN" << endl; 
      for(auto p : cats)
	cout << "     " << p.first << "      " << p.second << endl;
      cout << endl;
    }
   cb.AddObservations({"*"}, {Ana}, {Era}, {c}, cats);
  }
 


VC total_cats;
 // Add all the background processes
  int Nbkg = backgrounds.GetN();
  for(int b = 0; b < Nbkg; b++){
    Process proc = backgrounds[b];
    cout << "+ background process \"" << proc.Name() << "\"" << endl;
    for(auto ch : channels){
      int Ncat = chanMap[ch].GetN();
      CategoryList filled;
      if(verbose)
	cout << "    + Checking channel " << ch << " :" << endl;
      for(int c = 0; c < Ncat; c++){
	const Category& cat = chanMap[ch][c];
	if(FIT.IsThere(cat, proc)){
	  filled += cat;
	  if(verbose)
	    cout << "      + " << cat.GetLabel() << endl;
	}
      }

      if(filled.GetN() == 0)
	continue;

      VC cats = filled.GetCategories();
	for(auto c : cats) total_cats.push_back(c);      
cb.AddProcesses({"*"}, {Ana}, {Era}, {ch}, {proc.Name()}, cats, false);

    }
  }

  // Add all the signal processes                                                                   
  int Nsig = signals.GetN();
  for(int s = 0; s < Nsig; s++){
    Process proc = signals[s];
    cout << "  + signal process \"" << proc.Name() << "\"" << endl;
    for(auto ch : channels){
      int Ncat = chanMap[ch].GetN();
      CategoryList filled;
      if(verbose)
        cout << "    + Checking channel " << ch << " :" << endl;
      for(int c = 0; c < Ncat; c++){
        const Category& cat = chanMap[ch][c];
        if(FIT.Integral(cat, proc) > 1e-6){
          filled += cat;
          if(verbose)
            cout << "      + " << cat.GetLabel() << endl;
	}
      }

      if(filled.GetN() == 0)
        continue;

      VC cats = filled.GetCategories();
	for(auto c : cats) total_cats.push_back(c);      
      SM sig  = proc.GetSM();
      cb.AddProcesses(sig.second, {Ana}, {Era}, {ch}, {sig.first+"_"}, cats, true);

    }
  }
  
  FitConfiguration CONFIG;
  //CONFIG.Configure(cb, processes);

  VS bkg_rate;
//  bkg_rate += "ttbar"; //removing for ttbar hierarchy testing
//  bkg_rate += "Wjets"; //removing scale wjets for hierarchy
  bkg_rate += "ZDY";
//  bkg_rate += "QCD";
  bkg_rate += "DB";
  CONFIG.AddFloatingNorms(bkg_rate, cb, processes);
  
  VS bkg_rare;
  //bkg_rare += "ST";
  bkg_rare += "TB";
  CONFIG.AddRareNorms(bkg_rare, 0.4, cb, processes);
  
  CONFIG.AddCommonSys(cb, processes);
  CONFIG.AddFakeLeptonSys(cb, processes);
  CONFIG.AddSVSys(cb, processes);
  ProcessList proc_LF_Fakes = proc_fakes;
  proc_LF_Fakes = proc_LF_Fakes.Filter("f1");
  CONFIG.AddFakeSSSys(cb, proc_LF_Fakes);

  VS top;
  top += "ST";
  top += "ttbar";
  VS TopQCD;
  TopQCD += "QCD";
  TopQCD += "ttbar";
  TopQCD += "ST";
  ProcessList bkg_noTopQCD = backgrounds.RemoveOR(TopQCD);
  ProcessList bkg_noTop = backgrounds.RemoveOR(top);
  ProcessList bkg_noQCD = backgrounds.Remove("QCD");
  ProcessList Top_only = backgrounds.FilterOR(top);
  ProcessList QCD_only = backgrounds.Filter("QCD");
  //CONFIG.AddBJetSys(cb, backgrounds, "");
  //CONFIG.AddPTISRSys(cb, backgrounds, "");
  //CONFIG.AddgamTSys(cb, backgrounds, "");
  CONFIG.AddCommonBJetSys(cb, backgrounds);
  CONFIG.Add0LBJetSys(cb, bkg_noTop, "other_");
  CONFIG.Add0LBJetSys(cb, Top_only, "top_");
  CONFIG.Add1LBJetSys(cb, bkg_noTop, "other_");
  CONFIG.Add1LBJetSys(cb, Top_only, "top_");

  CONFIG.Add0LPTISRSys(cb, bkg_noQCD, "other_");
  CONFIG.Add0LPTISRSys(cb, QCD_only, "QCD_");
  CONFIG.Add1LPTISRSys(cb, backgrounds, "");

  CONFIG.Add0LgamTSys(cb, bkg_noQCD, "other_");
  CONFIG.Add0LgamTSys(cb, QCD_only, "QCD_");
  CONFIG.Add1LgamTSys(cb, backgrounds, "");
  CONFIG.AddCommongamTSys(cb, backgrounds);

  //CONFIG.AddLeptonQualityNormSys(cb, processes);
  //CONFIG.AddSJetLeptonQualityNormSys(cb, processes);
  CONFIG.AddLeptonCategoryNormSys(cb, processes);

  VS Wjets; //removing norm wjets for hierarchy leave VS for later hier call
  VS WjetsDY0L;
  Wjets += "Wjets";
  WjetsDY0L += "Wjets";
  WjetsDY0L += "ZDY";
  SystDict smW;
  SystDict smW0L;
  CONFIG.initSystDictW(smW);
  CONFIG.AddNormHierarchy( smW, Wjets, cb,processes) ;

  CONFIG.initSystDictW0L(smW0L);
  CONFIG.AddNormHierarchy( smW0L, WjetsDY0L, cb, processes);

//   CONFIG.AddSJetNormSys("Wjets", Wjets, cb, processes);



  VS ttbar;
  ttbar += "ttbar";
  SystDict smtt;
 // ttbar += "ST";
   CONFIG.initSystDictTtbar(smtt);
   CONFIG.AddNormHierarchy( smtt, ttbar, cb, processes) ;
//   CONFIG.AddSJetNormSys("ttbar", ttbar, cb, processes);

  VS QCD;
  QCD += "QCD";
  CONFIG.AddQCDNormSys("QCD", QCD, cb, processes);

/*
  VS Other;
  Other += "ttbar";
  Other += "ZDY";
  Other += "DB";
  Other += "ST";
  Other += "TB";
//  CONFIG.AddNormHierarchy( sm, Other, cb, processes);
CONFIG.AddSJetNormSys("Other", Other, cb, processes); 
//CONFIG.AddSJetNormSys("Rare", bkg_rare, cb, processes);
*/


 VS ZDYDB;
 ZDYDB += "ZDY";
 ZDYDB += "DB";
// CONFIG.AddSJetNormSys("ZDYDB", ZDYDB, cb, processes);
 CONFIG.AddLNormSys("ZDYDB", ZDYDB, cb, processes, std::vector<std::string>{"1L","2L","3L" }, 1.2);

 VS ST;
 ST += "ST";
 //ST += "TB";
 CONFIG.AddLNormSys("ST", ST, cb, processes, std::vector<std::string>{"0L","1L","2L"}, 1.2);
// CONFIG.AddSJetNormSys("STTB", STTB, cb, processes);

/*
VS ZDY;
ZDY += "ZDY";
CONFIG.AddSJetNormSys("ZDY",ZDY,cb, processes);

VS DB;
DB += "DB";
//DBTB += "TB";
CONFIG.AddSJetNormSys("DB",DB,cb, processes);
*/


  //cb.PrintSysts();

 using ch::syst::SystMap;
  using ch::syst::era;
  using ch::syst::channel;
  using ch::syst::bin_id;
  using ch::syst::process;
 
if(xsec_norm != -999.0){
  cout << "Setting signal cross section to: " << xsec_norm << endl;
  cb.cp().AddSyst(cb, "xsec_norm","rateParam",SystMap<>::init(xsec_norm));
  cb.AddDatacardLineAtEnd("nuisance edit freeze xsec_norm "+std::to_string(xsec_norm));
 }
else cout << "Nominal signal cross section: " << xsec_norm << endl;
  SystematicsTool SYS;
  Systematics shapeToNorm = SYS.GetConvertedSystematics();
 
 int Nsys = systematics.GetN();
  if(Nsys > 0){
    cout << "+ Adding shape systematics" << endl;
    for(int s = 0; s < Nsys; s++){  
      Systematic& sys = systematics[s];
      if(shapeToNorm.Contains(sys)){ 
        CONFIG.AddShapeSysAsNorm(sys,cb,FIT);
        continue;
      }
      ProcessList proc_sys;

      for(int p = 0; p < Nbkg; p++)
	if(FIT.HasSystematic(backgrounds[p], sys))
	  proc_sys += backgrounds[p];
//      for(int p = 0; p < Nsig; p++)
//	if(FIT.HasSystematic(signals[p], sys))
//	  proc_sys += signals[p];

      if(proc_sys.GetN() > 0){
	cout << "  + " << sys.Label() << endl;

	int Nproc = proc_sys.GetN();
	for(int p = 0; p < Nproc; p++){
	  Process proc = proc_sys[p];
	  // looping through categories to check that process/sys/cat is filled
	  VS cat_names;
//cout << "proc: " << proc.Name() << " with sys: " << sys.Label() << endl;
	  for(auto ch : channels){
	    int Ncat = chanMap[ch].GetN();
	    for(int c = 0; c < Ncat; c++){
	      const Category& cat = chanMap[ch][c];
	      if(FIT.IsThere(cat, proc, sys)){
		cat_names += cat.FullLabel();	
	      }
	    }
	  }
	  if(cat_names.size() > 0)
	  cb.cp().process(VS().a(proc.Name())).bin(cat_names)
		  .AddSyst(cb, sys.Label(), "shape", SystMap<>::init(1.00));
	}
      }
    }
  }

  cb.cp().backgrounds().ExtractShapes(InputFile,
				      "$BIN/$PROCESS",
				      "$BIN/$PROCESS_$SYSTEMATIC");
  cb.cp().signals().ExtractShapes(InputFile,
				  "$BIN/$PROCESS$MASS",
				  "$BIN/$PROCESS$MASS_$SYSTEMATIC");
  // autoMCStats
  if(doMCstats)
    cb.cp().SetAutoMCStats(cb, -1.);
  /*
    auto bbb = ch::BinByBinFactory()
    .SetAddThreshold(0.1)
    .SetFixNorm(true);

    bbb.AddBinByBin(cb.cp().backgrounds(), cb);

    // This function modifies every entry to have a standardised bin name of
    // the form: {analysis}_{channel}_{bin_id}_{era}
    // which is commonly used in the htt analyses
    ch::SetStandardBinNames(cb);
    //! [part8]

    //! [part9]
    // First we generate a set of bin names:
    set<string> bins = cb.bin_set();
    // This method will produce a set of unique bin names by considering all
    // Observation, Process and Systematic entries in the CombineHarvester
    // instance.

    // We create the output root file that will contain all the shapes.
    TFile output("htt_mt.input.root", "RECREATE");

  */

  // Loop through all signals and write a datacard, create output
  
//  VC cats = categories.GetCategories();

//set<pair<int,string>> VC_set(total_cats.begin(),total_cats.end());
//cout << "# categories for AddProcesses: " << VC_set.size() << " " << total_cats.size()<< endl;
//cout << "bin_set: " << cb.bin_set().size() << endl;
//cout << "bin set from Observations: " << cb.SetFromObs(std::mem_fn(&ch::Process::bin)).size() << endl;
//cout << "bin set from Systematics: " << cb.SetFromSysts(std::mem_fn(&ch::Process::bin)).size() << endl;
//cout << "imax: " << cb.SetFromProcs(std::mem_fn(&ch::Process::bin)).size() << endl;
//
//vector<string> binSet;
//vector<string> setFromProcs;
//for(auto b : cb.bin_set()) binSet.push_back(b);//cout << b << endl;
//for(auto b : cb.SetFromProcs(std::mem_fn(&ch::Process::bin))) setFromProcs.push_back(b);//cout << b << endl;
//int len = min(binSet.size(),setFromProcs.size());
//for(int i = 0; i < len; i++){
//  if(binSet[i] == setFromProcs[i]) continue;
//  else{ cout << i << endl; cout << "binSet"  << endl; cout << binSet[i-1] << " " << binSet[i] << " " << binSet[i+1] << endl;
//    cout << "setFromProcs" << endl; cout << setFromProcs[i-1] << " " << setFromProcs[i] << " " << setFromProcs[i+1] << endl; break; }
//
//}

  cout << "* Writing ouput to " << OutputFold << endl;
  string OutputFile = "";
  if(connect)
    OutputFile = "FitInput_"+Ana+"_"+Era+".root";
  else
    OutputFile = OutputFold+"/FitInput_"+Ana+"_"+Era+".root";

  if(!batch){
    gSystem->Exec(("mkdir -p "+OutputFold).c_str());
  
    string copy_cmd = "cp "+InputFile+" "+OutputFile;
    if(InputFile.find("xrootd") != std::string::npos)
      copy_cmd = "xrdcp "+InputFile+" "+OutputFile;
    cout << "COPY cmd:" << endl;
    cout << "   " << copy_cmd << endl;
    gSystem->Exec(copy_cmd.c_str());  
  }
  TFile output(OutputFile.c_str(), "UPDATE"); 

  cout << "  * Creating datacards" << endl;
	       	       
  // datacard/workspace with all categories
  string fold = OutputFold+"/all";
  if(connect)
    fold = "datacards/all/";
  gSystem->Exec(("mkdir -p "+fold).c_str());
  
  VSM masses = signals.GetSignalMasses();
  
  for(auto sm : masses){
    fold = OutputFold+"/all/"+sm.first;
    if(connect)
      fold = "datacards/all/"+sm.first;
    gSystem->Exec(("mkdir -p "+fold).c_str());
    
    for(auto m : sm.second){
      fold = OutputFold+"/all/"+sm.first+"/"+m;
      if(connect)
        fold = "datacards/all/"+sm.first+"/"+m;
      //cout << "making directory: " << fold << endl;
	gSystem->Exec(("mkdir -p "+fold).c_str());
      
      if(verbose)
	cout << "    * all channels " << sm.first+"_"+m << endl;

      cb.cp().mass({m, "*"})
	.WriteDatacard(fold+"/datacard.txt", output);
    }
  }

  // datacard/workspace for each channel
  if(doSepChan){
    for(auto ch : channels){
      fold = OutputFold+"/"+ch;
      if(connect)
        fold = "datacards/"+ch;
      gSystem->Exec(("mkdir -p "+fold).c_str());
      
      for(auto sm : masses){
	fold = OutputFold+"/"+ch+"/"+sm.first;
        if(connect)
          fold = "datacards/"+ch+"/"+sm.first;
	gSystem->Exec(("mkdir -p "+fold).c_str());
	
	for(auto m : sm.second){
	  fold = OutputFold+"/"+ch+"/"+sm.first+"/"+m;
          if(connect)
            fold = "datacards/"+ch+"/"+sm.first+"/"+m;
	  gSystem->Exec(("mkdir -p "+fold).c_str());
	  
	  if(verbose)
	    cout << "    * " << ch << " " << sm.first+"_"+m<< endl;
	  
	  cb.cp().channel({ch}).mass({m, "*"})
	    .WriteDatacard(fold+"/datacard.txt", output);
	}
      }
    }
  }

  output.Close();
 
  cout << "  * Creating workspaces" << endl;
  
  string cmd = "combineTool.py -M T2W -o workspace.root -i ";
  string icmd;

  /*
    channels += "all";
    for(auto ch : channels)
    for(auto sm : masses)
    for(auto m : sm.second){
    if(verbose)
    cout << "    * " << ch << " " << sm.first+"_"+m<< endl;

    icmd = cmd + OutputFold+"/"+ch+"/"+sm.first+"/"+m+"/datacard.txt ";
    icmd += "-m "+m;
    gSystem->Exec(icmd.c_str());
    }
  */
  cmd = "combineTool.py -M T2W -i "+OutputFold+"*/*/*/datacard.txt -o workspace.root --parallel 4";

  string cmd_condor = "combineTool.py -M T2W -i "+OutputFold+"*/*/*/datacard.txt -o workspace.root --job-mode condor ";
  if(connect)
  {
   cmd = "combineTool.py -M T2W -i "+OutputFold+"datacards/*/*/*/datacard.txt -o workspace.root --parallel 4";

   string cmd_condor = "combineTool.py -M T2W -i "+OutputFold+"datacards/*/*/*/datacard.txt -o workspace.root --job-mode connect --input-file "+OutputFold+InputFile+" --sub-opts='+ProjectName=\"cms.org.ku\" \n request_memory = 8 GB \n' ";
  }

  string cmd_condor_CERN = "--sub-opts='+JobFlavour=\"espresso\" \\n request_memory = 4 GB'";
  string cmd_condor_T3 = "--sub-opts='Requirements = (Machine != \"red-node000.unl.edu\") && (Machine != \"red-c2325.unl.edu\") \\n request_memory = 4 GB'";
  
  if(workspace)
    gSystem->Exec(cmd.c_str());
  else {
    cout << "To build workspaces type:" << endl << endl;
    cout << "    " << cmd << endl << endl;
    cout << "Or, to run using condor, something like:" << endl << endl;
    cout << "    " << cmd_condor << endl << endl;
    cout << "   with options for CERN:" << endl << endl;
    cout << "    " << cmd_condor_CERN << endl << endl;
    cout << "   or options for UNL T3:" << endl << endl;
    cout << "    " << cmd_condor_T3 << endl << endl;
  }

}
	       
	     

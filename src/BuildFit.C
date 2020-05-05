#include <string>
#include <map>
#include <set>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
#include "CombineHarvester/CombineTools/interface/CombineHarvester.h"
#include "CombineHarvester/CombineTools/interface/Observation.h"
#include "CombineHarvester/CombineTools/interface/Process.h"
#include "CombineHarvester/CombineTools/interface/Utilities.h"
#include "CombineHarvester/CombineTools/interface/Systematics.h"
#include "CombineHarvester/CombineTools/interface/BinByBin.h"

#include "TSystem.h"

#include "FitReader.hh"

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

  bool addCat = false; // all categories
  VS   cat_to_add;

  bool addSys = false; // all available shape systematics
  VS   sys_to_add;

  bool bprint  = false;
  bool verbose = false;
  int  year    = 2017;
    
  for(int i = 0; i < argc; i++){
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
    if(strncmp(argv[i],"++cat", 5) == 0){
      addCat = true;
    }
    if(strncmp(argv[i],"+cat", 4) == 0){
      i++;
      cat_to_add += string(argv[i]);
    }
    if(strncmp(argv[i],"++sys", 5) == 0){
      addSys = true;
    }
    if(strncmp(argv[i],"+sys", 4) == 0){
      i++;
      sys_to_add += string(argv[i]);
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
    cout << "   ++cat               add allcategories" << endl;
    cout << "   +cat [label]        add categories matching label" << endl;
    
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

  ProcessList processes; 
  for(auto p : proc_to_add)
    processes += FIT.GetProcesses().Filter(p);
  if(addBkg)
    processes += FIT.GetProcesses().Filter(kBkg);
  if(addSig)
    processes += FIT.GetProcesses().Filter(kSig);
  for(auto p : proc_to_rem)
    processes = processes.Remove(p);

  VS channels;
  if(addChan)
    channels = FIT.GetChannels();
  else {
    VS chs = FIT.GetChannels();
    for(auto c : chan_to_add)
      for(auto f : chs)
	if(c == f){
	  channels += c;
	  break;
	}
  }
 
  CategoryList categories;
  if(addCat)
    categories = FIT.GetCategories();
  else
    for(auto c : cat_to_add)
      categories += FIT.GetCategories().Filter(c);

  Systematics systematics;
  for(auto s : sys_to_add)
    systematics += FIT.GetSystematics().Filter(s);
  if(addSys)
    systematics += FIT.GetSystematics();

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  
  // prepare processes
  ProcessList signals = processes.Filter(kSig);
  VSM masses  = signals.GetSignalMasses();

  ProcessList backgrounds = processes.Filter(kBkg).Remove("_Fake"); 

  // prepare channels/categories
  map<string,CategoryList> chanMap;
  for(auto c : channels)
    chanMap[c] = categories.Filter(c);
  categories.Clear();
  for(auto c : channels)
    categories += chanMap[c];

  ch::CombineHarvester cb;
  if(verbose)
    cb.SetVerbosity(3);

  // Add all the channels/categories to CombineHarvester as observations
  for(auto c : channels){
    cout << "+ channel \"" << c << "\"";
    if(!verbose)
      cout << endl;
    else 
      cout << " with categories/bins:" << endl;
    
    VC cats = chanMap[c].GetCategories();
    
    if(verbose){
      cout << "  $BIN_ID   $BIN" << endl; 
      for(auto p : cats)
	cout << "     " << p.first << "      " << p.second << endl;
      cout << endl;
    }

    cb.AddObservations({"*"}, {Ana}, {Era}, {c}, cats);
  }
  
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
	if(FIT.Integral(cat, proc) > 0.){
	  filled += cat;
	  if(verbose)
	    cout << "      + " << cat.GetLabel() << endl;
	}
      }

      if(filled.GetN() == 0)
	continue;

      VC cats = filled.GetCategories();
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
        if(FIT.Integral(cat, proc) > 0.){
          filled += cat;
          if(verbose)
            cout << "      + " << cat.GetLabel() << endl;
	}
      }

      if(filled.GetN() == 0)
        continue;

      VC cats = filled.GetCategories();
      SM sig  = proc.GetSM();
      cb.AddProcesses(sig.second, {Ana}, {Era}, {ch}, {sig.first}, cats, true);

    }
  }
  
  using ch::syst::SystMap;
  using ch::syst::era;
  using ch::syst::bin_id;
  using ch::syst::process;


  cb.cp().signals()
    .AddSyst(cb, "lumi_$ERA", "lnN", SystMap<era>::init
	     ({"2016"}, 1.022)
	     ({"2017"}, 1.022)
	     ({"2018"}, 1.022));
  
  cb.cp().backgrounds()
    .AddSyst(cb, "lumi_$ERA", "lnN", SystMap<era>::init
	     ({"2016"}, 1.022)
	     ({"2017"}, 1.022)
	     ({"2018"}, 1.022));
  
  int Nsys = systematics.GetN();
  if(Nsys > 0){
    cout << "+ Adding shape systematics" << endl;
    for(int s = 0; s < Nsys; s++){
      Systematic& sys = systematics[s];
      ProcessList proc_sys;

      for(int p = 0; p < Nbkg; p++)
	if(FIT.HasSystematic(backgrounds[p], sys))
	  proc_sys += backgrounds[p];
      for(int p = 0; p < Nsig; p++)
	if(FIT.HasSystematic(signals[p], sys))
	  proc_sys += signals[p];

      if(proc_sys.GetN() > 0){
	cout << "  + " << sys.Label() << endl;
	cb.cp().process(proc_sys.GetProcesses())
	  .AddSyst(cb, sys.Label(), "shape", SystMap<>::init(1.00));
      }
    }
  }

  
  cb.cp().backgrounds().ExtractShapes(InputFile,
				      "$PROCESS/$PROCESS_$BIN",
				      "$PROCESS/$PROCESS_$SYSTEMATIC_$BIN");
  cb.cp().signals().ExtractShapes(InputFile,
				  "$PROCESS_$MASS/$PROCESS_$MASS_$BIN",
				  "$PROCESS_$MASS/$PROCESS_$MASS_$SYSTEMATIC_$BIN");
  
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
  
  VC cats = categories.GetCategories();

  gSystem->Exec(("mkdir -p "+OutputFold).c_str());
  TFile output((OutputFold+"/FitInput_"+Era+".root").c_str(), "RECREATE"); 
	       	       
  for(int s = 0; s < Nsig; s++){
    Process p = signals[s];
    SM sm = p.GetSM();
    string fold = OutputFold+"/"+p.Name();

    gSystem->Exec(("mkdir -p "+fold).c_str());

    // all categories combined datacard
    cb.cp().process({sm.first}).mass({sm.second[0], "*"})
      .WriteDatacard(OutputFold+"/datacard_"+Era+"_"+p.Name()+".txt", output);
    // individual categories
    for(auto c : cats)
      cb.cp().bin({c.second}).process({sm.first}).mass({sm.second[0], "*"})
        .WriteDatacard(fold+"/datacard_"+Era+"_"+p.Name()+"_"+c.second+".txt", output); 
  }
}
	       
	     

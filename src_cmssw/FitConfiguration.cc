#include "FitConfiguration.hh"


///////////////////////////////////////////
////////// FitConfiguration class
///////////////////////////////////////////

void FitConfiguration::Configure(ch::CombineHarvester& cb, ProcessList& processes){

  using ch::syst::SystMap;
  using ch::syst::era;
  using ch::syst::channel;
  using ch::syst::bin_id;
  using ch::syst::process;

  ProcessList signals = processes.Filter(kSig);
  ProcessList backgrounds = processes.Filter(kBkg); 

  // normalizations
  VS bkg_rate;
  VS bkg_rare;

  bkg_rate += "ttbar";
  bkg_rate += "Wjets";
  bkg_rate += "DB";
  bkg_rate += "ZDY";
  
  bkg_rare += "ST";
  bkg_rare += "TB";

  for(auto p : bkg_rate){
    ProcessList plist = backgrounds.Filter(p);
    cb.cp().process(plist.GetProcesses())
      .AddSyst(cb, "scale_"+p, "rateParam", SystMap<>::init(1.0));
  }
  
  for(auto p : bkg_rare){
    ProcessList plist = backgrounds.Filter(p);
    cb.cp().process(plist.GetProcesses())
      .AddSyst(cb, "lumi_$ERA", "lnN", SystMap<era>::init
	       ({"2016"}, 1.022)
	       ({"2017"}, 1.022)
	       ({"2018"}, 1.022));
     cb.cp().process(plist.GetProcesses())
       .AddSyst(cb, "scale_"+p, "lnN", SystMap<>::init(1.2));
  }
  
  // signal luminosity uncertainty
  cb.cp().signals()
    .AddSyst(cb, "lumi_$ERA", "lnN", SystMap<era>::init
	     ({"2016"}, 1.022)
	     ({"2017"}, 1.022)
	     ({"2018"}, 1.022));

  // signal xsec uncertainty
  cb.cp().signals()
    .AddSyst(cb, "sig_xsec", "lnN", SystMap<>::init(1.05));
  
  //want to add PTISR systematic - just bkgs in every region that matches string "PTISR1"
//  auto all_bins = cb.cp().channel({"*1L*"}).bin_set(); //need to define individual bin_sets for channels separately?
// //cb.cp().backgrounds().PrintObs();
//for(auto bin : all_bins){
//cout << "bin" << endl;
//    cout << bin << endl;
//
//}
//cb.cp().backgrounds().bin(bin).AddSyst(cb,"PTISR","lnN",SystMap<>::init(1.1));
  //cb.cp().backgrounds().bin(bin).AddSyst(cb,"gamT"","lnN",SystMap<>::init




}


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
  bkg_rate += "QCD";
  
  bkg_rare += "ST";
  bkg_rare += "TB";

  // allow for regex filtering
  cb.SetFlag("filters-use-regex", true);

  for(auto p : bkg_rate){
    ProcessList plist = backgrounds.Filter(p);
    cb.cp().process(plist.GetProcesses())
      .AddSyst(cb, "scale_"+p, "rateParam", SystMap<>::init(1.0));
    //cb.cp().process(plist.GetProcesses()).PrintProcs();
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

  ///////////////////////
  // lepton fake factors
  ///////////////////////

  // // overall factors for amount of each type of fake
  //cb.cp().backgrounds().process(VS().a(".*_Fakes_muf0.*")).PrintProcs();
  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf0.*"))
    .AddSyst(cb, "Fakes_muf0", "lnN", SystMap<>::init(1.3));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf1.*"))
    .AddSyst(cb, "Fakes_muf1", "lnN", SystMap<>::init(1.3));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf0.*"))
    .AddSyst(cb, "Fakes_elf0", "lnN", SystMap<>::init(1.3));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf1.*"))
    .AddSyst(cb, "Fakes_elf1", "lnN", SystMap<>::init(1.3));

  // // SFs for lepton ID factors
  // cb.cp().backgrounds().process(VS().a(".*_Fakes_muf0.*")).bin(VS().a(".*muB.*")).PrintObs();
  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf0.*")).bin(VS().a(".*muB.*"))
    .AddSyst(cb, "IDISO_muf0", "lnN", SystMap<>::init(1.05));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf1.*")).bin(VS().a(".*muB.*"))
    .AddSyst(cb, "IDISO_muf1", "lnN", SystMap<>::init(1.05));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf0.*")).bin(VS().a(".*elB.*"))
    .AddSyst(cb, "IDISO_elf0", "lnN", SystMap<>::init(1.05));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf1.*")).bin(VS().a(".*elB.*"))
    .AddSyst(cb, "IDISO_elf1", "lnN", SystMap<>::init(1.05));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf0.*")).bin(VS().a(".*muS.*"))
    .AddSyst(cb, "SIP3D_muf0", "lnN", SystMap<>::init(1.05));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf1.*")).bin(VS().a(".*muS.*"))
    .AddSyst(cb, "SIP3D_muf1", "lnN", SystMap<>::init(1.05));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf0.*")).bin(VS().a(".*elS.*"))
    .AddSyst(cb, "SIP3D_elf0", "lnN", SystMap<>::init(1.05));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf1.*")).bin(VS().a(".*elS.*"))
    .AddSyst(cb, "SIP3D_elf1", "lnN", SystMap<>::init(1.05));

  // ///////////////////////
  // // SV factors
  // ///////////////////////
  cb.cp().backgrounds().bin(VS().a(".*1svS.*"))
    .AddSyst(cb, "SV_eff", "lnN", SystMap<>::init(1.08));

  cb.cp().backgrounds().bin(VS().a(".*2svS.*"))
    .AddSyst(cb, "SV_eff", "lnN", SystMap<>::init(1.16));

  // cb.cp().backgrounds().bin(VS().a(".*SVeta1.*")).PrintObs();
  cb.cp().backgrounds().bin(VS().a(".*SVeta1.*")).process(VS().a("ttbar").a("ST"))
    .AddSyst(cb, "SV_eta_top", "lnN", SystMap<>::init(1.05));

  cb.cp().backgrounds().bin(VS().a(".*SVeta1.*")).process(VS().a("Wjets").a("ZDY").a("TB"))
  .AddSyst(cb, "SV_eta_other", "lnN", SystMap<>::init(1.05));

  ///////////////////////
  // kinematic factors
  ///////////////////////
  //cb.cp().backgrounds().bin(VS().a(".*gamT1.*")).PrintObs();
  cb.cp().backgrounds().bin(VS().a(".*gamT1.*"))
    .AddSyst(cb, "gamT", "lnN", SystMap<>::init(1.05));

  cb.cp().backgrounds().bin(VS().a(".*PTISR1.*"))
    .AddSyst(cb, "PTISR", "lnN", SystMap<>::init(1.05));

  // turn off regex filtering
  cb.SetFlag("filters-use-regex", false)
    ;

}


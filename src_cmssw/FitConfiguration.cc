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

  bool QCDnorms = true;
  bool WJetsnorms = true;
  bool WJetsnorms_highLow = false;
  bool bkgNorms_noQCD_noWjets = true;
  bool bkgNorms_bosonTopSplit = false;
  ProcessList signals = processes.Filter(kSig);
  ProcessList backgrounds = processes.Filter(kBkg); 

  // normalizations
  VS bkg_rate;
  VS bkg_rare;
  VS bkg_boson;
  VS bkg_top;

  bkg_rate += "ttbar";
  bkg_rate += "Wjets";
  bkg_rate += "ZDY";
  bkg_rate += "QCD";
  //bkg_rate += "ST";
  bkg_rate += "TB";
  
  bkg_rare += "ST";
  bkg_rare += "DB";
  bkg_rare += "TB";

  bkg_boson += "Wjets";
  bkg_boson += "ZDY";
  bkg_boson += "TB";
  bkg_boson += "DB";

  bkg_top += "ttbar";
  bkg_top += "ST";


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
    if(p == "DB"){
	cb.cp().bin(VS().a(".*2L.*").a(".*3L.*")).process(plist.GetProcesses())
	  .AddSyst(cb,"scale_"+p,"rateParam",SystMap<>::init(1.0));
      	cb.cp().bin(VS().a(".*0L.*").a(".*1L.*")).process(plist.GetProcesses())
	  .AddSyst(cb,"scale_"+p,"lnN",SystMap<>::init(1.2));
    }
    else 
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
//  cb.cp().backgrounds().bin(VS().a(".*gamT1.*"))
//  .AddSyst(cb, "gamT", "lnN", SystMap<>::init(1.05));
//
//  cb.cp().backgrounds().bin(VS().a(".*PTISR1.*"))
//    .AddSyst(cb, "PTISR", "lnN", SystMap<>::init(1.05));


  cb.cp().backgrounds().bin(VS().a(".*0L.*.*0j.*S.*.*gamT1.*").a(".*0L.*.*1j.*S.*.*gamT1.*"))
    .AddSyst(cb, "gamT_0L_01jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*2j.*S.*.*gamT1.*").a(".*0L.*.*3j.*S.*.*gamT1.*").a(".*0L.*.*4j.*S.*.*gamT1.*").a(".*0L.*.*5j.*S.*.*gamT1.*"))
    .AddSyst(cb, "gamT_0L_2345jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*0j.*S.*.*gamT1.*").a(".*1L.*.*1j.*S.*.*gamT1.*"))
    .AddSyst(cb, "gamT_1L_01jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*2j.*S.*.*gamT1.*").a(".*1L.*.*3j.*S.*.*gamT1.*").a(".*1L.*.*4j.*S.*.*gamT1.*"))
    .AddSyst(cb, "gamT_1L_234jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*0j.*S.*.*gamT1.*").a(".*2L.*.*1j.*S.*.*gamT1.*"))
    .AddSyst(cb, "gamT_2L_01jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*2j.*S.*.*gamT1.*").a(".*2L.*.*3j.*S.*.*gamT1.*"))
    .AddSyst(cb, "gamT_2L_23jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*3L.*.*0j.*S.*.*gamT1.*").a(".*3L.*.*1j.*S.*.*gamT1.*"))
    .AddSyst(cb, "gamT_3L_01jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*0j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_0jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*1j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_1jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*2j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_2jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*3j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_3jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*4j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_4jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*5j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_5jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*0j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_0jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*1j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_1jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*2j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_2jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*3j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_3jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*4j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_4jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*0j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_2L_0jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*1j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_2L_1jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*2j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_2L_2jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*3j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_2L_3jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*3L.*.*0j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_3L_0jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*3L.*.*1j.*S.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_3L_1jS", "lnN", SystMap<>::init(1.1));

  if(WJetsnorms_highLow){
  cb.cp().process({"Wjets"}).bin(VS().a(".*0L.*.*0j.*S.*").a(".*0L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_Wjets_0L_01jS", "lnN", SystMap<>::init(1.8));
  
  cb.cp().process({"Wjets"}).bin(VS().a(".*0L.*.*2j.*S.*").a(".*0L.*.*3j.*S.*").a(".*0L.*.*4j.*S.*").a(".*0L.*.*5j.*S.*"))
    .AddSyst(cb, "norm_Wjets_0L_2345jS", "lnN", SystMap<>::init(1.8));

  cb.cp().process({"Wjets"}).bin(VS().a(".*1L.*.*0j.*S.*").a(".*1L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_Wjets_1L_01jS", "lnN", SystMap<>::init(1.8));
  
  cb.cp().process({"Wjets"}).bin(VS().a(".*1L.*.*2j.*S.*").a(".*1L.*.*3j.*S.*").a(".*1L.*.*4j.*S.*"))
    .AddSyst(cb, "norm_Wjets_1L_234jS", "lnN", SystMap<>::init(1.8));

  cb.cp().process({"Wjets"}).bin(VS().a(".*2L.*.*0j.*S.*").a(".*2L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_Wjets_2L_01jS", "lnN", SystMap<>::init(1.8));
  
  cb.cp().process({"Wjets"}).bin(VS().a(".*2L.*.*2j.*S.*").a(".*2L.*.*3j.*S.*"))
    .AddSyst(cb, "norm_Wjets_2L_23jS", "lnN", SystMap<>::init(1.8));
  
  cb.cp().process({"Wjets"}).bin(VS().a(".*3L.*.*0j.*S.*").a(".*3L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_Wjets_3L_01jS", "lnN", SystMap<>::init(1.8));
  }
  if(WJetsnorms){
    ProcessList plist = backgrounds.Filter("Wjets");
  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*0j.*S.*"))
    .AddSyst(cb, "norm_Wjets_0L_0jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_Wjets_0L_1jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*2j.*S.*"))
    .AddSyst(cb, "norm_Wjets_0L_2jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*3j.*S.*"))
    .AddSyst(cb, "norm_Wjets_0L_3jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*4j.*S.*"))
    .AddSyst(cb, "norm_Wjets_0L_4jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*5j.*S.*"))
    .AddSyst(cb, "norm_Wjets_0L_5jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*0j.*S.*"))
    .AddSyst(cb, "norm_Wjets_1L_0jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_Wjets_1L_1jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*2j.*S.*"))
    .AddSyst(cb, "norm_Wjets_1L_2jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*3j.*S.*"))
    .AddSyst(cb, "norm_Wjets_1L_3jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*4j.*S.*"))
    .AddSyst(cb, "norm_Wjets_1L_4jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*0j.*S.*"))
    .AddSyst(cb, "norm_Wjets_2L_0jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_Wjets_2L_1jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*2j.*S.*"))
    .AddSyst(cb, "norm_Wjets_2L_2jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*3j.*S.*"))
    .AddSyst(cb, "norm_Wjets_2L_3jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*0j.*S.*"))
    .AddSyst(cb, "norm_Wjets_3L_0jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_Wjets_3L_1jS", "lnN", SystMap<>::init(1.2));
  }
  if(QCDnorms){
      ProcessList plist = backgrounds.Filter("QCD");
    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*0j.*S.*"))
      .AddSyst(cb, "norm_QCD_0L_0jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_QCD_0L_1jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*2j.*S.*"))
    .AddSyst(cb, "norm_QCD_0L_2jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*3j.*S.*"))
    .AddSyst(cb, "norm_QCD_0L_3jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*4j.*S.*"))
    .AddSyst(cb, "norm_QCD_0L_4jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*5j.*S.*"))
    .AddSyst(cb, "norm_QCD_0L_5jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*0j.*S.*"))
    .AddSyst(cb, "norm_QCD_1L_0jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_QCD_1L_1jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*2j.*S.*"))
    .AddSyst(cb, "norm_QCD_1L_2jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*3j.*S.*"))
    .AddSyst(cb, "norm_QCD_1L_3jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*4j.*S.*"))
    .AddSyst(cb, "norm_QCD_1L_4jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*0j.*S.*"))
    .AddSyst(cb, "norm_QCD_2L_0jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_QCD_2L_1jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*2j.*S.*"))
    .AddSyst(cb, "norm_QCD_2L_2jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*3j.*S.*"))
    .AddSyst(cb, "norm_QCD_2L_3jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*0j.*S.*"))
    .AddSyst(cb, "norm_QCD_3L_0jS", "lnN", SystMap<>::init(1.5));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_QCD_3L_1jS", "lnN", SystMap<>::init(1.5));
  }
  if(bkgNorms_noQCD_noWjets){
    ProcessList plist = backgrounds.Remove("QCD");
    plist = plist.Remove("Wjets");
    plist.Print();
  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*0j.*S.*"))
    .AddSyst(cb, "norm_bkgs_0L_0jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_bkgs_0L_1jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*2j.*S.*"))
    .AddSyst(cb, "norm_bkgs_0L_2jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*3j.*S.*"))
    .AddSyst(cb, "norm_bkgs_0L_3jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*4j.*S.*"))
    .AddSyst(cb, "norm_bkgs_0L_4jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*5j.*S.*"))
    .AddSyst(cb, "norm_bkgs_0L_5jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*0j.*S.*"))
    .AddSyst(cb, "norm_bkgs_1L_0jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_bkgs_1L_1jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*2j.*S.*"))
    .AddSyst(cb, "norm_bkgs_1L_2jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*3j.*S.*"))
    .AddSyst(cb, "norm_bkgs_1L_3jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*4j.*S.*"))
    .AddSyst(cb, "norm_bkgs_1L_4jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*0j.*S.*"))
    .AddSyst(cb, "norm_bkgs_2L_0jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_bkgs_2L_1jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*2j.*S.*"))
    .AddSyst(cb, "norm_bkgs_2L_2jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*0j.*S.*"))
    .AddSyst(cb, "norm_bkgs_3L_0jS", "lnN", SystMap<>::init(1.2));

  cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*1j.*S.*"))
    .AddSyst(cb, "norm_bkgs_3L_1jS", "lnN", SystMap<>::init(1.2));
  }
  if(bkgNorms_bosonTopSplit){
    for(auto p : bkg_boson){
      ProcessList plist = backgrounds.Filter(p);
    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*0j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_0L_0jS", "lnN", SystMap<>::init(1.8));
  
    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*1j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_0L_1jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*2j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_0L_2jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*3j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_0L_3jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*4j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_0L_4jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*5j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_0L_5jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*0j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_1L_0jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*1j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_1L_1jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*2j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_1L_2jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*3j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_1L_3jS", "lnN", SystMap<>::init(1.8));
    
    cb.cp().backgrounds().bin(VS().a(".*1L.*.*4j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_1L_4jS", "lnN", SystMap<>::init(1.8));

    cb.cp().backgrounds().bin(VS().a(".*2L.*.*0j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_2L_0jS", "lnN", SystMap<>::init(1.8));

    cb.cp().backgrounds().bin(VS().a(".*2L.*.*1j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_2L_1jS", "lnN", SystMap<>::init(1.8));

    cb.cp().backgrounds().bin(VS().a(".*2L.*.*2j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_2L_2jS", "lnN", SystMap<>::init(1.8));

    cb.cp().backgrounds().bin(VS().a(".*2L.*.*3j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_2L_3jS", "lnN", SystMap<>::init(1.8));

    cb.cp().backgrounds().bin(VS().a(".*3L.*.*0j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_3L_0jS", "lnN", SystMap<>::init(1.8));

    cb.cp().backgrounds().bin(VS().a(".*3L.*.*1j.*S.*"))
      .AddSyst(cb, "norm_bkgs_boson_3L_1jS", "lnN", SystMap<>::init(1.8));

    }
    for(auto p : bkg_top){
      ProcessList plist = backgrounds.Filter(p);
    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*0j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_0L_0jS", "lnN", SystMap<>::init(1.8));
  
    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*1j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_0L_1jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*2j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_0L_2jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*3j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_0L_3jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*4j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_0L_4jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*5j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_0L_5jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*0j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_1L_0jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*1j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_1L_1jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*2j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_1L_2jS", "lnN", SystMap<>::init(1.8));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*3j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_1L_3jS", "lnN", SystMap<>::init(1.8));
    
    cb.cp().backgrounds().bin(VS().a(".*1L.*.*4j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_1L_4jS", "lnN", SystMap<>::init(1.8));

    cb.cp().backgrounds().bin(VS().a(".*2L.*.*0j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_2L_0jS", "lnN", SystMap<>::init(1.8));

    cb.cp().backgrounds().bin(VS().a(".*2L.*.*1j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_2L_1jS", "lnN", SystMap<>::init(1.8));

    cb.cp().backgrounds().bin(VS().a(".*2L.*.*2j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_2L_2jS", "lnN", SystMap<>::init(1.8));

    cb.cp().backgrounds().bin(VS().a(".*2L.*.*3j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_2L_3jS", "lnN", SystMap<>::init(1.8));

    cb.cp().backgrounds().bin(VS().a(".*3L.*.*0j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_3L_0jS", "lnN", SystMap<>::init(1.8));

    cb.cp().backgrounds().bin(VS().a(".*3L.*.*1j.*S.*"))
      .AddSyst(cb, "norm_bkgs_top_3L_1jS", "lnN", SystMap<>::init(1.8));

    }
  }






  // turn off regex filtering
  cb.SetFlag("filters-use-regex", false);

}


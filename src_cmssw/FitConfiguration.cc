#include "FitConfiguration.hh"


///////////////////////////////////////////
////////// FitConfiguration class
///////////////////////////////////////////

using ch::syst::SystMap;
using ch::syst::era;
using ch::syst::channel;
using ch::syst::bin_id;
using ch::syst::process;

void FitConfiguration::Configure(ch::CombineHarvester& cb, ProcessList& processes){
  
  ProcessList signals = processes.Filter(kSig);
  ProcessList backgrounds = processes.Filter(kBkg); 
//cout << "Era set size: " << cb.era_set().size() << endl;
//cout << "Era: " << *cb.era_set().begin() << endl;
  bool QCDnorms = true;
  bool bkgNorms_noQCD_noWjets;
  bool WJetsnorms;
  bool bkgNorms_noQCD;
    WJetsnorms = true;
    bkgNorms_noQCD_noWjets = true;
    bkgNorms_noQCD = false;
  // bool WJetsnorms_highLow = false;
  //  bool bkgNorms_bosonTopSplit = false;

  // normalizations
  VS bkg_rate;
  VS bkg_rare;
  VS bkg_boson;
  VS bkg_top;

  bkg_rate += "ttbar";
  bkg_rate += "Wjets";
  bkg_rate += "ZDY";
  bkg_rate += "QCD";
  bkg_rate += "DB";
  
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
//  cb.cp().backgrounds().bin(VS().a(".*1svS.*")).AddSyst(cb, "SV_eff", "lnN", SystMap<>::init(1.08));
   cb.cp().backgrounds().bin(VS().a(".*1svS.*"))
      .AddSyst(cb, "SV_eff", "rateParam", SystMap<>::init(1.00));


 // cb.cp().backgrounds().bin(VS().a(".*2svS.*")).AddSyst(cb, "SV_eff", "lnN", SystMap<>::init(1.16));
   cb.cp().backgrounds().bin(VS().a(".*2svS.*"))
      .AddSyst(cb, "SV_eff_2svS", "rateParam", SystMapFunc<>::init
 	("(@0*@0)","SV_eff"));
  
//  cb.PrintAll();

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


  cb.cp().backgrounds().bin(VS().a(".*0L.*.*0j.*S_.*.*gamT1.*").a(".*0L.*.*1j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_0L_01jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*2j.*S_.*.*gamT1.*").a(".*0L.*.*3j.*S_.*.*gamT1.*").a(".*0L.*.*4j.*S_.*.*gamT1.*").a(".*0L.*.*5j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_0L_2345jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*0j.*S_.*.*gamT1.*").a(".*1L.*.*1j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_1L_01jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*2j.*S_.*.*gamT1.*").a(".*1L.*.*3j.*S_.*.*gamT1.*").a(".*1L.*.*4j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_1L_234jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*0j.*S_.*.*gamT1.*").a(".*2L.*.*1j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_2L_01jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*2j.*S_.*.*gamT1.*").a(".*2L.*.*3j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_2L_23jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*3L.*.*0j.*S_.*.*gamT1.*").a(".*3L.*.*1j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_3L_01jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*0j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_0jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*1j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_1jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*2j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_2jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*3j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_3jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*4j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_4jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*5j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_5jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*0j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_0jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*1j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_1jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*2j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_2jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*3j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_3jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*4j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_4jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*0j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_2L_0jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*1j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_2L_1jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*2j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_2L_2jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*3j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_2L_3jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*3L.*.*0j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_3L_0jS", "lnN", SystMap<>::init(1.1));

  cb.cp().backgrounds().bin(VS().a(".*3L.*.*1j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_3L_1jS", "lnN", SystMap<>::init(1.1));

  // if(WJetsnorms_highLow){
  // cb.cp().process({"Wjets"}).bin(VS().a(".*0L.*.*0j.*S_.*").a(".*0L.*.*1j.*S_.*"))
  //   .AddSyst(cb, "norm_Wjets_0L_01jS", "lnN", SystMap<>::init(1.8));
  // 
  // cb.cp().process({"Wjets"}).bin(VS().a(".*0L.*.*2j.*S_.*").a(".*0L.*.*3j.*S_.*").a(".*0L.*.*4j.*S_.*").a(".*0L.*.*5j.*S_.*"))
  //   .AddSyst(cb, "norm_Wjets_0L_2345jS", "lnN", SystMap<>::init(1.8));

  // cb.cp().process({"Wjets"}).bin(VS().a(".*1L.*.*0j.*S_.*").a(".*1L.*.*1j.*S_.*"))
  //   .AddSyst(cb, "norm_Wjets_1L_01jS", "lnN", SystMap<>::init(1.8));
  // 
  // cb.cp().process({"Wjets"}).bin(VS().a(".*1L.*.*2j.*S_.*").a(".*1L.*.*3j.*S_.*").a(".*1L.*.*4j.*S_.*"))
  //   .AddSyst(cb, "norm_Wjets_1L_234jS", "lnN", SystMap<>::init(1.8));

  // cb.cp().process({"Wjets"}).bin(VS().a(".*2L.*.*0j.*S_.*").a(".*2L.*.*1j.*S_.*"))
  //   .AddSyst(cb, "norm_Wjets_2L_01jS", "lnN", SystMap<>::init(1.8));
  // 
  // cb.cp().process({"Wjets"}).bin(VS().a(".*2L.*.*2j.*S_.*").a(".*2L.*.*3j.*S_.*"))
  //   .AddSyst(cb, "norm_Wjets_2L_23jS", "lnN", SystMap<>::init(1.8));
  // 
  // cb.cp().process({"Wjets"}).bin(VS().a(".*3L.*.*0j.*S_.*").a(".*3L.*.*1j.*S_.*"))
  //   .AddSyst(cb, "norm_Wjets_3L_01jS", "lnN", SystMap<>::init(1.8));
  // }
  if(WJetsnorms){
    ProcessList plist = backgrounds.Filter("Wjets");
    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_0L_0jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_0L_1jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*2j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_0L_2jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*3j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_0L_3jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*4j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_0L_4jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*5j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_0L_5jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_1L_0jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_1L_1jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*2j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_1L_2jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*3j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_1L_3jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*4j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_1L_4jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_2L_0jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_2L_1jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*2j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_2L_2jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*3j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_2L_3jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_3L_0jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_Wjets_3L_1jS", "lnN", SystMap<>::init(1.2));
  }
  if(QCDnorms){
    ProcessList plist = backgrounds.Filter("QCD");
    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_QCD_0L_0jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_QCD_0L_1jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*2j.*S_.*"))
      .AddSyst(cb, "norm_QCD_0L_2jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*3j.*S_.*"))
      .AddSyst(cb, "norm_QCD_0L_3jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*4j.*S_.*"))
      .AddSyst(cb, "norm_QCD_0L_4jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*5j.*S_.*"))
      .AddSyst(cb, "norm_QCD_0L_5jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_QCD_1L_0jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_QCD_1L_1jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*2j.*S_.*"))
      .AddSyst(cb, "norm_QCD_1L_2jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*3j.*S_.*"))
      .AddSyst(cb, "norm_QCD_1L_3jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*4j.*S_.*"))
      .AddSyst(cb, "norm_QCD_1L_4jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_QCD_2L_0jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_QCD_2L_1jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*2j.*S_.*"))
      .AddSyst(cb, "norm_QCD_2L_2jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*3j.*S_.*"))
      .AddSyst(cb, "norm_QCD_2L_3jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_QCD_3L_0jS", "lnN", SystMap<>::init(1.5));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_QCD_3L_1jS", "lnN", SystMap<>::init(1.5));
  }
  if(bkgNorms_noQCD){
    ProcessList plist = backgrounds.Remove("QCD");
    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_0L_0jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_0L_1jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*2j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_0L_2jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*3j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_0L_3jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*4j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_0L_4jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*5j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_0L_5jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_1L_0jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_1L_1jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*2j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_1L_2jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*3j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_1L_3jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*4j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_1L_4jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_2L_0jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_2L_1jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*2j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_2L_2jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_3L_0jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_3L_1jS", "lnN", SystMap<>::init(1.2));
  }
  if(bkgNorms_noQCD_noWjets){
    ProcessList plist = backgrounds.Remove("QCD");
    plist = plist.Remove("Wjets");
    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_0L_0jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_0L_1jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*2j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_0L_2jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*3j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_0L_3jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*4j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_0L_4jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*5j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_0L_5jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_1L_0jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_1L_1jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*2j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_1L_2jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*3j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_1L_3jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*4j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_1L_4jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_2L_0jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_2L_1jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*2L.*.*2j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_2L_2jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*0j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_3L_0jS", "lnN", SystMap<>::init(1.2));

    cb.cp().process(plist.GetProcesses()).bin(VS().a(".*3L.*.*1j.*S_.*"))
      .AddSyst(cb, "norm_bkgs_3L_1jS", "lnN", SystMap<>::init(1.2));
  }
  // if(bkgNorms_bosonTopSplit){
  //   for(auto p : bkg_boson){
  //     ProcessList plist = backgrounds.Filter(p);
  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*0j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_0L_0jS", "lnN", SystMap<>::init(1.8));
  // 
  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*1j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_0L_1jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*2j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_0L_2jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*3j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_0L_3jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*4j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_0L_4jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*5j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_0L_5jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*0j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_1L_0jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*1j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_1L_1jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*2j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_1L_2jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*3j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_1L_3jS", "lnN", SystMap<>::init(1.8));
  //   
  //   cb.cp().backgrounds().bin(VS().a(".*1L.*.*4j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_1L_4jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().backgrounds().bin(VS().a(".*2L.*.*0j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_2L_0jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().backgrounds().bin(VS().a(".*2L.*.*1j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_2L_1jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().backgrounds().bin(VS().a(".*2L.*.*2j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_2L_2jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().backgrounds().bin(VS().a(".*2L.*.*3j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_2L_3jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().backgrounds().bin(VS().a(".*3L.*.*0j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_3L_0jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().backgrounds().bin(VS().a(".*3L.*.*1j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_boson_3L_1jS", "lnN", SystMap<>::init(1.8));

  //   }
  //   for(auto p : bkg_top){
  //     ProcessList plist = backgrounds.Filter(p);
  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*0j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_0L_0jS", "lnN", SystMap<>::init(1.8));
  // 
  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*1j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_0L_1jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*2j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_0L_2jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*3j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_0L_3jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*4j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_0L_4jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*0L.*.*5j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_0L_5jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*0j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_1L_0jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*1j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_1L_1jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*2j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_1L_2jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().process(plist.GetProcesses()).bin(VS().a(".*1L.*.*3j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_1L_3jS", "lnN", SystMap<>::init(1.8));
  //   
  //   cb.cp().backgrounds().bin(VS().a(".*1L.*.*4j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_1L_4jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().backgrounds().bin(VS().a(".*2L.*.*0j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_2L_0jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().backgrounds().bin(VS().a(".*2L.*.*1j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_2L_1jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().backgrounds().bin(VS().a(".*2L.*.*2j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_2L_2jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().backgrounds().bin(VS().a(".*2L.*.*3j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_2L_3jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().backgrounds().bin(VS().a(".*3L.*.*0j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_3L_0jS", "lnN", SystMap<>::init(1.8));

  //   cb.cp().backgrounds().bin(VS().a(".*3L.*.*1j.*S_.*"))
  //     .AddSyst(cb, "norm_bkgs_top_3L_1jS", "lnN", SystMap<>::init(1.8));

  //   }
  // }






  // turn off regex filtering
  cb.SetFlag("filters-use-regex", false);

}

void FitConfiguration::AddFloatingNorms(VS& float_procs, ch::CombineHarvester& cb, ProcessList& all_procs){
  cb.SetFlag("filters-use-regex", true);

  for(auto p : float_procs){
    ProcessList plist = all_procs.Filter(kBkg).Filter(p);
    cb.cp().process(plist.GetProcesses())
      .AddSyst(cb, "scale_"+p, "rateParam", SystMap<>::init(1.0));
  }
  
  cb.SetFlag("filters-use-regex", false);
}

void FitConfiguration::AddRareNorms(VS& rare_procs, double uncert, ch::CombineHarvester& cb, ProcessList& all_procs){
  cb.SetFlag("filters-use-regex", true);

  for(auto p : rare_procs){
    ProcessList plist = all_procs.Filter(kBkg).Filter(p);
    cb.cp().process(plist.GetProcesses())
      .AddSyst(cb, "lumi_$ERA", "lnN", SystMap<era>::init
	       ({"2016"}, 1.022)
	       ({"2017"}, 1.022)
	       ({"2018"}, 1.022));
    
    cb.cp().process(plist.GetProcesses())
      .AddSyst(cb, "scale_"+p, "lnN", SystMap<>::init(1.+uncert));
  }
  
  cb.SetFlag("filters-use-regex", false);
}

void FitConfiguration::AddCommonSys(ch::CombineHarvester& cb, ProcessList& processes){
  cb.SetFlag("filters-use-regex", true);

  // signal xsec uncertainty
  cb.cp().signals()
    .AddSyst(cb, "sig_xsec", "lnN", SystMap<>::init(1.05));
  
  cb.SetFlag("filters-use-regex", false);
}

void FitConfiguration::AddFakeLeptonSys(ch::CombineHarvester& cb, ProcessList& processes){
  cb.SetFlag("filters-use-regex", true);

  // // overall factors for amount of each type of fake
  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf0.*"))
    .AddSyst(cb, "Fakes_muf0", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf1.*"))
    .AddSyst(cb, "Fakes_muf1", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf0.*"))
    .AddSyst(cb, "Fakes_elf0", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf1.*"))
    .AddSyst(cb, "Fakes_elf1", "rateParam", SystMap<>::init(1.0));

  // // SFs for lepton ID factors
  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf0.*")).bin(VS().a(".*muB.*"))
    .AddSyst(cb, "IDISO_muf0", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf1.*")).bin(VS().a(".*muB.*"))
    .AddSyst(cb, "IDISO_muf1", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf0.*")).bin(VS().a(".*elB.*"))
    .AddSyst(cb, "IDISO_elf0", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf1.*")).bin(VS().a(".*elB.*"))
    .AddSyst(cb, "IDISO_elf1", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf0.*")).bin(VS().a(".*muS.*"))
    .AddSyst(cb, "SIP3D_muf0", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf1.*")).bin(VS().a(".*muS.*"))
    .AddSyst(cb, "SIP3D_muf1", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf0.*")).bin(VS().a(".*elS.*"))
    .AddSyst(cb, "SIP3D_elf0", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf1.*")).bin(VS().a(".*elS.*"))
    .AddSyst(cb, "SIP3D_elf1", "rateParam", SystMap<>::init(1.0));
  
  cb.SetFlag("filters-use-regex", false);
}

void FitConfiguration::AddSVSys(ch::CombineHarvester& cb, ProcessList& processes){
  cb.SetFlag("filters-use-regex", true);

  cb.cp().backgrounds().bin(VS().a(".*1svS.*"))
    .AddSyst(cb, "SV_eff", "lnN", SystMap<>::init(1.08));

  cb.cp().backgrounds().bin(VS().a(".*2svS.*"))
    .AddSyst(cb, "SV_eff", "lnN", SystMap<>::init(1.16));

  cb.cp().backgrounds().bin(VS().a(".*SVeta1.*")).process(VS().a("ttbar").a("ST"))
    .AddSyst(cb, "SV_eta_top", "lnN", SystMap<>::init(1.05));

  cb.cp().backgrounds().bin(VS().a(".*SVeta1.*")).process(VS().a("Wjets").a("ZDY").a("TB"))
    .AddSyst(cb, "SV_eta_other", "lnN", SystMap<>::init(1.05));
  
  cb.SetFlag("filters-use-regex", false);
}

void FitConfiguration::AddKinematicSys(ch::CombineHarvester& cb, ProcessList& processes){
  cb.SetFlag("filters-use-regex", true);

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*0j.*S_.*.*gamT1.*").a(".*0L.*.*1j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_0L_01jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*2j.*S_.*.*gamT1.*").a(".*0L.*.*3j.*S_.*.*gamT1.*").a(".*0L.*.*4j.*S_.*.*gamT1.*").a(".*0L.*.*5j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_0L_2345jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*0j.*S_.*.*gamT1.*").a(".*1L.*.*1j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_1L_01jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*2j.*S_.*.*gamT1.*").a(".*1L.*.*3j.*S_.*.*gamT1.*").a(".*1L.*.*4j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_1L_234jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*0j.*S_.*.*gamT1.*").a(".*2L.*.*1j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_2L_01jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*2j.*S_.*.*gamT1.*").a(".*2L.*.*3j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_2L_23jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*3L.*.*0j.*S_.*.*gamT1.*").a(".*3L.*.*1j.*S_.*.*gamT1.*"))
    .AddSyst(cb, "gamT_3L_01jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*0j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_0jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*1j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_1jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*2j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_2jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*3j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_3jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*4j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_4jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*0L.*.*5j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_0L_5jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*0j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_0jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*1j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_1jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*2j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_2jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*3j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_3jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*1L.*.*4j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_1L_4jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*0j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_2L_0jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*1j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_2L_1jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*2j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_2L_2jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*2L.*.*3j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_2L_3jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*3L.*.*0j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_3L_0jS", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().bin(VS().a(".*3L.*.*1j.*S_.*.*PTISR1.*"))
    .AddSyst(cb, "PTISR_3L_1jS", "rateParam", SystMap<>::init(1.0));
  
  cb.SetFlag("filters-use-regex", false);
}

void FitConfiguration::AddSJetNormSys(const string& label, VS& procs, ch::CombineHarvester& cb, ProcessList& processes){
  cb.SetFlag("filters-use-regex", true);

  std::map<string, VS> SJets;
  
  //ProcessList plist = processes.Filter(kBkg).Filter("Wjets");
  
  ProcessList plist;
  for(auto p : procs){
    plist += processes.Filter(kBkg).Filter(p);
  }

  SJets["0L_0jS"] = VS().a(".*0L.*.*0j.*S_.*");
  SJets["0L_1jS"] = VS().a(".*0L.*.*1j.*S_.*");
  SJets["0L_2jS"] = VS().a(".*0L.*.*2j.*S_.*");
  SJets["0L_3jS"] = VS().a(".*0L.*.*3j.*S_.*");
  SJets["0L_4jS"] = VS().a(".*0L.*.*4j.*S_.*");
  SJets["0L_5jS"] = VS().a(".*0L.*.*5j.*S_.*");
  SJets["1L_0jS"] = VS().a(".*1L.*.*0j.*S_.*");
  SJets["1L_1jS"] = VS().a(".*1L.*.*1j.*S_.*");
  SJets["1L_2jS"] = VS().a(".*1L.*.*2j.*S_.*");
  SJets["1L_3jS"] = VS().a(".*1L.*.*3j.*S_.*");
  SJets["1L_4jS"] = VS().a(".*1L.*.*4j.*S_.*");
  SJets["2L_0jS"] = VS().a(".*2L.*.*0j.*S_.*");
  SJets["2L_1jS"] = VS().a(".*2L.*.*1j.*S_.*");
  SJets["2L_2jS"] = VS().a(".*2L.*.*2j.*S_.*");
  SJets["2L_3jS"] = VS().a(".*2L.*.*3j.*S_.*");
  SJets["3L_0jS"] = VS().a(".*3L.*.*0j.*S_.*");
  SJets["3L_1jS"] = VS().a(".*3L.*.*1j.*S_.*");

  for(auto s : SJets){
    string name = "norm_" + label + s.first;
    cb.cp().process(plist.GetProcesses()).bin(s.second)
      .AddSyst(cb, name, "lnN", SystMap<>::init(1.2));
  }
   
  cb.SetFlag("filters-use-regex", false);
}

void FitConfiguration::AddShapeSysAsNorm(const Systematic& sys, ch::CombineHarvester& cb, FitReader& FIT){
  cb.SetFlag("filters-use-regex", true);

  ProcessList processes = FIT.GetProcesses();
  CategoryList categories = FIT.GetCategories();
  VS channels = FIT.GetChannels();

  // prepare channels/categories
  map<string,CategoryList> chanMap;
  for(auto c : channels)
    chanMap[c] = categories.Filter(c);
  categories.Clear();
  for(auto c : channels)
    categories += chanMap[c];
  
  int Nproc = processes.GetN();
  int Ncat  = categories.GetN();

  for(int ip = 0; ip < Nproc; ip++){
    Process p = processes[ip];
    
    if(!FIT.HasSystematic(p, sys))
      continue;

    for(int ic = 0; ic < Ncat; ic++){
      Category c = categories[ic];

      if(!FIT.IsThere(c, p) ||
	 !FIT.IsThere(c, p, sys))
	continue;
      
      double nom = FIT.Integral(c, p);
//cout << "nom: " << nom << endl;
      if(nom <= 0.) continue;
//cout << "cat: " << c.FullLabel() << " proc: " << p.Name() << " sys: " << sys.Label() << endl;      
      double up  = FIT.Integral(c, p, sys.Up());
//cout << "up: " << up << endl;     
 double dn  = FIT.Integral(c, p, sys.Down());
//cout << "down: " << dn << endl;     
      double err = up - dn;
  //   cout << "up: " << up << " down: " << dn << " err: " << err << endl; 
      if(fabs(err) < fabs(up-nom)) err = up - nom;
      if(fabs(err) < fabs(nom-dn)) err = nom - dn;

    // cout << "1 err: " << err << endl;
      err = 1. + err/2./nom;
    // cout << "2 err: " << err << endl;
      if(err > 0.)
      cb.cp().process(VS().a(p.Name())).bin(VS().a(c.FullLabel()))
	.AddSyst(cb, "norm_"+sys.Label(), "lnN", SystMap<>::init(err));

    }
  }
  
  cb.SetFlag("filters-use-regex", false);
}

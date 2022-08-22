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
  cb.cp().backgrounds().process(VS().a(".*_Fakes_.*muf0.*"))
    .AddSyst(cb, "Fakes_muf0", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_.*muf1.*"))
    .AddSyst(cb, "Fakes_muf1", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_.*elf0.*"))
    .AddSyst(cb, "Fakes_elf0", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_.*elf1.*"))
    .AddSyst(cb, "Fakes_elf1", "rateParam", SystMap<>::init(1.0));
/* temporarily removing these until updated
  // // SFs for lepton ID factors
  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf0.*")).bin(VS().a(".*mu.*bron.*"))
    .AddSyst(cb, "IDISO_muf0", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf1.*")).bin(VS().a(".*mu.*bron.*"))
    .AddSyst(cb, "IDISO_muf1", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf0.*")).bin(VS().a(".*el.*bron.*"))
    .AddSyst(cb, "IDISO_elf0", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf1.*")).bin(VS().a(".*el.*bron.*"))
    .AddSyst(cb, "IDISO_elf1", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf0.*")).bin(VS().a(".*mu.*slvr.*"))
    .AddSyst(cb, "SIP3D_muf0", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_muf1.*")).bin(VS().a(".*mu.*slvr.*"))
    .AddSyst(cb, "SIP3D_muf1", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf0.*")).bin(VS().a(".*el.*slvr.*"))
    .AddSyst(cb, "SIP3D_elf0", "rateParam", SystMap<>::init(1.0));

  cb.cp().backgrounds().process(VS().a(".*_Fakes_elf1.*")).bin(VS().a(".*el.*slvr.*"))
    .AddSyst(cb, "SIP3D_elf1", "rateParam", SystMap<>::init(1.0));
 */ 
  cb.SetFlag("filters-use-regex", false);
}

void FitConfiguration::AddBJetSys(ch::CombineHarvester& cb, ProcessList& processes){
  cb.SetFlag("filters-use-regex", true);

  cb.cp().backgrounds().bin(VS().a(".*0L.*_0j.*1bISR.*").a(".*0L.*_1j.*1bISR.*"))
     .AddSyst(cb, "BTAG_ISR_0L_01J", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*0L.*_2j.*1bISR.*"))
     .AddSyst(cb, "BTAG_ISR_0L_2J", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*0L.*_3j.*1bISR.*").a(".*0L.*_4j.*1bISR.*").a(".*0L.*5j.*1bISR.*"))
     .AddSyst(cb, "BTAG_ISR_0L_345J", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*0L.*_1j.*1b.*S.*"))
     .AddSyst(cb, "BTAG_S_0L_1J", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*0L.*_2j.*1bS.*"))
     .AddSyst(cb, "BTAG_S_0L_2J", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*0L.*_2j.*2bS.*"))
     .AddSyst(cb, "BTAG_S_0L_2J_2b", "rateParam", SystMapFunc<>::init
         ("(@0*@0)","BTAG_S_0L_2J"));

  cb.cp().backgrounds().bin(VS().a(".*0L.*_3j.*1bS.*").a(".*0L.*_4j.*1bS.*").a(".*0L.*5j.*1bS.*"))
     .AddSyst(cb, "BTAG_S_0L_345J", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*0L.*_3j.*2bS.*").a(".*0L.*_4j.*2bS.*").a(".*0L.*5j.*2bS.*"))
     .AddSyst(cb, "BTAG_S_0L_345J_2b", "rateParam", SystMapFunc<>::init
         ("(@0*@0)","BTAG_S_0L_345J"));

  cb.cp().backgrounds().bin(VS().a(".*1L.*_0j.*1bISR.*").a(".*1L.*_1j.*1bISR.*"))
     .AddSyst(cb, "BTAG_ISR_1L_01J", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*1L.*_2j.*1bISR.*"))
     .AddSyst(cb, "BTAG_ISR_1L_2J", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*0L.*_3j.*1bISR.*").a(".*0L.*4j.*1bISR.*"))
     .AddSyst(cb, "BTAG_ISR_1L_34J", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*1L.*_1j.*1.*bS.*"))
     .AddSyst(cb, "BTAG_S_1L_1J", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*1L.*_2j.*1bS.*"))
     .AddSyst(cb, "BTAG_S_1L_2J", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*1L.*_2j.*2bS.*"))
     .AddSyst(cb, "BTAG_S_1L_2J_2b", "rateParam", SystMapFunc<>::init
         ("(@0*@0)","BTAG_S_1L_2J"));

  cb.cp().backgrounds().bin(VS().a(".*1L.*_3j.*1bS.*").a(".*1L.*4j.*1bS.*"))
     .AddSyst(cb, "BTAG_S_1L_34J", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*1L.*_3j.*2bS.*").a(".*1L.*4j.*2bS.*"))
     .AddSyst(cb, "BTAG_S_1L_34J_2b", "rateParam", SystMapFunc<>::init
         ("(@0*@0)","BTAG_S_1L_34J"));

  cb.cp().backgrounds().bin(VS().a(".*2L.*1bISR.*"))
     .AddSyst(cb, "BTAG_ISR_2L", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*2L.*1j1bS.*"))
     .AddSyst(cb, "BTAG_S_2L_1J", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*2L.*ge2jge1bS.*"))
     .AddSyst(cb, "BTAG_S_2L_2J", "rateParam", SystMap<>::init(1.00));

  cb.SetFlag("filters-use-regex", false);
}

void FitConfiguration::AddSVSys(ch::CombineHarvester& cb, ProcessList& processes){
  cb.SetFlag("filters-use-regex", true);

  cb.cp().backgrounds().bin(VS().a(".*1svS.*"))
     .AddSyst(cb, "SV_eff", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*2svS.*"))
    .AddSyst(cb, "SV_eff_2svS", "rateParam", SystMapFunc<>::init
 	("(@0*@0)","SV_eff"));

  cb.cp().backgrounds().bin(VS().a(".*SVeta1.*")).process(VS().a("ttbar").a("ST"))
    .AddSyst(cb, "SV_eta_top", "rateParam", SystMap<>::init(1.00));

  cb.cp().backgrounds().bin(VS().a(".*SVeta1.*")).process(VS().a("Wjets").a("ZDY").a("TB"))
    .AddSyst(cb, "SV_eta_other", "rateParam", SystMap<>::init(1.00));
  
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
void FitConfiguration::appSystDict( SystDict& sd , std::string label, std::vector<int> list ){
        std::vector<std::pair<int,int>> plist{};
        for(int i=0; i< list.size(); i= i+2){
                plist.push_back( std::pair<int,int>(list.at(i), list.at(i+1)));
        }
        sd[label] = plist;
}
void FitConfiguration::initSystDict( SystDict& sd){
        //hardcoding structures for hierarchy

	//wjets hierarchy        	
	appSystDict(sd, "Wjets0L_0jS_d3", std::vector<int>{0,0, -1,1 });
        appSystDict(sd, "Wjets0L_1jS_d2", std::vector<int>{0,1, 0,0, -1,0 });
        appSystDict(sd, "Wjets0L_2jS_d1", std::vector<int>{1,0, 0,2, 0,0, 0,1, 0,3, 0,4, 0,5, -1,0 });
        appSystDict(sd, "Wjets0L_3jS_d2", std::vector<int>{0,3, 0,4, 0,5, -1,0 });
        appSystDict(sd, "Wjets0L_4jS_d3", std::vector<int>{0,4, 0,5, -1,1 });
        appSystDict(sd, "Wjets0L_5jS_d4", std::vector<int>{0,5, -1,1});

	appSystDict(sd, "Wjets1L_0jS_d0", std::vector<int>{1,0, 1,1, 1,2, 1,3, 1,4, -1,0 });
        appSystDict(sd, "Wjets1L_1jS_d1", std::vector<int>{1,1, 1,2, 1,3, 1,4, -1,0 });
        appSystDict(sd, "Wjets1L_2jS_d2", std::vector<int>{1,2, 1,3, 1,4, -1,0 });
        appSystDict(sd, "Wjets1L_3jS_d3", std::vector<int>{1,3, 1,4, -1,1});
        appSystDict(sd, "Wjets1L_4jS_d4", std::vector<int>{1,4, -1,1});

	appSystDict(sd, "Wjets2L_0jS_d1", std::vector<int>{1,0, 2,0, 2,1, 2,2, -1,0 });
        appSystDict(sd, "Wjets2L_1jS_d2", std::vector<int>{2,0, 2,1, -1,1 });
        appSystDict(sd, "Wjets2L_2jS_d3", std::vector<int>{2,2, -1,1 });

	appSystDict(sd, "Wjets3L_0jS_d1", std::vector<int>{1,0, 2,0, 3,0, 3,1, -1,1});
        appSystDict(sd, "Wjets3L_1jS_d2", std::vector<int>{3,1, -1,1});

/*
	//ttjets decoupled norms
	appSystDict(sd, "norm_ttbar0L_0jS", std::vector<int>{0,0, -1,1 });
        appSystDict(sd, "norm_ttbar0L_1jS", std::vector<int>{0,1, -1,1 });
        appSystDict(sd, "norm_ttbar0L_2jS", std::vector<int>{0,2, -1,1 });
        appSystDict(sd, "norm_ttbar0L_3jS", std::vector<int>{0,3, -1,1 });
        appSystDict(sd, "norm_ttbar0L_4jS", std::vector<int>{0,4, -1,1 });
        appSystDict(sd, "norm_ttbar0L_5jS", std::vector<int>{0,5, -1,1 });

        appSystDict(sd, "norm_ttbar1L_0jS", std::vector<int>{1,0, -1,1 });
        appSystDict(sd, "norm_ttbar1L_1jS", std::vector<int>{1,1, -1,1 });
        appSystDict(sd, "norm_ttbar1L_2jS", std::vector<int>{1,2, -1,1 });
        appSystDict(sd, "norm_ttbar1L_3jS", std::vector<int>{1,3, -1,1 });
        appSystDict(sd, "norm_ttbar1L_4jS", std::vector<int>{1,4, -1,1 });

        appSystDict(sd, "norm_ttbar2L_0jS", std::vector<int>{2,0, -1,1 });
        appSystDict(sd, "norm_ttbar2L_1jS", std::vector<int>{2,1, -1,1 });
        appSystDict(sd, "norm_ttbar2L_2jS", std::vector<int>{2,2, -1,1 });

        appSystDict(sd, "norm_ttbar3L_0jS", std::vector<int>{3,0, -1,1});
        appSystDict(sd, "norm_ttbar3L_1jS", std::vector<int>{3,1, -1,1});


	//ttbar hierarchy
	appSystDict(sd, "norm_ttbar0L_0jS", std::vector<int>{0,0, -1,1 });
        appSystDict(sd, "norm_ttbar0L_1jS", std::vector<int>{0,1, 0,0  -1,1 });
        appSystDict(sd, "norm_ttbar0L_2jS", std::vector<int>{0,2, 0,1, 0,0, -1,0 });
        appSystDict(sd, "norm_ttbar0L_3jS", std::vector<int>{2,2, 0,3, 0,2, 0,1, 0,0, 0,4, 0,5, -1,0 });
        appSystDict(sd, "norm_ttbar0L_4jS", std::vector<int>{0,4, 0,5, -1,0 });
        appSystDict(sd, "norm_ttbar0L_5jS", std::vector<int>{0,5, -1,1 });

        appSystDict(sd, "norm_ttbar1L_0jS", std::vector<int>{1,0, -1,1 });
        appSystDict(sd, "norm_ttbar1L_1jS", std::vector<int>{1,1, 1,0, -1,0 });
        appSystDict(sd, "norm_ttbar1L_2jS", std::vector<int>{1,2, 1,1, 1,0, -1,0 });
        appSystDict(sd, "norm_ttbar1L_3jS", std::vector<int>{2,2, 1,3, 1,2, 1,1, 1,0, 1,4, -1,0 });
        appSystDict(sd, "norm_ttbar1L_4jS", std::vector<int>{1,4, -1,1 });

        appSystDict(sd, "norm_ttbar2L_0jS", std::vector<int>{2,0, -1,1 });
        appSystDict(sd, "norm_ttbar2L_1jS", std::vector<int>{2,1, 2,0, -1,0 });
        appSystDict(sd, "norm_ttbar2L_2jS", std::vector<int>{2,2, 2,1, -1,0 });

        appSystDict(sd, "norm_ttbar3L_0jS", std::vector<int>{3,0, -1,1});
        appSystDict(sd, "norm_ttbar3L_1jS", std::vector<int>{2,2, 3,1, 3,0, -1,1});
*/
/*
	appSystDict(sd, "Other0L_0jS_d3", std::vector<int>{0,0, -1,1 });
        appSystDict(sd, "Other0L_1jS_d2", std::vector<int>{0,1, 0,0, -1,0 });
        appSystDict(sd, "Other0L_2jS_d1", std::vector<int>{2,2, 0,2, 0,0, 0,1, 0,3, 0,4, 0,5, -1,0 });
        appSystDict(sd, "Other0L_3jS_d2", std::vector<int>{0,3, 0,4, 0,5, -1,0 });
        appSystDict(sd, "Other0L_4jS_d3", std::vector<int>{0,4, 0,5, -1,0 });
        appSystDict(sd, "Other0L_5jS_d4", std::vector<int>{0,5, -1,1});
*/
	

	
}
void FitConfiguration::AddNormHierarchy( SystDict& sd, VS& proc, ch::CombineHarvester& cb, ProcessList& processes){
	cb.SetFlag("filters-use-regex", true);
	//create standard string mapping
	std::map<int, string> Lmap;
	std::map<int, string> Jmap;
	Lmap[0] = ".*0L.*.*";
	Lmap[1] = ".*1L.*.*";
	Lmap[2] = ".*2L.*.*";
        Lmap[3] = ".*3L.*.*";
	Jmap[0] = "0j.*S_.*";
	Jmap[1] = "1j.*S_.*";
	Jmap[2] = "2j.*S_.*";
	Jmap[3] = "3j.*S_.*";
	Jmap[4] = "4j.*S_.*";
	Jmap[5] = "5j.*S_.*";

	ProcessList plist;
  	for(auto p : proc){
    	plist += processes.Filter(kBkg).Filter(p);
  	}

	std::cout<<"printing data structure"<<std::endl;
	//loop over test data structure and print
	for(auto s: sd){
		std::cout<<"KEY "<<s.first<<std::endl;
		std::cout<<"VALUE ";
		for(int i=0; i<s.second.size(); i++){
			std::cout<<"("<<s.second[i].first<<","<<s.second[i].second<<") ";	
		}
		std::cout<<std::endl;
	}


	VS systRegex;
	int rate=0; //in pairs with key==-1 --> 0=rateParam 1=lnN
	std::string paramType;
	double paramInit;
	for(auto s: sd){
		std::cout<<"systematic named "<<s.first<<"\n";
		std::cout<<"generating regex string ... \n";
		for(int i=0; i<s.second.size(); i++){
			if( s.second[i].first != -1 ){
				systRegex.a( Lmap[ s.second[i].first ] + Jmap[ s.second[i].second ]);
			}
			if( s.second[i].first == -1 ){
				rate= s.second[i].second;
			}
			
		}
		for(int j=0; j<systRegex.size(); j++){
			std::cout<<systRegex.at(j)<<" ";
		}
		std::cout<<"\n";
		std::cout<<"paramter type rate == "<<rate<<"\n";
		
		if(rate == 0 ){
			paramType = "rateParam";
			paramInit = 1.00;
		}
		if(rate == 1 ){
			paramType = "lnN";
			paramInit = 1.2;
		}
		cb.cp().process(plist.GetProcesses()).bin(VS(systRegex)).AddSyst(cb, s.first , paramType , SystMap<>::init(paramInit));
			
		systRegex.clear();
	}
	
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

#include "TSystem.h"

#include "../include/PlotReader.hh"

void LoopPlotter(const string& inputfile = "BPI_2017.root", const string& outfold = "plots"){
  string year = "2018";

  
  PlotReader* plotter = new PlotReader(inputfile);
  plotter->SetPlotTitle("CR "+year+" Data + Simulation");
 
  VS proc;
  proc.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("Data");
  VS proc_obj;
  proc_obj.a("Bkg").a("Data");
  
  VS var_kin;
  var_kin.a("MET").a("RISR").a("Mperp").a("PTISR").a("Gperp");
  VS var_cnt;
  var_cnt.a("NSjet").a("Nbjet").a("NSbjet").a("NSV").a("Nel").a("Nmu");
  VS var_obj;
  var_obj.a("Eta").a("Phi").a("Pt");
  VS obj;
  obj.a("Jet").a("Bjet").a("El").a("Mu").a("SV");

  VS reg_kin;
  reg_kin.a("0L_0J").a("0L_1J").a("0L_2J").a("0L_3J").a("0L_4J").a("0L_5J");
  reg_kin.a("gold1L").a("slvr1L").a("bron1L").a("1L_0J").a("1L_1J").a("1L_2J").a("1L_3J").a("1L_4J");
  reg_kin.a("gold2L").a("slvr2L").a("bron2L").a("2L_0J").a("2L_1J").a("2L_2J");
  reg_kin.a("gold3L").a("slvr3L").a("bron3L").a("3L_0J").a("3L_1J");

  VS reg_cnt;
  reg_cnt.a("incl").a("0L").a("1L").a("2L").a("3L");

  gSystem->Exec(("rm -fr "+outfold).c_str());
  gSystem->Exec(("mkdir "+outfold).c_str());
  
  for(auto r : reg_kin){
    for(auto v : var_kin){
      TCanvas* can;
      string scan = v+"_"+r+"_"+year;
      if(v == "Mperp")
  	can = (TCanvas*) plotter->Plot1Dstack(scan,v, proc, r, true, 4, 0., 650.);
      else if(v == "RISR" || v == "Gperp")
  	can = (TCanvas*) plotter->Plot1Dstack(scan,v, proc, r, true, 8);
      else
  	can = (TCanvas*) plotter->Plot1Dstack(scan,v, proc, r, true, 4);
      can->SaveAs((outfold+"/"+scan+".gif").c_str());
      delete can;
    }
  }

  for(auto r : reg_cnt){
    for(auto v : var_cnt){
      TCanvas* can;
      string scan = v+"_"+r+"_"+year;
      if(v == "NSjet")
  	can = (TCanvas*) plotter->Plot1Dstack(scan,v, proc, r, true, 16, 0., 6.5);
      else if(v == "NSbjet" || v == "Nbjet")
	can = (TCanvas*) plotter->Plot1Dstack(scan,v, proc, r, true, 32, 0., 6.5);
      else 
  	can = (TCanvas*) plotter->Plot1Dstack(scan,v, proc, r, true, 32, 0., 3.5);
      can->SaveAs((outfold+"/"+scan+".gif").c_str());
      delete can;
    }
  }

  // for(auto o : obj){
  //   for(auto v : var_obj){
  //     string var = o+v;
  //     TCanvas* can;
  //     string scan = var+"_"+year;
  //     can = (TCanvas*) plotter->Plot1Dstack(scan, var, proc_obj, "p", true, 4);
  //   }
  // }


}

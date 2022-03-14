#include "TSystem.h"

#include "../include/PlotReader.hh"

void TestPlotter(const string& inputfile = "BPI_2017.root"){

  PlotReader* plotter = new PlotReader(inputfile);
  plotter->SetPlotTitle("CR 2017 Data + Simulation");
 
  VS all;
  all.a("ttbar").a("ST").a("DB").a("ZDY").a("TB").a("QCD").a("Wjets").a("Data");

  TCanvas* can = plotter->Plot1Dstack("test_can",
				      "RISR", all, "gold2L_2J");

}

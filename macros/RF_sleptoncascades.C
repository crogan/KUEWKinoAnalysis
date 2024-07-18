/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2019, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_H_to_WlnuWlnu.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 June
//
//   This file is part of RestFrames.
//
//   RestFrames is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   RestFrames is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with RestFrames. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#if (!defined(__CINT__) && !defined(__CLING__))
#define COMPILER
#endif
#if defined(__MAKECINT__) || defined(__ROOTCLING__) || defined(COMPILER)
#include "RestFrames/RestFrames.hh"
#else
RestFrames::RFKey ensure_autoload(1);
#endif

using namespace RestFrames;

// used for getting index of category name
int get_cat_index(const vector<string>& vect_names, const string& cat_name){
  if(std::find(vect_names.begin(), vect_names.end(), cat_name) != vect_names.end())
    return std::distance(vect_names.begin(), std::find(vect_names.begin(), vect_names.end(), cat_name));
  else return -1;
}

void RF_sleptoncascades(const std::string& output_name =
			   "output_sleptoncascades.root"){

  // generator options

  // backgrounds
  bool WZ = false;
  bool ZZ = false;
  bool WWZ_inv = false;
  bool WWZ_lep = false;

  // signals
  // production modes
  bool SmuSnu = true;
  bool SmuSmu = false;
  bool SnuSnu = false;
  bool compressed = true;

  // decay modes
  bool Smua_Z = true;
  bool Smub_Z = false;
  bool Snua_Z = false;
  bool Snub_Z = false;

  bool leptonic_Za = true;
  bool leptonic_Zb = false;
  bool leptonic_Wa = false;
  bool leptonic_Wb = false;

  // set particle masses and widths
  double mW   = 80.385; // GeV, PDG 2016
  double mZ   = 91.19;  // GeV, PDG 2016
  double wW   = 2.085;
  double wZ   = 2.5;
  double mL   = 0.106; // muons
  double mJ   = 1.;    // jets
  double mN   = 0.;
  double mSmu = 700.;
  double mSnu = 650.;
  double mN2  = 500.;
  double mC1  = 450.;
  double mN1  = 100.;

  if(compressed){
    mSmu = 300.;
    mSnu = 289.;
    mN2  = 280.;
    mC1  = 270.;
    mN1  = 260.;
  }

  // Number of events to generate
  int Ngen = 10000;

  double LAB_Pt = 300.;
  double LAB_eta = 2.5;

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing generator frames and tree..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  ppLabGenFrame     LAB_Gen("LAB_Gen","LAB");
  DecayGenFrame     SmuSnu_Gen("SmuSnu_Gen","#tilde{#mu}^{ #pm}_{L} #tilde{#nu}_{L}");
  DecayGenFrame     SmuSmu_Gen("SmuSmu_Gen","#tilde{#mu}^{ #pm}_{L} #tilde{#mu}^{ #mp}_{L}");
  DecayGenFrame     SnuSnu_Gen("SnuSnu_Gen","#tilde{#nu}_{L} #tilde{#nu}_{L}");
  DecayGenFrame     Smua_Gen("Smua_Gen","#tilde{#mu}^{ #pm}_{L}");
  DecayGenFrame     Smub_Gen("Smub_Gen","#tilde{#mu}^{ #pm}_{L}");
  DecayGenFrame     Snua_Gen("Snua_Gen","#tilde{#nu}_{L}");
  DecayGenFrame     Snub_Gen("Snub_Gen","#tilde{#nu}_{L}");
  ResonanceGenFrame X2a_Gen("X2a_Gen","#tilde{#chi}^{ 0}_{2}");
  ResonanceGenFrame X2b_Gen("X2b_Gen","#tilde{#chi}^{ 0}_{2}");
  ResonanceGenFrame X2c_Gen("X2c_Gen","#tilde{#chi}^{ 0}_{2}");
  ResonanceGenFrame X2d_Gen("X2d_Gen","#tilde{#chi}^{ 0}_{2}");
  ResonanceGenFrame C1a_Gen("C1a_Gen","#tilde{#chi}^{ #pm}_{1}");
  ResonanceGenFrame C1b_Gen("C1b_Gen","#tilde{#chi}^{ #pm}_{1}");
  ResonanceGenFrame C1c_Gen("C1c_Gen","#tilde{#chi}^{ #pm}_{1}");
  ResonanceGenFrame C1d_Gen("C1d_Gen","#tilde{#chi}^{ #pm}_{1}");
  ResonanceGenFrame Za_Gen("Za_Gen","Z");
  ResonanceGenFrame Zb_Gen("Zb_Gen","Z");
  ResonanceGenFrame Zc_Gen("Zc_Gen","Z");
  ResonanceGenFrame Zd_Gen("Zd_Gen","Z");
  ResonanceGenFrame Wa_Gen("Wa_Gen","W");
  ResonanceGenFrame Wb_Gen("Wb_Gen","W");
  ResonanceGenFrame Wc_Gen("Wc_Gen","W");
  ResonanceGenFrame Wd_Gen("Wd_Gen","W");
  VisibleGenFrame   La_Gen("La_Gen","#it{l}"); // direct from sparticle 
  VisibleGenFrame   Lb_Gen("Lb_Gen","#it{l}"); // direct from sparticle 
  VisibleGenFrame   Lc_Gen("Lc_Gen","#it{l}"); // direct from sparticle
  VisibleGenFrame   Ld_Gen("Ld_Gen","#it{l}"); // direct from sparticle
  VisibleGenFrame   J1a_Gen("J1a_Gen","#it{j}"); 
  VisibleGenFrame   J2a_Gen("J2a_Gen","#it{j}"); 
  VisibleGenFrame   L1a_Gen("L1a_Gen","#it{l}"); 
  VisibleGenFrame   L2a_Gen("L2a_Gen","#it{l}"); 
  VisibleGenFrame   J1b_Gen("J1b_Gen","#it{j}"); 
  VisibleGenFrame   J2b_Gen("J2b_Gen","#it{j}"); 
  VisibleGenFrame   J1c_Gen("J1c_Gen","#it{j}"); 
  VisibleGenFrame   J2c_Gen("J2c_Gen","#it{j}"); 
  VisibleGenFrame   J1d_Gen("J1d_Gen","#it{j}"); 
  VisibleGenFrame   J2d_Gen("J2d_Gen","#it{j}"); 
  VisibleGenFrame   L2b_Gen("L2b_Gen","#it{l}"); 
  InvisibleGenFrame N2b_Gen("N2b_Gen","#nu");    
  InvisibleGenFrame X1a_Gen("X1a_Gen","#tilde{#chi}^{ 0}_{1}");
  InvisibleGenFrame X1b_Gen("X1b_Gen","#tilde{#chi}^{ 0}_{1}");
  InvisibleGenFrame X1c_Gen("X1c_Gen","#tilde{#chi}^{ 0}_{1}");
  InvisibleGenFrame X1d_Gen("X1d_Gen","#tilde{#chi}^{ 0}_{1}");
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  // Background Gen Frames
  DecayGenFrame     WZ_Gen_Bkg("WZ_Gen_Bkg","WZ");
  DecayGenFrame     ZZ_Gen_Bkg("ZZ_Gen_Bkg","ZZ");
  DecayGenFrame     WWZ_Gen_Bkg("WWZ_Gen_Bkg","WWZ");
  ResonanceGenFrame Wa_Gen_Bkg("Wa_Gen_Bkg","W_{#it{a}}");
  ResonanceGenFrame Wb_Gen_Bkg("Wb_Gen_Bkg","W_{#it{b}}");
  ResonanceGenFrame Za_Gen_Bkg("Za_Gen_Bkg","Z_{#it{a}}");
  ResonanceGenFrame Zb_Gen_Bkg("Zb_Gen_Bkg","Z_{#it{b}}");  
  VisibleGenFrame   L1b_Gen("L1b_Gen","#it{l}"); // from Wb (leptonic)
  VisibleGenFrame   L1c_Gen("L1c_Gen","#it{l}"); // from Z (leptonic)
  VisibleGenFrame   L2c_Gen("L2c_Gen","#it{l}"); // from Z (leptonic)
  VisibleGenFrame   L2d_Gen("L2d_Gen","#it{l}"); // from Z (leptonic)
  InvisibleGenFrame N1a_Gen("N1a_Gen","#nu");    // from Wa (leptonic)
  InvisibleGenFrame N1b_Gen("N1b_Gen","#nu");    // from Wb (leptonic)
  InvisibleGenFrame N1c_Gen("N1c_Gen","#nu");    // from Z (invisible)
  InvisibleGenFrame N2c_Gen("N2c_Gen","#nu");    // from Z (invisible)
  InvisibleGenFrame N2d_Gen("N2d_Gen","#nu");    // from Z (invisible)

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  std::string plot_title = "";
  
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  bool bool_Smua = false;
  bool bool_Smub = false;
  bool bool_Snua = false;
  bool bool_Snub = false;

  bool bool_Za = false;
  bool bool_Zb = false;
  bool bool_Wa = false;
  bool bool_Wb = false;

  if(SmuSnu){
    LAB_Gen.SetChildFrame(SmuSnu_Gen);
    SmuSnu_Gen.AddChildFrame(Smua_Gen);
    SmuSnu_Gen.AddChildFrame(Snub_Gen);
    bool_Smua = true;
    bool_Snub = true;
    plot_title += "#tilde{#mu}^{ #pm}_{L} #tilde{#nu}_{L} #rightarrow ";
  }
  else if(SmuSmu){
    LAB_Gen.SetChildFrame(SmuSmu_Gen);
    SmuSmu_Gen.AddChildFrame(Smua_Gen);
    SmuSmu_Gen.AddChildFrame(Smub_Gen);
    bool_Smua = true;
    bool_Smub = true;
    plot_title += "#tilde{#mu}^{ #pm}_{L} #tilde{#mu}^{ #mp}_{L} #rightarrow ";
  }
  else if(SnuSnu){
    LAB_Gen.SetChildFrame(SnuSnu_Gen);
    SnuSnu_Gen.AddChildFrame(Snua_Gen);
    SnuSnu_Gen.AddChildFrame(Snub_Gen);
    bool_Snua = true;
    bool_Snub = true;
    plot_title += "#tilde{#nu}_{L} #tilde{#nu}_{L} #rightarrow ";
  }
  else if(WZ){
    LAB_Gen.SetChildFrame(WZ_Gen_Bkg);
    WZ_Gen_Bkg.AddChildFrame(Za_Gen_Bkg);
    WZ_Gen_Bkg.AddChildFrame(Wa_Gen_Bkg);
    Za_Gen_Bkg.AddChildFrame(L1a_Gen); 
    Za_Gen_Bkg.AddChildFrame(L2a_Gen); 
    //Wa_Gen_Bkg.AddChildFrame(L1b_Gen);
    //Wa_Gen_Bkg.AddChildFrame(N1b_Gen);
    Wa_Gen_Bkg.AddChildFrame(J1a_Gen);
    Wa_Gen_Bkg.AddChildFrame(J2a_Gen);
    plot_title += "WZ";
  }
  else if(ZZ){
    LAB_Gen.SetChildFrame(ZZ_Gen_Bkg);
    ZZ_Gen_Bkg.AddChildFrame(Za_Gen_Bkg);
    ZZ_Gen_Bkg.AddChildFrame(Zb_Gen_Bkg);
    Za_Gen_Bkg.AddChildFrame(L1a_Gen);
    Za_Gen_Bkg.AddChildFrame(L2a_Gen);
    //Zb_Gen_Bkg.AddChildFrame(L1b_Gen);
    //Zb_Gen_Bkg.AddChildFrame(L2b_Gen);
    Zb_Gen_Bkg.AddChildFrame(J1a_Gen);
    Zb_Gen_Bkg.AddChildFrame(J2a_Gen);
    plot_title += "ZZ";
  }
  else if(WWZ_lep || WWZ_inv){
    LAB_Gen.SetChildFrame(WWZ_Gen_Bkg);
    WWZ_Gen_Bkg.AddChildFrame(Wa_Gen_Bkg);
    WWZ_Gen_Bkg.AddChildFrame(Wb_Gen_Bkg);
    WWZ_Gen_Bkg.AddChildFrame(Za_Gen_Bkg);
    Wa_Gen_Bkg.AddChildFrame(L1a_Gen);
    Wa_Gen_Bkg.AddChildFrame(N1a_Gen);
    if(WWZ_lep){
      Wb_Gen_Bkg.AddChildFrame(J1a_Gen);
      Wb_Gen_Bkg.AddChildFrame(J2a_Gen);
      Za_Gen_Bkg.AddChildFrame(L1c_Gen);
      Za_Gen_Bkg.AddChildFrame(L2c_Gen);
     plot_title += "WWZ_lep";
    }
    else{
      Wb_Gen_Bkg.AddChildFrame(L1b_Gen);
      Wb_Gen_Bkg.AddChildFrame(N1b_Gen);
      //Za_Gen_Bkg.AddChildFrame(N1c_Gen);
      //Za_Gen_Bkg.AddChildFrame(N2c_Gen);
      Za_Gen_Bkg.AddChildFrame(J1a_Gen);
      Za_Gen_Bkg.AddChildFrame(J2a_Gen);
     plot_title += "WWZ_inv";
    }
  }
  else{
    g_Log << LogError << "Need to specify process to generate" << LogEnd;
  }

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
 
  if(SmuSnu || SmuSmu || SnuSnu){
    if(bool_Smua){
      if(Smua_Z){
        Smua_Gen.AddChildFrame(X2a_Gen);
        Smua_Gen.AddChildFrame(La_Gen);
        X2a_Gen.AddChildFrame(Za_Gen);
        X2a_Gen.AddChildFrame(X1a_Gen);
        bool_Za = true;
        plot_title += "#tilde{#chi}^{ 0}_{2} #it{l} ";
      }
      else{
        Smua_Gen.AddChildFrame(C1a_Gen);
        Smua_Gen.AddChildFrame(N1a_Gen);
        C1a_Gen.AddChildFrame(Wa_Gen);
        C1a_Gen.AddChildFrame(X1a_Gen);
        bool_Wa = true;
        plot_title += "#tilde{#chi}^{ #pm}_{1} #nu ";
      }
    }
    if(bool_Snua){
      if(Snua_Z){
        Snua_Gen.AddChildFrame(X2a_Gen);
        Snua_Gen.AddChildFrame(N1a_Gen);
        X2a_Gen.AddChildFrame(Za_Gen);
        X2a_Gen.AddChildFrame(X1a_Gen);
        bool_Za = true;
        plot_title += "#tilde{#chi}^{ 0}_{2} #nu ";
      }
      else{
        Snua_Gen.AddChildFrame(C1a_Gen);
        Snua_Gen.AddChildFrame(La_Gen);
        C1a_Gen.AddChildFrame(Wa_Gen);
        C1a_Gen.AddChildFrame(X1a_Gen);
        bool_Wa = true;
        plot_title += "#tilde{#chi}^{ #pm}_{1} #it{l} ";
      }
    }
    if(bool_Smub){
      if(Smub_Z){
        Smub_Gen.AddChildFrame(X2b_Gen);
        Smub_Gen.AddChildFrame(Lb_Gen);
        X2b_Gen.AddChildFrame(Zb_Gen);
        X2b_Gen.AddChildFrame(X1b_Gen);
        bool_Zb = true;
        plot_title += "#tilde{#chi}^{ 0}_{2} #it{l} ";
      }
      else{
        Smub_Gen.AddChildFrame(C1b_Gen);
        Smub_Gen.AddChildFrame(N1b_Gen);
        C1b_Gen.AddChildFrame(Wb_Gen);
        C1b_Gen.AddChildFrame(X1b_Gen);
        bool_Wb = true;
        plot_title += "#tilde{#chi}^{ #pm}_{1} #nu ";
      }
    }
    if(bool_Snub){
      if(Snub_Z){
        Snub_Gen.AddChildFrame(X2b_Gen);
        Snub_Gen.AddChildFrame(N1b_Gen);
        X2b_Gen.AddChildFrame(Zb_Gen);
        X2b_Gen.AddChildFrame(X1b_Gen);
        bool_Zb = true;
        plot_title += "#tilde{#chi}^{ 0}_{2} #nu ";
      }
      else{
        Snub_Gen.AddChildFrame(C1b_Gen);
        Snub_Gen.AddChildFrame(Lb_Gen);
        C1b_Gen.AddChildFrame(Wb_Gen);
        C1b_Gen.AddChildFrame(X1b_Gen);
        bool_Wb = true;
        plot_title += "#tilde{#chi}^{ #pm}_{1} #it{l} ";
      }
    }
    plot_title += "#rightarrow ";
    if(bool_Za){
      if(!leptonic_Za){
        Za_Gen.AddChildFrame(J1a_Gen); 
        Za_Gen.AddChildFrame(J2a_Gen); 
        plot_title += "Z (#it{j}#it{j})";
      }
      else{
        Za_Gen.AddChildFrame(L1a_Gen); 
        Za_Gen.AddChildFrame(L2a_Gen); 
        plot_title += "Z (#it{l}#it{l})";
      }
    }
    if(bool_Wa){
      if(!leptonic_Wa){
        Wa_Gen.AddChildFrame(J1c_Gen);
        Wa_Gen.AddChildFrame(J2c_Gen);
        plot_title += "W (#it{j}#it{j})";
      }
      else{
        Wa_Gen.AddChildFrame(L2c_Gen);
        Wa_Gen.AddChildFrame(N2c_Gen);
        plot_title += "W (#it{l}#nu)";
      }
    }
    if(bool_Zb){
      if(!leptonic_Zb){
        Zb_Gen.AddChildFrame(J1b_Gen); 
        Zb_Gen.AddChildFrame(J2b_Gen); 
        plot_title += " Z (#it{j}#it{j})";
      }
      else{
        Zb_Gen.AddChildFrame(L1b_Gen); 
        Zb_Gen.AddChildFrame(L2b_Gen); 
        plot_title += " Z (#it{l}#it{l})";
      }
    }
    if(bool_Wb){
      if(!leptonic_Wb){
        Wb_Gen.AddChildFrame(J1d_Gen);
        Wb_Gen.AddChildFrame(J2d_Gen);
        plot_title += " W (#it{j}#it{j})";
      }
      else{
        Wb_Gen.AddChildFrame(L2d_Gen);
        Wb_Gen.AddChildFrame(N2d_Gen);
        plot_title += " W (#it{l}#nu)";
      }
    }
  }

  if(LAB_Gen.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized generator tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator tree" << LogEnd;								    
  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//

  SmuSnu_Gen.SetVariableMass();
  SmuSmu_Gen.SetVariableMass();
  SnuSnu_Gen.SetVariableMass();
  Smua_Gen.SetMass(mSmu);
  Snua_Gen.SetMass(mSnu);
  Smub_Gen.SetMass(mSmu);
  Snub_Gen.SetMass(mSnu);
  X2a_Gen.SetMass(mN2);
  X2b_Gen.SetMass(mN2);
  X2c_Gen.SetMass(mN2);
  X2d_Gen.SetMass(mN2);
  C1a_Gen.SetMass(mC1);
  C1b_Gen.SetMass(mC1);
  C1c_Gen.SetMass(mC1);
  C1d_Gen.SetMass(mC1);
  X1a_Gen.SetMass(mN1);
  X1b_Gen.SetMass(mN1);
  X1c_Gen.SetMass(mN1);
  X1d_Gen.SetMass(mN1);
  Za_Gen.SetMass(mZ);
  Za_Gen.SetWidth(wZ);
  Zb_Gen.SetMass(mZ);
  Zb_Gen.SetWidth(wZ);
  Zc_Gen.SetMass(mZ);
  Zc_Gen.SetWidth(wZ);
  Zd_Gen.SetMass(mZ);
  Zd_Gen.SetWidth(wZ);
  Wa_Gen.SetMass(mW);
  Wa_Gen.SetWidth(wW);
  Wb_Gen.SetMass(mW);
  Wb_Gen.SetWidth(wW);
  Wc_Gen.SetMass(mW);
  Wc_Gen.SetWidth(wW);
  Wd_Gen.SetMass(mW);
  Wd_Gen.SetWidth(wW);
  La_Gen.SetMass(mL);
  Lb_Gen.SetMass(mL);
  Lc_Gen.SetMass(mL);
  Ld_Gen.SetMass(mL);
  J1a_Gen.SetMass(mJ);
  J2a_Gen.SetMass(mJ);
  J1b_Gen.SetMass(mJ);
  J2b_Gen.SetMass(mJ);
  J1c_Gen.SetMass(mJ);
  J2c_Gen.SetMass(mJ);
  J1d_Gen.SetMass(mJ);
  J2d_Gen.SetMass(mJ);
  L2b_Gen.SetMass(mL);
  N2b_Gen.SetMass(0.);

  WZ_Gen_Bkg.SetVariableMass(true);
  ZZ_Gen_Bkg.SetVariableMass(true);
  WWZ_Gen_Bkg.SetVariableMass(true);
  Wa_Gen_Bkg.SetMass(mW);
  Wa_Gen_Bkg.SetWidth(wW);
  Wb_Gen_Bkg.SetMass(mW);
  Wb_Gen_Bkg.SetWidth(wW);
  Za_Gen_Bkg.SetMass(mZ);
  Za_Gen_Bkg.SetWidth(wZ);
  Zb_Gen_Bkg.SetMass(mZ);
  Zb_Gen_Bkg.SetWidth(wZ);
  L1a_Gen.SetMass(mL);
  L2a_Gen.SetMass(mL);
  L1b_Gen.SetMass(mL);
  L1c_Gen.SetMass(mL);
  L2c_Gen.SetMass(mL);
  L2d_Gen.SetMass(mL);
  N1a_Gen.SetMass(0.);
  N1b_Gen.SetMass(0.);
  N1c_Gen.SetMass(0.);
  N2c_Gen.SetMass(0.);
  N2d_Gen.SetMass(0.);

  // set lepton pT and eta cuts
//  La_Gen.SetPtCut(1.);                  Lb_Gen.SetPtCut(1.);
//  La_Gen.SetEtaCut(2.5);                 Lb_Gen.SetEtaCut(2.5);
//  L1a_Gen.SetEtaCut(2.5);                 L1a_Gen.SetEtaCut(2.5);
//  L2a_Gen.SetEtaCut(2.5);                 L2a_Gen.SetEtaCut(2.5);
//  // set jet pT and eta cuts
//  J1a_Gen.SetPtCut(2.);  J1b_Gen.SetPtCut(2.);  J2a_Gen.SetPtCut(2.);  J2b_Gen.SetPtCut(2.);
//  J1a_Gen.SetEtaCut(2.5); J1b_Gen.SetEtaCut(2.5); J2a_Gen.SetEtaCut(2.5); J2b_Gen.SetEtaCut(2.5);

  if(LAB_Gen.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized generator analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing generator analysis" << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////
  g_Log << LogInfo << "Initializing reconstruction frames and trees..." << LogEnd;
  /////////////////////////////////////////////////////////////////////////////////////////

  LabRecoFrame             LAB("LAB","LAB");
  DecayRecoFrame           CM("CM","CM");
  DecayRecoFrame           X3a("X3a","#tilde{#chi}_{ 3a}"); 
  DecayRecoFrame           X3b("X3b","#tilde{#chi}_{ 3b}");
  DecayRecoFrame           X2a("X2a","#tilde{#chi}_{ 2a}");
  DecayRecoFrame           X2b("X2b","#tilde{#chi}_{ 2b}");
  VisibleRecoFrame         L1a("L1a","#it{l}_{1a}");
  VisibleRecoFrame         L1b("L1b","#it{l}_{1b}");
  VisibleRecoFrame         L2a("L2a","#it{l}_{2a}");
  VisibleRecoFrame         L2b("L2b","#it{l}_{2b}");
  VisibleRecoFrame         Ja("Ja","#it{j}_{a}");
  VisibleRecoFrame         Jb("Jb","#it{j}_{b}");
  InvisibleRecoFrame       X1a("X1a","#tilde{#chi}_{ 1a}");
  InvisibleRecoFrame       X1b("X1b","#tilde{#chi}_{ 1b}");
  SelfAssemblingRecoFrame  saL2b("saL2b","lep_{2b}");
  SelfAssemblingRecoFrame  saL1a("saL1a","lep_{1a}");
  SelfAssemblingRecoFrame  saL1b("saL1b","lep_{1b}");
  SelfAssemblingRecoFrame  saJa("saJa","jet_{a}");
  SelfAssemblingRecoFrame  saJb("saJb","jet_{b}");

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
  
  LAB.SetChildFrame(CM);
  CM.AddChildFrame(X3a);
  CM.AddChildFrame(X3b);
  X3a.AddChildFrame(L2a);
  X3b.AddChildFrame(saL2b);
  saL2b.AddChildFrame(L2b);
  X3a.AddChildFrame(X2a);
  X3b.AddChildFrame(X2b);
  X2a.AddChildFrame(saL1a);
  X2a.AddChildFrame(saJa);
  X2a.AddChildFrame(X1a);
  X2b.AddChildFrame(saL1b);
  X2b.AddChildFrame(saJb);
  X2b.AddChildFrame(X1b);
  saL1a.AddChildFrame(L1a);
  saL1b.AddChildFrame(L1b);
  saJa.AddChildFrame(Ja);
  saJb.AddChildFrame(Jb);

  if(LAB.InitializeTree())
    g_Log << LogInfo << "...Successfully initialized reconstruction tree" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing reconstruction tree" << LogEnd;

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//
 
  // Invisible Group
  InvisibleGroup INV("INV","#tilde{#chi}_{ 1} Jigsaws");
  INV.AddFrame(X1a);
  INV.AddFrame(X1b);

  // Set X1 X1 mass equal to l l mass
  SetMassInvJigsaw X1X1M("X1X1M", "M_{#chi_{1a}#chi_{1b}} = m_{#it{l}#it{l}}");
  INV.AddJigsaw(X1X1M);

  SetRapidityInvJigsaw X1X1R("NuNuR", "#eta_{#chi_{1a}#chi_{1b}} = #eta_{#it{l}#it{l}}");
  INV.AddJigsaw(X1X1R);
  X1X1R.AddVisibleFrames(CM.GetListVisibleFrames());

  MinMassesSqInvJigsaw InvSplit("InvSplit", "INV -> #tilde{#chi_{1a}}+ #tilde{#chi_{1b}}", 2);
  //ContraBoostInvJigsaw InvSplit("InvSplit", "INV -> #tilde{#chi_{1a}}+ #tilde{#chi_{1b}}");
  INV.AddJigsaw(InvSplit);
  InvSplit.AddVisibleFrames(X3a.GetListVisibleFrames(),0);
  InvSplit.AddVisibleFrames(X3b.GetListVisibleFrames(),1);
  InvSplit.AddInvisibleFrame(X1a, 0);
  InvSplit.AddInvisibleFrame(X1b, 1);

  // Jet Jigsaws

  CombinatoricGroup COMB_J("COMB_J", "Combinatoric System of Jets");
  MinMassesSqCombJigsaw CombSplit_J("CombSplit_J","Minimize M_{#tilde{#chi}_{ 2a}}^{2} and M_{#tilde{#chi}_{ 2b}}^{2}",2,2);

  COMB_J.AddFrame(Ja);
  COMB_J.SetNElementsForFrame(Ja,0);
  COMB_J.AddFrame(Jb);
  COMB_J.SetNElementsForFrame(Jb,0);

  COMB_J.AddJigsaw(CombSplit_J);
  CombSplit_J.AddCombFrame(Ja,0);
  CombSplit_J.AddCombFrame(Jb,1);
  CombSplit_J.AddObjectFrames(X3a.GetListVisibleFrames(),0);
  CombSplit_J.AddObjectFrames(X3b.GetListVisibleFrames(),1);

  // Leptonic Jigsaws

  CombinatoricGroup COMB_L("COMB_L", "Combinatoric System of Leptons");
  COMB_L.AddFrame(L2b);
  COMB_L.SetNElementsForFrame(L2b,0);
  COMB_L.AddFrame(L1a);
  COMB_L.SetNElementsForFrame(L1a,0);
  COMB_L.AddFrame(L1b);
  COMB_L.SetNElementsForFrame(L1b,0);

  MinMassesSqCombJigsaw CombSplit_L2("CombSplit_L2","Minimize M_{#tilde{#mu}^{ #pm}_{L}}^{2} and M_{#tilde{#nu}_{L}}^{2}",2,2);
  CombSplit_L2.AddCombFrame(L2b,1);
  CombSplit_L2.AddCombFrame(L1a,0);
  CombSplit_L2.AddCombFrame(L1b,1);
  CombSplit_L2.AddObjectFrames(X3a.GetListVisibleFrames(),0);
  CombSplit_L2.AddObjectFrames(X3b.GetListVisibleFrames(),1);

  MinMassesSqCombJigsaw CombSplit_Lb("CombSplit_Lb","Minimize M_{#tilde{#nu}_{L}} and M_{#tilde{#chi}_{ 2b}}",2,2);
  CombSplit_Lb.AddCombFrame(L2b,0);
  CombSplit_Lb.AddCombFrame(L1b,1);
  CombSplit_Lb.AddObjectFrame(L2b,0);
  CombSplit_Lb.AddObjectFrame(X2b,1);

  COMB_L.AddJigsaw(CombSplit_L2);
  COMB_L.AddJigsaw(CombSplit_Lb);

  if(LAB.InitializeAnalysis())
    g_Log << LogInfo << "...Successfully initialized analysis" << LogEnd;
  else
    g_Log << LogError << "...Failed initializing analysis" << LogEnd;	

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  TreePlot* treePlot = new TreePlot("TreePlot","TreePlot");
 
  treePlot->SetTree(LAB_Gen);
  treePlot->Draw("GenTree", "Generator Tree", true);

  treePlot->SetTree(LAB);
  treePlot->Draw("RecoTree", "Reconstruction Tree", true);

  treePlot->SetTree(INV);
  //treePlot->Draw("InvTree", "Invisible Jigsaws", true);

  treePlot->SetTree(COMB_L);
  //treePlot->Draw("LepTree", "Lepton Jigsaws", true);

  treePlot->SetTree(COMB_J);
  //treePlot->Draw("JetTree", "Jet Jigsaws", true);

  //-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//-//


  HistPlot* histPlot = new HistPlot("HistPlot", plot_title);
  histPlot->SetLabel2D(plot_title);
  int Nmass = 1; //mH.size();

  vector<RFList<const HistPlotCategory>> cat_cats; // meow lol

  RFList<const HistPlotCategory> cat_list_jets;
  vector<string> cat_names_jets;
  std::map<string,string> cat_map_jets;
  //cat_map_jets.insert(std::make_pair("Inclusive","Inclusive"));
  cat_map_jets.insert(std::make_pair("Jets_A","All Jets In A"));
  cat_map_jets.insert(std::make_pair("Jets_B","All Jets In B"));
  cat_map_jets.insert(std::make_pair("Jets_AB","Jets In A&B"));
  for(std::map<string,string>::iterator iter = cat_map_jets.begin(); iter != cat_map_jets.end(); ++iter){
    cat_list_jets += histPlot->GetNewCategory(iter->first,iter->second);
    cat_names_jets.push_back(iter->first);
  }
  //cat_cats.push_back(cat_list_jets);

  RFList<const HistPlotCategory> cat_list_leps;
  vector<string> cat_names_leps;
  std::map<string,string> cat_map_leps;
  cat_map_leps.insert(std::make_pair("Inclusive","Inclusive"));
  //cat_map_leps.insert(std::make_pair("Lep_2B","Lep In 2B"));
  //cat_map_leps.insert(std::make_pair("Lep_1B","Lep In 1B"));
  //cat_map_leps.insert(std::make_pair("Lep_1A","Lep In 1A"));
  for(std::map<string,string>::iterator iter = cat_map_leps.begin(); iter != cat_map_leps.end(); ++iter){
    cat_list_leps += histPlot->GetNewCategory(iter->first,iter->second);
    cat_names_leps.push_back(iter->first);
  }
  cat_cats.push_back(cat_list_leps);

  int max_lep_per_cat = 2; // 3 is max?
  RFList<const HistPlotCategory> cat_list_num_lep_2B;
  for(int n2B = 0; n2B <= max_lep_per_cat; n2B++){
    char sN2B[50], sN2Bcat[50];
    snprintf(sN2B,50,"%i Lep In 2B",n2B);
    snprintf(sN2Bcat,50, "%iLep2B",n2B);
    cat_list_num_lep_2B += histPlot->GetNewCategory(sN2Bcat,sN2B);
  }
  //cat_cats.push_back(cat_list_num_lep_2B);

  RFList<const HistPlotCategory> cat_list_num_lep_1B;
  for(int n1B = 0; n1B <= max_lep_per_cat; n1B++){
    char sN1B[50], sN1Bcat[50];
    snprintf(sN1B,50,"%i Lep In 1B",n1B);
    snprintf(sN1Bcat,50, "%iLep1B",n1B);
    cat_list_num_lep_1B += histPlot->GetNewCategory(sN1Bcat,sN1B);
  }
  //cat_cats.push_back(cat_list_num_lep_1B);

  RFList<const HistPlotCategory> cat_list_num_lep_1A;
  for(int n1A = 0; n1A <= max_lep_per_cat; n1A++){
    char sN1A[50], sN1Acat[50];
    snprintf(sN1A,50,"%i Lep In 1A",n1A);
    snprintf(sN1Acat,50, "%iLep1A",n1A);
    cat_list_num_lep_1A += histPlot->GetNewCategory(sN1Acat,sN1A);
  }
  //cat_cats.push_back(cat_list_num_lep_1A);

  int max_obj_per_cat = 3; // 4 is max?
  RFList<const HistPlotCategory> cat_list_num_obj_X2A;
  for(int nX2A = 0; nX2A <= max_obj_per_cat; nX2A++){
    char sNX2A[50], sNX2Acat[50];
    snprintf(sNX2A,50,"%i Obj In X2A",nX2A);
    snprintf(sNX2Acat,50, "%iObjX2A",nX2A);
    cat_list_num_obj_X2A += histPlot->GetNewCategory(sNX2Acat,sNX2A);
  }
  cat_cats.push_back(cat_list_num_obj_X2A);

  RFList<const HistPlotCategory> cat_list_num_obj_X2B;
  for(int nX2B = 0; nX2B <= max_obj_per_cat; nX2B++){
    char sNX2B[50], sNX2Bcat[50];
    snprintf(sNX2B,50,"%i Obj In X2B",nX2B);
    snprintf(sNX2Bcat,50, "%iObjX2B",nX2B);
    cat_list_num_obj_X2B += histPlot->GetNewCategory(sNX2Bcat,sNX2B);
  }
  cat_cats.push_back(cat_list_num_obj_X2B);

  RFList<const HistPlotCategory> cat_list_num_obj_X3A;
  for(int nX3A = 1; nX3A <= max_obj_per_cat; nX3A++){
    char sNX3A[50], sNX3Acat[50];
    snprintf(sNX3A,50,"%i Obj In X3A",nX3A);
    snprintf(sNX3Acat,50, "%iObjX3A",nX3A);
    cat_list_num_obj_X3A += histPlot->GetNewCategory(sNX3Acat,sNX3A);
  }
  //cat_cats.push_back(cat_list_num_obj_X3A);

  RFList<const HistPlotCategory> cat_list_num_obj_X3B;
  for(int nX3B = 0; nX3B <= max_obj_per_cat; nX3B++){
    char sNX3B[50], sNX3Bcat[50];
    snprintf(sNX3B,50,"%i Obj In X3B",nX3B);
    snprintf(sNX3Bcat,50, "%iObjX3B",nX3B);
    cat_list_num_obj_X3B += histPlot->GetNewCategory(sNX3Bcat,sNX3B);
  }
  //cat_cats.push_back(cat_list_num_obj_X3B);

  // define plotting vars
  double x_max = 1000.;
  double x_min = 0.; 
  if(compressed) x_max = 250.;
  const HistPlotVar& PTCM      = histPlot->GetNewVar("PTCM",  "p_{T}^{CM}", 0., 500., "[GeV]");
  const HistPlotVar& M_CM      = histPlot->GetNewVar("M_CM",  "M_{CM}", 0., 5000., "[GeV]");
  const HistPlotVar& M_X3a     = histPlot->GetNewVar("MX3a",   "M_{#tilde{#chi}_{ 3a}}", x_min, x_max, "[GeV]");
  const HistPlotVar& M_X3b     = histPlot->GetNewVar("MX3b",  "M_{#tilde{#chi}_{ 3b}}", x_min, x_max, "[GeV]");
  const HistPlotVar& M_X3a_CM     = histPlot->GetNewVar("MX3a_CM",   "M_{#tilde{#chi}_{ 3a}}^{CM}", x_min, x_max/2.5, "[GeV]");
  const HistPlotVar& M_X3b_CM     = histPlot->GetNewVar("MX3b_CM",  "M_{#tilde{#chi}_{ 3b}}^{CM}", x_min, x_max/2.5, "[GeV]");
  const HistPlotVar& M_X3a_Vis     = histPlot->GetNewVar("MX3a_Vis",   "M_{#tilde{#chi}_{ 3a}}^{Vis}", x_min, x_max/2.5, "[GeV]");
  const HistPlotVar& M_X3b_Vis     = histPlot->GetNewVar("MX3b_Vis",  "M_{#tilde{#chi}_{ 3b}}^{Vis}", x_min, x_max/2.5, "[GeV]");
  const HistPlotVar& M_X2a_CM     = histPlot->GetNewVar("MX2a_CM",   "M_{#tilde{#chi}_{ 2a}}^{CM}", x_min, x_max/3.5, "[GeV]");
  const HistPlotVar& M_X2b_CM     = histPlot->GetNewVar("MX2b_CM",  "M_{#tilde{#chi}_{ 2b}}^{CM}", x_min, x_max/3.5, "[GeV]");
  const HistPlotVar& M_X2a_Vis     = histPlot->GetNewVar("MX2a_Vis",   "M_{#tilde{#chi}_{ 2a}}^{Vis}", x_min, x_max/3.5, "[GeV]");
  const HistPlotVar& M_X2b_Vis     = histPlot->GetNewVar("MX2b_Vis",  "M_{#tilde{#chi}_{ 2b}}^{Vis}", x_min, x_max/3.5, "[GeV]");
  const HistPlotVar& M_X2a     = histPlot->GetNewVar("MX2a",  "M_{#tilde{#chi}_{ 2a}}", x_min, x_max, "[GeV]");
  const HistPlotVar& M_X2b     = histPlot->GetNewVar("MX2b",  "M_{#tilde{#chi}_{ 2b}}", x_min, x_max, "[GeV]");
  const HistPlotVar& Mperp     = histPlot->GetNewVar("Mperp", "M_{#perp} ", 0., x_max, "[GeV]");
  const HistPlotVar& Mperp2     = histPlot->GetNewVar("Mperp", "M_{#perp}  2 ", 0., x_max, "[GeV]");
  const HistPlotVar& M_Diff_MX3a_X2a = histPlot->GetNewVar("M_Diff_MX3a_X2a", "M_Diff_X3a_X2a", x_min, x_max, "[GeV]");
  const HistPlotVar& M_Diff_MX3b_X2b = histPlot->GetNewVar("M_Diff_MX3b_X2b", "M_Diff_X3b_X2b", x_min, x_max, "[GeV]");
  const HistPlotVar& M_Ratio_MX3a_X2a = histPlot->GetNewVar("M_Ratio_MX3a_X2a", "M_Ratio_MX3a_X2a", 0., 1.0, "");
  const HistPlotVar& M_Ratio_MX3b_X2b = histPlot->GetNewVar("M_Ratio_MX3b_X2b", "M_Ratio_MX3b_X2b", 0., 1.0, "");
  const HistPlotVar& RVis_Top = histPlot->GetNewVar("RVis_Top","RVis^{Top}", 0., 3.0, "");
  const HistPlotVar& RVis_Bot = histPlot->GetNewVar("RVis_Bot","RVis^{Bot}", 0., 3.0, "");
  const HistPlotVar& RVis = histPlot->GetNewVar("RVis","RVis",0.,5.,"");
  const HistPlotVar& RVisRatio_AB = histPlot->GetNewVar("RVisRatio_AB","RVisRatio_AB",0.,3.,"");
  const HistPlotVar& RVisRatio_AB2 = histPlot->GetNewVar("RVisRatio_AB2","RVisRatio_AB2",0.,1.,"");
  const HistPlotVar& RVisRatio_AB3 = histPlot->GetNewVar("RVisRatio_AB3","RVisRatio_AB3",0.,3.,"");
  const HistPlotVar& RVisRatio_AB4 = histPlot->GetNewVar("RVisRatio_AB4","RVisRatio_AB4",0.,1.,"");

  for(int c = 0; c < int(cat_cats.size()); c++){
    //histPlot->AddPlot(PTCM, cat_cats[c]);
    //histPlot->AddPlot(M_CM, cat_cats[c]);

//    histPlot->AddPlot(M_X3a, cat_cats[c]);
//    histPlot->AddPlot(M_X3b, cat_cats[c]);
//    histPlot->AddPlot(M_X2a, cat_cats[c]);
//    histPlot->AddPlot(M_X2b, cat_cats[c]);
    //histPlot->AddPlot(M_Diff_MX3a_X2a, cat_cats[c]);
    //histPlot->AddPlot(M_Diff_MX3b_X2b, cat_cats[c]);
    //histPlot->AddPlot(Mperp, cat_cats[c]);
    //histPlot->AddPlot(Mperp2, cat_cats[c]);
    //histPlot->AddPlot(M_Ratio_MX3a_X2a, cat_cats[c]);
    //histPlot->AddPlot(M_Ratio_MX3b_X2b, cat_cats[c]);
    //histPlot->AddPlot(RVis_Top, cat_cats[c]);
    //histPlot->AddPlot(RVis_Bot, cat_cats[c]);
    //histPlot->AddPlot(RVis, cat_cats[c]);
    //histPlot->AddPlot(RVisRatio_AB, cat_cats[c]);
    //histPlot->AddPlot(RVisRatio_AB2, cat_cats[c]);
    //histPlot->AddPlot(RVisRatio_AB3, cat_cats[c]);
    //histPlot->AddPlot(RVisRatio_AB4, cat_cats[c]);
    
    //histPlot->AddPlot(Mperp, Mperp2, cat_cats[c]);
    //histPlot->AddPlot(RVis, Mperp, cat_cats[c]); //
    //histPlot->AddPlot(RVis, Mperp2, cat_cats[c]);
    //histPlot->AddPlot(RVisRatio_AB, Mperp, cat_cats[c]);
    //histPlot->AddPlot(RVisRatio_AB2, Mperp, cat_cats[c]);
    //histPlot->AddPlot(RVisRatio_AB3, Mperp, cat_cats[c]); //
    //histPlot->AddPlot(RVisRatio_AB3, RVis, cat_cats[c]); //
    //histPlot->AddPlot(RVisRatio_AB4, Mperp, cat_cats[c]);
    //histPlot->AddPlot(RVisRatio_AB3, RVisRatio_AB4, cat_cats[c]);
    //histPlot->AddPlot(RVis_Top, Mperp, cat_cats[c]);
    //histPlot->AddPlot(RVis_Bot, Mperp, cat_cats[c]);
    //histPlot->AddPlot(M_Diff_MX3a_X2a, Mperp, cat_cats[c]);
    //histPlot->AddPlot(M_Diff_MX3b_X2b, Mperp, cat_cats[c]);
    //histPlot->AddPlot(M_Diff_MX3a_X2a, M_Diff_MSnu_X2b, cat_cats[c]);
    //histPlot->AddPlot(M_Ratio_MX3b_X2a, Mperp, cat_cats[c]);
    //histPlot->AddPlot(M_Ratio_MX3b_X2b, M_X3b, cat_cats[c]);
    //histPlot->AddPlot(M_Ratio_MX3a_X2a, M_X3a, cat_cats[c]);
    //histPlot->AddPlot(M_slep, M_, cat_cats[c]);
    //histPlot->AddPlot(M_X2a, M_X2b, cat_cats[c]);
    //histPlot->AddPlot(M_X3a, M_X2b, cat_cats[c]);
    //histPlot->AddPlot(M_X3a, M_X3b, cat_cats[c]);
    //histPlot->AddPlot(M_X3a_CM, M_X3b_CM, cat_cats[c]);
    //histPlot->AddPlot(M_X3b_Vis, M_X3b_CM, cat_cats[c]);
    histPlot->AddPlot(M_X2a_CM, M_X2b_CM, cat_cats[c]);
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  // check to make sure the right vis frames are generated
  // RFList<const RestFrame> RF_List_VisFrames_test = LAB_Gen.GetListVisibleFrames();
  // for(int RF = 0; RF < int(RF_List_VisFrames_test.GetN()); RF++)
  //   std::cout << RF_List_VisFrames_test[RF].GetName() << std::endl;

  for(int m = 0; m < Nmass; m++){
    //g_Log << LogInfo << "Generating events for H^{0} mass = " << mH[m] << LogEnd;

    LAB_Gen.InitializeAnalysis();
  
    for(int igen = 0; igen < Ngen; igen++){
      if(igen%((std::max(Ngen,10))/10) == 0)
	g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

      // generate event
      LAB_Gen.ClearEvent();                            // clear the gen tree

      vector<TLorentzVector> vect_Leps;
      vector<TLorentzVector> vect_Jets;
      vector<double> vect_Leps_Pt;
      vector<RFKey> vect_Lep_RFKey;
      vector<RFKey> vect_Jet_RFKey;

      // give kick to CM
      LAB_Gen.SetTransverseMomentum(10.);
      //LAB_Gen.SetLongitudinalMomentum(LAB_Pt*TMath::SinH(gRandom->Gaus(0.,LAB_eta)));
      
      LAB_Gen.AnalyzeEvent();                          // generate a new event

      // analyze event
      LAB.ClearEvent();                                  // clear the reco tree

      TVector3 MET = LAB_Gen.GetInvisibleMomentum();     // Get the MET from gen tree
      MET.SetZ(0.);
      INV.SetLabFrameThreeVector(MET);                   // Set the MET in reco tree

      RFList<const RestFrame> RF_List_VisFrames = LAB_Gen.GetListVisibleFrames();
      for(int RF = 0; RF < int(RF_List_VisFrames.GetN()); RF++){
        // find all generated leptons
        if(RF_List_VisFrames[RF].GetName().find("L") != std::string::npos){
          vect_Leps_Pt.push_back(RF_List_VisFrames[RF].GetFourVector().Pt());
          vect_Leps.push_back(RF_List_VisFrames[RF].GetFourVector());
        }
        else // if not leptons then vis frames must be jets
          vect_Jets.push_back(RF_List_VisFrames[RF].GetFourVector());
      }
      // index of lepton with highest pt
      int max_lep_index = std::distance(vect_Leps_Pt.begin(), std::max_element(vect_Leps_Pt.begin(), vect_Leps_Pt.end()));
      L2a.SetLabFrameFourVector(vect_Leps[max_lep_index]);
      vect_Leps.erase(vect_Leps.begin()+max_lep_index);
      vect_Leps_Pt.clear();
      for(int i=0; i<int(vect_Leps.size()); i++){
        vect_Lep_RFKey.push_back(COMB_L.AddLabFrameFourVector(vect_Leps[i]));
      }
      for(int i=0; i<int(vect_Jets.size()); i++){
        vect_Jet_RFKey.push_back(COMB_J.AddLabFrameFourVector(vect_Jets[i]));
      }

      LAB.AnalyzeEvent(); // analyze the event

      // Check # Jets In 'A' Side
      int nJA = 0;
      for(int j = 0; j < int(vect_Jet_RFKey.size()); j++){
        if(COMB_J.GetFrame(vect_Jet_RFKey[j]) == Ja) nJA++;
      }
      // Check # Jets In 'B' Side
      int nJB = 0;
      for(int j = 0; j < int(vect_Jet_RFKey.size()); j++){
        if(COMB_J.GetFrame(vect_Jet_RFKey[j]) == Jb) nJB++;
      }

      int nL2B = 0;
      for(int l = 0; l < int(vect_Lep_RFKey.size()); l++){
        if(COMB_L.GetFrame(vect_Lep_RFKey[l]) == L2b) nL2B++;
      }

      int nL1A = 0;
      for(int l = 0; l < int(vect_Lep_RFKey.size()); l++){
        if(COMB_L.GetFrame(vect_Lep_RFKey[l]) == L1a) nL1A++;
      }

      int nL1B = 0;
      for(int l = 0; l < int(vect_Lep_RFKey.size()); l++){
        if(COMB_L.GetFrame(vect_Lep_RFKey[l]) == L1b) nL1B++;
      }

      // Reconstruction-level observables
      PTCM    = CM.GetFourVector().Pt();
      M_CM    = CM.GetMass();
      M_X3a   = X3a.GetMass();
      M_X3b   = X3b.GetMass();
      M_X2a   = X2a.GetMass();
      M_X2b   = X2b.GetMass();


      TLorentzVector vP_L2a_CM = L2a.GetFourVector(CM);
      TLorentzVector vP_L1a_CM = saL1a.GetFourVector(CM);
      TLorentzVector vP_L2b_CM = saL2b.GetFourVector(CM);
      TLorentzVector vP_L1b_CM = saL1b.GetFourVector(CM);
      TLorentzVector vP_Ja_CM  = saJa.GetFourVector(CM);
      TLorentzVector vP_Jb_CM  = saJb.GetFourVector(CM);
      TLorentzVector vP_Ia_CM  = X1a.GetFourVector(CM);
      TLorentzVector vP_Ib_CM  = X1b.GetFourVector(CM);
      TLorentzVector vP_CM     = CM.GetFourVector();

      //TVector3 boostVis = (vP_L2a_CM+vP_L1a_CM+vP_L2b_CM+vP_L1b_CM+vP_Ja_CM+vP_Jb_CM).BoostVector();
      //TVector3 boostInv = (vP_Ia_CM+vP_Ib_CM).BoostVector();
      //TVector3 daBoost = vP_CM.Vect().Unit();

      //boostVis = (boostVis.Dot(daBoost))*daBoost;
      //boostInv = (boostInv.Dot(daBoost))*daBoost;

      //if((!std::isnan(boostVis.Mag())) && (boostVis.Mag() < 1)){
      //  vP_Ja_CM.Boost(-boostVis);
      //  vP_Jb_CM.Boost(-boostVis); 
      //  vP_L2a_CM.Boost(-boostVis);
      //  vP_L1a_CM.Boost(-boostVis);
      //  vP_L2b_CM.Boost(-boostVis);
      //  vP_L1b_CM.Boost(-boostVis);
      //} else {
      //  vP_Ja_CM.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_Ja_CM.M()));
      //  vP_Jb_CM.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_Jb_CM.M()));
      //  vP_L2a_CM.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_L2a_CM.M()));
      //  vP_L1a_CM.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_L1a_CM.M()));
      //  vP_L2b_CM.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_L2b_CM.M()));
      //  vP_L1b_CM.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_L1b_CM.M()));
      //}

      //if((!std::isnan(boostInv.Mag())) && (boostInv.Mag() < 1)){
      //  vP_Ia_CM.Boost(-boostInv);
      //  vP_Ib_CM.Boost(-boostInv);
      //} else {
      //  vP_Ia_CM.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_Ia_CM.M()));
      //  vP_Ib_CM.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_Ib_CM.M()));
      //}

      TLorentzVector vP_VisA_CM = (vP_L2a_CM + vP_L1a_CM + vP_Ja_CM);
      vP_VisA_CM.SetPtEtaPhiM(vP_VisA_CM.Pt(),vP_VisA_CM.Eta(),vP_VisA_CM.Phi(),0.);
      TLorentzVector vP_VisB_CM = (vP_L2b_CM + vP_L1b_CM + vP_Jb_CM);
      vP_VisB_CM.SetPtEtaPhiM(vP_VisB_CM.Pt(),vP_VisB_CM.Eta(),vP_VisB_CM.Phi(),0.);
      vP_Ia_CM.SetPtEtaPhiM(vP_Ia_CM.Pt(),vP_Ia_CM.Eta(),vP_Ia_CM.Phi(),0.);
      vP_Ib_CM.SetPtEtaPhiM(vP_Ib_CM.Pt(),vP_Ib_CM.Eta(),vP_Ib_CM.Phi(),0.);

      M_X3a_CM = (vP_VisA_CM+vP_Ia_CM).M();
      M_X3b_CM = (vP_VisB_CM+vP_Ib_CM).M();

      M_X3a_Vis = X3a.GetListVisibleFrames().GetMass();
      M_X3b_Vis = X3b.GetListVisibleFrames().GetMass();

      TLorentzVector vP_VisA_CM2 = (vP_L1a_CM + vP_Ja_CM);
      vP_VisA_CM2.SetPtEtaPhiM(vP_VisA_CM2.Pt(),vP_VisA_CM2.Eta(),vP_VisA_CM2.Phi(),0.);
      TLorentzVector vP_VisB_CM2 = (vP_L1b_CM + vP_Jb_CM);
      vP_VisB_CM2.SetPtEtaPhiM(vP_VisB_CM2.Pt(),vP_VisB_CM2.Eta(),vP_VisB_CM2.Phi(),0.);

      M_X2a_CM = (vP_VisA_CM2+vP_Ia_CM).M();
      M_X2b_CM = (vP_VisB_CM2+vP_Ib_CM).M();

      M_X2a_Vis = X2a.GetListVisibleFrames().GetMass();
      M_X2b_Vis = X2b.GetListVisibleFrames().GetMass();

      double MX3a_BoostT = (vP_L2a_CM + vP_L1a_CM + vP_Ja_CM).M();
      double MX3b_BoostT = (vP_L2b_CM + vP_L1b_CM + vP_Jb_CM).M();
      Mperp = sqrt((MX3a_BoostT*MX3a_BoostT + MX3b_BoostT*MX3b_BoostT)/sqrt(2.));  // good var

      TLorentzVector vP_L1a_X2a = saL1a.GetFourVector(X2a);
      TLorentzVector vP_Ja_X2a = saJa.GetFourVector(X2a);
      TLorentzVector vP_L1b_X2b = saL1b.GetFourVector(X2b);
      TLorentzVector vP_Jb_X2b = saJb.GetFourVector(X2b);
      TLorentzVector vP_Ia_X2a = X1a.GetFourVector(X2a);
      TLorentzVector vP_Ib_X2b = X1b.GetFourVector(X2b);
      TLorentzVector vP_X2 = X2a.GetFourVector(X3a)+X2b.GetFourVector(X3b);

      //TVector3 boostVis2 = (vP_L1a_X2a+vP_Ja_X2a+vP_L1b_X2b+vP_Jb_X2b).BoostVector();
      //TVector3 boostInv2 = (vP_Ia_X2a+vP_Ib_X2b).BoostVector();
      //TVector3 daBoost2 = vP_X2.Vect().Unit();

      //boostVis2 = (boostVis2.Dot(daBoost2))*daBoost2;
      //boostInv2 = (boostInv2.Dot(daBoost2))*daBoost2;

      //if((!std::isnan(boostVis2.Mag())) && (boostVis2.Mag() < 1)){
      //  vP_Ja_X2a.Boost(-boostVis2);
      //  vP_Jb_X2b.Boost(-boostVis2);
      //  vP_L1a_X2a.Boost(-boostVis2);
      //  vP_L1b_X2b.Boost(-boostVis2);
      //} else {
      //  vP_Ja_X2a.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_Ja_X2a.M()));
      //  vP_Jb_X2b.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_Jb_X2b.M()));
      //  vP_L1a_X2a.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_L1a_X2a.M()));
      //  vP_L1b_X2b.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_L1b_X2b.M()));
      //}

      //if((!std::isnan(boostInv2.Mag())) && (boostInv2.Mag() < 1)){
      //  vP_Ia_X2a.Boost(-boostInv2);
      //  vP_Ib_X2b.Boost(-boostInv2);
      //} else {
      //  vP_Ia_X2a.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_Ia_X2a.M()));
      //  vP_Ib_X2b.SetVectM(TVector3(0.,0.,0.),std::max(0.,vP_Ib_X2b.M()));
      //}

      double MX2a_BoostT = (vP_L1a_X2a + vP_Ja_X2a).M();
      double MX2b_BoostT = (vP_L1b_X2b + vP_Jb_X2b).M();
      Mperp2 = sqrt((MX2a_BoostT*MX2a_BoostT + MX2b_BoostT*MX2b_BoostT)/sqrt(2.));

      M_Diff_MX3a_X2a = X3a.GetFourVector().M() - X2a.GetFourVector().M();
      M_Diff_MX3b_X2b = X3b.GetFourVector().M() - X2b.GetFourVector().M();
      M_Ratio_MX3a_X2a = X2a.GetFourVector().M()/X3a.GetFourVector().M();
      M_Ratio_MX3b_X2b = X2b.GetFourVector().M()/X3b.GetFourVector().M();

      // R_Vis
      TVector3 vPVIS = (vP_L1a_CM + vP_L2a_CM + vP_L1b_CM + vP_L2b_CM + vP_Ja_CM + vP_Jb_CM).Vect();
      TVector3 vPINV = (vP_Ia_CM + vP_Ib_CM).Vect();
      TVector3 vPLEP = (vP_L1a_CM + vP_L2a_CM + vP_L1b_CM + vP_L2b_CM).Vect();
      TVector3 vPBot = (vP_L1a_CM + vP_L1b_CM + vP_Ja_CM + vP_Jb_CM).Vect(); // bottom of reco tree
      TVector3 vPBot_LEP = (vP_L1a_CM + vP_L1b_CM).Vect(); // bottom of reco tree (LEP only)
      TVector3 vPTop = (vP_L2a_CM + vP_L2b_CM).Vect(); // top of reco tree
      //R_Vis = fabs(vPINV.Dot(vPVIS.Unit())/vPVIS.Mag());
      //R_Vis = fabs(vPVIS.Dot(vPINV.Unit())/vPINV.Mag());
      RVis_Top = fabs(vPTop.Dot(vPBot.Unit())/vPBot.Mag()); // project top onto bottom
      RVis_Bot = fabs(vPBot.Dot(vPTop.Unit())/vPTop.Mag()); // project bottom onto top
      RVis = fabs(vPINV.Dot(vPLEP.Unit())/vPLEP.Mag()); // project INV onto LEP // good var

      RVisRatio_AB = (vPTop - vPBot_LEP).Mag()/(vPTop + vPBot_LEP).Mag();
      RVisRatio_AB3 = (vPTop - vPBot).Mag()/(vPTop + vPBot).Mag(); // good var
      RVisRatio_AB2 = fabs((vPTop.Mag() - vPBot_LEP.Mag())/(vPTop.Mag() + vPBot_LEP.Mag()));
      RVisRatio_AB4 = fabs((vPTop.Mag() - vPBot.Mag())/(vPTop.Mag() + vPBot.Mag())); // maybe good var



      // fill hists

      int Inclusive_index = get_cat_index(cat_names_jets, "Inclusive");
      if(Inclusive_index != -1)
        histPlot->Fill(cat_list_jets[Inclusive_index]);
      Inclusive_index = get_cat_index(cat_names_leps, "Inclusive");
      if(Inclusive_index != -1)
        histPlot->Fill(cat_list_leps[Inclusive_index]);

      if(nJB == 0){ // All jets in 'A' side
        int Jet_A_index = get_cat_index(cat_names_jets, "Jets_A");
        if(Jet_A_index != -1)
          histPlot->Fill(cat_list_jets[Jet_A_index]);
      }
      if(nJA == 0){ // All jets in 'B' side
        int Jet_B_index = get_cat_index(cat_names_jets, "Jets_B");
        if(Jet_B_index != -1)
          histPlot->Fill(cat_list_jets[Jet_B_index]);
      }
      if(nJA > 0 && nJB > 0){ // At least one jet in each side
        int Jet_AB_index = get_cat_index(cat_names_jets, "Jets_AB");
        if(Jet_AB_index != -1)
          histPlot->Fill(cat_list_jets[Jet_AB_index]);
      }

      if(nL2B > 0){
        int Lep_2B_index = get_cat_index(cat_names_leps, "Lep_2B");
        if(Lep_2B_index != -1)
          histPlot->Fill(cat_list_leps[Lep_2B_index]);
      }
      if(nL1A > 0){
        int Lep_1A_index = get_cat_index(cat_names_leps, "Lep_1A");
        if(Lep_1A_index != -1)
          histPlot->Fill(cat_list_leps[Lep_1A_index]);
      }
      if(nL1B > 0){
        int Lep_1B_index = get_cat_index(cat_names_leps, "Lep_1B");
        if(Lep_1B_index != -1)
          histPlot->Fill(cat_list_leps[Lep_1B_index]);
      }

      if(nL2B < max_lep_per_cat) histPlot->Fill(cat_list_num_lep_2B[nL2B]);
      else histPlot->Fill(cat_list_num_lep_2B[max_lep_per_cat]);
      if(nL1B < max_lep_per_cat) histPlot->Fill(cat_list_num_lep_1B[nL1B]);
      else histPlot->Fill(cat_list_num_lep_1B[max_lep_per_cat]);
      if(nL1A < max_lep_per_cat) histPlot->Fill(cat_list_num_lep_1A[nL1A]);
      else histPlot->Fill(cat_list_num_lep_1A[max_lep_per_cat]);

      int nX2A = nL1A+nJA;
      int nX2B = nL1B+nJB;
      if(nX2A < max_obj_per_cat) histPlot->Fill(cat_list_num_obj_X2A[nX2A]);
      else histPlot->Fill(cat_list_num_obj_X2A[max_obj_per_cat]);
      if(nX2B < max_obj_per_cat) histPlot->Fill(cat_list_num_obj_X2B[nX2B]);
      else histPlot->Fill(cat_list_num_obj_X2B[max_obj_per_cat]);

      int nX3A = nX2A+1;
      int nX3B = nX2B+nL2B;
      if(nX3A < max_obj_per_cat) histPlot->Fill(cat_list_num_obj_X3A[nX3A]);
      else histPlot->Fill(cat_list_num_obj_X3A[max_obj_per_cat]);
      if(nX3B < max_obj_per_cat) histPlot->Fill(cat_list_num_obj_X3B[nX3B]);
      else histPlot->Fill(cat_list_num_obj_X3B[max_obj_per_cat]);

    }
    LAB_Gen.PrintGeneratorEfficiency();
  }

  histPlot->Draw();

  TFile fout(output_name.c_str(),"RECREATE");
  fout.Close();
  histPlot->WriteOutput(output_name);
  histPlot->WriteHist(output_name);
  treePlot->WriteOutput(output_name);
  
  g_Log << LogInfo << "Finished" << LogEnd;
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  RF_sleptoncascades();
  return 0;
}
#endif

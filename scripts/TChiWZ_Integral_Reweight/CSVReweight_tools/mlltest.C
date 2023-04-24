#define mlltest_cxx
#include "mlltest.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void mlltest::Loop()
{
//   In a ROOT session, you can do:
//      root> .L mlltest.C
//      root> mlltest t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      
       //Int_t           genNZff;
   	   //vector<double>  *genPT_Zff;
   	   //vector<double>  *genEta_Zff;
       //vector<double>  *genPhi_Zff;
   	   //vector<double>  *genM_Zff;
      
      TLorentzVector f1;
      TLorentzVector f2;
      
      f1.SetPtEtaPhiM(genPT_Zff->at(0), genEta_Zff->at(0), genPhi_Zff->at(0), genM_Zff->at(0));
      f2.SetPtEtaPhiM(genPT_Zff->at(1), genEta_Zff->at(1), genPhi_Zff->at(1), genM_Zff->at(1));
  
      TLorentzVector f12;
      f12=f1+f2;
      
      hPS->Fill(f12.M(),1.0);
      hOS->Fill(f12.M(), wt_OS*wt_int_OS);
      hSS->Fill(f12.M(), wt_SS*wt_int_SS);
      
     
   }
  
   hPS->SetLineColor(kBlack);
   hOS->SetLineColor(kRed);
   hSS->SetLineColor(kBlue);
   hPS->Draw();
   hOS->Draw("SAMES");
   hSS->Draw("SAMES");
   
   
}

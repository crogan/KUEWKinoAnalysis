#include <math.h>

//TChiWZ_METUncer_GenMET_1000093
int getSetIntersectionForPoint( std::set<std::string>& nomSet, std::set<std::string>& varSet,std::string gPoint ){

	int case1=1;
	int case2=2;
	int case3=3;
	
	bool inNom=false;
	for(auto& nom : nomSet){
		if( nom == gPoint ){
			inNom = true;
			break;		
		}	
	}
	bool inVar=false;
	for(auto& var : varSet){
		if( var == gPoint ){
			inVar = true;
			break;
		}
	}
	if( inNom && inVar ) return 1;
	if( !inNom && inVar ) return 2;
	if( inNom && !inVar ) return 3;


}
std::set<std::string> getSuperSetForPoints( std::set<std::string>& nomSet, std::set<std::string>& varSet ){
	std::set<std::string> superset{};
	for(auto& str : nomSet){
		superset.insert(str);
	//	std::cout<<"inserting "<<str<<"\n";
	}			
	for(auto& str : varSet){
		superset.insert(str);
	//	std::cout<<"inserting "<<str<<"\n";
	}
	return superset;
}
std::vector<double> getDeltaN( TH1D* hsig, TH1D* hvar ){
	
	int nBins = hsig->GetNbinsX();//bins in both had better be identical
		
	std::vector<double> deltaN(nBins);
	double dN=0.;
//	std::cout<<"\nprocessing variation\n";
	for(int i=1; i<= nBins; i++){
		dN =  hsig->GetBinContent(i) - hvar->GetBinContent(i) ;
//		std::cout<<fabs(dN)<<" ";
		deltaN[i-1] = fabs(dN) ;		
	}
//	std::cout<<"\n";
	return deltaN;
			
}
double propagateError( int ibin, TH1D* nomHist, TH1D* varHist ){
	double errX = nomHist->GetBinError(ibin);
	double errY = varHist->GetBinError(ibin);
	double errZ = std::sqrt(errX*errX* + errY*errY);
	return errZ;
}
bool checkIfEmptyHist( TH1D* histDown ){
	int nBins = histDown->GetNbinsX();
	bool empty = true;
	for(int i=1; i<= nBins; i++){
		if( histDown->GetBinContent(i) != 1e-8 ){
			empty= false;
		}	
	}		
	return empty;
}
void createHistograms( TDirectoryFile* dir, TFile* fout, std::string sigName, std::string gridPoint, int caseNumber){
	//h1D_TChiWZ_METUncer_GenMET_3750355_Ch1L_lpm_gold_ge4j0bS_ge1jge1bISR_PTISR1_gamT1
	//h1D_TChiWZ_METUncer_UnClustDown_4250385_Ch1L_lpm_gold_ge4j0bS_ge1jge1bISR_PTISR1_gamT1

	//if(gridPoint == "") return;	
//	std::cout<<"sigName="<<sigName<<" gridPoint="<<gridPoint<<" caseNumber="<<caseNumber<<"\n";	

	//craft the Get strings for the histograms
	std::string nomHist = sigName+"_"+gridPoint;
//	std::string varHist = sigName+"_METUncer_GenMET_"+gridPoint;
	std::string varHist = sigName+"_"+gridPoint+"_METUncer_GenMET";
	std::string dirName = std::string(dir->GetName());
	std::vector<double> deltaN{};
//	std::cout<< "dirName="<<dirName<<" nomHist="<<nomHist<<" varHist="<<varHist<<"\n";

	if( caseNumber == 1 ){
		TH1D *hsig, *hvar;
		hsig =  (TH1D*) dir->Get(nomHist.c_str());
		hvar =  (TH1D*) dir->Get(varHist.c_str());
		//std::cout<<"init original histograms\n";
		deltaN = getDeltaN( hsig, hvar );	
		
		
	//	dii->cd();
		TH1D* newUp = (TH1D*) hsig->Clone();
	//	newUp->SetName( (sigName+"_METUncer_GenMETUp_"+gridPoint).c_str() );
	//	newUp->SetTitle( ("h1D_"+sigName+"_METUncer_GenMETUp_"+gridPoint+"_"+dirName).c_str() );
		newUp->SetName( (sigName+"_"+gridPoint+"_METUncer_GenMETUp").c_str() );
		newUp->SetTitle( ("h1D_"+sigName+"_"+gridPoint+"_METUncer_GenMETUp_"+dirName).c_str() );
	
		TH1D* newDn = (TH1D*) hsig->Clone();
	//	newDn->SetName( (sigName+"_METUncer_GenMETDown_"+gridPoint).c_str() );
	//	newDn->SetTitle( ("h1D_"+sigName+"_METUncer_GenMETDown_"+gridPoint+"_"+dirName).c_str() );
		newDn->SetName( (sigName+"_"+gridPoint+"_METUncer_GenMETDown").c_str() );
		newDn->SetTitle( ("h1D_"+sigName+"_"+gridPoint+"_METUncer_GenMETDown_"+dirName).c_str() );
	


		for(int i=0; i<deltaN.size(); i++){
			//do up
//			std::cout<<" adding dn/2="<<deltaN[i]/2.<<" ";
			newUp->SetBinContent(i+1, (newUp->GetBinContent(i+1) + deltaN[i]/2.) );
			newUp->SetBinError(i+1, propagateError(i+1, hsig, hvar) );
			//do down
			if(  hsig->GetBinContent(i+1) > deltaN[i]/2.  ){
				newDn->SetBinContent(i+1, (newDn->GetBinContent(i+1) - deltaN[i]/2.) );
				newDn->SetBinError(i+1, propagateError(i+1, hsig, hvar) );
			} 	
			else{
				newDn->SetBinContent(i+1, 1e-8 );
                                newDn->SetBinError(i+1, propagateError(i+1, hsig, hvar) );	
			}
		}
		//histograms are now populated for case 1, write all histograms that are not empty (do not write a down hist that is all 1e-8)
		dir->cd();
		if(!checkIfEmptyHist(newDn))
		newDn->Write( newDn->GetName(), TObject::kOverwrite);
		
		newUp->Write( newUp->GetName() , TObject::kOverwrite);
		std::string namecycle = varHist+";1";
		dir->Delete(namecycle.c_str());
		
		delete newDn;
		delete newUp;
		return;
	}
	if( caseNumber == 2 ){
		//only make up histogram, there is no nominal here
		TH1D *hvar;
                hvar =  (TH1D*) dir->Get(varHist.c_str());
		TH1D* newUp = (TH1D*) hvar->Clone();
		if(newUp == NULL) return;
        //        newUp->SetName( (sigName+"_METUncer_GenMETUp_"+gridPoint).c_str() );
        //        newUp->SetTitle( ("h1D_"+sigName+"_METUncer_GenMETUp_"+gridPoint+"_"+dirName).c_str() );
	        newUp->SetName( (sigName+"_"+gridPoint+"_METUncer_GenMETUp").c_str() );
                newUp->SetTitle( ("h1D_"+sigName+"_"+gridPoint+"_METUncer_GenMETUp_"+dirName).c_str() );

		int nbinsx = newUp->GetNbinsX();
		double case2error=0.;
		for(int i=1; i<=nbinsx; i++){
			newUp->SetBinContent(i, newUp->GetBinContent(i)/2.);
			case2error = std::sqrt( (0.25)* newUp->GetBinError(i)*newUp->GetBinError(i) );
			newUp->SetBinError(i, case2error); 
		}
		dir->cd();
		newUp->Write( newUp->GetName() , TObject::kOverwrite);
                std::string namecycle = varHist+";1";
                dir->Delete(namecycle.c_str());
		delete newUp;
		return;
	}
	if( caseNumber == 3 ){
		return;
	}
	

}
void createGenMETVariations( TDirectoryFile* dir, TFile* fout  ){	
	//create the set of signal grid points (procs)
	std::set<std::string> nom_sigs{};
	std::set<std::string> genMET_sigs{};
	//possible variations: "JESUncer_TotalUp" "JESUncer_TotalDown" "JERUncer_TotalUp" "JERUncer_TotalDown" "METUncer_UnClustUp" "METUncer_UnClustDown" "METUncer_GenMET"
	
	TList* temp = dir->GetListOfKeys();
        TList* tl = (TList*) temp->Clone();
        std::cout<<"Num Histograms in dir:  "<< tl->GetSize() <<" \n";
        std::string tmpHist{};
	TKey *key;
        TIter nextkey( tl );
	

	 while((key=(TKey*)nextkey())){ //histogram loop///////////////////
                tmpHist = key->GetName();      
	//	std::cout<<"found hist: "<<tmpHist<<"\n";
		if(tmpHist.find("Up") == std::string::npos && tmpHist.find("Down") == std::string::npos && tmpHist.find("GenMET") == std::string::npos){
			nom_sigs.insert(tmpHist);
		}
		if(tmpHist.find("GenMET") != std::string::npos){
			genMET_sigs.insert(tmpHist);	
		}
	}
	

	std::string sigName;
	std::set<std::string> nomNumberSet{};
	std::set<std::string> varNumberSet{};
	std::string sigNumber;

	//check nominal sigs
	//std::cout<<"Found this nominal set \n";
	 for (auto& str : nom_sigs) {
       		// std::cout << str << ' ';
		 std::istringstream iss(str);
		 std::getline(iss, sigName, '_');
		 iss >> sigNumber;
		 nomNumberSet.insert(sigNumber);
    	}
	//no nominal bugs and only 1 genmet histogram fix
	if(sigName == ""){
	for(auto& str : genMET_sigs){
		std::istringstream iss(str);
                std::getline(iss, sigName, '_');
	}
	}
	
	//std::cout<<"\n Found this GenMET set \n";
	 for (auto& str : genMET_sigs){
		//sigNumber="";
		//std::cout<< str << '\n';
		std::istringstream iss(str);
		std::getline(iss, sigNumber, '_');
		std::getline(iss, sigNumber, '_');
		//std::getline(iss, sigNumber, '_');
		//std::getline(iss, sigNumber, '_');
		//it'll be the second substring	
		varNumberSet.insert(sigNumber);
	}
	
	/*
	std::cout<<"\n Found this signal \n"<< sigName << "\n";
	std::cout<<"\n Nominal set of grid points \n";
	for (auto& str : nomNumberSet){
		std::cout<< str << ' ';
	}
	std::cout<<"\n GenMET set of grid points \n";
	for (auto& str : varNumberSet){
		std::cout<< str << ' ';
	}
	std::cout<<"\n";	
	*/
	std::set<std::string> supersetNumber;
	supersetNumber = getSuperSetForPoints( nomNumberSet, varNumberSet );

	//for (auto&str : supersetNumber){
	//	std::cout<<"superset gp "<< str <<" \n";
//	}

	//analyze case per grid point in super set
	int caseNumber{};
	for(auto& gp : supersetNumber){
		caseNumber= getSetIntersectionForPoint(nomNumberSet, varNumberSet, gp);
	//	std::cout<<"Case for: "<<gp<<"="<<caseNumber<<" ";
		createHistograms( dir,  fout,  sigName, gp, caseNumber);
		//std::cout<<"\n";
	}
	std::cout<<"\n";
 

		
}
std::string getSubProcString( std::string proc, std::string subproc ){
	std::istringstream ss(proc);
	std::string token;
	std::string token2;
	std::getline(ss, token, '_');
	ss >> token2;
	std::string newsubproc;
	newsubproc= token+"_"+subproc+"_"+token2;
	return newsubproc;	
}
std::set<std::string> replaceProcTree( TTree* oldTree  ){

	//loop over the proc tree make a set of nominal signals
	//create variables for ttree to load into
	std::string* proc{};
	std::string* subproc{};
	int* type{};
	std::set<std::string> nominal_proc_set{};
	oldTree->SetBranchAddress("proc",&proc);
	oldTree->SetBranchAddress("subproc", &subproc);
	oldTree->SetBranchAddress("type", &type);

	Long64_t nentries = oldTree->GetEntries();
	std::cout<<"Looping over "<<nentries<<" procs\n";
	for(Long64_t i=0; i< nentries; i++){
		oldTree->GetEntry(i);
		if((*proc).find("Up") == std::string::npos && (*proc).find("Down") == std::string::npos && (*proc).find("GenMET") == std::string::npos){
			nominal_proc_set.insert(*proc);
	//		std::cout<<"adding nominal: "<<*proc<<"\n";	
		}
	}
	//delete oldTree;	
	return nominal_proc_set;
/*	//TTree* newTree = new TTree("temp", "temp");
	std::string newProc;
	std::string newSubProc;
	int newtype;
	newTree.Branch("proc", &newProc);
	newTree.Branch("subproc", &newSubProc);
	newTree.Branch("type",&newtype);

	//loop the set and fill a new tree with the proper procs and subprocs
	newtype=1;
	for(auto &nom_proc : nominal_proc_set){
			newProc=nom_proc;
		for(auto &sub_proc : subproc_list){
			//newSubProc=sub_proc;
			newSubProc = getSubProcString( nom_proc, sub_proc );
			//std::cout<<"subproc: "<<newSubProc<<"\n";
			//subproc convention should probably match hist name 
				
			newTree.Fill();
		}
	}
//	return newTree;
		
*/

}
 

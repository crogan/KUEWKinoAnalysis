#include <iostream>
#include "TH1D.h"
#include <set>
#include <vector>
#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TKey.h"

//bool procHasUpDn( std::string procname, TDirectory* catdir, std::pair<std::string,std::string> sys_string_pair){
		
	
//}
//return a set of processes that have up and down
std::string stripString( std::string procName, std::pair<std::string,std::string> sys_string_pair ){
//	std::cout<<"extracting substring\n";
	if(procName.find(sys_string_pair.first) != std::string::npos){
                return procName.erase( procName.find(sys_string_pair.first) ); //assume always erase to npos
        }
	if(procName.find(sys_string_pair.second) != std::string::npos){
                return procName.erase( procName.find(sys_string_pair.second) );
        }
	return std::string(" ");
} 
bool containsStringPair( std::string procName, std::pair<std::string,std::string> sys_string_pair ){
//	std::cout<<"comparing "<<procName<<" with "<<sys_string_pair.first<<" and "<< sys_string_pair.second <<"  \n";

	if(procName.length() > sys_string_pair.first.length() && procName.find(sys_string_pair.first) != std::string::npos){
		return true;	
	}
	if(procName.length() > sys_string_pair.second.length() && procName.find(sys_string_pair.second) != std::string::npos){
		return true;
	}
	return false;
}
std::set<std::string> getProcSet_updn( TDirectory* catdir, std::pair<std::string,std::string> sys_string_pair){
	//scan for string pair first or second, collect process attached to string
	std::set<std::string> procSet{};
	TList* proclist = catdir->GetListOfKeys();
	std::string procName{};
//	std::cout<<"checking Tlist proclist of size "<< proclist->GetSize()<< "\n";
	//proclist->Print();
	TKey *key;
	TIter nextkey(proclist);
//	while((key=(TKey*)nextkey())){
//		std::cout<<key->GetName()<<" ";
//	}
//	std::cout<<"\n";

	while((key=(TKey*)nextkey())){
		procName = std::string( key->GetName());
//		std::cout<<" checking proc "<< procName <<" \n";
		if( containsStringPair( procName, sys_string_pair )){
	//		std::cout<<"sub string found \n";
	//		std::cout<< procName <<" "<<stripString(procName,sys_string_pair)<<"\n";			
			procSet.insert( stripString( procName, sys_string_pair ));
		}
//		std::cout<<" finished checking proc \n"; 		
	}
	return procSet;
}
std::set<std::string> createSuperSet( std::set<std::string> set16, std::set<std::string> set17, std::set<std::string> set18 ){
	std::set<std::string> superset{};
	for (auto& proc: set16) superset.insert(proc);
	for (auto& proc: set17) superset.insert(proc);
	for (auto& proc: set18) superset.insert(proc);
	return superset; 
}
TDirectory* createDirectoryCopy( TDirectoryFile* tdir, TFile* ofile ){
	
	TDirectory* subdir = ofile->mkdir(tdir->GetName());
	//ofile->cd(tdir->GetName());
	subdir->cd();
	TKey *key;
	TIter nextkey( tdir->GetListOfKeys() );
	TH1D* th1;
	while((key=(TKey*)nextkey())){
		//ofile->WriteTObject( (TH1D*) ofile->Get(key->GetName()), key->GetName() );	
		
		th1 = (TH1D*) tdir->Get(key->GetName());
		
		th1->Write(key->GetName(),TObject::kOverwrite);
	//	ofile->Write(th1->GetName(), TObject::kOverwrite);
	//	key->ReadObj();
	//	ofile->Write();
	}
	//ofile->cd();
	//return the tdir so we can write to it
	return subdir;
}
TH1D* checkForNominal( TDirectoryFile* tdir, std::string hname, std::pair<std::string, std::string> sys_string_pair   ){
	//in some cases up/down variations exist but not the nominal, 
	//if this is true, copy an up/down hist and set entries to 1e-8
	//else return the nominal histogram
//	std::cout<<"checking For Proc "<<hname<<"\n";
	int nbinsx;
	TH1D* h;
	//if( tdir->Get( hname.c_str()) ){
	if( tdir->FindKey( hname.c_str()) ){
		h = (TH1D*) tdir->Get( hname.c_str() );
	//	std::cout<<"found nominal\n";
		return h;
	}
	else{
		//if( tdir->Get( (hname+sys_string_pair.first).c_str() ) != 0 ){
		if( tdir->FindKey( (hname+sys_string_pair.first).c_str()) ){
			 h = (TH1D*) tdir->Get( (hname+sys_string_pair.first).c_str() );
		}
		else if( tdir->FindKey( (hname+sys_string_pair.second).c_str() ) ){
			h = (TH1D*) tdir->Get( (hname+sys_string_pair.second).c_str() );
		}
		else{
	//		std::cout<<"Can not find a hist!\n";
		}
		h = (TH1D*) h->Clone();
		h->SetTitle( hname.c_str() );
		h->SetName( hname.c_str() );
		nbinsx = h->GetNbinsX();
		for(int i=1; i<=nbinsx; i++){
			h->SetBinContent(i,1e-8);
		}
		return h;
	}

}
//check if histogram exists (variation specifically, the nominal check is for the non primary sets
TH1D* checkForHist( TDirectoryFile* tdir, std::string hname, std::string sys_string, std::string other_sys ){
	TH1D* h;
	int nbinsx;
	 if( tdir->FindKey( (hname+sys_string).c_str()) ){
                         h = (TH1D*) tdir->Get( (hname+sys_string).c_str() );
                	 return h;
	}
	else if(tdir->FindKey( (hname).c_str())){
                
                
         //               std::cout<<"Can not find a hist!\n";
                
		h = (TH1D*) tdir->Get( hname.c_str());
                h = (TH1D*) h->Clone();
                h->SetTitle( (hname+sys_string).c_str() );
                h->SetName( (hname+sys_string).c_str() );
                nbinsx = h->GetNbinsX();
                for(int i=1; i<=nbinsx; i++){
                        h->SetBinContent(i,1e-8);
                }
                return h;
	}
	else if(tdir->FindKey( (hname+other_sys).c_str())){
		 h = (TH1D*) tdir->Get( (hname+other_sys).c_str());
                h = (TH1D*) h->Clone();
                h->SetTitle( (hname+sys_string).c_str() );
                h->SetName( (hname+sys_string).c_str() );
                nbinsx = h->GetNbinsX();
                for(int i=1; i<=nbinsx; i++){
                        h->SetBinContent(i,1e-8);
                }
                return h;
	}
	//std::cout<<"Can not find a hist!\n";
	return NULL;

}
void createPogVariations( TDirectoryFile* f_primary, TDirectoryFile* f_secondary1, TDirectoryFile* f_secondary2, 
	std::set<std::string> superset, std::set<std::string> primary_set, std::set<std::string> secondary1_set, std::set<std::string> secondary2_set,
	std::pair<std::string, std::string>  sys_string_pair, std::pair<std::string, std::string> out_sys_string, TFile* ofile){
	
	//clone the primary to write it to output file
	//TDirectoryFile* outDir = (TDirectoryFile*) f_primary->Clone();	
	//outDir->SetName( f_primary->GetName() );
	//outDir->SetTitle( f_primary->GetTitle() );	
	//debugging only do DB
	//superset = {"DB","ST"};
	
	TDirectory* subdir;
	//f_primary->cd();
	//check if dir is copied yet
	if(ofile->GetDirectory(f_primary->GetName()) == 0){
		subdir = createDirectoryCopy( f_primary,  ofile );
	}
	else{
		subdir = ofile->GetDirectory(f_primary->GetName());
	}

	bool is_in_primary, is_in_secondary1, is_in_secondary2;
	for( auto& proc : superset ){
	//	std::cout<<"doing process "<< proc << "\n";
		is_in_primary     = primary_set.find(proc) != primary_set.end();
		is_in_secondary1  = secondary1_set.find(proc) != secondary1_set.end();
		is_in_secondary2  = secondary2_set.find(proc) != secondary2_set.end();
	//	std::cout<<"set contents: "<<is_in_primary<<" "<<is_in_secondary1<<" "<<is_in_secondary2<<"\n";
	//CASE1 all three processes exist
		if( is_in_primary && is_in_secondary1 && is_in_secondary2 ){
			//do first then second
		//	TH1D* bdn = (TH1D*) f_primary->Get( (proc+sys_string_pair.first).c_str() );
		//	TH1D* bup = (TH1D*) f_primary->Get( (proc+sys_string_pair.second).c_str() );
			//TH1D* bdn = checkForNominal( f_primary, proc, sys_string_pair );
                       // TH1D* bup = checkForNominal( f_primary, proc, sys_string_pair );
		//	TH1D* bdn = checkForNominal( f_primary, proc
			TH1D* bdn = checkForHist( f_primary, proc, sys_string_pair.first, sys_string_pair.second);
			TH1D* bup = checkForHist( f_primary, proc, sys_string_pair.second, sys_string_pair.first);
			

	//		std::cout<<" getting: "<< proc+sys_string_pair.first<<"\n";
	//		std::cout<<" newdn name "<<bdn->GetName()<<"\n";

			TH1D* newdn = (TH1D*) bdn->Clone();
			TH1D* newup = (TH1D*) bup->Clone();
	//		std::cout<<" newdn name "<<bdn->GetName()<<"\n";
			//get the other years
			//TH1D* sec1 = (TH1D*) f_secondary1->Get( proc.c_str() );
			//TH1D* sec2 = (TH1D*) f_secondary2->Get( proc.c_str() );
			TH1D* sec1 = checkForNominal( f_secondary1, proc, sys_string_pair );
			TH1D* sec2 = checkForNominal( f_secondary2, proc, sys_string_pair );			

			newdn->Add(sec1);
			newdn->Add(sec2);
			newup->Add(sec1);
			newup->Add(sec2);

			//rename and write to output
		//	newdn->SetName( (std::string(newdn->GetName())+out_sys_string).c_str() );
		//	newup->SetName( (std::string(newup->GetName())+out_sys_string).c_str() );
			newdn->SetTitle( (proc+out_sys_string.first).c_str() );
			newup->SetTitle( (proc+out_sys_string.second).c_str() );


			subdir->cd();
			newdn->Write( (proc+out_sys_string.first).c_str(), TObject::kOverwrite);
			newup->Write( (proc+out_sys_string.second).c_str(), TObject::kOverwrite);
			
			delete newdn;
			delete newup;
			delete bdn;
			delete bup;
			//ofile->Write();	
		}//end case 1
	//CASE2 exists in primary but not one or both secondaries .. this is a more general implementation of case 1
		if( is_in_primary && !(is_in_secondary1 && is_in_secondary2) ){
		//	TH1D* bdn = (TH1D*) f_primary->Get( (proc+sys_string_pair.first).c_str() );
                  //      TH1D* bup = (TH1D*) f_primary->Get( (proc+sys_string_pair.second).c_str() );
			//TH1D* bdn = checkForNominal( f_primary, proc, sys_string_pair );
                        //TH1D* bup = checkForNominal( f_primary, proc, sys_string_pair );
                        TH1D* bdn = checkForHist( f_primary, proc, sys_string_pair.first, sys_string_pair.second);
                        TH1D* bup = checkForHist( f_primary, proc, sys_string_pair.second, sys_string_pair.first);

			TH1D* newdn = (TH1D*) bdn->Clone();
                        TH1D* newup = (TH1D*) bup->Clone();
			std::vector<TH1D*> htmp{};
			if( is_in_secondary1 ){
				//TH1D* sec1 = (TH1D*) f_secondary1->Get( proc.c_str() );
				TH1D* sec1 = checkForNominal( f_secondary1, proc, sys_string_pair );
				htmp.push_back(sec1);
			}
			if( is_in_secondary2 ){
				//TH1D* sec2 = (TH1D*) f_secondary2->Get( proc.c_str() );
				TH1D* sec2 = checkForNominal( f_secondary2, proc, sys_string_pair );
				htmp.push_back(sec2);
			}
			for(int i=0; i<htmp.size(); i++){
				newdn->Add( htmp[i] );
				newup->Add( htmp[i] );	
			}
			newdn->SetTitle( (proc+out_sys_string.first).c_str() );
                        newup->SetTitle( (proc+out_sys_string.second).c_str() );
		
			subdir->cd();
                        newdn->Write( (proc+out_sys_string.first).c_str(), TObject::kOverwrite);
                        newup->Write( (proc+out_sys_string.second).c_str(), TObject::kOverwrite);

			delete newdn;
			delete newup;
			for(int i=0; i< htmp.size(); i++){
				delete htmp[i];
			}	
		}//end case 2	
	//CASE3 exists in secondary but not in primary
		if( !is_in_primary ){// it is guaranteed to exist in one of the other sets
			std::vector<TH1D*> htmp{};
			if( is_in_secondary1 ){
			      // TH1D* sec1 = (TH1D*) f_secondary1->Get( proc.c_str() );
			      	TH1D* sec1 = checkForNominal( f_secondary1, proc, sys_string_pair );
                                htmp.push_back(sec1);
                        }
                        if( is_in_secondary2 ){
                             //   TH1D* sec2 = (TH1D*) f_secondary2->Get( proc.c_str() );
                             	TH1D* sec2 = checkForNominal( f_secondary2, proc, sys_string_pair );
                                htmp.push_back(sec2);
                        }
			//use the secondary index 0 as the up//dn template
			TH1D* newdn = (TH1D*) htmp[0]->Clone();
			TH1D* newup = (TH1D*) htmp[0]->Clone();
			//only add the other histogram if it exists
			if( htmp.size() > 1 ){
				newdn->Add( htmp[1] );
				newup->Add( htmp[1] );
			}
			newdn->SetTitle( (proc+out_sys_string.first).c_str() );
                        newup->SetTitle( (proc+out_sys_string.second).c_str() );

                        subdir->cd();
                        newdn->Write( (proc+out_sys_string.first).c_str(), TObject::kOverwrite);
                        newup->Write( (proc+out_sys_string.second).c_str(), TObject::kOverwrite);

			delete newdn;
			delete newup;
			for(int i=0; i< htmp.size(); i++){
                                delete htmp[i];
                        }
		}//end case 3
	}
	


}
void printSet(std::set<std::string> set){
	std::cout<<"Printing set: ";
        for (auto& proc : set ) {
        std::cout << proc << ' ';
        }
        std::cout<<"\n";

}
void loadSuperSuperSet( std::set<std::string>& bigset, std::set<std::string>& littleset){
	for (auto& proc: littleset) bigset.insert(proc);	
}
//TTree* updateProcTree(TTree* old_tree,std::set<std::string> supersuperset, std::vector<std::string> sys_strings ){
TTree* updateProcTree(std::set<std::string> supersuperset, std::vector<std::string> sys_strings ){
	std::string proc,subproc;
	int type;
	TTree* newProcTree = new TTree("Process","Process");
	newProcTree->Branch("proc", &proc);
	newProcTree->Branch("subproc", &subproc);
	newProcTree->Branch("type", &type);

	std::cout<<"Filling New tree\n";
	for( auto& iproc : supersuperset ){
		proc = iproc;
		//ASSUME WE ONLY PROCESS BKG -> SET TYPE TO ENUM kBkg =0
		type = 0;
		for(int i=0; i<sys_strings.size(); i++){
			subproc = proc+sys_strings[i];
			newProcTree->Fill();
		}
	}
//	newProcTree->Print();
	return newProcTree;

}
void generateSuperSuperSet(TTree* proctree, std::set<std::string>& set){
	std::string* proc=0;
	proctree->SetBranchAddress("proc",&proc);
	Int_t nEntries = (Int_t) proctree->GetEntries();
	for(Int_t i=0; i<nEntries; i++){
		proctree->GetEntry(i);
//		std::cout<<"proc "<<*proc<<"\n";
		set.insert(*proc);
	}
//	std::cout<<"proc tree read complete\n";
}
void copyAndUpdateTrees(TFile* ifile, std::string ofilename, std::vector<std::string> str_register_list){
//write trees first! buffer cant handle it at the end, trees won't write if you do it at the end
        //get the old Process tree, read it and create a list of all possible 16 processes
        TTree *oldtree;
        ifile->GetObject("Process", oldtree);
        std::set<std::string> supersuperset{};
        generateSuperSuperSet(oldtree, supersuperset);
        delete oldtree;
        ifile->GetObject("Process", oldtree);
        //remake file and update the process tree witht the new potential sys strings
        TFile* ofile = new TFile(ofilename.c_str(), "RECREATE");
        //std::vector<std::string sys_strings_to_register16 = {"_BTAGHF16_SFDown","_BTAGHF16_SFUp","_BTAGLF16_SFDown","_BTAGLF16_SFUp"};
        auto newtree = oldtree->CloneTree();
        auto addtree = updateProcTree( supersuperset, str_register_list );
        TList *list = new TList();
        list->Add(oldtree);
        list->Add(addtree);
        TTree *combinedTree = TTree::MergeTrees(list);
        combinedTree->SetName("Process");
        //combinedTree->Print();
        combinedTree->Write("",TObject::kOverwrite);
        // copy over the category tree
        TTree *temp_tcat  = (TTree*)ifile->Get("Category");
        temp_tcat->CloneTree()->Write();
	delete list;
        delete oldtree;
        delete newtree;
        delete combinedTree;
        delete temp_tcat;
        ifile->Close();
        ofile->Close();
        delete ifile;
        delete ofile;

}

void ProcessVariations(std::pair<std::string,std::string> input_sys_pair,std::pair<std::string,std::string> out_sys16, std::pair<std::string,std::string> out_sys17, std::pair<std::string,std::string> out_sys18,  TDirectoryFile* dir16 , TDirectoryFile* dir17,  TDirectoryFile* dir18, TFile*  fout16, TFile*  fout17, TFile*  fout18  ){
	
	std::set<std::string> procSet16 = getProcSet_updn( dir16, input_sys_pair);
        std::set<std::string> procSet17 = getProcSet_updn( dir17, input_sys_pair);
        std::set<std::string> procSet18 = getProcSet_updn( dir18, input_sys_pair);
        std::set<std::string>superset = createSuperSet( procSet16, procSet17, procSet18);
        createPogVariations( dir16, dir17, dir18, superset, procSet16, procSet17, procSet18, input_sys_pair, out_sys16, fout16);
	createPogVariations( dir17, dir16, dir18, superset, procSet17, procSet16, procSet18, input_sys_pair, out_sys17, fout17);
	createPogVariations( dir18, dir16, dir17, superset, procSet18, procSet16, procSet17, input_sys_pair, out_sys18, fout18);


}
/*

//void processBFI(){
int main( int argc, char *argv[] ){	
	//std::string path16 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B109/oof/BFS_B109_BKG16_DATA16_TChiWZ16_SYS_Ldef.root";	
	//std::string path17 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B109/oof/BFS_B109_BKG17_DATA17_TChiWZ17_SYS_Ldef.root"; 
	//std::string path18 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B109/oof/BFS_B109_BKG18_DATA18_TChiWZ18_SYS_Ldef.root";
	
//	std::string path16 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B116/BFS_B116_BKG16_DATA16_SYS16.root";
//	std::string path17 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B116/BFS_B116_BKG17_DATA17_SYS17.root";
//	std::string path18 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B116/BFS_B116_BKG18_DATA18_SYS18.root";

//	std::string path16 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B117/BFS_B117_BKG16_DATA16_SYS16.root";
 //       std::string path17 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B117/BFS_B117_BKG17_DATA17_SYS17.root";
//        std::string path18 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B117/BFS_B117_BKG18_DATA18_SYS18.root";

	//reduced sys builds to try and make diagnostics faster
//	std::string path16 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B121/BFS_B117_BKG16_DATA16_SYS16.root";
//	std::string path17 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B121/BFS_B117_BKG17_DATA17_SYS17.root";
//	std::string path18 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B121/BFS_B117_BKG18_DATA18_SYS18.root";

	std::string path16 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B125/BFS_B125_BKG16_DATA16_SYS16.root";
        std::string path17 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B125/BFS_B125_BKG17_DATA17_SYS17.root";
        std::string path18 = "/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B125/BFS_B125_BKG18_DATA18_SYS18.root";


	TFile* f16 = TFile::Open(path16.c_str());
	TFile* f17 = TFile::Open(path17.c_str());
	TFile* f18 = TFile::Open(path18.c_str());

	std::string outname16 = "BFI_B125_BKG16_DATA16_SYSYEAR16.root";
	std::string outname17 = "BFI_B125_BKG17_DATA17_SYSYEAR17.root";
	std::string outname18 = "BFI_B125_BKG18_DATA18_SYSYEAR18.root";

/////////////////////////////STEP 1 -- Udate proc/sys lists! //////////////////////////////////////////////////////
	//copy Cat tree, update procc tree and write it to output
	//pop these out for now
	//use these string as default (comment out default for b121)
	std::vector<std::string> sys_strings_to_register16 = {"_BTAGHF16_SFDown","_BTAGHF16_SFUp","_BTAGLF16_SFDown","_BTAGLF16_SFUp","_JESUncer16_TotalDown", "_JESUncer16_TotalUp", "_JERUncer16_TotalDown", "_JERUncer16_TotalUp", "_METUncer16_UnClustDown", "_METUncer16_UnClustUp"    };
	copyAndUpdateTrees(f16, outname16 , sys_strings_to_register16);

        //reduced year splitting strings for b121 (shortened version to get a diagnostic out faster)
	//std::vector<std::string> sys_strings_to_register16 = {"_BTAGHF16_SFDown","_BTAGHF16_SFUp","_BTAGLF16_SFDown","_BTAGLF16_SFUp","_JERUncer16_TotalDown", "_JERUncer16_TotalUp"  };
        //copyAndUpdateTrees(f16, outname16 , sys_strings_to_register16);

	//Repeat the whole tree writing process for 2017
	std::vector<std::string> sys_strings_to_register17 = {"_BTAGHF17_SFDown","_BTAGHF17_SFUp","_BTAGLF17_SFDown","_BTAGLF17_SFUp","_JESUncer17_TotalDown", "_JESUncer17_TotalUp", "_JERUncer17_TotalDown", "_JERUncer17_TotalUp", "_METUncer17_UnClustDown", "_METUncer17_UnClustUp" };
        copyAndUpdateTrees(f17, outname17 , sys_strings_to_register17);

	//reduced year splitting for b121
	//std::vector<std::string> sys_strings_to_register17 = {"_BTAGHF17_SFDown","_BTAGHF17_SFUp","_BTAGLF17_SFDown","_BTAGLF17_SFUp", "_JERUncer17_TotalDown", "_JERUncer17_TotalUp" };
        //copyAndUpdateTrees(f17, outname17 , sys_strings_to_register17);


	std::vector<std::string> sys_strings_to_register18 = {"_BTAGHF18_SFDown","_BTAGHF18_SFUp","_BTAGLF18_SFDown","_BTAGLF18_SFUp","_JESUncer18_TotalDown", "_JESUncer18_TotalUp", "_JERUncer18_TotalDown", "_JERUncer18_TotalUp", "_METUncer18_UnClustDown", "_METUncer18_UnClustUp" };
	copyAndUpdateTrees(f18, outname18 , sys_strings_to_register18);

	//reduced year splitting for b121
//	 std::vector<std::string> sys_strings_to_register18 = {"_BTAGHF18_SFDown","_BTAGHF18_SFUp","_BTAGLF18_SFDown","_BTAGLF18_SFUp", "_JERUncer18_TotalDown", "_JERUncer18_TotalUp" };
 //       copyAndUpdateTrees(f18, outname18 , sys_strings_to_register18);

	f16 = TFile::Open(path16.c_str());//open to get tlist of category keys to loop over
	//Code assumes each year has every category populated
	//Get the list of categories, clone and loop over it
	TList* temp = f16->GetListOfKeys();	
	TList* tl = (TList*) temp->Clone();
	std::cout<<"Num categories = "<< tl->GetSize()-2 <<" \n"; 
	std::string testCat{};
	TKey *key;
        TIter nextkey( tl );
	f16->Close();
	delete f16;


	TFile *fout16,*fout17,*fout18;
/////////////////////////////////STEP 2 -- LOOP OVER CATEGORIES & ADD HISTS //////////////////////////////////

        while((key=(TKey*)nextkey())){ //category loop///////////////////
      		testCat = key->GetName();

	//open and close all inputs and outputs each iteration to reduce time complexity significantly
	std::cout<<"Opening ROOT File\n";
	fout16 = new TFile(outname16.c_str(), "UPDATE");
	fout17 = new TFile(outname17.c_str(), "UPDATE");
	fout18 = new TFile(outname18.c_str(), "UPDATE");

//	if( !(f16->IsOpen()) ){
		f16 = TFile::Open(path16.c_str());
		f17 = TFile::Open(path17.c_str());
		f18 = TFile::Open(path18.c_str());
//	}

	
	if( testCat == "Process" || testCat == "Category"){
		fout16->Close();
        	fout17->Close();
        	fout18->Close();

        	f16->Close();
        	f17->Close();
      	 	 f18->Close();
        	delete fout16;
        	delete fout17;
        	delete fout18;
        	delete f16;
        	delete f17;
        	delete f18;
		continue;
	}
	
	//test category
	// Ch0L_0_0j1svS_ge1jISR_PTISR0_gamT0_SVeta0
//	std::string testCat = "Ch0L_0_0j1svS_ge1jISR_PTISR0_gamT0_SVeta0";
//	std::string testCat = "Ch1L_elm_gold_0j0svS_ge1jge1bISR_PTISR0";
//	std::string testCat = "Ch1L_elpm_slvr_2jS_ge1jISR_PTISR1_gamT0";
//	std::string testCat = "Ch1L_elm_gold_0j0svS_ge1jge1bISR_PTISR0";
	//std::cout<<"Analyzing category: Ch0L_0_0j1svS_ge1jISR_PTISR0_gamT0_SVeta0 \n";
	std::cout<<"Analyzing category: "<<testCat<<"\n";
	TDirectoryFile* dir16 = (TDirectoryFile*) f16->Get(testCat.c_str());
	TDirectoryFile* dir17 = (TDirectoryFile*) f17->Get(testCat.c_str());
	TDirectoryFile* dir18 = (TDirectoryFile*) f18->Get(testCat.c_str());

	TList* tprocl = dir16->GetListOfKeys();
	//tprocl->Print();	
	std::set<std::string> procSet16, procSet17, procSet18;
	std::pair<std::string,std::string> sys_string_pair;
	sys_string_pair.first = "_BTAGHF_SFDown";
	sys_string_pair.second= "_BTAGHF_SFUp";

	std::pair<std::string,std::string> out_sys_pair;
	out_sys_pair.first = "_BTAGHF16_SFDown";
	out_sys_pair.second = "_BTAGHF16_SFUp";

	procSet16 = getProcSet_updn( dir16, sys_string_pair);
	procSet17 = getProcSet_updn( dir17, sys_string_pair);
	procSet18 = getProcSet_updn( dir18, sys_string_pair);
	
	std::set<std::string> superset = createSuperSet( procSet16, procSet17, procSet18 );
	//load the superset into the supersuperset to register in process tree later
//	loadSuperSuperSet( supersuperset, superset);

	
         std::cout<<"Process superset: ";
        for (auto& proc : superset ) {
        std::cout << proc << ' ';
        }
        std::cout<<"\n";
	
	//debugging test cases where process is not present in other years
	//superset={"DB"};
	//case2
	//procSet16 = {"DB"}; procSet17={}; procSet18={"DB"};
	//case2b
	//procSet16 = {"DB"}; procSet17={}; procSet18={};
	//case3
	//procSet16 = {}; procSet17={"DB"}; procSet18={};
	//case3b
	//procSet16 = {}; procSet17={"DB"}; procSet18={"DB"};


	///Create pog variations for HF
	out_sys_pair.first = "_BTAGHF16_SFDown";
        out_sys_pair.second = "_BTAGHF16_SFUp";
	createPogVariations( dir16, dir17, dir18, superset, procSet16, procSet17, procSet18, sys_string_pair, out_sys_pair, fout16);

	out_sys_pair.first = "_BTAGHF17_SFDown";
        out_sys_pair.second = "_BTAGHF17_SFUp";
	createPogVariations( dir17, dir16, dir18, superset, procSet17, procSet16, procSet18, sys_string_pair, out_sys_pair, fout17);

	out_sys_pair.first = "_BTAGHF18_SFDown";
        out_sys_pair.second = "_BTAGHF18_SFUp";
	createPogVariations( dir18, dir16, dir17, superset, procSet18, procSet16, procSet17, sys_string_pair, out_sys_pair, fout18);	

	//do LF
	sys_string_pair.first =  "_BTAGLF_SFDown";
	sys_string_pair.second = "_BTAGLF_SFUp";

	//Make a new proc superset in case the proc are different for LF
	procSet16 = getProcSet_updn( dir16, sys_string_pair);
        procSet17 = getProcSet_updn( dir17, sys_string_pair);
        procSet18 = getProcSet_updn( dir18, sys_string_pair);
	
        superset = createSuperSet( procSet16, procSet17, procSet18 );
	//load supersuperset
//	loadSuperSuperSet( supersuperset, superset);


	//Create pog variations for LF
	out_sys_pair.first = "_BTAGLF16_SFDown";
        out_sys_pair.second = "_BTAGLF16_SFUp";
	createPogVariations( dir16, dir17, dir18, superset, procSet16, procSet17, procSet18, sys_string_pair, out_sys_pair, fout16);
        
	out_sys_pair.first = "_BTAGLF17_SFDown";
        out_sys_pair.second = "_BTAGLF17_SFUp";
	createPogVariations( dir17, dir16, dir18, superset, procSet17, procSet16, procSet18, sys_string_pair, out_sys_pair, fout17);
        
        out_sys_pair.first = "_BTAGLF18_SFDown";
        out_sys_pair.second = "_BTAGLF18_SFUp";
	createPogVariations( dir18, dir16, dir17, superset, procSet18, procSet16, procSet17, sys_string_pair, out_sys_pair, fout18);

	//do other variatons 
	//MUF	
	*/
/*
	 ProcessVariations(std::make_pair("_MuF_SFDown","_MuF_SFUp"),
	 std::make_pair("_MuF16_SFDown","_MuF16_SFUp"),
	 std::make_pair("_MuF17_SFDown","_MuF17_SFUp"),
	 std::make_pair("_MuF18_SFDown","_MuF18_SFUp"), dir16 , dir17, dir18, fout16, fout17, fout18  );

	//MUR
	ProcessVariations(std::make_pair("_MuR_SFDown","_MuR_SFUp"),
         std::make_pair("_MuR16_SFDown","_MuR16_SFUp"), 
         std::make_pair("_MuR17_SFDown","_MuR17_SFUp"),
         std::make_pair("_MuR18_SFDown","_MuR18_SFUp"), dir16 , dir17, dir18, fout16, fout17, fout18  );
	
	//PDF
	ProcessVariations(std::make_pair("_PDF_SFDown","_PDF_SFUp"),
         std::make_pair("_PDF16_SFDown","_PDF16_SFUp"), 
         std::make_pair("_PDF17_SFDown","_PDF17_SFUp"),
         std::make_pair("_PDF18_SFDown","_PDF18_SFUp"), dir16 , dir17, dir18, fout16, fout17, fout18  );
*/	
 /*
	//JES
	ProcessVariations(std::make_pair("_JESUncer_TotalDown","_JESUncer_TotalUp"),
         std::make_pair("_JESUncer16_TotalDown","_JESUncer16_TotalUp"),
         std::make_pair("_JESUncer17_TotalDown","_JESUncer17_TotalUp"),
         std::make_pair("_JESUncer18_TotalDown","_JESUncer18_TotalUp"), dir16 , dir17, dir18, fout16, fout17, fout18  );

	//JER
	ProcessVariations(std::make_pair("_JERUncer_TotalDown","_JERUncer_TotalUp"),
         std::make_pair("_JERUncer16_TotalDown","_JERUncer16_TotalUp"),
         std::make_pair("_JERUncer17_TotalDown","_JERUncer17_TotalUp"),
         std::make_pair("_JERUncer18_TotalDown","_JERUncer18_TotalUp"), dir16 , dir17, dir18, fout16, fout17, fout18  );

	ProcessVariations(std::make_pair("_METUncer_UnClustDown","_METUncer_UnClustUp"),
         std::make_pair("_METUncer16_UnClustDown","_METUncer16_UnClustUp"),
         std::make_pair("_METUncer17_UnClustDown","_METUncer17_UnClustUp"),
         std::make_pair("_METUncer18_UnClustDown","_METUncer18_UnClustUp"), dir16 , dir17, dir18, fout16, fout17, fout18  );
	

	//Close all inputs and outputs each iteration
//	std::cout<<"closing 16 output...\n";
        fout16->Close();
//        std::cout<<"closing 17 output...\n";
        fout17->Close();
 //       std::cout<<"closing 18 output...\n";
        fout18->Close();
	
//	std::cout<<"closing inputs...\n";
	f16->Close();
	f17->Close();
	f18->Close();
	delete fout16;
	delete fout17;
	delete fout18;
	delete f16;
	delete f17;
	delete f18;

	//debug break
//	break;

}//end category loop
}//end macro

*/

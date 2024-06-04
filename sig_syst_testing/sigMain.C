


#include "../bpog_syst_testing/processBFI.C"
#include "processSigBFI.C"

/*
This macro takes in a BFI which has signal systematics (genMET in particular)
The output is a BFI where additional Up/Down histograms of genMET have been created
The output will only have genMET up/down and should be hadded into the main BFI
*/
int main(int argc, char *argv[] ){


	 std::cout<< "running with args \n";
        for(int i=0; i<argc; i++){
                std::cout<<argv[i]<<"\n";
        }


        std::string fpath = std::string(argv[1]);
        
	TFile* f = TFile::Open(fpath.c_str());
              
//	std::string outname = std::string(argv[2]);
       	
	//assume bpog processing will already add the additional tree sys to the systematic list
	//we only address genMET	
//	std::vector<std::string> sys_strings_to_register = {};
//        copyAndUpdateTrees(f, outname , sys_strings_to_register);

	//f = TFile::Open(path16.c_str());//open to get tlist of category keys to loop over
        //Code assumes each year has every category populated
        //Get the list of categories, clone and loop over it
        TList* temp = f->GetListOfKeys();
        TList* tl = (TList*) temp->Clone();
        std::cout<<"Num categories = "<< tl->GetSize()-2 <<" \n";
        std::string testCat{};
        TKey *key;
        TIter nextkey( tl );
        f->Close();
        delete f;


        //TFile *fout;



         while((key=(TKey*)nextkey())){ //category loop///////////////////
                testCat = key->GetName();

	//testcat debug
	// testCat ="Ch1L_elpm_bron_ge4jS_ge1jISR_PTISR0_gamT0";
        //open and close all inputs and outputs each iteration to reduce time complexity significantly
        std::cout<<"Opening ROOT File\n";
        f = new TFile(fpath.c_str(), "UPDATE");
 
       if( testCat == "Process" || testCat == "Category"){
                f->Close();
                delete f;
                continue;
        }

	std::cout<<"Analyzing category: "<<testCat<<"\n";
        TDirectoryFile* dir = (TDirectoryFile*) f->Get(testCat.c_str());	
        createGenMETVariations( dir, f  );

	f->Close();
	delete f;
	//break;//debug  break
	}
	
	/*std::vector<std::string> subproc_list{"JESUncer_TotalUp", "JESUncer_TotalDown", "JERUncer_TotalUp", "JERUncer_TotalDown", "METUncer_UnClustUp", "METUncer_UnClustDown", "METUncer_GenMETUp", "METUncer_GenMETDown" };
	
	//f = new TFile(fpath.c_str(), "READ");
	f= TFile::Open(fpath.c_str());
	//gDirectory->pwd();
	TTree* oldTree = (TTree*) f->Get("Process");
	std::string oldName = std::string(oldTree->GetName());
	std::string oldTitle = std::string(oldTree->GetTitle());
	std::cout<<"allocating tree\n";
	//TTree* newTree= new TTree("temptree","temptree");
//	TTree newTree("temptree","temptree");
//	f->ls();
//	gDirectory->pwd();
//	std::cout<<"GDIR "<<gDirectory->GetName()<<"\n";
//	std::cout<<"newtree dir "<<newTree.GetDirectory()->GetName()<<"\n";
//	newTree=replaceProcTree(oldTree,newTree, subproc_list );
	std::set<std::string> nominalprocset;

	nominalprocset = replaceProcTree(oldTree);
	f->Close();
	//f->ls();
	//f->cd();
//	std::cout<<"attempt to delete: "<<oldName+";1";
//	gDirectory->Delete((oldName+";1").c_str());	
	//f->Close();
	//newTree->SetName(oldName.c_str());
	//newTree->SetTitle(oldTitle.c_str());
*///	f->cd();
/*	TFile* f2 = new TFile("tempProcTree.root","RECREATE");
	//newTree->AutoSave();
	//newTree->Write(newTree->GetName() , TObject::kOverwrite);
	f2->cd();
	newTree.Write();
//	f->WriteTObject(newTree);
//	f->Flush();
//	f->Write();
	f2->Close();
	f->Close();
//	delete f;
//

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

*/

return 0;
}

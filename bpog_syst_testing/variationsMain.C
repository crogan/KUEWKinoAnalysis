#include <iostream>
#include "TH1D.h"
#include <set>
#include <vector>
#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TKey.h"
#include "processBFI.C"

//void processBFI(){
int main( int argc, char *argv[] ){	
	//input args are 6 strings, 1-3 are input BFI per year (16,17,18), 4-6 are ouput BFI per year (16,17,18)
	//example 2017
	// in: /uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/BFI_B125/BFS_B125_BKG17_DATA17_SYS17.root
	// out: BFI_B125_BKG17_DATA17_SYSYEAR17.root
	//
	std::cout<< "running with args \n";
	for(int i=0; i<argc; i++){
		std::cout<<argv[i]<<"\n";
	}
	std::string path16 = std::string(argv[1]);
        std::string path17 = std::string(argv[2]);
        std::string path18 = std::string(argv[3]);

	TFile* f16 = TFile::Open(path16.c_str());
	TFile* f17 = TFile::Open(path17.c_str());
	TFile* f18 = TFile::Open(path18.c_str());

	std::string outname16 = std::string(argv[4]);
	std::string outname17 =	std::string(argv[5]);
	std::string outname18 = std::string(argv[6]);

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
		//break;

}//end category loop
}//end macro

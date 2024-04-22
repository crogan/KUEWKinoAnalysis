
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



void jankyTTreeMacro(std::string inputFile){
	
	TFile* f = TFile::Open(inputFile.c_str());
	TTree* oldTree = (TTree*) f->Get("Process");
	std::string oldName = std::string(oldTree->GetName());
        std::string oldTitle = std::string(oldTree->GetTitle());
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
                      std::cout<<"adding nominal: "<<*proc<<"\n";     
                }
        }

	f->Close();

	std::vector<std::string> subproc_list{"JESUncer_TotalUp", "JESUncer_TotalDown", "JERUncer_TotalUp", "JERUncer_TotalDown", "METUncer_UnClustUp", "METUncer_UnClustDown", "METUncer_GenMETUp", "METUncer_GenMETDown" };	
	//std::vector<std::string> subproc_list{"METUncer_GenMETUp", "METUncer_GenMETDown"};
	TFile* fnew = new TFile("testtree.root","RECREATE");
	TTree* newTree= new TTree(oldName.c_str(),oldTitle.c_str());	
	std::string newProc;
        std::string newSubProc;
        int newtype;
        newTree->Branch("proc", &newProc);
        newTree->Branch("subproc", &newSubProc);
        newTree->Branch("type",&newtype);

        //loop the set and fill a new tree with the proper procs and subprocs
        newtype=1;
        for(auto &nom_proc : nominal_proc_set){
                        newProc=nom_proc;
                for(auto &sub_proc : subproc_list){
                        //newSubProc=sub_proc;
                       // newSubProc = getSubProcString( nom_proc, sub_proc );
                       	newSubProc = nom_proc+"_"+sub_proc;
                        //std::cout<<"subproc: "<<newSubProc<<"\n";
                        //subproc convention should probably match hist name 
                        newTree->Fill();
                }
        }
	//the nominal proc also needs the nominal name as sub proc to be picked up
	for(auto &nom_proc : nominal_proc_set){
		newProc=nom_proc;
		newSubProc=nom_proc;
		newTree->Fill();
	}

	newTree->Write();
	fnew->Close();

	TFile* fdelete = new TFile(inputFile.c_str(), "UPDATE");
	fdelete->Delete("Process;1");
	fdelete->Close();

}

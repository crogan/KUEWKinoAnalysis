std::vector<std::string> ReadList( std::string fileList){
        std::vector<std::string> filevec{};
        std::ifstream f;
        f.open(fileList);
        std::string ifile{};
        if( f.is_open()){
                while( f >> ifile ) {
                //      std::cout << "adding item: "<<ifile<<"\n";
                        filevec.push_back(ifile);
                }
                f.close();
        }
        else{ std::cout<< "filelist not found\n"; }
        return filevec;
}


void ScanRMS(std::string filelist){

    std::vector<std::string> filevec;
    filevec = ReadList(filelist);
    std::string fname{};
    for(int i=0; i<filevec.size(); i++){
        fname = filevec[i]+".root";
        TFile *f = TFile::Open(fname.c_str());
        TList* tl = f->GetListOfKeys();
        TIter next(f->GetListOfKeys());
        TKey *key;
	std::string hname{};
	TH1D *h;
	double stddev=-1;
        while ((key=(TKey*)next())) {
            //std::cout<<key->GetName()<<"\n";
	    hname = std::string(key->GetName());
	    h = (TH1D*) f->Get(hname.c_str());
	    stddev = h->GetStdDev();
	    std::cout<<hname<<" "<<stddev<<"\n";
        }
	f->Close();
    // break;
    }

}

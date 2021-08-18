#include "E_bank.hh"

E_bank::E_bank(){};

E_bank::E_bank(int year, std::string f16, std::string f17, std::string f18, std::string histPath){
	//set default year to draw from
	_year = year;
	//is there TnP of fullsimfastsim sf?
	int canv_primitive_index=1;
	if(histPath==""){
		canv_primitive_index=0;
	}	

	TFile* f1 = TFile::Open(f16.c_str());
	f1->cd(histPath.c_str());
	std::string name = (gDirectory->GetListOfKeys()->At(0)->GetName());
	populateMap((TCanvas*) f1->Get((histPath+name).c_str()),_map16,canv_primitive_index);
	f1->Close();

	TFile* f2 = TFile::Open(f17.c_str());
	f2->cd(histPath.c_str());
	name = (gDirectory->GetListOfKeys()->At(0)->GetName());
	populateMap((TCanvas*) f2->Get((histPath+name).c_str()),_map17,canv_primitive_index);
	f2->Close();

	TFile* f3 = TFile::Open(f18.c_str());
	f3->cd(histPath.c_str());
	name = (gDirectory->GetListOfKeys()->At(0)->GetName());
	populateMap((TCanvas*) f3->Get((histPath+name).c_str()),_map18,canv_primitive_index);
	f3->Close();

}

void E_bank::populateMap(TCanvas* hCanv ,std::map<std::pair<double,double>, std::pair<double,double> >& map , int CanvIdx){
	
	TList* t = hCanv->GetListOfPrimitives();//index 0 TFrame, index 1 TH1
	//TCanvas* cnew = new TCanvas();//fullsim fast sim is on index 0 though
	TH2D* h = (TH2D*) hCanv->GetPrimitive(t->At(CanvIdx)->GetName());
	//h->Draw();
	//x needs to always be eta
	//y needs to always b pt
	int nbinsX,nbinsY;
	nbinsX = h->GetNbinsX();
	nbinsY = h->GetNbinsY();
	for(int i=1; i<=nbinsX; i++){
		for(int j=1; j<=nbinsY; j++){
			//std::cout<<"X: "<<i<<" "<<h->GetXaxis()->GetBinLowEdge(i)<<"  Y: "<<j<<" "<<h->GetYaxis()->GetBinLowEdge(j) <<"  "<<h->GetBinContent(i,j)<<" "<<h->GetBinError(i,j)<<std::endl;
			map[ std::make_pair(h->GetYaxis()->GetBinLowEdge(j),h->GetXaxis()->GetBinLowEdge(i)) ] = std::make_pair(h->GetBinContent(i,j),h->GetBinError(i,j));
			
		}
	}
	//std::cout<<std::endl;
	//grab and store upper edges for plotting utility
	 _ptUpperEdge= h->GetYaxis()->GetBinUpEdge(nbinsY);
	 _etaUpperEdge= h->GetXaxis()->GetBinUpEdge(nbinsX);
}

void E_bank::setYear(int year){
	_year = year;
}
	
void E_bank::printMap(std::map<std::pair<double,double>, std::pair<double,double> >& map){
	std::cout<<"Printing Map ... "<<std::endl;
	for (auto& itr : map){
		std::cout<<"pt: "<<itr.first.first<<" eta: "<<itr.first.second<<" ";
		std::cout<<"eff: "<<itr.second.first<<" +/- "<<itr.second.second<<" ";
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
}
//std::pair centralvalue first, error second
/*
std::pair<double,double> E_bank::getPair(std::pair<double,double> key, int year=0){
	if(year>0)_year=year;
	if(_year == 2016) return _map16[key];	
	if(_year == 2017) return _map17[key];
	if(_year == 2018) return _map18[key];
	return std::make_pair(-1,-1);
}
double E_bank::getEfficiency(std::pair<double,double> key, int year=0){
	if(year>0)_year=year;
	if(_year == 2016) return _map16[key].first;
	if(_year == 2017) return _map17[key].first;
	if(_year == 2018) return _map18[key].first;
	return -1;
}
double E_bank::getError(std::pair<double,double> key, int year=0){
	if(year>0)_year=year;
	if(_year == 2016) return _map16[key].second;
	if(_year == 2017) return _map17[key].second;
	if(_year == 2018) return _map18[key].second;	
	return -1;
}
*/
std::pair<double,double> E_bank::getPair(double pt, double eta, int year){
	if(year>0)_year=year;
	if(_year == 2016) return _map16[transformValue(pt,eta, _map16)];
	if(_year == 2017) return _map17[transformValue(pt,eta, _map17)];
	if(_year == 2018) return _map18[transformValue(pt,eta, _map18)];
	return std::make_pair(-1,-1);
}
double E_bank::getEfficiency(double pt, double eta, int year){
	if(year>0)_year=year;
	if(_year == 2016) return _map16[transformValue(pt,eta, _map16)].first;
	if(_year == 2017) return _map17[transformValue(pt,eta, _map17)].first;
	if(_year == 2018) return _map18[transformValue(pt,eta, _map18)].first;
	return -1;
}
double E_bank::getError(double pt, double eta, int year){
	if(year>0)_year=year;
	if(_year == 2016) return _map16[transformValue(pt,eta, _map16)].second;
	if(_year == 2017) return _map17[transformValue(pt,eta, _map17)].second;
	if(_year == 2018) return _map18[transformValue(pt,eta, _map18)].second;
	return -1;
}
//transform pair
/*Todo maybe*/

//transform value
std::pair<double,double> E_bank::transformValue(double pt, double eta, std::map<std::pair<double,double>, std::pair<double,double> >& map ){
	//iterate object
	double mindpt=999.;
	double mindeta=999.;
	double ptkey=0.,etakey=0.;
	for (auto& itr : map){
		if( (itr.first.first <= pt) && ((pt - itr.first.first)<mindpt )  ){
			mindpt = pt - itr.first.first;
			ptkey = itr.first.first;
		}  
		if( (itr.first.second <= eta) && ((eta - itr.first.second)<mindeta) ){
			mindeta = eta - itr.first.second;
			etakey = itr.first.second;
		}
	}
	return std::make_pair(ptkey,etakey);
	
}
void E_bank::addError(double error, std::map<std::pair<double,double>, std::pair<double,double> >& map){
	for (auto& itr : map){
		itr.second.second = std::sqrt(itr.second.second*itr.second.second + error*error);
	}
}
void E_bank::addError(std::vector<double> errors, std::map<std::pair<double,double>, std::pair<double,double> >& map){
	//loop map and generate a set of eta keys
	std::set<double> etakeys;
	for (auto& itr : map){
		etakeys.insert(itr.first.second);
	}
	//iterate set, combine associated errors based on eta
	int i=0;
	for (auto& eta_itr : etakeys){
		for( auto& map_itr : map ){
			if( eta_itr == map_itr.first.second){//etamatch
				map_itr.second.second = std::sqrt( map_itr.second.second*map_itr.second.second + errors[i]*errors[i] );
			}
		}
		i++;
	}
	
}
void E_bank::addError(double ptLow, double ptHigh ,std::vector<double> errors, std::map<std::pair<double,double>, std::pair<double,double> >& map){
        //loop map and generate a set of eta keys
        std::set<double> etakeys;
        for (auto& itr : map){
                etakeys.insert(itr.first.second);
        }
        //iterate set, combine associated errors based on eta
        int i=0;
        for (auto& eta_itr : etakeys){
                for( auto& map_itr : map ){
                        if( eta_itr == map_itr.first.second){//etamatch
				if( map_itr.first.first < ptHigh && map_itr.first.first >= ptLow){
                                map_itr.second.second = std::sqrt( map_itr.second.second*map_itr.second.second + errors[i]*errors[i] );
                        }}
                }
                i++;
        }

}

void E_bank::applySystematics(double error, int year){
	_syst0=error;//for now assume all years same error
	if(year==0){
		addError(error,_map16);
		addError(error,_map17);
		addError(error,_map18);
	}
	if(year==2016) addError(error,_map16);
	if(year==2017) addError(error,_map17);
	if(year==2018) addError(error,_map18);
}
void E_bank::applySystematics(std::vector<double> errors, int year){
	_syst = errors;
	if(year==0){
		addError(errors,_map16);
		addError(errors,_map17);
		addError(errors,_map18);
	}
	if(year==2016) addError(errors,_map16);
	if(year==2017) addError(errors,_map17);
	if(year==2018) addError(errors,_map18);
}
void E_bank::applySystematic_ptRange(double ptLow, double ptHigh, std::vector<
double>errors, int year){
	if(year==0){
                addError(ptLow,ptHigh,errors,_map16);
                addError(ptLow,ptHigh,errors,_map17);
                addError(ptLow,ptHigh,errors,_map18);
        }
        if(year==2016) addError(ptLow,ptHigh,errors,_map16);
        if(year==2017) addError(ptLow,ptHigh,errors,_map17);
        if(year==2018) addError(ptLow,ptHigh,errors,_map18);

}

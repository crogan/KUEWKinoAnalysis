#include "../include/SVDiscrTool.hh"

//#include "lwtnn/parse_json.hh"
//#include "/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/lwtnn/2.4-gnimlf3/include/lwtnn/parse_json.hh"
	 
#include <iostream>
#include <fstream>
#include <memory>

void SVDiscrTool::CreateNN(const std::string& json_file_name){

    std::ifstream jsonfile(json_file_name);

    auto config = lwt::parse_json(jsonfile);
    neural_network_ = our_make_unique<const lwt::LightweightNeuralNetwork>(config.inputs, config.layers, config.outputs);

}


std::map<std::string, double> SVDiscrTool::PROB(const Particle& SV){

    lwt::ValueMap var_map; 
                     
    var_map["Evt_pt"] = SV.Pt();
    var_map["Evt_eta"] = SV.Eta();
    var_map["Evt_mass"] = SV.M();
    var_map["Evt_d3d"] = SV.D3d();
    var_map["Evt_d3dsig"] = SV.D3dSig();
    var_map["Evt_dxy"] = SV.Dxy();
    var_map["Evt_costhetaSvPv"] = SV.CosTheta();
    var_map["Evt_ndof"] = SV.Ndof();

    
    auto nnout = neural_network_->compute(var_map);

    //std::map<std::string, double> prob_map = nnout->at("flavor");
    std::map<std::string, double> prob_out;
    for(const auto& out: nnout) {
        prob_out[out.first] = out.second;
    }

    return prob_out;
}

    

     

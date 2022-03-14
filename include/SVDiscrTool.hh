#ifndef SVDiscrTool_h
#define SVDiscrTool_h

#include "lwtnn/LightweightNeuralNetwork.hh"
//#include "/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/lwtnn/2.4-gnimlf3/include/lwtnn/LightweightNeuralNetwork.hh"
#include "Particle.hh"
#include <map>
#include <memory>


// so that the code compiles with older compilers
template<typename T, typename... Args>
std::unique_ptr<T> our_make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class SVDiscrTool {

public:
    //SVDiscrTool();
    void CreateNN(const std::string& json_file_name);

    std::unique_ptr<const lwt::LightweightNeuralNetwork> const& neural_network() const {return neural_network_;}

    std::map<std::string, double> PROB(const Particle& SV);

private:
    std::unique_ptr<const lwt::LightweightNeuralNetwork> neural_network_;


};

#endif

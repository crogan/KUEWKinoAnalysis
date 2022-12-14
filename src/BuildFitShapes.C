#include <string>
#include <map>
#include <set>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>

#include "FitInputEditor.hh"

using namespace std;

int main(int argc, char* argv[]) {
  string InputFile  = "";
  string OutputFile = "FitInput_new.root";

  bool smoothFakes = false;
  bool smoothQCD   = false;

  bool shapeFakes = false;
  bool shapeQCD   = false;
  bool shapeWjets   = false;

  bool addFakeData = false;

  bool sJetSysSplit = false;
  bool procSysSplit = false;

  bool bprint  = false;
  bool verbose = false;
    
  for(int i = 0; i < argc; i++){
    if(strncmp(argv[i],"--help", 6) == 0){
      bprint = true;
    }
    if(strncmp(argv[i],"-h", 2) == 0){
      bprint = true;
    }
    if(strncmp(argv[i],"--verbose", 9) == 0){
      verbose = true;
    }
    if(strncmp(argv[i],"-v", 2) == 0){
      verbose = true;
    }
    if(strncmp(argv[i],"-o", 2) == 0){
      i++;
      OutputFile = string(argv[i]);
    }
    if(strncmp(argv[i],"--output", 8) == 0){
      i++;
      OutputFile = string(argv[i]);
    }
    if(strncmp(argv[i],"-i", 2) == 0){
      i++;
      InputFile = string(argv[i]);
    }
    if(strncmp(argv[i],"--input", 7) == 0){
      i++;
      InputFile = string(argv[i]);
    }
    if(strncmp(argv[i],"-default", 8) == 0){
      smoothFakes = true;
      smoothQCD   = true;
      shapeFakes  = true;
      //shapeQCD    = true;
      shapeWjets  = false;
      sJetSysSplit = true;
    }
    if(strncmp(argv[i],"-smoothAll", 10) == 0){
      smoothFakes = true;
      smoothQCD   = true;
    }
    if(strncmp(argv[i],"-smoothFakes", 12) == 0){
      smoothFakes = true;
    }
    if(strncmp(argv[i],"-smoothQCD", 10) == 0){
      smoothQCD = true;
    }
    if(strncmp(argv[i],"-shapeAll", 10) == 0){
      shapeFakes = true;
      shapeQCD   = true;
      //shapeWjets   = true;
    }
    if(strncmp(argv[i],"-shapeFakes", 11) == 0){
      shapeFakes = true;
    }
    if(strncmp(argv[i],"-shapeQCD", 9) == 0){
      shapeQCD = true;
    }
    if(strncmp(argv[i],"-shapeWjets", 11) == 0){
      shapeWjets = true;
    }
    if(strncmp(argv[i],"-fakedata", 9) == 0){
      addFakeData = true;
    }
    if(strncmp(argv[i],"-sJetSplit", 9) == 0){
      sJetSysSplit = true;
    }
    if(strncmp(argv[i],"-procSplit", 9) == 0){
      procSysSplit = true;
    }
  }
    
 // if(!smoothFakes &&
 //    !smoothQCD &&
 //    !shapeFakes &&
 //    !shapeQCD &&
 //    !addFakeData)
    if(InputFile.empty())
    bprint = true;
  
  if(bprint){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << "  options:" << endl;
    cout << "   --help(-h)          print options" << endl;
    cout << "   --verbose(-v)       increase verbosity" << endl;
    cout << "   --input(-i) [file]  input root file" << endl;
    cout << "   --output(-o) [fold] output root file" << endl;
    cout << "   -default            runs the default smoothing and shape configuration as agreed upon by everyone" << endl;
    cout << "                       currently the default is -smoothFakes -smoothQCD -shapeFakes" << endl;
    cout << "   -smoothAll          smooth fake lep and QCD contributions" << endl;
    cout << "   -smoothFakes        smooth fake lep contributions" << endl;
    cout << "   -smoothQCD          smooth QCD contributions" << endl;
    cout << "   -shapeAll           add fake lep, Wjets and QCD systematic shape variations" << endl;
    cout << "   -shapeFakes         add fake lep systematic shape variations" << endl;
    cout << "   -shapeWjets         add Wjets systematic shape variations" << endl;
    cout << "   -shapeQCD           add QCD systematic shape variations" << endl;
    cout << "   -fakedata           add data_obs as sum of MC backgrounds" << endl;
    cout << "   -sJetSplit          split fake lepton shape systematics by S-jet multiplicities" << endl;
    cout << "   -procSplit          split fake lepton shape systematics by process groups" << endl;
 
    return 0;
  }

  ////////////////////////////////////////////////////////////////////                                  
  ////////////////////////////////////////////////////////////////////                                  
  ////////////////////////////////////////////////////////////////////

  FitInputEditor FIT(InputFile, procSysSplit);
  if(verbose){
    cout << "Input file " << InputFile << " contains:" << endl;
    FIT.PrintCategories();
    FIT.PrintProcesses();
  }

  if(smoothFakes)
    FIT.SmoothFakes();

  if(smoothQCD)
    FIT.SmoothQCD();

  if(shapeFakes)
    FIT.AddShapeSysFakes(sJetSysSplit);

  if(shapeQCD)
    FIT.AddShapeSysQCD();
  
  if(shapeWjets)
    FIT.AddShapeSysWjets();

  if(addFakeData)
    FIT.AddFakeData();
  else
    FIT.AddEmptyData();
  
  FIT.WriteFit(OutputFile);
}
	       
	     

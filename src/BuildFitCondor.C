#include <string>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cstdlib>

#include "TSystem.h"

#include "FitReader.hh"

using namespace std;

void WriteScript(const string& src_name,
		 const string& log_name,
		 const string& command,
		 const string& CERNqueue = "");

void WriteScriptConnect(const string& src_name,
		 const string& log_name,
		 const string& command,
		 const string& CERNqueue = "");

int main(int argc, char* argv[]) {
  int  maxN = 10;
  bool dryRun = false;
  bool connect = false;
  
  string InputFile = "test/FitInput_test.root";
  string OutputFold = "BuildFit_output";

  string CERNqueue = "";

  bool addSig = false; // all signals
  bool addBkg = false; // all backgrounds
  VS   proc_to_add;
  VS   proc_to_rem;

  bool addChan = false; // all channels
  VS   chan_to_add;
  VS   chan_to_rem;

  bool addCat = false; // all categories
  VS   cat_to_add;
  VS   cat_to_rem;

  bool addSys = false; // all available shape systematics
  VS   sys_to_add;
  VS   sys_to_rem;

  bool bprint  = false;
  bool verbose = false;
  int  year    = 2017;
  double xsec_norm = -999.0;

  bool doMCstats = false;

  bool doSepChan = false;
    
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
      OutputFold = string(argv[i]);
    }
    if(strncmp(argv[i],"--output", 8) == 0){
      i++;
      OutputFold = string(argv[i]);
    }
    if(strncmp(argv[i],"-i", 2) == 0){
      i++;
      InputFile = string(argv[i]);
    }
    if(strncmp(argv[i],"--input", 7) == 0){
      i++;
      InputFile = string(argv[i]);
    }
    if(strncmp(argv[i],"-CERN", 7) == 0){
      i++;
      CERNqueue = string(argv[i]);
    }
    if(strncmp(argv[i],"-year", 5) == 0){
      i++;
      year = std::stoi(argv[i]);
    }
    if(strncmp(argv[i],"++bkg", 5) == 0){
      addBkg = true;
    }
    if(strncmp(argv[i],"++sig", 5) == 0){
      addSig = true;
    }
    if(strncmp(argv[i],"+proc", 5) == 0){
      i++;
      proc_to_add += string(argv[i]);
    }
    if(strncmp(argv[i],"-proc", 5) == 0){
      i++;
      proc_to_rem += string(argv[i]);
    }
    if(strncmp(argv[i],"++chan", 6) == 0){
      addChan = true;
    }
    if(strncmp(argv[i],"+chan", 5) == 0){
      i++;
      chan_to_add += string(argv[i]);
    }
    if(strncmp(argv[i],"-chan", 5) == 0){
      i++;
      chan_to_rem += string(argv[i]);
    }
    if(strncmp(argv[i],"++cat", 5) == 0){
      addCat = true;
    }
    if(strncmp(argv[i],"+cat", 4) == 0){
      i++;
      cat_to_add += string(argv[i]);
    }
    if(strncmp(argv[i],"-cat", 4) == 0){
      i++;
      cat_to_rem += string(argv[i]);
    }
    if(strncmp(argv[i],"++sys", 5) == 0){
      addSys = true;
    }
    if(strncmp(argv[i],"+sys", 4) == 0){
      i++;
      sys_to_add += string(argv[i]);
    }
    if(strncmp(argv[i],"-sys", 4) == 0){
      i++;
      sys_to_rem += string(argv[i]);
    }
    if(strncmp(argv[i],"+MCstats", 8) == 0){
      doMCstats = true;
    }
    if(strncmp(argv[i],"-sepchan", 8) == 0){
      doSepChan = true;
    }
    if(strncmp(argv[i],"-maxN", 5) == 0){
      i++;
      maxN = std::max(1, std::stoi(argv[i]));
    }
    if(strncmp(argv[i],"--dry-run", 9) == 0){
      dryRun = true;
    } 
    if(strncmp(argv[i],"--connect", 9) == 0){
      connect = true;
    } 
    if(strncmp(argv[i],"--setXsec", 9) == 0){
      i++;
      xsec_norm = std::stod(argv[i]);
    }
  }
    
  if(!addBkg && !addSig && (proc_to_add.size() == 0))
    bprint = true;

  if(!addChan && (chan_to_add.size() == 0))
    bprint = true;

  if(!addCat && (cat_to_add.size() == 0))
    bprint = true;
  
  if(bprint){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << "  Condor submission options:" << endl;
    cout << "   -maxN [number]      maximum number of processes per job" << endl;
    cout << "   --dry-run           create output folders and scripts but don't submit" << endl;
    cout << "   -CERN [queue]       specify queue name for running at CERN" << endl;
    cout << "   --connect           for running inside a batch job on CMS connect" << endl;
    cout << "  BuildFit.x options:" << endl;
    cout << "   --help(-h)          print options" << endl;
    cout << "   --verbose(-v)       increase verbosity" << endl;
    cout << "   --input(-i) [file]  input root file" << endl;
    cout << "   --output(-o) [fold] folder path for output" << endl;
    cout << "   -year [year]        year of inputs" << endl;
    cout << "   ++bkg               add all bkgs" << endl;
    cout << "   ++sig               add all signals" << endl;
    cout << "   +proc [label]       add processes matching label" << endl;
    cout << "   -proc [label]       remove processes matching label" << endl;
    cout << "   ++chan              add all channels" << endl;
    cout << "   +chan [label]       add channels matching label" << endl;
    cout << "   -chan [label]       removes channels matching label" << endl;
    cout << "   ++cat               add all categories" << endl;
    cout << "   +cat [label]        add categories matching label" << endl;
    cout << "   -cat [label]        removes categories matching label" << endl;
    cout << "   ++sys               add all shape systematics" << endl;
    cout << "   +sys [label]        add systematics matching label" << endl;
    cout << "   -sys [label]        removes systematics matching label" << endl;
    cout << "   +MCstats            adds autoMCStats uncertainties" << endl;
    cout << "   -sepchan            make datacards for each group of channels separately" << endl;
    cout << "   --workspace(-w)     also build workspaces (note: faster not to, and run message)" << endl;
    cout << "Example: ./BuildFitCondor.x ++bkg +proc T2tt ++cat ++chan --connect -o name_of_BuildFit_output_folder/ -i name_of_BFI_root_file.root " << endl;

    return 0;
  }

  ////////////////////////////////////////////////////////////////////                                  
  ////////////////////////////////////////////////////////////////////                                  
  ////////////////////////////////////////////////////////////////////
  
  string Era = std::to_string(year);
  string Ana = "KUEWKino";

  FitReader FIT(InputFile);
  if(verbose){
    cout << "Input file " << InputFile << " contains:" << endl;
    FIT.PrintCategories();
    FIT.PrintProcesses();
  }

  ProcessList processes = FIT.GetProcesses().FilterOR(proc_to_add); 
  if(addBkg)
    processes += FIT.GetProcesses().Filter(kBkg);
  if(addSig)
    processes += FIT.GetProcesses().Filter(kSig);
  processes = processes.RemoveOR(proc_to_rem);

  // keep only the process-by-process fakes
  ProcessList proc_fakes = processes.Filter("_Fakes_");
  processes.Remove("Fakes");
  processes += proc_fakes;
  

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  
  // prepare processes
  ProcessList signals = processes.Filter(kSig);

  ProcessList backgrounds = processes.Filter(kBkg); 

  cout << "* Writing ouput to " << OutputFold << endl;
  gSystem->Exec(("rm -fr "+OutputFold).c_str());
  gSystem->Exec(("mkdir -p "+OutputFold).c_str());
  
  string OutputFile = OutputFold+"/FitInput_"+Ana+"_"+Era+".root";
  string copy_cmd = "cp "+InputFile+" "+OutputFile;
  if(InputFile.find("xrootd") != std::string::npos)
    copy_cmd = "xrdcp "+InputFile+" "+OutputFile;
  cout << "COPY cmd:" << endl;
  cout << "   " << copy_cmd << endl;
  gSystem->Exec(copy_cmd.c_str());
  
  string BuildFitCmd = "--batch ";
  if(verbose)
    BuildFitCmd += "--verbose ";
  BuildFitCmd += "--output " + OutputFold + " ";
  BuildFitCmd += "--input " + InputFile + " ";
  BuildFitCmd += Form("-year %d ", year);
  if(addChan)
    BuildFitCmd += "++chan ";
  for(auto c : chan_to_add)
    BuildFitCmd += "+chan " + c + " ";
  for(auto c : chan_to_rem)
    BuildFitCmd += "-chan " + c + " ";
  if(addCat)
    BuildFitCmd += "++cat ";
  for(auto c : cat_to_add)
    BuildFitCmd += "+cat " + c + " ";
  for(auto c : cat_to_rem)
    BuildFitCmd += "-cat " + c + " ";
  if(addSys)
    BuildFitCmd += "++sys ";
  for(auto s : sys_to_add)
    BuildFitCmd += "+sys " + s + " ";
  for(auto s : sys_to_rem)
    BuildFitCmd += "-sys " + s + " ";
  if(doMCstats)
    BuildFitCmd += "+MCstats ";
  if(doSepChan)
    BuildFitCmd += "-sepchan ";
  if(connect)
    BuildFitCmd += "--connect ";
  BuildFitCmd += Form("--setXsec %f ", xsec_norm);
 
  string SrcFold = OutputFold+"/src/";
  string LogFold = OutputFold+"/log/";
  gSystem->Exec(("mkdir -p "+SrcFold).c_str());
  gSystem->Exec(("mkdir -p "+LogFold).c_str());
  
  VS bkg = backgrounds.GetProcesses();
  for(auto b : bkg)
    BuildFitCmd += "+proc " + b + " ";
  
  ofstream condorsubmit;
  condorsubmit.open((OutputFold+"/condor_submit.sh").c_str());
  
  int Nproc = signals.GetN();
  vector<string> procs;
  string iBFCmd;
  int Njob = 0;
  for(int p = 0; p < Nproc; p++){
    procs.push_back(signals[p].Name());
    iBFCmd += "+proc "+signals[p].Name()+" ";
    cout << "Adding proc " << signals[p].Name() << endl;
    if(procs.size() >= maxN || p == Nproc-1){
      if(connect)
      {
        WriteScriptConnect(SrcFold+Form("submit_%d",Njob)+".sh",
		  LogFold+Form("job_%d",Njob)+".log",
		  BuildFitCmd+iBFCmd,
		  CERNqueue);
      }
      else
      {
        WriteScript(SrcFold+Form("submit_%d",Njob)+".sh",
		  LogFold+Form("job_%d",Njob)+".log",
		  BuildFitCmd+iBFCmd,
		  CERNqueue);
      }
      condorsubmit << "condor_submit " << SrcFold << "submit_" << Njob << +".sh" << endl;

      procs.clear();
      iBFCmd = "";
      Njob++;
    }
  }
  condorsubmit.close();

  if(dryRun){
    cout << "Submit condor jobs by typing:" << endl;
    cout << "   source " << OutputFold << "/condor_submit.sh" << endl;
  } else {
    cout << "Submitting condor jobs" << endl;
    gSystem->Exec(("source "+OutputFold+"/condor_submit.sh").c_str());
  }
}
	       
	     
void WriteScriptConnect(const string& src_name,
		 const string& log_name,
		 const string& command,
		 const string& CERNqueue){

  string tar = command.substr(command.find("output ")+7,command.find("input")-10-command.find("output "))+"/config_BuildFit.tgz";
  if(gSystem->AccessPathName(tar.c_str()))
  {
   gSystem->Exec("mkdir -p config_BuildFit");
   gSystem->Exec("cp BuildFit.x config_BuildFit/");
   gSystem->Exec("cp scripts/cmssw_setup_connect.sh config_BuildFit/");
   gSystem->Exec("cp scripts/setup_RestFrames_connect.sh config_BuildFit/");
   string input_file = command.substr(command.find("input")+5,command.find(".root")-command.find("input"));
   gSystem->Exec(("cp --parents "+input_file+" config_BuildFit/").c_str());
   gSystem->Exec("tar -czf config_BuildFit.tgz config_BuildFit/");
   gSystem->Exec(("mv config_BuildFit.tgz "+command.substr(command.find("output ")+7,command.find("input")-10-command.find("output "))).c_str());
   gSystem->Exec("rm -r config_BuildFit/");
  }

  ofstream file;
  file.open(src_name); 
  string pwd = gSystem->pwd();
  file << "universe = vanilla" << endl;
  file << "executable = execute_script_BuildFit.sh" << endl;
  file << "getenv = True" << endl;
  file << "use_x509userproxy = true" << endl;
  file << "Arguments = " << command << endl;
  file << "output = " << log_name << ".out" << endl;
  file << "error = "  << log_name << ".err" << endl;
  file << "log = "    << log_name << ".log" << endl;
  file << "Requirements = (Machine != \"red-node000.unl.edu\") && (Machine != \"red-c2325.unl.edu\")" << endl;
  file << "request_memory = 4 GB" << endl;
  file << "transfer_input_files = /uscms/home/z374f439/nobackup/whatever_you_want/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2,"+command.substr(command.find("output ")+7,command.find("input")-10-command.find("output "))+"/config_BuildFit.tgz" << endl;
  file << "should_transfer_files = YES" << endl;
  file << "when_to_transfer_output = ON_EXIT" << endl;
  file << "transfer_output_files = datacards" << endl;
  file << "transfer_output_remaps = \"datacards = "+command.substr(command.find("output ")+7,command.find("input")-10-command.find("output "))+"/datacards"+"\"" << endl;
  file << "+ProjectName=\"cms.org.ku\""<< endl;
  file << "+REQUIRED_OS=\"rhel7\"" << endl;
  file << "+RequiresCVMFS=True" << endl;
  if(CERNqueue != "")
    file << "+JobFlavour=\"" << CERNqueue << "\"" << endl;
  file << "queue " << endl;
  file.close();  
}

void WriteScript(const string& src_name,
		 const string& log_name,
		 const string& command,
		 const string& CERNqueue){
  ofstream file;
  file.open(src_name);
  
  file << "universe = vanilla" << endl;
  file << "executable = BuildFit.x" << endl;
  file << "getenv = True" << endl;
  file << "use_x509userproxy = true" << endl;
  file << "Arguments = " << command << endl;
  file << "output = " << log_name << ".out" << endl;
  file << "error = "  << log_name << ".err" << endl;
  file << "log = "    << log_name << ".log" << endl;
  file << "Requirements = (Machine != \"red-node000.unl.edu\") && (Machine != \"red-c2325.unl.edu\")" << endl;
  file << "request_memory = 4 GB" << endl;
  if(CERNqueue != "")
    file << "+JobFlavour=\"" << CERNqueue << "\"" << endl;
  file <<"queue\n";						     
  file.close();  
}

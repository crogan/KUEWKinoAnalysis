#/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/BFI_B135_bugfix16_tthighdM16_NOM/condor_submit.sh
#/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/BFI_B135_bugfix16_tthighdM16_NOM/config_BuildFitInput.tgz
#/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/BFI_B135_bugfix16_tthighdM16_NOM/log
#/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/BFI_B135_bugfix16_tthighdM16_NOM/root
#/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/BFI_B135_bugfix16_tthighdM16_NOM/src
import glob
#SIG="tthighdM16"

#BFI_dir="BFI_B135_bugfix16_tthighdM16_NOM"
#SIG="tthighdM17"
#BFI_dir="BFI_B135_bugfix16_tthighdM17_NOM"

import sys
SIG=sys.argv[1]
BFI_dir=sys.argv[2]

#write common sub file

#scan submitfiles
subs = glob.glob(BFI_dir+"/src/*")
#print(subs)

subArgSet=[]
for sub in subs:
	subf = open(sub)
	lines = subf.readlines()
	for line in lines:
		if "Arguments =" in line:
			subArgSet.append(line)
	subf.close()
#	break
#print(subArgSet)
#exit()

print("writing in BFI_dir", BFI_dir)
subfile = open(BFI_dir+"/condorBFI_reconf_"+SIG+".sub","w")
subfile.write("universe = vanilla\n")
subfile.write("executable = condorBFI_reconf_"+SIG+".sh\n")
subfile.write("getenv = True\n")
subfile.write("use_x509userproxy = true\n")
subfile.write("arguments = $(ProcId)\n")
#Arguments = -path root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_v2/ +SMS +cat0L +cat1L +cat2L +cat3L -year 2016 -ifile 9 +proc tthighdM_10000700  -o BFI_10.root
subfile.write("output = ./log/BFI.$(ClusterId).$(ProcId).out\n")
subfile.write("error = ./log/BFI.$(ClusterId).$(ProcId).err\n")
subfile.write("log = ./log/BFI.$(ClusterId).log\n")
#Requirements = (Machine != "red-node000.unl.edu") && (Machine != "red-c2325.unl.edu")
subfile.write("request_memory = 2 GB\n")
subfile.write("request_cpus = 2\n")
subfile.write("request_disk = 300000\n")
#for LPC
subfile.write("transfer_input_files = /uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/csv/METTrigger/Parameters.csv,/uscms/home/z374f439/nobackup/whatever_you_want/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2,/uscms/home/janguian/nobackup/CMSSW_10_6_5/src/KUEWKinoAnalysis_NewNtuples/"+BFI_dir+"/config_BuildFitInput.tgz\n")

#subfile.write("transfer_input_files = /home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/csv/METTrigger/Parameters.csv,/ospool/cms-user/zflowers/public/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2,/ospool/cms-user/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/"+BFI_dir+"/config_BuildFitInput.tgz\n")
subfile.write("should_transfer_files = YES\n")
subfile.write("when_to_transfer_output = ON_EXIT\n")
subfile.write("transfer_output_files =  \n")
#transfer_output_remaps = " BFI_10.root =/ospool/cms-user/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/BFI_B135_bugfix16_tthighdM16_NOM/root//BFI_10.root "
remapstr = "BFI_${ProcID}.root=/ospool/cms-user/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/"+BFI_dir+"/root/BFI_${ProcId}.root"
#subfile.write("transfer_output_remaps = "+remapstr+"\n")
#subfile.write("+ProjectName=\"cms.org.cern\"\n")
#subfile.write("+REQUIRED_OS=\"rhel7\"\n")
#subfile.write("+RequiresCVMFS=True\n")
subfile.write("Requirements = HAS_SINGULARITY == True\n")
subfile.write("+ApptainerImage = \"/cvmfs/singularity.opensciencegrid.org/cmssw/cms:rhel7\"\n")
subfile.write("+ProjectName=\"cms.org.cern\"\n")
subfile.write("+DesiredOS=\"rhel7\"\n")
subfile.write("+RequiresCVMFS=True\n")

subfile.write("queue "+str(len(subArgSet)))
subfile.close()


aggfile = open(BFI_dir+"/condorBFI_reconf_"+SIG+".sh","w")
aggfile.write("#!/bin/bash\n")
aggfile.write("tar -xzf ./config_BuildFitInput.tgz\n")

aggfile.write("export SCRAM_ARCH=slc7_amd64_gcc700\n")
aggfile.write("source /cvmfs/cms.cern.ch/cmsset_default.sh\n")
aggfile.write("source ./config_BuildFitInput/cmssw_setup_connect.sh\n")
aggfile.write("cmssw_setup sandbox-CMSSW_10_6_5-6403d6f.tar.bz2\n")
aggfile.write("source ./config_BuildFitInput/setup_RestFrames_connect.sh\n")
aggfile.write("mv ./config_BuildFitInput/BtagSF .\n")
aggfile.write("export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/lhapdf/6.2.1-pafccj3/lib/\n")

#
aggfile.write("\n")
for i,bfiArg in enumerate(subArgSet):
	#adjust arg to use procId
    bfiArgtemp = bfiArg.rstrip().split(' ')[2:-1]
    bfiArgtemp = ' '.join(bfiArgtemp)
    bfiArgtemp= bfiArgtemp +" BFI_${1}.root" 

    aggfile.write("if [ $1 -eq "+str(i)+" ]; then\n") 
    aggfile.write("./config_BuildFitInput/BuildFitInput.x "+bfiArgtemp +"\n"  )
    aggfile.write("fi\n")

#aggfile.write("mv *AsymptoticLimits*.root ../../../\n")
aggfile.close()

print("BFI reconfigured submission ready!")
print("pushd "+BFI_dir+"; condor_submit condorBFI_reconf_"+SIG+".sub; popd")

'''
#submit.sh
universe = vanilla
executable = execute_script_BuildFitInput.sh
getenv = True
use_x509userproxy = true
Arguments = -path root://cmseos.fnal.gov//store/user/lpcsusylep/NTUPLES_v2/ +SMS +cat0L +cat1L +cat2L +cat3L -year 2016 -ifile 9 +proc tthighdM_10000700  -o BFI_10.root
output = BFI_B135_bugfix16_tthighdM16_NOM/log/job_10.log.out
error = BFI_B135_bugfix16_tthighdM16_NOM/log/job_10.log.err
log = BFI_B135_bugfix16_tthighdM16_NOM/log/job_10.log.log
Requirements = (Machine != "red-node000.unl.edu") && (Machine != "red-c2325.unl.edu")
request_memory = 2 GB
transfer_input_files = /home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/csv/METTrigger/Parameters.csv,/ospool/cms-user/zflowers/public/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2,/ospool/cms-user/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/BFI_B135_bugfix16_tthighdM16_NOM/root//../config_BuildFitInput.tgz
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
transfer_output_files =  BFI_10.root
transfer_output_remaps = " BFI_10.root =/ospool/cms-user/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/BFI_B135_bugfix16_tthighdM16_NOM/root//BFI_10.root "
+ProjectName="cms.org.cern"
+REQUIRED_OS="rhel7"
+RequiresCVMFS=True
queue
'''

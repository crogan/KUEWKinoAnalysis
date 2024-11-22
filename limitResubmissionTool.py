#/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/BF_B135_bugfix16_TChipmWW_MCstats/datacards/all/TChipmWW
#higgsCombine.Test.AsymptoticLimits.mH5760530.root
#BF_B135_bugfix16_TChipmWW_MCstats

import glob
import os
import shutil

BF_header = "BF_B135_bugfix16_"
BF_footer = "_MCstats"
#BF_header = "BF_B136_"
#BF_footer = ""

#SIG="TChipmWW"
#SIG="TChiWZ"
#cpu="request_cpus = 4"
#disk="request_disk = 2400000 KB"
#mem="request_memory = 4000 MB"

#SIG="T2tt"
#SIG="HinoN2C1"
#SIG="TSlepSleptot"
#SIG="TSlepSlepmueL"
#SIG="TSlepSlepmueR"
#SIG="TSlepSlepmuLR"
#SIG="TSlepSlepeLR"
#SIG="TSlepSlepMUL"
#SIG="TSlepSlepMUR"
#SIG="TSlepSlepEL"
#SIG="TSlepSlepER"

#SIG="TChipmWW"
#SIG="tthighdM"
#SIG="WZhighdM"
#SIG="bWhighdM"
#SIG="WWhighdM"
SIG="T2bW"
#SIG="T2bb"
#SIG="T2cc"
#SIG="TSlepSnu"

disk="request_disk = 20000000 KB"
mem="request_memory = 4000 MB"
cpu="request_cpus = 4"







BF_dir = BF_header+ SIG + BF_footer

dc_path = BF_dir+"/datacards/all/"+SIG


dcs = glob.glob(dc_path+"/*")
#print(dcs)

for i,dc in enumerate(dcs):
	jN = dc.split("/")[-1]
	dcs[i] = jN

#print(dcs)

lims = glob.glob(BF_dir+"/higgsCombine.Test.AsymptoticLimits.*.root")
#print(lims)
for i,lim in enumerate(lims):
	jN = lim.split("/")[-1]
	jN = jN.split(".")[-2]
	jN = jN[2:]
	lims[i] = jN

#print(lims)

dcsSet = set(dcs)
limSet = set(lims)

subDiff = dcsSet.difference(limSet)
print("Analyzing:", BF_dir)
print("found", len(subDiff), "missing jobs")
print("generating resubmission scripts for this job list:")
print(subDiff)

#exit()
print("Using these configurations:")
print(cpu)
print(disk)
print(mem)
print("Generating resubmission scripts...")


subfile = open(BF_dir+"/condor_resub_lim.sub","w")
subfile.write("executable = condor_resub_lim.sh\n")
subfile.write("universe = vanilla\n")
subfile.write("use_x509userproxy = true\n")
subfile.write("getenv = True\n")
subfile.write("arguments = $(ProcId)\n")
subfile.write("output                = combine_task.$(ClusterId).$(ProcId).out\n")
subfile.write("error                 = combine_task.$(ClusterId).$(ProcId).err\n")
subfile.write("log                   = combine_task.$(ClusterId).log\n")
subfile.write("Requirements = HAS_SINGULARITY == True\n")
subfile.write("+ApptainerImage = \"/cvmfs/singularity.opensciencegrid.org/cmssw/cms:rhel7\"\n")
subfile.write("+ProjectName=\"cms.org.cern\"\n")
subfile.write("+DesiredOS=\"rhel7\"\n")
subfile.write("+RequiresCVMFS=True\n")

# Periodically retry the jobs every 10 minutes, up to a maximum of 5 retries.
#periodic_release =  (NumJobStarts < 3) && ((CurrentTime - EnteredCurrentStatus) > 6s

subfile.write("preserve_relative_paths = True\n")
subfile.write("transfer_input_files = /ospool/cms-user/zflowers/public/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2,/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/scripts/cmssw_setup_connect.sh,datacards.tar.gz,FitInput_KUEWKino_2017.root\n")

subfile.write("transfer_output_files =\n")

subfile.write(disk+"\n")
subfile.write(mem+"\n")
subfile.write(cpu+"\n")
subfile.write("queue "+ str(len(subDiff)))

subfile.close()


shfile = open(BF_dir+"/condor_resub_lim.sh","w")
shfile.write("#!/bin/bash\n")
shfile.write("ulimit -s unlimited\n")
shfile.write("set -e\n")
shfile.write("export SCRAM_ARCH=slc7_amd64_gcc700\n")
shfile.write("source /cvmfs/cms.cern.ch/cmsset_default.sh\n")
#wget --quiet --no-check-certificate http://stash.osgconnect.net/+zflowers/cmssw_setup_connect.sh 
shfile.write("source cmssw_setup_connect.sh\n")
#wget --quiet --no-check-certificate http://stash.osgconnect.net/+zflowers/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2
#cmssw_setup sandbox-CMSSW_10_6_5-6403d6f.tar.bz2
#mkdir -p cmssw-tmp/CMSSW_10_6_5/src//
#cd cmssw-tmp/CMSSW_10_6_5/src//
shfile.write("cmssw_setup sandbox-CMSSW_10_6_5-6403d6f.tar.bz2\n")
shfile.write("mkdir -p cmssw-tmp/CMSSW_10_6_5/src//\n")
shfile.write("cp --parents datacards.tar.gz cmssw-tmp/CMSSW_10_6_5/src//\n")
shfile.write("cd cmssw-tmp/CMSSW_10_6_5/src//\n")
shfile.write("eval `scramv1 runtime -sh`\n")

shfile.write("tar -xf datacards.tar.gz\n")
shfile.write("cp ../../../FitInput_KUEWKino_2017.root .\n")
shfile.write("\n")
for i,gpt in enumerate(subDiff):

	shfile.write("if [ $1 -eq "+str(i)+" ]; then\n")
	shfile.write("  combine --cminDefaultMinimizerStrategy 0 -M AsymptoticLimits -d datacards/all/"+SIG+"/"+str(gpt)+"/datacard.txt -m "+str(gpt)+" -n .Test\n")
	shfile.write("fi\n")

shfile.write("mv *AsymptoticLimits*.root ../../../\n")
shfile.close()
os.chmod(BF_dir+"/condor_resub_lim.sh", 0o0777)
print("Regenerating datacard tarball...")
os.system("tar -czf "+BF_dir+"/datacards.tar.gz -C "+BF_dir+" datacards" )
print("new tarball ready")
print("Scripts ready! To run do this:")
print("pushd "+BF_dir+"; condor_submit condor_resub_lim.sub; popd")


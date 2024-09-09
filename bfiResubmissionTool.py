import glob
import os
import shutil
import sys
#print(glob.glob("/home/adam/*"))



#test on this BF dir
#BF_dir = "BF_B135_TChipmWW_MCstats"
###job configuration
#cpu="request_cpus = 4"
#disk="request_disk = 15000000 KB"
#mem="request_memory = 4000 MB"


#BF_dir = "BF_B135_bugfix16_TChiWZ_MCstats"
#BF_dir = "BF_B135_bugfix16_HinoN2C1_MCstats"
#BF_dir = "BFI_B135_bugfix16_tthighdM_SYS/BFI_B135_bugfix16_tthighdM16_JERUncer_TotalDown"
#sig = "tthighdM16"

BF_dir = sys.argv[1]
sig = sys.argv[2]

#cpu="request_cpus = 4"
#disk="request_disk = 2000000 KB"
#mem="request_memory = 4000 MB"

#BF_dir = "BF_B135_bugfix16_T2tt_MCstats"
#BF_dir = "BF_B135_bugfix16_TChipmWW_MCstats"
cpu="request_cpus = 4"
disk="request_disk = 3500000 KB"
mem="request_memory = 6000 MB"


#BF_dir = "BF_B135_bugfix16_TSlepSlepEL_MCstats"  
#BF_dir = "BF_B135_bugfix16_TSlepSlepER_MCstats" 
#BF_dir = "BF_B135_bugfix16_TSlepSlepMUL_MCstats" 
#BF_dir = "BF_B135_bugfix16_TSlepSlepMUR_MCstats" 
#BF_dir = "BF_B135_bugfix16_TSlepSlepeLR_MCstats"
#BF_dir = "BF_B135_bugfix16_TSlepSlepmuLR_MCstats" 
#BF_dir = "BF_B135_bugfix16_TSlepSlepmueL_MCstats"
#BF_dir = "BF_B135_bugfix16_TSlepSlepmueR_MCstats" 
#BF_dir = "BF_B135_bugfix16_TSlepSleptot_MCstats"

#cpu="request_cpus = 4"
#disk="request_disk = 1500000 KB"
#mem="request_memory = 4000 MB"




subs = glob.glob(BF_dir+"/src/*")
#print(subs)
for i,sub in enumerate(subs):
    jN = sub.split("/")[-1]
    jN = jN[:-3]
    jN = jN.split("_")[-1]
    subs[i] = jN

#print(subs)

#logs = glob.glob(BF_dir+"/log/*.out")
#logs = glob.glob(BF_dir+"/*.root")
logs = glob.glob(BF_dir+"/root/*.root")
for i,log in enumerate(logs):
    jN = log.split("/")[-1]
   # jN = jN[:-8]
    jN = jN.split(".")[-2]
    jN = jN.split("_")[-1]
    logs[i] = jN

#print(logs)

subSet = set(subs)
logSet = set(logs)

subDiff = subSet.difference(logSet)
print("#Analyzing:", BF_dir)
print("#found", len(subDiff), "missing jobs")


print("#generating resubmission scripts for this job list:")
#print(subDiff)
if( len(subDiff) == 0):
    exit()


xferInputs="/home/jsingera/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/csv/METTrigger/Parameters.csv,/ospool/cms-user/zflowers/public/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2,/ospool/cms-user/jsingera/CMSSW_10_6_5/src/KUEWKinoAnalysis_treeSysDev/"
xferInputs=xferInputs+BF_dir+"/config_BuildFitInput.tgz"
#generate sub file

resubfile = "condorBFI_resub.sub"
shfile = "condorBFI_resub.sh"

subOut = open(BF_dir+"/"+resubfile,"w")

subOut.write("universe = vanilla\n")
subOut.write("executable ="+shfile+"\n") 
subOut.write("getenv = True\n")
subOut.write("use_x509userproxy = true\n")
subOut.write("arguments = $(ProcId)\n")
subOut.write("output = ./log/BFI.resub.$(ClusterId).$(ProcId).out\n")
subOut.write("error = ./log/BFI.resub.$(ClusterId).$(ProcId).err\n")
subOut.write("log = ./log/BFI.resub.$(ClusterId).log\n")
subOut.write("request_memory = 2 GB\n")
subOut.write("request_cpus = 2\n")
subOut.write("request_disk = 300000\n")
subOut.write("transfer_input_files = "+xferInputs+"\n") 
subOut.write("should_transfer_files = YES\n")
subOut.write("when_to_transfer_output = ON_EXIT\n")
subOut.write("transfer_output_files = \n")
subOut.write("Requirements = HAS_SINGULARITY == True\n")
subOut.write("+ApptainerImage = \"/cvmfs/singularity.opensciencegrid.org/cmssw/cms:rhel7\"\n")
subOut.write("+ProjectName=\"cms.org.cern\"\n")
subOut.write("+DesiredOS=\"rhel7\"\n")
subOut.write("+RequiresCVMFS=True\n")
subOut.write("queue "+str(len(subDiff))+"\n")

subOut.close();

cmds={}
shIn = BF_dir+"/condorBFI_reconf_"+sig+".sh"
shread = open( shIn, 'r')
lines = shread.readlines()
nextNum = -1
getNextLine = False
for line in lines:
    if(getNextLine):
        cmds[nextNum]=line
        getNextLine=False
        continue
    if("if [ $1 -eq" in line):
        num = line.split(" ")
        num = int( num[-3] )
        for j in subDiff:
            if (int(j) == num):
                getNextLine=True
                nextNum = j
                continue
shread.close()

shOut = open(BF_dir+"/"+shfile,"w") 
shOut.write("#!/bin/bash\n")
shOut.write("tar -xzf ./config_BuildFitInput.tgz\n")
shOut.write("export SCRAM_ARCH=slc7_amd64_gcc700\n")
shOut.write("source /cvmfs/cms.cern.ch/cmsset_default.sh\n")
shOut.write("source ./config_BuildFitInput/cmssw_setup_connect.sh\n")
shOut.write("cmssw_setup sandbox-CMSSW_10_6_5-6403d6f.tar.bz2\n")
shOut.write("source ./config_BuildFitInput/setup_RestFrames_connect.sh\n")
shOut.write("mv ./config_BuildFitInput/BtagSF .\n")
shOut.write("export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/lhapdf/6.2.1-pafccj3/lib/\n")
shOut.write("\n")

ctr=0
for key in cmds:
    cmd = cmds[key]
    shOut.write("if [ $1 -eq "+str(ctr)+" ]; then\n")
    rename_cmd = cmd.split(" ")[:-1]
    rename_cmd.append("BFI_"+str(key)+".root\n")
    rename_cmd = " ".join(rename_cmd)
    shOut.write(rename_cmd)
    shOut.write("fi\n")
    ctr= ctr+1

shOut.close()

print("pushd "+BF_dir+"; condor_submit "+resubfile+"; popd")

print()
'''

resub_directory = BF_dir+"/resub_src" 

if os.path.exists(resub_directory):
    shutil.rmtree(resub_directory)
os.mkdir(resub_directory) 
print("Directory '% s' created" % resub_directory) 

print("Using these configurations:")
print(cpu)
print(disk)
print(mem)
print("Generating resubmission scripts...")
for key in subDiff:
#	print("procesing #",key)
	subfile = BF_dir+"/src/submit_"+key+".sh"
	resubfile = BF_dir+"/resub_src/submit_"+key+".sh"
#	print(subfile)
	subIn = open(subfile,"r")
	subOut = open(resubfile,"w")
	lines = subIn.readlines()
	for line in lines:
		if( "request_memory" in line):
			subOut.write(cpu+"\n")
			subOut.write(disk+"\n")
			subOut.write(mem+"\n")

		else:
			subOut.write(line)
	subIn.close()
	subOut.close()

print("Generating submission aggregation script")
resub_agg_file = BF_dir+"/condor_resubmit.sh"
aggfile = open(resub_agg_file, "w")
for key in subDiff:
	line = "condor_submit "+BF_dir+"/resub_src/submit_"+key+".sh\n"
	aggfile.write(line)

aggfile.close()
os.chmod(resub_agg_file, 0o0777)
print("done, to launch datacards do this:")
print("./"+BF_dir+"/condor_resubmit.sh")
	
	
#in here
#vim condor_submit.sh
#condor_submit BF_B135_bugfix16_HinoN2C1_MCstats/src/submit_0.sh

#BF_B135_TChiWZ_MCstats/log/job_114.log.out
#how many sub src files are there?
#ls BF_dir/src
'''

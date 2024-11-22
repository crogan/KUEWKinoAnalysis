import glob
import os
import shutil
import math
#print(glob.glob("/home/adam/*"))



#test on this BF dir
#BF_dir = "BF_B135_TChipmWW_MCstats"
###job configuration
#cpu="request_cpus = 4"
#disk="request_disk = 15000000 KB"
#mem="request_memory = 4000 MB"


#BF_dir = "BF_B135_bugfix16_TChipmWW_MCstats"
#BF_dir = "BF_B135_bugfix16_TChiWZ_MCstats"
#BF_dir = "BF_B135_bugfix16_HinoN2C1_MCstats"
#cpu="request_cpus = 4"
#disk="request_disk = 2000000 KB"
#mem="request_memory = 4000 MB"

#BF_dir = "BF_B135_bugfix16_T2tt_MCstats"
#BF_dir = "BF_B135_bugfix16_TChipmWW_MCstats"
#BF_dir = "BF_B135_bugfix16_tthighdM_MCstats"
##BF_dir = "BF_B136_WZhighdM"
#BF_dir = "BF_B136_bWhighdM"
#BF_dir = "BF_B136_WWhighdM"
#BF_dir = "BF_B136_T2bW"#
BF_dir = "BF_B135_bugfix16_T2bW_MCstats"
#BF_dir = "BF_B135_bugfix16_T2bb_MCstats"
#BF_dir = "BF_B135_bugfix16_T2cc_MCstats"
#BF_dir = "BF_B135_bugfix16_TSlepSnu_MCstats"
#BF_dir = "BF_B135_bugfix16_tthighdM_MCstats"
#BF_dir = "BF_B135_bugfix16_WZhighdM_MCstats"
#BF_dir = "BF_B135_bugfix16_WWhighdM_MCstats"

cpu="request_cpus = 4"
disk="request_disk = 4500000 KB"
mem="request_memory = 6000 MB"

#limitJobs = True
limitJobs = False
maxMP = 600
maxdM = 300
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

def getMasses( masspoint ):
    #print("assessing masspoint", masspoint)
    MP = math.trunc( float(masspoint)/10000. ) 
    MP10k = MP*10000
    MC = int(masspoint) - MP10k
    dM = MP - MC
    return [MP,MC,dM]
def jobLimiter(BF_dir, maxMP, maxdM, jobList ):
    reducedJobList = []
    for key in jobList:
        subfile = BF_dir+"/src/submit_"+key+".sh"
        subIn = open(subfile,"r")
        lines = subIn.readlines()
        for line in lines:
            if( "Arguments" in line):
            #signal i hope is always the last one 
                sig = line.split(' ')[-2]
                masspoint = sig.split('_')[-1]
                #print("scanning Args:", sig, masspoint)
                masses = getMasses(masspoint)
                if( (masses[0] <= maxMP) and (masses[2] <= maxdM) ):
                    #print("saving masspoint",sig, masspoint, masses[0], masses[2])
                    reducedJobList.append(key)
                break

    
    print("using jobLimiter: subDiff reduction-", len(jobList),"->",len(reducedJobList))
    print("used limits MP<=",maxMP,"dM<=",maxdM)
    return reducedJobList



subs = glob.glob(BF_dir+"/src/*")
#print(subs)
for i,sub in enumerate(subs):
    jN = sub.split("/")[-1]
    jN = jN[:-3]
    jN = jN.split("_")[-1]
    subs[i] = jN

#print(subs)

logs = glob.glob(BF_dir+"/log/*.out")
for i,log in enumerate(logs):
    jN = log.split("/")[-1]
    jN = jN[:-8]
    jN = jN.split("_")[-1]
    logs[i] = jN

#print(logs)

subSet = set(subs)
logSet = set(logs)

subDiff = subSet.difference(logSet)
print("Analyzing:", BF_dir)
if(limitJobs):
    subDiff = jobLimiter(BF_dir, maxMP, maxdM, subDiff )

#exit()
print("found", len(subDiff), "missing jobs")
#exit()

print("generating resubmission scripts for this job list:")
print(subDiff)



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
            if("queue" in line):
                subOut.write("Requirements = HAS_SINGULARITY == True\n")
                subOut.write("+ApptainerImage = \"/cvmfs/singularity.opensciencegrid.org/cmssw/cms:rhel7\"\n")
                subOut.write("+ProjectName=\"cms.org.cern\"\n")
                subOut.write("+DesiredOS=\"rhel7\"\n")
                subOut.write("+RequiresCVMFS=True\n")
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

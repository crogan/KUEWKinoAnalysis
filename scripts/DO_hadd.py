import os, sys, commands, time
from glob import glob as glob
from subprocess import Popen as pop
import subprocess

if __name__ == "__main__":

    argv_pos = 1

    OUT_DIR = "dum"
    IN_DIR = "dum"
    
    if '-odir' in sys.argv:
        p = sys.argv.index('-odir')
        OUT_DIR = sys.argv[p+1]
        argv_pos += 2
    if '-idir' in sys.argv:
        p = sys.argv.index('-idir')
        IN_DIR = sys.argv[p+1]
        argv_pos += 2

    if not len(sys.argv) > 1 or '-h' in sys.argv or '--help' in sys.argv or OUT_DIR == "dum" or IN_DIR == "dum":
        print "Usage: %s [-idir /path/input_dir] [-odir /path/output_dir]" % sys.argv[0]
        print
        sys.exit(1)

    print "Input Directory: %s" % (IN_DIR)
    print "Output Directory: %s" % (OUT_DIR)
        
    # create and organize output folders
    os.system("mkdir -p "+OUT_DIR)
    os.system("mkdir -p HADD_logs/")

    skip_list = [
        #"SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8",
        #"TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8_Summer16_102X",
        "TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8_Summer16_102X",
    ]

    if os.path.exists("scripts/startup_C.so") is False:
        os.system("cd scripts && root.exe -b -l -q startup.C+ && cd ..")

    os.environ["LD_PRELOAD"] = os.environ["PWD"]+"/scripts/startup_C.so"
    hadd_big_processes = {}
    for target in os.listdir(IN_DIR):
        skip = False
        for dataset in skip_list:
            if dataset in target:
                skip = True
        if skip:
            continue

        #print target
        #haddcmd = "hadd -f "+OUT_DIR+"/"+target+".root "
        #for i in range(0,10):
            #os.system("mkdir -p "+IN_DIR+"/"+target+"/"+target+"_"+str(i))
            #os.system("mv "+IN_DIR+"/"+target+"/"+target+"_*"+str(i)+".root "+IN_DIR+"/"+target+"/"+target+"_"+str(i))
            #os.system("LD_PRELOAD=scripts/startup_C.so hadd -f -j 4 "+IN_DIR+"/"+target+"/"+target+"_"+str(i)+".root "+IN_DIR+"/"+target+"/"+target+"_"+str(i)+"/*.root")

        hadd_sml_processes = []
        if os.path.exists("HADD_logs/"+"/"+target) is True:
            os.system("rm -r HADD_logs/"+"/"+target)
        os.system("mkdir -p HADD_logs/"+"/"+target)
        for i in range(0,10):
            os.system("mkdir -p "+IN_DIR+"/"+target+"/"+target+"_"+str(i))
            for f in glob(os.path.join(IN_DIR+"/"+target+"/"+target+"_*"+str(i)+".root")):
                os.system("mv "+f+" "+IN_DIR+"/"+target+"/"+target+"_"+str(i)+"/") 
            hadd_sml_processes.append(pop("hadd -f "+IN_DIR+"/"+target+"/"+target+"_"+str(i)+".root "+IN_DIR+"/"+target+"/"+target+"_"+str(i)+"/*.root",stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True))

        for hadd_sml in hadd_sml_processes:
            if hadd_sml.poll() is True:
                hadd_sml.wait()
            out,err = hadd_sml.communicate()
            if err != "":
                err_log = open("HADD_logs/"+"/"+target+"/"+target+"_"+str(i)+".err","a")
                err_log.write(err)
                err_log.close()

        if len(hadd_big_processes) >= 10:
            for target, hadd_big in hadd_big_processes.items():
                if hadd_big.poll() is not None:
                    hadd_big.wait()
                    out,err = hadd_big.communicate()
                    if err != "":
                        print("Outputting error to: HADD_logs/"+"/"+target+".err")
                        err_log = open("HADD_logs/"+"/"+target+".err","a")
                        err_log.write(err)
                        err_log.close()
                    hadd_big_processes.pop(target,None)
                elif len(hadd_big_processes) < 10:
                    tmp_pop = pop("hadd -f "+OUT_DIR+"/"+target+".root "+IN_DIR+"/"+target+"/*.root",stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True)
                    hadd_big_processes[str(target)] = tmp_pop
            
        else:
            tmp_pop = pop("hadd -f "+OUT_DIR+"/"+target+".root "+IN_DIR+"/"+target+"/*.root",stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True)
            hadd_big_processes[str(target)] = tmp_pop
        

    for target, hadd_big in hadd_big_processes.items():
        if hadd_big.poll() is not None:
            hadd_big.wait()
            out,err = hadd_big.communicate()
            if err != "":
                print("Outputting error to: HADD_logs/"+"/"+target+".err")
                err_log = open("HADD_logs/"+"/"+target+".err","a")
                err_log.write(err)
                err_log.close()
            if hadd_big.poll() is None:
                hadd_big_processes.pop(target,None)
    if len(hadd_big_processes) == 0:
        print("Finished Merging Files")
    else:
        print("Possible Error! Some hadd jobs may still be running!")

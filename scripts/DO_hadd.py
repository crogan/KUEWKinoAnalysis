import os, sys, commands, time

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

    skip_list = [
        #"SMS-T2bW_TuneCP2_13TeV-madgraphMLM-pythia8",
        #"SMS-T2tt_mStop-400to1200_TuneCP2_13TeV-madgraphMLM-pythia8",
    ]

    isFirstDir = True
    if os.path.exists("scripts/startup_C.so") is False:
        os.system("cd scripts && root.exe -b -l -q startup.C+ && cd ..")
    for dirs in os.walk(IN_DIR):
        skip = False
        target = dirs[0].split("/")
        target = target[-1]
        if isFirstDir:
            isFirstDir = False
            continue
        for dataset in skip_list:
            if dataset in target:
                skip = True
        if skip:
            continue

        #print target
        #haddcmd = "hadd -f "+OUT_DIR+"/"+target+".root "
        for i in range(0,10):
            os.system("mkdir -p "+IN_DIR+"/"+target+"/"+target+"_"+str(i))
            os.system("mv "+IN_DIR+"/"+target+"/"+target+"_*"+str(i)+".root "+IN_DIR+"/"+target+"/"+target+"_"+str(i))
            os.system("LD_PRELOAD=scripts/startup_C.so hadd -f "+IN_DIR+"/"+target+"/"+target+"_"+str(i)+".root "+IN_DIR+"/"+target+"/"+target+"_"+str(i)+"/*.root")
        haddcmd = "LD_PRELOAD=scripts/startup_C.so hadd -f "+OUT_DIR+"/"+target+".root "+IN_DIR+"/"+target+"/*.root"
        #print haddcmd
        os.system(haddcmd)

    print("Finished Merging Files")

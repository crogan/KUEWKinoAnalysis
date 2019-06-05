#! /usr/bin/env python

import os, sys, commands, time

#look for the current directory
#######################################
pwd = os.environ['PWD']
home = os.environ['HOME']
#######################################
RUN_DIR = pwd
TEMP = pwd
EXE  = "MakeReducedNtuple.x"
#EXE  = "MakeEventCount.x"
#OUT  = "/home/t3-ku/crogan/NTUPLES/Processing/"
OUT = pwd
LIST = "default.list"
QUEUE = ""
TREE = "stopTreeMaker/AUX"
MAXN = 25

def new_listfile(rootlist, listfile):
    mylist = open(listfile,'w')
    for f in rootlist:
        mylist.write(f+" \n")
    mylist.close()

def create_filelist(rootlist, dataset, filetag):
    listlist = []
    listcount = 0
    
    sublist = []
    for f in rootlist:
        sublist.append(f)
        if len(sublist) >= MAXN and MAXN > 0:
            listfile = "%s/%s_%s_%d.list" % (listdir, dataset, filetag, listcount)
            new_listfile(sublist, listfile)
            listlist.append(listfile)
            sublist = []
            listcount += 1

    if len(sublist) > 0:
        listfile = "%s/%s_%s_%d.list" % (listdir, dataset, filetag, listcount)
        new_listfile(sublist, listfile)
        listlist.append(listfile)

    return listlist

def write_sh(srcfile,ifile,ofile,lfile,dataset,filetag,evtcnt):
    fsrc = open(srcfile,'w')
    fsrc.write('universe = vanilla \n')
    fsrc.write('executable = '+EXE+" \n")
    fsrc.write('getenv = True \n')
    fsrc.write('Arguments = ');
    fsrc.write('-ilist='+ifile+" ")
    fsrc.write('-ofile='+ofile+" ")
    fsrc.write('-tree='+TREE+" ")
    if DO_SMS == 1:
        fsrc.write('--sms ')
    fsrc.write('-dataset='+dataset+" ")
    fsrc.write('-filetag='+filetag+" ")
    fsrc.write('-eventcount='+evtcnt+" \n")
    fsrc.write('output = '+lfile+" \n")
    fsrc.write('queue \n')
    #fsrc.write('cd '+RUN_DIR+" \n")
    #fsrc.write('source ../RestFrames/setup_RestFrames.sh \n')
    fsrc.close()

if __name__ == "__main__":
    if not len(sys.argv) > 1 or '-h' in sys.argv or '--help' in sys.argv:
        print "Usage: %s [-q queue] [-tree treename] [-list listfile.list] [-maxN N] [--sms]" % sys.argv[0]
        print
        sys.exit(1)

    argv_pos = 1
    DO_SMS = 0
  
    if '-q' in sys.argv:
        p = sys.argv.index('-q')
        QUEUE = sys.argv[p+1]
        argv_pos += 2
    if '-list' in sys.argv:
        p = sys.argv.index('-list')
        LIST = sys.argv[p+1]
        argv_pos += 2
    if '-tree' in sys.argv:
        p = sys.argv.index('-tree')
        TREE = sys.argv[p+1]
        argv_pos += 2
    if '-maxN' in sys.argv:
        p = sys.argv.index('-maxN')
        MAXN = int(sys.argv[p+1])
        argv_pos += 2
    if '--sms' in sys.argv:
        DO_SMS = 1
        argv_pos += 1

    print "maxN is %d" % MAXN

    # input sample list
    listfile = LIST
    listname = listfile.split("/")
    listname = listname[-1]

    print listname

    NAME = listname.replace(".list",'')
    
    print NAME
    print RUN_DIR
        
    # create and organize output folders
    TARGET  = RUN_DIR+"/"+NAME+"/"
    os.system("rm -rf "+TARGET)
    os.system("mkdir -p "+TARGET)
    listdir = TARGET+"list/"
    srcdir  = TARGET+"src/"
    logdir  = TARGET+"log/"
    os.system("mkdir -p "+listdir)
    os.system("mkdir -p "+logdir)
    os.system("mkdir -p "+srcdir)

    # make EventCount file and folder
    evtcntdir  = TARGET+"evtcnt/"
    os.system("mkdir -p "+evtcntdir)
    os.system("hadd "+evtcntdir+"EventCount.root root/EventCount/*.root")
    evtcnt = evtcntdir+"EventCount.root"
    
    # output root files
    ROOT = OUT+"/"+NAME+"/"
    if ROOT == TARGET:
        ROOT = ROOT+"root/"

    # make output folders
    os.system("rm -rf "+ROOT)
    os.system("mkdir -p "+ROOT)

    hints = []
    hints.append("0000")
    hints.append("0001")
    hints.append("0002")
    hints.append("0003")
    hints.append("0004")

    datasetlist = []

    knowntags = ["Fall17","Autumn18","Summer16","TuneCP5","TuneCUETP"]
    
    with open(listfile,'r') as mylist:
        inputlist = mylist.readlines()

        # get dataset name (filetag) for bookkeeping
        for fold in inputlist:
            fold = fold.strip('\n\r')
            print "Processing folder %s" % fold

            filetag = "unknown"
            for ktag in knowntags:
                if ktag in fold:
                    filetag = ktag
                    break

            
            dataset = fold.split("/")
            found = 0
            for hint in hints:
                index = 0
                for sub in dataset:
                    if hint in sub:
                        dataset = dataset[index-3]
                        found = 1
                        break
                    index += 1
                if found == 1:
                    break
            if found == 0:
                continue

            rootlist = [os.path.join(fold,f) for f in os.listdir(fold) if (os.path.isfile(os.path.join(fold, f)) and (".root" in f))]
            if len(rootlist) < 1:
                continue
            
            if len(datasetlist) == 0:
                datasetlist.append((dataset,filetag,rootlist))
                os.system("mkdir -p "+ROOT+dataset+"_"+filetag+"/")
                continue
            
            tagtuple = [item for item in datasetlist if (item[0] == dataset and item[1] == filetag)]
            if len(tagtuple) == 0:
                datasetlist.append((dataset,filetag,rootlist))
                os.system("mkdir -p "+ROOT+dataset+"_"+filetag+"/")
                continue

            p = datasetlist.index(tagtuple[0])
            datasetlist[p][2].extend(rootlist)

    for (dataset,filetag,rootlist) in datasetlist:
        listlist = create_filelist(rootlist, dataset, filetag)

        for f in listlist:
            filename = f.split("/")
            filename = filename[-1]
            name = filename.replace(".list",'')
            write_sh(srcdir+name+".sh",f,ROOT+dataset+"_"+filetag+"/"+name+".root",logdir+name+".log",dataset,filetag,evtcnt)
            os.system('condor_submit '+srcdir+name+".sh")
            
    

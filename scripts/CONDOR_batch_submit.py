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
OUT  = "/home/t3-ku/crogan/NTUPLES/Processing/"
# OUT = RUN_DIR+"/SAMPLES/"
TARGET = "default"
QUEUE = ""
TREE = "stopTreeMaker/AUX"


def write_sh(srcfile,ifile,ofile,lfile):
    fsrc = open(srcfile,'w')
    fsrc.write('universe = vanilla \n')
    fsrc.write('executable = '+EXE+" \n")
    fsrc.write('getenv = True \n')
    fsrc.write('Arguments = ');
    fsrc.write('-ifile='+ifile+" ")
    fsrc.write('-ofile='+ofile+" ")
    fsrc.write('-tree='+TREE+" \n")
    fsrc.write('output = '+lfile+" \n")
    fsrc.write('queue \n')
    #fsrc.write('cd '+RUN_DIR+" \n")
    #fsrc.write('source ../RestFrames/setup_RestFrames.sh \n')
    fsrc.close()

if __name__ == "__main__":
    if not len(sys.argv) > 1 or '-h' in sys.argv or '--help' in sys.argv:
        print "Usage: %s [-q queue] [-name DEST] [-sfold,sfile,-bfold,-bfile] [-Njob Njob]" % sys.argv[0]
        print
        sys.exit(1)

    argv_pos = 1
  
    if '-q' in sys.argv:
        p = sys.argv.index('-q')
        QUEUE = sys.argv[p+1]
        argv_pos += 2
    if '-list' in sys.argv:
        p = sys.argv.index('-list')
        TARGET = sys.argv[p+1]
        argv_pos += 2
    if '-tree' in sys.argv:
        p = sys.argv.index('-tree')
        TREE = sys.argv[p+1]
        argv_pos += 2

    # input sample list
    listfile = "samples/"+TARGET+".list"
        
    # create and organize output folders
    ROOT = OUT+"/"+TARGET+"/"
    TARGET  = RUN_DIR+"/"+TARGET+"/"
    srcdir  = TARGET+"src/"
    logdir  = TARGET+"log/"

    # make output folders
    os.system("rm -rf "+TARGET)
    os.system("mkdir -p "+TARGET)
    os.system("mkdir -p "+logdir)
    os.system("mkdir -p "+srcdir)
    os.system("rm -rf "+ROOT)
    os.system("mkdir -p "+ROOT)

    with open(listfile,'r') as f:
        inputlist = f.readlines()
        for line in inputlist:
            line = line.split()
            line = line[0]
            filename = line.split("/")
            filename = filename[-1]
            name = filename.strip(".root");
            write_sh(srcdir+name+".sh",line,ROOT+filename,logdir+name+".log")
            os.system('condor_submit '+srcdir+name+".sh")
    

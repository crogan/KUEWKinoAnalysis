#! /usr/bin/env python

import os, sys, commands, time


# ----------------------------------------------------------- #
# Parameters
# ----------------------------------------------------------- #
# current working directory
pwd         = os.environ['PWD']
RUN_DIR     = pwd
jobEXE      = "execute_script.sh"
EXE         = "MakeReducedNtuple_NANO.x"
RESTFRAMES  = './scripts/setup_RestFrames_connect.sh'
CMSSW_SETUP = './scripts/cmssw_setup_connect.sh'
TREE        = "Events"
USER        = os.environ['USER']
OUT_BASE    = "/stash/user/"+USER+"/NTUPLES/Processing"
LIST        = "default.list"
QUEUE       = ""
SPLIT       = 1
# ----------------------------------------------------------- #

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
        if len(sublist) >= 1:
            listfile = "%s/%s_%s_%d.list" % (listdir_sam, dataset, filetag, listcount)
            new_listfile(sublist, listfile)
            listlist.append(listfile)
            sublist = []
            listcount += 1

    if len(sublist) > 0:
        listfile = "%s/%s_%s_%d.list" % (listdir_sam, dataset, filetag, listcount)
        new_listfile(sublist, listfile)
        listlist.append(listfile)

    return listlist

def write_sh(srcfile,ifile,ofile,logfile,outfile,errfile,dataset,filetag,n):
    fsrc = open(srcfile,'w')
    fsrc.write('universe = vanilla \n')
    fsrc.write('executable = '+jobEXE+" \n")
    fsrc.write('use_x509userproxy = true \n')
    fsrc.write('Arguments = ');
    fsrc.write('-ilist=$(Item) ')
    fsrc.write('-ofile='+ofile.split('/')[-1]+" ")
    fsrc.write('-tree='+TREE+" ")
    if DO_SMS == 1:
        fsrc.write('--sms ')
    if DO_DATA == 1:
        fsrc.write('--data ')
    if SYS == 1 and DO_DATA != 1:
        fsrc.write('--sys ')
    fsrc.write('-dataset='+dataset+" ")
    fsrc.write('-filetag='+filetag+" ")
    fsrc.write('-eventcount='+EVTCNT+" ")
    fsrc.write('-filtereff='+FILTEREFF+" ")
    fsrc.write('-json='+JSON+" ")
    fsrc.write('-pu='+PUFOLD+" ")
    fsrc.write('-btag='+BTAGFOLD+" ")
    fsrc.write('-jme='+JMEFOLD+" ")
    fsrc.write('-svfile='+SVFILE+" ")
    fsrc.write('-metfile='+METFILE+" ")
    fsrc.write('-prefirefile='+PREFIREFILE+" ")
    splitstring = '-split=%s,%d\n' % ('$$([$(Step)+1])', n)
    fsrc.write(splitstring)

    outlog = outfile+".out"
    errlog = errfile+".err"
    loglog = logfile+".log"
    fsrc.write('output = '+outlog+" \n")
    fsrc.write('error = '+errlog+" \n")
    fsrc.write('log = '+loglog+" \n")
    fsrc.write('Requirements = (Machine != "red-node000.unl.edu" && Machine != "ncm*.hpc.itc.rwth-aachen.de" && Machine != "mh-epyc7662-8.t2.ucsd.edu")\n')
    fsrc.write('request_memory = 2 GB \n')
    #fsrc.write('priority = 10 \n')
    #fsrc.write('+RequiresCVMFS = True \n')
    #fsrc.write('+RequiresSharedFS = True \n')

    transfer_input = 'transfer_input_files = '+TARGET+'config.tgz,/stash/user/zflowers/public/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2\n'
    fsrc.write(transfer_input)

    fsrc.write('should_transfer_files = YES\n')
    fsrc.write('when_to_transfer_output = ON_EXIT\n')

    transfer_out_files = 'transfer_output_files = '+ofile.split('/')[-1]+'\n'
    fsrc.write(transfer_out_files)

    transfer_out_remap = 'transfer_output_remaps = "'+ofile.split('/')[-1]+'='+ofile
    transfer_out_remap += '"\n'
    fsrc.write(transfer_out_remap)
    
    fsrc.write('+ProjectName="cms.org.ku"\n')
    fsrc.write('+REQUIRED_OS="rhel7"\n')
    fsrc.write('queue '+str(n)+' from '+ifile+'\n')
    fsrc.close()

if __name__ == "__main__":
    if not len(sys.argv) > 1 or '-h' in sys.argv or '--help' in sys.argv:
        print "Usage: %s [-q queue] [-tree treename] [-list listfile.list] [-split S] [--sms] [--data] [--sys] [--dry-run] [--verbose] [--count]" % sys.argv[0]
        sys.exit(1)

    argv_pos    = 1
    DO_SMS      = 0
    DO_DATA     = 0
    DRY_RUN     = 0
    COUNT       = 0
    VERBOSE     = 0
    SYS         = 0
  
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
    if '-split' in sys.argv:
        p = sys.argv.index('-split')
        SPLIT = int(sys.argv[p+1])
        argv_pos += 2
    if '--sms' in sys.argv:
        DO_SMS = 1
        argv_pos += 1
    if '--data' in sys.argv:
        DO_DATA = 1
        argv_pos += 1
    if '--dry-run' in sys.argv:
        DRY_RUN = 1
        argv_pos += 1
    if '--count' in sys.argv:
        COUNT = 1
        argv_pos += 1
    if '--verbose' in sys.argv:
        VERBOSE = 1
        argv_pos += 1
    if '--sys' in sys.argv:
        SYS = 1
        argv_pos += 1
        
    if SPLIT <= 1:
        SPLIT = 1
    
    print " --- Preparing condor submission to create ntuples."
    if DO_DATA:
        print " --- Processing Data"

    if DO_SMS:
        print " --- Processing SMS"
    
    if SYS:
        print " --- Processing SYS"

    if COUNT:
        print " --- Only Counting (No Processing)"

    # input sample list
    listfile = LIST
    listname = listfile.split("/")
    listname = listname[-1]

    NAME = listname.replace(".list",'')
    
    # create and organize output folders
    TARGET  = RUN_DIR+"/"+NAME+"/"
    if not COUNT:
        os.system("rm -rf "+TARGET)
        os.system("mkdir -p "+TARGET)
    listdir = TARGET+"list/"
    srcdir  = TARGET+"src/"
    logdir  = TARGET+"log/"
    outdir  = TARGET+"out/"
    errdir  = TARGET+"err/"
    if not COUNT:
        os.system("mkdir -p "+listdir)
        os.system("mkdir -p "+logdir)
        os.system("mkdir -p "+outdir)
        os.system("mkdir -p "+errdir)
    os.system("mkdir -p "+srcdir)

    # make config directory
    config = TARGET+"config/"
    if not COUNT:
        os.system("mkdir -p "+config)

    # NOTE: there is a bug for setting the hadd verbosity level, "hadd -v 0".
    # The hadd verbosity option only works in ROOT 6.18/00 and later.
    # https://github.com/root-project/root/issues/11372
    # https://github.com/root-project/root/pull/3914

    if not COUNT:

        # make EventCount file
        os.system("hadd "+config+"EventCount.root root/EventCount/*.root > /dev/null")
        EVTCNT = "./config/EventCount.root"

        # make FilterEff file 
        os.system("hadd "+config+"FilterEff.root root/FilterEff/*.root > /dev/null")
        FILTEREFF = "./config/FilterEff.root"

        # make json file
        os.system("cat json/GoodRunList/*.txt > "+config+"GRL_JSON.txt")
        os.system("echo -n $(tr -d '\n' < "+config+"GRL_JSON.txt) > "+config+"GRL_JSON.txt")
        JSON = "./config/GRL_JSON.txt"

        # copy PU root files
        os.system("cp -r root/PU "+config+".")
        PUFOLD = "./config/PU/"

        # copy BTAG SF files
        os.system("cp -r root/BtagSF "+config+".")
        os.system("cp -r csv/BtagSF/* "+config+"BtagSF/.")
        BTAGFOLD = "./config/BtagSF/"

        # copy JME files
        os.system("cp -r data/JME "+config+".")
        JMEFOLD = "./config/JME/"

        # copy MET trigger files
        os.system("cp -r csv/METTrigger "+config+".")
        METFILE = "./config/METTrigger/Parameters.csv"

        # copy Prefire files
        os.system("cp -r root/Prefire "+config+".")
        PREFIREFILE = "./config/Prefire/Prefire.root"

        # copy SV NN model
        os.system("cat json/lwtnn/nano_train_model.json > "+config+"NNmodel.json")
        SVFILE = "./config/NNmodel.json"
        
        os.system("cp "+EXE+" "+config+".")
        os.system("cp "+RESTFRAMES+" "+config+".")
        os.system("cp "+CMSSW_SETUP+" "+config+".")

    # output root files
    OUT_DIR = OUT_BASE+"/"+NAME+"/"
    if OUT_DIR == TARGET:
        OUT_DIR = OUT_DIR+"root/"

    total_root_files = 0

    datasetlist     = []
    clean_inputlist = []
    input_info      = {}

    knowntags = ["Fall17_94X","Autumn18_102X","Summer16_94X","Fall17_102X","Summer16_102X","Summer20UL16_102X","Summer20UL16APV_102X","Summer20UL17_102X","Summer20UL18_102X","RunIISummer20UL17NanoAODv9"]
    
    n_samples = 0
    with open(listfile,'r') as mylist:
        inputlist = mylist.readlines()

        for flist in inputlist:
            # skip commented lines (skip if # is anywhere in line)
            if '#' in flist:
                continue

            flist = flist.strip('\n\r')
            clean_inputlist.append(flist)
            input_info[flist] = {}

            listfile = LIST
            listname = listfile.split("/")
            listname = listname[-1]

            n_samples = n_samples+1

            dataset = flist.split("/")
            dataset = dataset[-1]
            dataset = dataset.replace(".txt",'')

            filetag = ""
            for ktag in knowntags:
                if ktag in flist:
                    filetag = ktag

            # get list of ROOT files 
            rootlist = []
            with open(flist,'r') as myflist:
                inputfilelist = myflist.readlines()

                for afile in inputfilelist:
                    afile = afile.strip('\n\r')
                    rootlist.append(afile);
            
            n_root_files        = len(rootlist)
            n_jobs              = SPLIT * n_root_files
            total_root_files    += n_root_files
            
            input_info[flist]["n_root_files"]   = n_root_files
            input_info[flist]["n_jobs"]         = n_jobs

            if len(datasetlist) == 0:
                datasetlist.append((dataset,filetag,rootlist))
                if not COUNT:
                    os.system("rm -rf "+OUT_DIR+dataset+"_"+filetag+"/")
                    os.system("mkdir -p "+OUT_DIR+dataset+"_"+filetag+"/")
                continue
            
            tagtuple = [item for item in datasetlist if item[0] == dataset]
            if len(tagtuple) == 0:
                datasetlist.append((dataset,filetag,rootlist))
                if not COUNT:
                    os.system("rm -rf "+OUT_DIR+dataset+"_"+filetag+"/")
                    os.system("mkdir -p "+OUT_DIR+dataset+"_"+filetag+"/")
                continue

            p = datasetlist.index(tagtuple[0])
            datasetlist[p][2].extend(rootlist)

    for (dataset,filetag,rootlist) in datasetlist:
        if not COUNT:
            os.system("mkdir -p "+os.path.join(listdir, dataset+'_'+filetag))
            listdir_sam = os.path.join(listdir, dataset+'_'+filetag)
            listlist = create_filelist(rootlist, dataset, filetag)
            overlist_name = listdir_sam+'/'+dataset+'_'+filetag+'_list.list'
            with open(overlist_name,'w') as overlist:
                newlistlist = ['config/'+'/'.join(l.split('/')[-3:])+'\n' for l in listlist]
                overlist.writelines(newlistlist)
                overlist.close()

        if not COUNT:
            os.system("mkdir -p "+os.path.join(logdir, dataset+'_'+filetag))
            os.system("mkdir -p "+os.path.join(outdir, dataset+'_'+filetag))
            os.system("mkdir -p "+os.path.join(errdir, dataset+'_'+filetag))

            file_name = os.path.join(OUT_DIR, dataset+'_'+filetag, overlist_name.split('/')[-1].replace('_list.list', '_$(ItemIndex)_$(Step)'))

            logfile = os.path.join(logdir, dataset+'_'+filetag, file_name.split('/')[-1])
            outfile = os.path.join(outdir, dataset+'_'+filetag, file_name.split('/')[-1])
            errfile = os.path.join(errdir, dataset+'_'+filetag, file_name.split('/')[-1])

            script_name = srcdir+'_'.join([dataset, filetag])+'.submit'
            write_sh(script_name, overlist_name, file_name+'.root', logfile, outfile, errfile, dataset, filetag, SPLIT)

    if not COUNT:
        #print listdir
        os.system("cp -r "+listdir+" "+config)
        #print "creating tarball from: ", TARGET
        os.system("tar -C "+config+"/../ -czf "+TARGET+"/config.tgz config")

    submit_dir  = srcdir        
    submit_list = [os.path.join(submit_dir, f) for f in os.listdir(submit_dir) if (os.path.isfile(os.path.join(submit_dir, f)) and ('.submit' in f))]
    #n_samples   = len(submit_list)
    total_jobs  = SPLIT * total_root_files

    # don't submit jobs if --dry-run is used
    if not DRY_RUN or not COUNT:
        for f in submit_list:
            print "submitting: {0}".format(f)
            os.system('condor_submit ' + f)
    
    # Number of ROOT files and jobs per sample 
    if VERBOSE:
        for f in clean_inputlist:
            n_root_files    = input_info[f]["n_root_files"] 
            n_jobs          = input_info[f]["n_jobs"] 
            n_jobs = SPLIT * n_root_files
            print "sample: {0}".format(f)
            print(" - number of root files  = {0}".format(n_root_files))
            print(" - number of jobs        = {0}".format(n_jobs))
    
    # Summary Info
    print "----------------------------"
    print "Condor Submission Info"
    print "----------------------------"
    print "sample list:             {0}".format(LIST)
    print "working directory:       {0}".format(TARGET)
    print "output directory:        {0}".format(OUT_DIR)
    print "number of samples:       {0}".format(n_samples)
    print "split:                   {0}".format(SPLIT)
    print "total input root files:  {0}".format(total_root_files)
    print "total condor jobs:       {0}".format(total_jobs)
    print "----------------------------"

    if DRY_RUN:
        print "The option --dry-run was used; no jobs were submitted."
    else:
        print "Congrats... your jobs were submitted!"


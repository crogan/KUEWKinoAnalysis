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
        print "Usage: %s [-odir /path/output_dir] [-odir /path/output_dir]" % sys.argv[0]
        print
        sys.exit(1)

    print "Input Directory: %s" % (IN_DIR)
    print "Output Directory: %s" % (OUT_DIR)
        
    # create and organize output folders
    os.system("rm -rf "+OUT_DIR)
    os.system("mkdir -p "+OUT_DIR)

    for dirs in os.walk(IN_DIR):
        target = dirs[0].split("/")
        target = target[-1]
        print target
        haddcmd = "hadd "+OUT_DIR+"/"+target+".root "
        haddcmd += IN_DIR+"/"+target+"/*.root"
        print haddcmd
        os.system(haddcmd)

# ------------------------------------------------------------------------------------------------
# Adopted from https://github.com/ku-cms/SUSYCascades/blob/master/MakeNtuple_Connect/CheckJobs.py
# Example call of script from SUSYCascades:
#    python3 scripts/CheckFiles.py -d Summer23_130X/ -o ../../../NTUPLES/Processing/Summer23_130X/ -e -r
# ------------------------------------------------------------------------------------------------
import os, argparse, subprocess, itertools
USER = os.environ['USER']

def getMissingFiles(outputDir,nSplit,nList):
    baseTupleList = list(itertools.product(range(nList), range(nSplit)))
    for filename in os.listdir(outputDir):
        filename_str = filename.split(".")[0]
        Tuple = (int(filename_str.split("_")[-2]),int(filename_str.split("_")[-1]))
        if Tuple in baseTupleList:
            baseTupleList.remove(Tuple)
    return baseTupleList

def makeSubmitScript(tuple_pairs,submitName,resubmit,skipClean):
    tuple_filelist = f"{submitName}_tuple.txt"
    with open(tuple_filelist,'w') as tuple_file:
        for tuple_pair in tuple_pairs:
            tuple_file.write(f"{tuple_pair[0]},{tuple_pair[1]}\n")
    newFileName = f"{submitName}_tuple.submit"
    os.system(f"cp {submitName}_single.submit {newFileName}")
    with open(newFileName,'r') as file:
        file_content = file.read()
    file_content = file_content.replace("0_0","$(list)_$(split)")
    file_content = file_content.replace("-split=1","-split=$$([$(split)+1])")
    file_content = file_content.replace("X_0.list","X_$(list).list")
    file_content = file_content.replace("queue",f"queue list,split from {tuple_filelist}")
    with open(newFileName, 'w') as file:
        file.write(file_content)
    if resubmit:
        os.system(f"condor_submit {newFileName}")
    if skipClean:
        os.system(f"rm {newFileName}")

# Check condor jobs
def checkJobs(workingDir,outputDir,skipMissing,skipSmall,skipErr,resubmit,skipClean,maxResub):
    print("Running over the directory '{0}'.".format(workingDir))
    print("------------------------------------------------------------")
    srcDir = os.listdir(workingDir+"/src/")
    for file in srcDir:
        if "X.submit" not in file:
            continue
        DataSetName = file.split(".")[0]
        resubmitFiles = []
        if(not skipMissing):
            with open(workingDir+"/src/"+DataSetName+".submit") as file:
                file.seek(0,2)
                file.seek(file.tell()-2,0)
                while file.read(1) != "\n" and file.tell() > 0:
                    file.seek(file.tell()-2,0)
                last_line = file.readline().strip()
                nSplit = int(last_line.split(" ")[1])
            listDir = os.path.abspath(workingDir+"/list/"+DataSetName+"/")
            listFiles = os.listdir(listDir)
            numList = len([listFile for listFile in listFiles if os.path.isfile(os.path.join(listDir,listFile))])
            numList = numList - 1 # remove master list file
            nJobsSubmit = numList*nSplit
            bash = "ls "+outputDir+DataSetName+" | wc -l"
            nJobsOutput = int(subprocess.check_output(['bash','-c', bash]).decode())
            resubmitFiles = getMissingFiles(outputDir+DataSetName,nSplit,numList)
            print(f"Got {len(resubmitFiles)} missing files for dataset {DataSetName}")
        if(not skipSmall):
            bash = "find "+outputDir+DataSetName+" -type f -size -10k"
            smallFiles = subprocess.check_output(['bash','-c', bash]).decode()
            smallFiles = smallFiles.split("\n")
            smallFiles.remove('')
            num_small = 0
            for smallFile in smallFiles:
                smallFile = smallFile.split(".root")[0]
                Tuple = (int(smallFile.split("_")[-2]),int(smallFile.split("_")[-1]))
                resubmitFiles.append(Tuple)
                num_small = num_small+1
            print(f"Got {num_small} small files for dataset {DataSetName}")
        if(not skipErr):
            bash = "grep -v -e \"Warning\" -e \"WARNING\" -e \"TTree::SetBranchStatus\" -e \"libXrdSecztn.so\" "+ workingDir +"/err/"+DataSetName+"/*.err"
            errorFiles = subprocess.check_output(['bash','-c',bash]).decode()
            errorFiles = errorFiles.split("\n")
            errorFiles.remove('')
            for errorFile in errorFiles:
                errorFile = errorFile.split(".err")[0]
                Tuple = (int(errorFile.split("_")[-2]),int(errorFile.split("_")[-1]))
                if Tuple not in resubmitFiles:
                    resubmitFiles.append(Tuple)
            print("Got error files for dataset",DataSetName)
        if len(resubmitFiles) >= maxResub:
            print(f"You are about to make {len(resubmitFiles)} and resubmit {len(resubmitFiles)} jobs for dataset: {DataSetName}!")
            print(f"You should double check there are no issues with your condor submissions")
            print(f"If you are confident you want to resubmit, then you should rerun this script with -l {len(resubmitFiles)}")
        else:
            makeSubmitScript(resubmitFiles,workingDir+"/src/"+DataSetName,resubmit,skipClean)

def main():
    # options
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--directory", "-d", default="Summer22_130X/", help="working directory for condor submission")
    parser.add_argument("--output", "-o", default="/ospool/cms-user/"+USER+"/NTUPLES/Processing/Summer22_130X/", help="output area for root files")
    parser.add_argument("--skipResubmit", "-r", action='store_false', help="do not resubmit files")
    parser.add_argument("--skipMissing", "-m", action='store_true', help="skip checking missing files")
    parser.add_argument("--skipSmall", "-s", action='store_true', help="skip checking small files")
    parser.add_argument("--skipErr", "-e", action='store_true', help="skip checking err files")
    parser.add_argument("--skipClean", "-c", action='store_true', help="skip cleaning up new submission files")
    parser.add_argument("--maxResub", "-l", default=100, help="max number of jobs to resubmit")

    options = parser.parse_args()
    directory = options.directory	
    output = options.output
    resubmit = options.skipResubmit
    skipMissing = options.skipMissing
    skipSmall = options.skipSmall
    skipErr = options.skipErr
    skipClean = options.skipClean
    maxResub = options.maxResub

    checkJobs(directory,output,skipMissing,skipSmall,skipErr,resubmit,skipClean,maxResub)

if __name__ == "__main__":
    main()

# tools.py

import os
import csv
import glob
import ROOT

# creates directory if it does not exist
def makeDir(dir_name):
    if not os.path.exists(dir_name):
        os.makedirs(dir_name)

# get numbers from a string using a delimiter separation
def getNumbers(string, delimiter):
    numbers = [int(s) for s in string.split(delimiter) if s.isdigit()]
    return numbers

# determine if number is in range
def numberInRange(number, range_min, range_max):
    if number >= range_min and number <= range_max:
        return True
    else:
        return False

# takes a csv file as input and outputs data in a matrix
def getData(input_file):
    data = []
    with open(input_file, "r") as f:
        reader = csv.reader(f)
        for row in reader:
            data.append(row)
    return data

# get chain from list of ROOT files
def getChain(input_files, num_files):
    verbose = True
    
    # use num_files as max if it is not negative
    if num_files >= 0:
        input_files = input_files[0:num_files]
    n_input_files = len(input_files)
    
    # Create TChain
    chain = ROOT.TChain('pixelTree')
    
    # Add files to chain
    if verbose:
        print("Adding {0} file(s) to chain:".format(n_input_files))
    for f in input_files:
        if verbose:
            print(" - {0}".format(f))
        chain.Add(f)
    
    return chain

# get list of local files
def get_file_list(dir_):
    file_list_tmp = [os.path.join(dir_, f) for f in os.listdir(dir_) if (os.path.isfile(os.path.join(dir_, f)) and ('.root' in f))]
    return file_list_tmp

# get list of local files using glob
def get_file_list_glob(directory, pattern="*.root"):
    d = directory
    if d[-1] != "/":
        d += "/"
    return glob.glob(d + pattern)

# get list of EOS files
def get_eos_file_list(path, eosurl="root://cmseos.fnal.gov"):
    output = [] 
    with eosls(path, "", eosurl) as files:
        for f in files:
            name = f.strip()
            # add ROOT files to list
            if name.endswith(".root"):
                full_name = "{0}/{1}".format(eosurl, name)
                output.append(full_name)
    return output

# eosls command using xrdfs
def eosls(path, option="", eosurl="root://cmseos.fnal.gov"):
    return os.popen("xrdfs %s ls %s %s" % (eosurl, option, path))

# eosrm command using xrdfs
def eosrm(path, option="", eosurl="root://cmseos.fnal.gov"):
    return os.popen("xrdfs %s rm %s %s" % (eosurl, option, path))



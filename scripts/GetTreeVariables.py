# GetTreeVariables.py
# Caleb J. Smith
# February 7, 2019

# print TTree variables to file

import argparse
import ROOT

def main():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--input_file",  "-i", required=True,     help="input root file")
    parser.add_argument("--tree",        "-t", default="Events",  help="tree in root file that you wish to print")
    parser.add_argument("--directory",   "-d", default="",        help="directory in root file (optional)")
    
    options     = parser.parse_args()
    input_file  = options.input_file
    tree        = options.tree
    directory   = options.directory
    
    # open the ROOT file
    tFile = ROOT.TFile.Open(input_file)

    # set path to tree
    path_to_tree = ""
    if directory:
        path_to_tree = "{0}/{1}".format(directory, tree)
    else:
        path_to_tree = tree

    # debug
    print("path_to_tree: {0}".format(path_to_tree))
    
    # get the tree using its path
    tTree = tFile.Get(path_to_tree)

    # print the tree
    output = tTree.Print()

if __name__ == "__main__":
    main()


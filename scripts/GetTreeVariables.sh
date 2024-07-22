#!/bin/bash
# GetTreeVariables.sh
# Caleb J. Smith
# February 7, 2019

# print TTree variables to file

# Example syntax for ROOT file
#
# Note: -d (directory in ROOT file) is optional
#
# GetTreeVariables.sh -n <root_tree_name> -d <root_dir_name> -o <output_name> -r <root_file>
#

# Arguments:
# - name of tree in ROOT file
# - name of directory in ROOT file (optional)
# - output text file to save variables from tree
# - input either a text file (listing root files) or root file
treeName=
dirName=
outputName=
textFile=
rootFile=

# path to python script
#pythonScript=scripts/GetTreeVariables.py
#pythonScript=/home/$USER/bin/GetTreeVariables.py
pythonScript=/uscms/home/$USER/bin/GetTreeVariables.py

# options that require an argument should have a colon after the letter
while getopts n:d:o:t:r: option
do
    case "${option}"
    in
    n) treeName=${OPTARG};;
    d) dirName=${OPTARG};;
    o) outputName=${OPTARG};;
    t) textFile=${OPTARG};;
    r) rootFile=${OPTARG};;
    esac
done

# check for tree name
if [[ -z "$treeName" ]]
then
    echo "Please provide a tree name using the -n option."
    exit 1
fi

# check for output file
if [[ -z "$outputName" ]]
then
    echo "Please provide a output file name (without .txt extension) using the -o option."
    exit 1
fi

# check for text or root file
if [[ -z "$textFile" && -z "$rootFile" ]]
then
    echo "Please provide either text file using the -t option or a root file using the -r option."
    exit 1
fi

# you cannot give both a text and a root file
if [[ !(-z "$textFile") && !(-z "$rootFile") ]]
then
    echo "Please do not provide a text file and a root file; you must use one or the other."
    exit 1
fi

if [[ !(-z "$textFile") ]]
then
    rootFile=$(head $textFile -n 1)
fi

outputFile="$outputName"_variables.txt
outputFileClean="$outputName"_variables_clean.txt

# output tree variables to file
# -i : input root file
# -t : tree in root file that you wish to print 
# -d : directory in root file (optional) 
if [[ -z "$dirName" ]]
then
    python $pythonScript -i $rootFile -t $treeName > $outputFile
else
    python $pythonScript -i $rootFile -t $treeName -d $dirName > $outputFile
fi

# cleaned output (one line per variable) to file
grep Br $outputFile > $outputFileClean

# number of variables (branches) in the tree
numVars=$(wc -l $outputFileClean | cut -f1 -d " ")
echo "There are $numVars variables in the tree." 
echo "Please see $outputFileClean for a complete list."

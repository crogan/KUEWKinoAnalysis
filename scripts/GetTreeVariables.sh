#!/bin/bash
# GetTreeVariables.sh
# Caleb J. Smith
# February 7, 2019

# Arguments:
# - name of tree
# - output text file to save variables from tree
# - input either a text file (listing root files) or root file
treeName=
outputName=
textFile=
rootFile=

# path to python script
pythonScript=scripts/GetTreeVariables.py

# all options require values, hence we have a colon after each
while getopts n:o:t:r: option
do
    case "${option}"
    in
    n) treeName=${OPTARG};;
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
python $pythonScript -i $rootFile -t $treeName > $outputFile

# cleaned output (one line per variable) to file
grep Br $outputFile > $outputFileClean

# number of variables (branches) in the tree
numVars=$(wc -l $outputFileClean | cut -f1 -d " ")
echo "There are $numVars variables in the tree." 
echo "Please see $outputFileClean for a complete list."

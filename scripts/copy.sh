#!/bin/bash
# copy.sh
# Caleb J. Smith
# July 15, 2022

# start time
start=$(date +%s)

outputDir=
textFile=
inputURL="root://cmsxrootd.fnal.gov/"
outputURL="root://cmseos.fnal.gov/"

#echo $inputURL
#echo $outputURL

# all options require values, hence we have a colon after each
while getopts d:t: option
do
    case "${option}" in
        d) outputDir=${OPTARG};;
        t) textFile=${OPTARG};;
    esac
done

# check for output dir
if [[ -z "$outputDir" ]]
then
    echo "Please provide an output directory using the -d option."
    exit 1
fi

# check for text file
if [[ -z "$textFile" ]]
then
    echo "Please provide a text file that lists ROOT files to copy using the -t option."
    exit 1
fi

# check that text file exists
if [[ !(-f "$textFile") ]]
then
    echo "The text file '$textFile' does not exist! Quitting."
    exit 1
fi

echo "------------------------------------------------"
echo "Copying files listed in $textFile to $outputDir."
echo "------------------------------------------------"

lines=$(cat $textFile)
for line in $lines
do
    #echo ${line}
    path1="${inputURL}${line}"
    path2="${outputURL}${outputDir}"
    full_command="xrdcp ${path1} ${path2}"
    echo ${full_command}
    ${full_command}
done

# end time
end=$(date +%s)

# run time
run_time=$((end-start))

echo "---------------------------"
echo "start time: $start"
echo "end time: $end"
echo "run time: $run_time seconds"
echo "---------------------------"


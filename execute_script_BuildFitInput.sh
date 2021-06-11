#!/bin/bash
wget --no-check-certificate http://stash.osgconnect.net/+zflowers/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2

tar -xzf ./config_BuildFitInput.tgz

export SCRAM_ARCH=slc7_amd64_gcc700
source /cvmfs/cms.cern.ch/cmsset_default.sh

source ./config_BuildFitInput/cmssw_setup_connect.sh

cmssw_setup sandbox-CMSSW_10_6_5-6403d6f.tar.bz2

source ./config_BuildFitInput/setup_RestFrames_connect.sh 

./config_BuildFitInput/BuildFitInput.x "$@"

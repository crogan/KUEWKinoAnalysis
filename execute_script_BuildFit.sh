#!/bin/bash
wget --quiet --no-check-certificate http://stash.osgconnect.net/+zflowers/sandbox-CMSSW_10_6_5-6403d6f.tar.bz2

tar -xzf ./config_BuildFit.tgz

mv config_BuildFit/* ./

export SCRAM_ARCH=slc7_amd64_gcc700
source /cvmfs/cms.cern.ch/cmsset_default.sh

source cmssw_setup_connect.sh

cmssw_setup sandbox-CMSSW_10_6_5-6403d6f.tar.bz2

source setup_RestFrames_connect.sh 

./config_BuildFit/BuildFit.x "$@"

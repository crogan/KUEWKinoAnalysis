# KUEWKinoAnalysis
**KUEWKinoAnalysis**

---------------------
CMSSW-dependent build 
---------------------

In order for the KUEWKinoAnalysis package to build the `BuildFit.x`
executable, the user must make sure that **CMSSW** is available, and
include the **KEWKinoAnalysis** package in the correct location in the
**CMSSW** directory structure. You will also need the **CombineHarvester**
and **HiggsAnalysis** **CMSSW** packages. These packages must be included
in the **CMSSW** directory structure as:

- CMSSW_X_Y_Z
  - src
  - CombineHarvester
  - HiggsAnalysis
  - KUEWKinoAnalysis


Installation instructions can be found in the INSTALL file provided with the 
RestFrames distribution.

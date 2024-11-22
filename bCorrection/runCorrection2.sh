


cd ../macros
PROC=Wjets
Y=2017
ud=0
FS=0
flav=0
root -l "bCorrection.C(\"${PROC}\",${Y},${flav},${ud},${FS})"
        


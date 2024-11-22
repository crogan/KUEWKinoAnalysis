


cd ../macros
#Y=2016
Y=2017
#Y=2018
ud=0 #upddown -> 0=nominal
FS=0

#Array=("DB" "ST" "TB" "ZDY" "QCD" "Wjets")
Array=("DB" "ST" "TB" "ZDY" "QCD" "Wjets_Pt-100To250" "Wjets_Pt-250To400" "Wjets_Pt-400To600" "Wjets_Pt-50To100" "Wjets_Pt-600ToInf")
#Array=("Wjets_Pt-400To600")
#Array=("Wjets")
for i in ${Array[@]};
do
	flav=1
	root -l -b -q "bCorrection.C(\"${i}\",${Y},${flav},${ud},${FS})"
	flav=2
	root -l -b -q "bCorrection.C(\"${i}\",${Y},${flav},${ud},${FS})"
	flav=0
	root -l -b -q "bCorrection.C(\"${i}\",${Y},${flav},${ud},${FS})"
        
done



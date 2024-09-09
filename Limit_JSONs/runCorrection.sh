



#SIG='T2tt'
#SIG='T2cc'
#SIG='T2bW'
#SIG='TChiWZ'
#SIG='TChipmWW'
#SIG='TSlepSlep'

sigArray=("T2tt" "T2cc" "T2bW" "TChiWZ" "TChipmWW")
for i in ${sigArray[@]};
do
        echo $i
	IN=./original/B135_bugfix16_${i}_limit.json
	OUT=./xsec_corrected/B135_bugfix16_${i}_limit_xsec.json
	python3 xsecCorrection.py ${i} ${IN} ${OUT} > ${i}_corr.txt
	echo " "
done

#in the case slep snu
SIG='TChipmSlepSnu'
SNAME='TSlepSnu'
IN=./original/B135_bugfix16_${SNAME}_limit.json
OUT=./xsec_corrected/B135_bugfix16_${SNAME}_limit_xsec.json
echo ${SIG}
python3 xsecCorrection.py ${SIG} ${IN} ${OUT} > ${SNAME}_corr.txt

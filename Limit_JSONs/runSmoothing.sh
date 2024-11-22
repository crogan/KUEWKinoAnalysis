


#SIG='T2tt'
#SIG='T2cc'
#SIG='T2bW'
#SIG='TChiWZ'
#SIG='TChipmWW'
#SIG='TSlepSlep'
#SIG='TChiWZSuper'
#SIG='TChipmWWSuper'
#SIG='T2ttSuper'
#sigArray=("T2tt" "T2cc" "T2bW" "TChiWZ" "TChipmWW" "TSlepSnu" "TChiWZSuper" "TChipmWWSuper" "T2ttSuper")
#sigArray=("T2ttSuper" "T2bW")
#sigArray=("TChiWZSuper")
sigArray=("T2bW")
for i in ${sigArray[@]};
do
        echo $i
        IN=./xsec_corrected/B135_bugfix16_${i}_limit_xsec.json
        OUT=./smoothed/B135_bugfix16_${i}_limit_xsec_smooth.json
        python3 smoothJSON.py ${IN} ${OUT} > ${i}_smooth.txt
        echo " "
done


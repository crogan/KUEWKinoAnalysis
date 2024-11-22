


#CatNames.list


#./cat.x batchAll.list Ch1L_elpm_bron_0j0svS_ge1jISR_PTISR0

f = open('CatNames.list','r')

o = open('launcher.sh','w')

for x in f:
    print(x)
    cat= x.rstrip()
    o.write("./cat.x batch3_half.list "+cat+"\n")

o.close()
f.close()
    




import pandas as pd
import numpy as np

import glob
import math
import sys

import csv


#csvdir1 = "/content/drive/My_Drive/Colab/cmsanal/BFI_t2tt_Limits_binsV1_run2_total/"
#csvdir2 = "/content/drive/My_Drive/Colab/cmsanal/BFI_t2tt_Limits_binsV1_run2_total_S/"

csvdir1 = "BFI_t2tt_Limits_binsV1_run2_total/"
csvdir2 = "BFI_t2tt_Limits_binsV1_run2_total_S/"
#csvdir1 = sys.argv[1]
#csvdir2 = sys.argv[2]
all_bg_files = glob.glob(csvdir1+"/BG_*L/*.csv")
all_s_files = glob.glob(csvdir2+"/T2tt_*L/*.csv")

signal_set = ["T2tt_5000490"]

#pd.set_option('display.max_colwidth',None)
#pd.set_option('display.max_columns', None)
#pd.set_option('expand_frame_repr', False)
#pd.set_option('display.max_rows', None)


li = []
li2 = []
info = []

for filename in all_bg_files:
    df = pd.read_csv(filename, index_col=None, header=0, delimiter=" ")
    li.append(df)

allbgframe = pd.concat(li, axis=0, ignore_index=True)
allbgframe = allbgframe.loc[ ((allbgframe['PType'] == 'all-bkg')) ]
allbgframe = allbgframe[ ['RegionName','BinN','NL','NJ','Yield','Error'] ]
allbgframe['Rel Err'] = allbgframe.Error/allbgframe.Yield
allbgframe['Yield'] = allbgframe['Yield'].round(4)
allbgframe['idx']=allbgframe['RegionName']+allbgframe['BinN'].astype(str)

print(allbgframe)

for filename in all_s_files:
    df = pd.read_csv(filename, index_col=None, header=0, delimiter=" ")
    li2.append(df)

allsigframe = pd.concat(li2, axis=0, ignore_index=True)
allsigframe = allsigframe.loc[ ((allsigframe['PType'] == 'T2tt_5000490')) ]
allsigframe = allsigframe[ ['RegionName','BinN','NL','NJ','Yield','Error'] ]
allsigframe['Rel Err'] = allsigframe.Error/allsigframe.Yield
allsigframe['Yield'] = allsigframe['Yield'].round(4)
allsigframe['idx']=allsigframe['RegionName']+allsigframe['BinN'].astype(str)

dictdf=allbgframe[['idx','Yield']]
dictdf = dictdf.set_index('idx').to_dict()['Yield']

allsigframe['backyield']= allsigframe['idx'].map(dictdf).apply(lambda x: x)#x[0] if type(x)==list else x
allsigframe['s over b'] = allsigframe.Yield/allsigframe.backyield**0.5
#allsigframe['s over b'] = allsigframe.Error/allsigframe.Yield
#allsigframe['s over b'] = allsigframe['s over b'].round(4)

#[allsigframe[:0].argsort()]

print("info sorted")
isortallsig = allsigframe.sort_values('s over b',ascending=False)
print(isortallsig)

print("Full bkgd shape")
print(allbgframe.shape)

print("Full sig shape")
#print(allsigframe)
print(allsigframe.shape)

#print("allsigframe")
#print(allsigframe)
#print(allsigframe[0:])

#print(allbgframe)
#allbgframe = allbgframe.reset_index()
print("Yield < 0.01")
subframe = allbgframe.loc[(allbgframe['Yield']<0.01)]
subframe['Yield'] = subframe['Yield'].round(5)
print(subframe.shape)
#print(subframe)




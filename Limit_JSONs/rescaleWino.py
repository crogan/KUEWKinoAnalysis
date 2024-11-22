import json

# Opening JSON file
#f = open('B135_bugfix16_TChiWZ_limit_smooth.json')
#f = open('./smoothed/B135_bugfix16_TChiWZ_limit_xsec_smooth.json')
f = open('./smoothed/B135_bugfix16_TChiWZSuper_limit_xsec_smooth.json')

SF=1./0.2352
print('SF',SF)
# returns JSON object as 
# a dictionary
data = json.load(f)
f.close()

keys = ['exp+1','exp+2','exp-1','exp-2','exp0','obs']

#print(data)
for gpt in data:
	#print(gpt)
	for key in keys:
		#print(key, data[gpt][key])
		data[gpt][key] = SF* data[gpt][key]
		#print(key, data[gpt][key])
	#break


#with open('./smoothed/B135_bugfix16_HinoN2C1_limit_xsec_smooth.json', 'w') as f:
with open('./smoothed/B135_bugfix16_HinoN2C1Super_limit_xsec_smooth.json','w') as f:
	 json.dump(data, f, ensure_ascii=False, indent=4)

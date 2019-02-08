import os

users = [ 'zflowers/Ewkinos', 'malazaro/Ewkinos', 'crogan/EWKino', 'eschmitz/Ewkinos', 'jaking/Ewkinos', 'aabreuna/ewkino_samples' ]

for user in users:
    base_dir = '/mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/'+user

    for dirs in os.walk(base_dir):
        if '000' in dirs[0]:
            if 'failed' in dirs[0]: continue
            if '0' == dirs[0][-2] and '0' == dirs[0][-3] and '0' == dirs[0][-4]:
                print dirs[0]

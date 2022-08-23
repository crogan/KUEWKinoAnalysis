# compareLimits.py

import os
import json

# load data from json file
def readJson(json_file):
    data = {}
    # check that json file exists
    if os.path.isfile(json_file):
        # load json file
        with open(json_file, "r") as input_file:
            data = json.load(input_file)
    else:
        # json file does not exist
        print("ERROR: the json file {0} does not exist.".format(json_file))
    return data

# check that dictionaries have the same keys 
def checkKeys(data_1, data_2):
    keys_1 = list(data_1.keys())
    keys_2 = list(data_2.keys())
    result = (keys_1 == keys_2)
    return result

def compare(data_1, data_2):
    # check that dictionaries have the same keys 
    keysMatch = checkKeys(data_1, data_2)
    if keysMatch:
        # TODO: for keys, convert strings to numbers...
        keys = list(data_1.keys())
        keys.sort()
        for key in keys:
            exp0_1  = data_1[key]["exp0"]
            exp0_2  = data_2[key]["exp0"]
            diff    = exp0_2 - exp0_1
            ratio   = exp0_2 / exp0_1
            print("{0}: {1}, {2}, {3}, {4}".format(key, exp0_1, exp0_2, diff, ratio))
    else:
        print("ERROR: dictionaries do not have the same keys.")

def compareLimits():
    json_file_1 = "limits_TChiWZ_2016_binsV1.json"
    json_file_2 = "limits_TChiWZ_2016_binsV2.json"
    data_1      = readJson(json_file_1)
    data_2      = readJson(json_file_2)
    compare(data_1, data_2)

def main():
    compareLimits()

if __name__ == "__main__":
    main()


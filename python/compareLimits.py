# compareLimits.py

import os
import json
import csv

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

def compare(data_1, data_2, output_csv_name):
    # check that dictionaries have the same keys 
    keysMatch = checkKeys(data_1, data_2)
    output_column_titles = ["key", "r_1", "r_2", "r_2 - r_1", "r_2 / r_1", "(r_2 - r_1) / r_1"]
    if keysMatch:
        # output to csv file
        with open(output_csv_name, 'w') as output_csv:
            output_writer = csv.writer(output_csv)
            # write to csv file
            output_writer.writerow(output_column_titles)
            # TODO: for keys, convert strings to numbers...
            keys = list(data_1.keys())
            keys.sort()
            for key in keys:
                r_1         = data_1[key]["exp0"]
                r_2         = data_2[key]["exp0"]
                diff        = r_2 - r_1
                ratio       = r_2 / r_1
                perc_diff   = (r_2 - r_1) / r_1
                #print("{0}: {1}, {2}, {3}, {4}, {5}".format(key, r_1, r_2, diff, ratio, perc_diff))
                output_row = [key, r_1, r_2, diff, ratio, perc_diff]
                # write to csv file
                output_writer.writerow(output_row)
    else:
        print("ERROR: dictionaries do not have the same keys.")

def compareLimits():
    output_csv_name = "TChiWZ_2016_binsV1andV2.csv"
    json_file_1 = "limits_TChiWZ_2016_binsV1.json"
    json_file_2 = "limits_TChiWZ_2016_binsV2.json"
    data_1      = readJson(json_file_1)
    data_2      = readJson(json_file_2)
    compare(data_1, data_2, output_csv_name)
    output_csv_name = "T2tt_2016_binsV1andV2.csv"
    json_file_1 = "limits_T2tt_2016_binsV1.json"
    json_file_2 = "limits_T2tt_2016_binsV2.json"
    data_1      = readJson(json_file_1)
    data_2      = readJson(json_file_2)
    compare(data_1, data_2, output_csv_name)

def main():
    compareLimits()

if __name__ == "__main__":
    main()


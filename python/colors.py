# colors.py

import os
import json
import ROOT

# xkcd color survey
# see https://xkcd.com/color/rgb.txt
# see https://xkcd.com/color/rgb/
# download text file using "wget https://xkcd.com/color/rgb.txt"
# generate json file from the text file by running main()

def printError(file_name):
    print("ERROR: The required file \"{0}\" does not exist.".format(file_name))
    print(" - First, download rgb.txt using \"wget https://xkcd.com/color/rgb.txt\".")
    print(" - Then, run the script \"python python/colors.py\" to produce \"rbg.json\".")

# print color, RGB, and index
def printColors():
    json_file = "rgb.json"
    # check that json file exists
    if not os.path.isfile(json_file):
        printError(json_file)
    else:
        with open(json_file, "r") as input_file:
            data = json.load(input_file)
            for color in data:
                index = ROOT.TColor.GetColor(data[color])
                print("{0}: {1}: {2}".format(color, data[color], index))

# return TColor index given xkcd color using RGB json file 
def getColorIndex(color):
    index = -1
    json_file = "rgb.json"
    # check that json file exists
    if not os.path.isfile(json_file):
        printError(json_file)
    else:
        with open(json_file, "r") as input_file:
            data = json.load(input_file)
            # if color in map, get RGB from map
            if color in data:
                index = ROOT.TColor.GetColor(data[color])
                #print("{0}: {1}: {2}".format(color, data[color], index)) 
            # otherwise, assume RBG are provided
            else:
                print("WARNING: The color '{0}' is not found in the map loaded from {1}. Assuming '{0}' is an RGB color and passing to ROOT.TColor.GetColor(color).".format(color, json_file))
                index = ROOT.TColor.GetColor(color)
    return index

# make RGB json file from xkcd colors text file
def main():
    color_map   = {}
    input_name  = "rgb.txt"
    output_name = "rgb.json"
    
    # check that input file exists
    if not os.path.isfile(input_name):
        printError(input_name)
        return
    
    with open(input_name, "r") as input_file:
        for line in input_file:
            line = line.strip()
            if "#" in line and line[0] != "#":
                #print(line)
                split = line.split("#")
                key = split[0].strip()
                value = "#" + split[1].strip()
                color_map[key] = value
    
    with open(output_name, "w") as output_file:
        json.dump(color_map, output_file, indent=4, sort_keys=True)
    
    print("Created {0}".format(output_name))

if __name__ == "__main__":
    # run main() to create json file
    main()
    
    # example of getting color index
    #color = "tomato"
    #index = getColorIndex(color)
    #print("color: {0}, index: {1}".format(color, index))
    
    # print colors
    #printColors()


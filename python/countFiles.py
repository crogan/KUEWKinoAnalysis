# countFiles.py

# Count the number of ROOT files in subdirectories.
# Print the results and save results to a csv file.
#
# Example syntax:
# python python/countFiles.py --directory <path_to_directory> --csv <path_to_csv>
#

import os
import glob
import argparse
import tools

# process directory containing subdirectories
def processDir(directory, csv):
    pattern = "*.root"
    subdirectories = tools.get_subdirectories(directory)
    print("subdirectories: {0}".format(subdirectories))
    for subdirectory in subdirectories:
        path = "{0}/{1}".format(directory, subdirectory)
        n_files = tools.count_files_with_pattern(path, pattern)
        print("{0}: {1}".format(subdirectory, n_files))

def run():
    # options
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--directory",  "-d", default="", help="directory containing subdirectories that contain ROOT files (required)")
    parser.add_argument("--csv",        "-c", default="", help="output csv file name (optional)")

    options     = parser.parse_args()
    directory   = options.directory
    csv         = options.csv

    # check that directory is set
    if not directory:
        print("ERROR: 'directory' is not set. Please provide a directory using the -d option.")
        return
    
    processDir(directory, csv)

def main():
    run()

if __name__ == "__main__":
    main()


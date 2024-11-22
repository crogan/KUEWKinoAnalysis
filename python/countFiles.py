# countFiles.py

# Count the number of ROOT files in subdirectories.
# Print the results and save results to a csv file.
#
# Example syntax:
# python python/countFiles.py --directory <path_to_directory> --csv <path_to_csv>
#

import argparse
import tools

# process directory containing subdirectories
def processDir(directory, pattern, csv):
    total_files = 0
    output_data = []
    subdirectories = tools.get_subdirectories(directory)
    # sort subdirectories alphabetically
    subdirectories.sort()
    #print("subdirectories: {0}".format(subdirectories))
    
    # headers for csv
    output_data.append(["sample", "n_files"])

    print("Sample: number of files")
    for subdirectory in subdirectories:
        path = "{0}/{1}".format(directory, subdirectory)
        # get the number of files in the subdirectory
        n_files = tools.count_files_with_pattern(path, pattern)
        total_files += n_files
        output_data.append([subdirectory, n_files])
        print("{0}: {1}".format(subdirectory, n_files))
    
    print("Total number of files: {0}".format(total_files))

    # if csv file name is set, then save data to csv file
    if csv:
        tools.writeCSV(csv, output_data)

def run():
    # options
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--directory",  "-d", default="",       help="directory containing subdirectories that contain ROOT files (required)")
    parser.add_argument("--pattern",    "-p", default="*.root", help="pattern for root file names (optional)")
    parser.add_argument("--csv",        "-c", default="",       help="output csv file name (optional)")

    options     = parser.parse_args()
    directory   = options.directory
    pattern     = options.pattern
    csv         = options.csv

    # check that directory is set
    if not directory:
        print("ERROR: 'directory' is not set. Please provide a directory using the -d option.")
        return
    
    processDir(directory, pattern, csv)

def main():
    run()

if __name__ == "__main__":
    main()


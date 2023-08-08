# removeFiles.py

import os
import time
import tools
import argparse

# TODO

# DONE
# - List all files in a directory.
# - Extract number from file name.
# - Add min/max number range as arguments for files to delete.
# - Print files in min/max range and count files.
# - Add flag to remove files (run deletion).
# - Write eosrm command.
# - Use eosrm command.
# - Update get_eos_file_list() to use a pattern

# Get file number from the file name.
def getFileNumber(file_name):
    # Remove file extension from the file name.
    file_name_no_ext = os.path.splitext(file_name)[0]
    
    # Get numbers from file name without the file extension.
    delimiter = '_'
    numbers = tools.getNumbers(file_name_no_ext, delimiter)
    n_numbers = len(numbers)
    
    # Check for exactly one number.
    if n_numbers == 0:
        print("ERROR: No numbers found in the file name '{0}' using the delimiter '{1}'.".format(file_name, delimiter))
        return -999
    elif n_numbers > 1:
        print("ERROR: Multiple numbers ({0} numbers) found in the file name '{1}' using the delimiter '{2}'.".format(n_numbers, file_name, delimiter))
        return -999
    else:
        return numbers[0]

def removeEOSFiles():
    # options
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--min_number", "-a", default=-1,   help="minimum file number to delete (inclusive)")
    parser.add_argument("--max_number", "-b", default=-1,   help="maximum file number to delete (inclusive)")
    parser.add_argument("--directory",  "-d", default="",   help="directory containing root files")
    parser.add_argument("--pattern",    "-p", default="",   help="pattern for root file names")
    parser.add_argument("--remove",     "-r", default=False, action="store_true", help="remove files; flag to run file deletion")

    options     = parser.parse_args()
    min_number  = int(options.min_number)
    max_number  = int(options.max_number)
    directory   = options.directory
    pattern     = options.pattern
    remove      = options.remove

    debug = False

    # EOS URL
    eos_url = "root://cmseos.fnal.gov"
    
    # EOS URL tag: include backslash at the end!
    eos_url_tag = eos_url
    if eos_url_tag[-1] != "/":
        eos_url_tag += "/"

    if not directory:
        print("ERROR: 'directory' is not set. Please provide a directory using the -d option.")
        return
    
    if not pattern:
        print("ERROR: 'pattern' is not set. Please provide a pattern using the -p option.")
        return

    # require min_number >= 0 and max_number >= 0
    if min_number < 0 or max_number < 0:
        print("ERROR: Please provide min/max file numbers >= 0 to delete (inclusive) using the -a (min) and -b (max) options.")
        return

    # require min_number <= max_number
    if min_number > max_number:
        print("ERROR: min_number ({0}) is greater than max_number ({1}); please change so that min_number is less than or equal to max_number.".format(min_number, max_number))
        return
    
    # print parameters
    print("----------------------------")
    print("remove: {0}".format(remove))
    print("directory: {0}".format(directory))
    print("pattern: {0}".format(pattern))
    print("min_number: {0}".format(min_number))
    print("max_number: {0}".format(max_number))
    print("----------------------------")
    
    files_matching = tools.get_eos_file_list(directory, pattern)
    files_matching_in_range = []
    
    # get matching files in range
    print("matching files in range [{0}, {1}]:".format(min_number, max_number))
    for f in files_matching:
        file_name = os.path.basename(f)
        file_number = getFileNumber(file_name)
        if debug:
            print("Checking file '{0}': file_number = {1}".format(file_name, file_number))
        if tools.numberInRange(file_number, min_number, max_number):
            files_matching_in_range.append(f)
            print(" - {0}; file number: {1}".format(file_name, file_number))
    
    n_files_matching            = len(files_matching)
    n_files_matching_in_range   = len(files_matching_in_range)
    
    print("----------------------------")
    print("Number of files containing the pattern '{0}': {1}".format(pattern, n_files_matching))
    print("Number of files containing the pattern '{0}' and in the number range [{1}, {2}]: {3}".format(pattern, min_number, max_number, n_files_matching_in_range))
    print("----------------------------")

    # remove files
    if remove:
        for f in files_matching_in_range:
            # remove EOS URL tag
            f_to_rm = f.replace(eos_url_tag, "")
            
            #print("f: {0}".format(f))
            #print("f_to_rm: {0}".format(f_to_rm))
            
            tools.eosrm(f_to_rm)
        
        print("These files were removed!")
        print("Have a great day!")
    else:
        print("These files were not removed.")
        print("Use the -r flag to remove the files matching the pattern in the specified number range.")
    
    print("----------------------------")

def main():
    t_start = time.time()
    removeEOSFiles()
    t_stop  = time.time()
    t_run   = t_stop - t_start
    print("run time (sec): {0:.3f}".format(t_run))

if __name__ == "__main__":
    main()


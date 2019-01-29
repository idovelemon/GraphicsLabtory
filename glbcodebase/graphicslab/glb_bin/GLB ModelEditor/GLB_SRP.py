"""
    GraphicsLaboratory(GLB) shader resource header file preprocessor - GLB_SRP
"""

import os
import sys

SourceShaderResourceFileName = "glb.gsh"
DestinationShaderResourceFileName = "test.gsr"

def srp_core(source):
    # Check file exsist?
    if os.path.isfile(source) is False:
        print("Invalid file:" + source)
        return []

    # Get directory path name
    source_directory_path_name = os.path.dirname(source)
    if len(source_directory_path_name) > 0:
        source_directory_path_name = source_directory_path_name + '/'
    
    # Read file line by line
    source_file = open(source, "r", encoding='UTF-8')
    source_string = []

    while True:
        line = source_file.readline()
        if line is '':
            break
        if line.find("#include") is not -1:
            left_quot_index = line.find('\"')
            right_quot_index = line.rfind('\"')
            new_source_file = line[left_quot_index + 1:right_quot_index]
            new_source_string = srp_core(source_directory_path_name + new_source_file)
            if len(new_source_string) is 0:
                return []
            source_string = source_string + ["\n",] + new_source_string + ["\n",]
        else:
            source_string.append(line)
    source_file.close()

    return source_string

def srp(source, dest):
    source_string = srp_core(source)
    if len(source_string) is 0:
        print("Error:No source shader string!")
        return
    
    dest_file = open(dest, "w")
    for str in source_string:
        print(str)
        dest_file.write(str)
    dest_file.close()

# if __name__ == "__main__":
#     SourceShaderResourceFileName = input("SourceFileName:")
#     DestinationShaderResourceFileName = input("DestFileName:")
#     srp(SourceShaderResourceFileName, DestinationShaderResourceFileName)
#     print("=====finish======")
#     pass

if __name__ == "__main__":
    SourceShaderResourceFileName = sys.argv[1]
    DestinationShaderResourceFileName = sys.argv[2]
    srp(SourceShaderResourceFileName, DestinationShaderResourceFileName)
    print("=====finish======")
    pass
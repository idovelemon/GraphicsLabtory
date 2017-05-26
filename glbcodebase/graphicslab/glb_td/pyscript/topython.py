#/usr/env/bin/python32

# Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
# Author: i_dovelemon[1322600812@qq.com]
# Date: 2016 / 07 / 09
# Brief: Convert the normal c function to specific python api call function

"""
    Convert the c function to python function
"""
import sys
import re

TOKEN_KEYWORD = 0
TOKEN_IDENTITY = 1
TOKEN_LEFT_BRACKET = 2
TOKEN_RIGHT_BRACKET = 3
TOKEN_DELIMITER = 4

DELIMITERS = [' ', '(', ')', ';', ',']

KEYWORDS = ['float','int', 'void', "const", "char*"]

class CParameterDesc:
    def __init__(self):
        self.param_type = "void"
        self.param_identity = ""

class CFunctionDesc:
    def __init__(self):
        self.return_type = "void"
        self.function_name = ""
        self.args = []

def preprocess(c_header_file):
    file_buf = []
    while True:
        buf = c_header_file.readline()
        if buf == '':
            break
        elif buf[0] == '/' and buf[1] == '/':
            continue
        elif buf[0] == '#':
            continue
        elif buf[0] == '\n':
            continue
        else:
            for c in buf:
                file_buf.append(c)
        
    return file_buf   

def parse_tokentype(lex_word):
    if lex_word in KEYWORDS:
        return TOKEN_KEYWORD
    elif lex_word == '(':
        return TOKEN_LEFT_BRACKET
    elif lex_word == ')':
        return TOKEN_RIGHT_BRACKET
    elif lex_word == ';':
        return TOKEN_DELIMITER
    else:
        return TOKEN_IDENTITY
    
def lex(buf):
    segments = []
    segment = []
    lex_word = ""
    for c in buf:
        if c in DELIMITERS:
            if lex_word != '':  
                lex_item = {"type":parse_tokentype(lex_word), "value":lex_word}
                segment.append(lex_item)
                lex_word = ""
            if c == ';':
                segments.append(segment)
                segment = []
            elif c == '(':
                segment.append({"type":TOKEN_LEFT_BRACKET, "value":'('})
            elif c == ')':
                segment.append({"type":TOKEN_RIGHT_BRACKET, "value":')'})
        elif c == '\n':
             continue
        else:
             lex_word += c
     
    return segments
    
def parse(segments):
    c_function_descs = []
    
    for segment in segments:
        c_function_desc = CFunctionDesc()
        
        # Return type
        lex_item = segment[0]
        if lex_item["type"] != TOKEN_KEYWORD:
            assert("Invalid segment")
            continue
        else:
            c_function_desc.return_type = lex_item["value"]
        
        # Function name second
        lex_item = segment[1]
        if lex_item["type"] != TOKEN_IDENTITY:
            assert("Invalid segment")
            continue
        else:
            c_function_desc.function_name = lex_item["value"]
        
        # Left bracket
        lex_item = segment[2]
        if lex_item["type"] != TOKEN_LEFT_BRACKET:
            assert("Invaild segment")
            continue   
        
        # Parameters
        #for i in range(3, len(segment) - 2):
        i = 3
        while i < len(segment) - 2:
            const_prefix = ""
            if segment[i]["type"] == TOKEN_KEYWORD and segment[i]["value"] == "const":
                const_prefix = "const "
                i = i + 1
            if segment[i]["type"] != TOKEN_KEYWORD or segment[i + 1]["type"] != TOKEN_IDENTITY:
                assert("Invalid segment")
                i = i + 1
                continue
            else:
                param = CParameterDesc()
                param.param_type = const_prefix + segment[i]["value"]
                param.param_identity = segment[i + 1]["value"]
                c_function_desc.args.append(param)
            i = i + 1
                       
        # Right bracket
        lex_item = segment[len(segment) - 1]
        if lex_item["type"] != TOKEN_RIGHT_BRACKET:
            assert("Invalid segment")
            continue
        
        # Save the function desc
        c_function_descs.append(c_function_desc)
    
    return c_function_descs

def write_python_function(c_function_desc, python_header_file):
    header = "static PyObject*" + " "
    header += c_function_desc.function_name + "(PyObject* self, PyObject* args) {\n"
    for param in c_function_desc.args:
        header += "\t" + param.param_type + " " + param.param_identity + ";\n"
    if c_function_desc.return_type != "void":
        header += "\t" + c_function_desc.return_type + " ret;\n"
    if len(c_function_desc.args) > 0:
        header += '\tif (!PyArg_ParseTuple(args, "'
        for param in c_function_desc.args:
            if param.param_type == "float":
                header += 'f'
            elif param.param_type == "int":
                header += 'i'
            elif param.param_type == "const char*":
                header += 's'
            else:
                # Not implementation yet
                pass
        header += '", '
        
        param_index = 0
        for param in c_function_desc.args:
            header += "&" + param.param_identity
            if param_index < len(c_function_desc.args) - 1:
                header += ", "
                param_index += 1
        header += ")) {\n"
        header += "\t\t return NULL;\n\t}\n\t"
        if c_function_desc.return_type != "void":
            header += "ret = " + c_function_desc.function_name + "("
        else:
            header += c_function_desc.function_name + "("
            
        param_index = 0
        for param in c_function_desc.args:
            header +=  param.param_identity
            if param_index < len(c_function_desc.args) - 1:
                header += ", "
                param_index += 1       
         
        header += ");\n"
    else:
        if c_function_desc.return_type != "void":
            header += "\tret = " + c_function_desc.function_name + "("
        else:
            header += "\t" + c_function_desc.function_name + "("
            
        param_index = 0
        for param in c_function_desc.args:
            header +=  param.param_identity
            if param_index < len(c_function_desc.args) - 1:
                header += ", "
                param_index += 1
        header += ");\n"                   
    header += "\treturn Py_BuildValue(";
    if c_function_desc.return_type == "void":
         header += '"");'
    elif c_function_desc.return_type == "int":
         header += '"i", ret);'
    elif c_function_desc.return_type == "float":
         header += '"f", ret);'
         
    header += "\n}"
    python_header_file.write(header + "\n\n")

def write_python_def(c_function_descs, python_header_file):
    header = "static PyMethodDef s_HostAPI_MethodDef[] = {\n"
    
    for c_function_desc in c_function_descs:
        header += '\t{"' + c_function_desc.function_name + '", '
        header += c_function_desc.function_name + ', '
        header += "METH_VARARGS, NULL},\n"
    
    header += "\t{NULL, NULL, NULL, NULL}\n};"
    python_header_file.write(header)
    
def topythonfunction(c_function_descs, python_header_file):
    python_header_file.write('#include "topython_function.h"\n\n')
    
    # Write python function
    for c_function_desc in c_function_descs:
        write_python_function(c_function_desc, python_header_file)
        print(c_function_desc.function_name + " ok")
        
    # Write Method def
    write_python_def(c_function_descs, python_header_file)
    print("s_HostAPI_MethodDef ok")
    
def parseheader(c_header_buf):
    segments = lex(c_header_buf)
    return parse(segments)
    
def topython(c_header_file, python_header_file):
    c_header_buf = preprocess(c_header_file)     
    c_function_descs = parseheader(c_header_buf)   
    topythonfunction(c_function_descs, python_header_file)

def main():
    file = open("topython_function.h", "r", encoding = "utf-8")
    topython_inl_file = open("topython.inl", "w", encoding = "utf-8")
    if file is not None and topython_inl_file is not None:
        topython(file, topython_inl_file)
    file.close()
    topython_inl_file.close()

if __name__ == "__main__":
    main()
    print("Press any key to continue...")
    input()

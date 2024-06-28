import os
import sys
from subprocess import Popen, PIPE
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

"""
Finds the degeneracy for all edited graphs
"""
def solve_new_k(filename):
    print(filename)
    process = Popen(["../rounding/test-rounding",
                "-k",
                filename], stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    data = stdout.decode("utf-8").split()
    return data

"""
Finds the optimal edit set
"""
def solve_opt_edit(filename, r):
    print(filename)
    process = Popen(["../rounding/test-rounding",
                "-o",
                r,
                filename], stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    data = stdout.decode("utf-8").split()
    # print(data)
    return data

"""
Finds the kernel and makes ds approx
"""
def solve_DS_approx(filename, r):
    print(filename)
    process = Popen(["../rounding/test-rounding",
                "-d",
                r,
                filename], stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    data = stdout.decode("utf-8").split(';')
    # print(data)
    return data

"""
Temp b/c my kernelization alg is broken somewhere
Use the CPLEX solver to find the min DS
"""
def solve_DS_skip(filename, r):
    print(filename)
    process = Popen(["../rounding/test-rounding",
                "-p",
                r,
                filename], stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    data = stdout.decode("utf-8").split(',')
    return data

"""
Double check that kernel alg works
"""
def check_kernel(filename, exact, r):
    # print(filename)
    process = Popen(["../rounding/test-rounding",
                "-w",
                filename,
                exact,
                r], stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    data = stdout.decode("utf-8").split(' ')
    if len(data) > 1:
        print(filename)
        print(r)
    print(data)
    return data

def print_bejamin():
    init = 20
    b1 = 24
    b2 = 180
    h = 906
    # rule 5 to rule 4
    for i in range(b1+1):
        print(str(init+i) + " 5 6")
    # rule 4 to rule 3
    init = init+b1+1
    for i in range(b2):
        print(str(i+init) + " 7 17")
    print(str(b2+init) + " 7 17 1 2")
    init = init+b2+1
    for i in range(h-3):
        print(str(i+init) + " 8")
    print(str(h-3+init) + " 8 13")

def find_opt_edit(graphFolder, df, nodes):
    [rows, cols] = df.shape
    opt_edit = [0 for i in range(rows)]
    opt_edit_size = [0 for i in range(rows)]
    for root, dirs, files in os.walk(graphFolder):
        for f in files:
            if (f[0] != 'e'):
                tokens = f.split('-')
                if (int(tokens[0]) == nodes):
                    #for each r
                    for r in range(int(tokens[1])):
                    idx = df.index[(df['file'] == f)] &
                         (df['r'].astype(str) == str(r+1))]
                    data = solve_opt_edit(root + "/" + f,str(r+1))
                    for i in idx:
                        opt_edit[i] =  data[0:len(data)-1]
                        opt_edit_size[i] = data[len(data)-1]
    return opt_edit, opt_edit_size

def check_all_kernels(graphFolder, df, nodes):
    [rows, cols] = df.shape
    for root, dirs, files in os.walk(graphFolder):
        for f in files:
            if (f[0] != 'e'):
                tokens = f.split('-')
                if (int(tokens[0]) == nodes):
                    #for each r
                    for r in range(int(tokens[1])):
                    idx = df.index[(df['file'] == f)] &
                         (df['r'].astype(str) == str(r+1))]
                    data = solve_opt_edit(root + "/" + f,str(r+1))
                    for i in idx:
                        opt_edit[i] =  data[0:len(data)-1]
                        opt_edit_size[i] = data[len(data)-1]
    return opt_edit, opt_edit_size

"""
Finds the optimal edit set
"""
def main():
    graphFolder = "../experiments/large-graphs/"
    csv = "outputDSlarge-Feb-24.csv"
    df = pd.read_csv(csv)
    [rows, cols] = df.shape

    df.to_csv(csv)

if __name__ == '__main__':
    main()

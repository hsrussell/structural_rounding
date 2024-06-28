import os
import sys
from subprocess import Popen, PIPE
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

"""
Use the CPLEX solver to find the min VC
"""
def solve_IS(filename):
    print(filename)
    process = Popen(["python3",
                "/home/hsrussel/git/test-rounding/optimal/solver.py",
                filename], stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    data = stdout.decode("utf-8").split()
    return data

"""
Use the CPLEX solver to find the min DS
"""
def solve_DS(filename):
    print(filename)
    process = Popen(["python3",
                "/home/hsrussel/git/test-rounding/optimal/solver.py",
                "--formulation=DS",
                filename], stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    data = stdout.decode("utf-8").split()
    return data

def add_indices_to_csv(graphFolder, df):
    [rows, cols] = df.shape
    id = [0 for i in range(rows)]
    for root, dirs, files in os.walk(graphFolder):
        for f in files:
            if (f[0] != 'e'):
                idx = df.index[(df['file'] == f)]
                for i in idx:
                    tokens = f.split('-')
                    id[i] = int(tokens[2])
    return id

def find_all_IS(graphFolder, df):
    [rows, cols] = df.shape
    VC_partial = [0 for i in range(rows)]
    VC_full = [0 for i in range(rows)]
    VC_partial_size = [0 for i in range(rows)]
    VC_full_size = [0 for i in range(rows)]
    for root, dirs, files in os.walk(graphFolder):
        for f in files:
            # Solve IS
            data = solve_IS(root + "/" + f)
            # original graph
            if (f[0] != 'e'):
                idx = df.index[df.file == f]
                for i in idx:
                    VC_full_size[i] = int(data[8])
                    VC_full[i] = data[20:]
            # edited graph
            else :
                tokens = f.split('-')
                token = tokens[5].split('.')
                org = f[5:len(f)-4]
                file_tokens = org.split('-')
                file_token = '-'.join(file_tokens[:(len(file_tokens)-1)]);
                file_token += '.txt'
                idx = df.index[(df['file'] == file_token) &
                    (df['r'].astype(str) == token[0])]
                for i in idx:
                    VC_partial_size[i] = int(data[8])
                    VC_partial[i] = data[20:]
    return [VC_partial, VC_partial_size, VC_full, VC_full_size]

def find_all_DS(graphFolder, df):
    [rows, cols] = df.shape
    DS_partial = [0 for i in range(rows)]
    DS_full = [0 for i in range(rows)]
    DS_partial_size = [0 for i in range(rows)]
    DS_full_size = [0 for i in range(rows)]
    for root, dirs, files in os.walk(graphFolder):
        for f in files:
            # Solve DS
            data = solve_DS(root + "/" + f)
            # original graph
            if (f[0] != 'e'):
                idx = df.index[df.file == f]
                for i in idx:
                    DS_full_size[i] = int(data[8])
                    DS_full[i] = data[20:]
            # edited graph
            else :
                tokens = f.split('-')
                token = tokens[5].split('.')
                org = f[5:len(f)-4]
                file_tokens = org.split('-')
                file_token = '-'.join(file_tokens[:(len(file_tokens)-1)]);
                file_token += '.txt'
                idx = df.index[(df['file'] == file_token) &
                    (df['r'].astype(str) == token[0])]
                for i in idx:
                    DS_partial_size[i] = int(data[8])
                    DS_partial[i] = data[20:]
    return [DS_partial, DS_partial_size, DS_full, DS_full_size]

"""
Find the vertex cover all generated graphs and save output
"""
def main():
    graphFolder = "../experiments/large-graphs/"
    csv = "outputDSlarge-Feb-27.csv"
    df = pd.read_csv(csv)
    id = add_indices_to_csv(graphFolder, df)
    [DS_partial, DS_partial_size, DS_full, DS_full_size] = find_all_DS(graphFolder,df)
    df = df.assign(DS_partial=pd.Series(DS_partial).values)
    df = df.assign(DS_partial_size=pd.Series(DS_partial_size).values)
    df = df.assign(DS_full=pd.Series(DS_full).values)
    df = df.assign(DS_full_size=pd.Series(DS_full_size).values)

    df = df.assign(id=pd.Series(id).values)
    df.sort_values(by=["file"], inplace=True)
    df.to_csv(csv)

if __name__ == '__main__':
    main()

import networkx as nx
import os
import sys
from subprocess import Popen, PIPE
import time

"""
Finds the degeneracy for the graph
"""
def solve_new_k(filename):
    process = Popen(["../rounding/test-rounding",
                "-k",
                filename], stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    data = stdout.decode("utf-8").split()
    print("k: " + data[0])
    return int(data[0])

def find_core_sizes(filename):
    process = Popen(["../rounding/test-rounding",
                "-c",
                filename], stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    data = stdout.decode("utf-8").split("\n")
    print(data)
    return data

def run_ds_all(filename, r):
    process = Popen(["../rounding/test-rounding",
                "-p",
                r,
                filename], stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    data = stdout.decode("utf-8").split("\n")
    print(data)
    return data

def main():
    count = 100
    nn = [300]
    pp = [[0.01,0.015,0.02,0.025,0.03,0.035,0.04,0.044,0.05,0.054,0.059,0.064,0.067,0.072,0.075,0.08,0.084,0.089,0.092]]

    # nn = [200,400,600,800,1000]
    # # n = 200, 400, 600, 800, 1000
    # # k = 2,3,4
    # pp = [[0.01,  0.02,  0.03],
    #     [  0.007, 0.01,  0.015],
    #     [  0.005, 0.008, 0.01],
    #     [  0.003, 0.005, 0.007],
    #     [  0.003, 0.004, 0.006]]
    # nn = [200,400,600,800,1000]
    # # n = 200, 400, 600, 800, 1000
    # # k = 5,...,9
    # pp = [[0.038, 0.043, 0.052, 0.06,  0.066],
    #     [  0.02,  0.022, 0.026, 0.03,  0.034],
    #     [  0.012, 0.015, 0.017, 0.02,  0.022],
    #     [  0.009, 0.011, 0.013, 0.015, 0.017],
    #     [  0.008, 0.009, 0.011, 0.012, 0.013]]
    # # n = 200, 400, 600, 800, 1000
    # # k = 10,11,...,20
    # pp = [[0.073, 0.08,   0.085,  0.093, 0.1,   0.108, 0.115, 0.118, 0.125,  0.13,  0.14],
    #     [  0.037, 0.039,  0.043,  0.047, 0.05,  0.053, 0.056, 0.059, 0.063,  0.067, 0.069],
    #     [  0.025, 0.027,  0.029,  0.031, 0.033, 0.035, 0.038, 0.04,  0.042,  0.044, 0.046],
    #     [  0.019, 0.0205, 0.022,  0.024, 0.025, 0.027, 0.029, 0.030, 0.032,  0.033, 0.035],
    #     [  0.015, 0.016,  0.0175, 0.019, 0.02,  0.021, 0.023, 0.024, 0.0255, 0.027, 0.028]]
    path = "../experiments/large-graphs/"
    filename = 'tempAdj.txt'
    for row,n in enumerate(nn):
        for k,p in enumerate(pp[row]):
            # for idx in range(10):
            #     g = nx.fast_gnp_random_graph(n,p)
            #     nx.write_adjlist(g,filename)
            #     solve_new_k(filename)
            folder = path + str(n) + "-" + str(k+2)
            os.mkdir(folder)
            for idx in range(25):
                r = 0
                while(r != k+2):
                    g = nx.fast_gnp_random_graph(n,p)
                    filename = folder + "/" + str(n) + "-" + str(k+2) + "-" + str(idx) + "-graph.txt"
                    nx.write_adjlist(g,filename)
                    r = solve_new_k(filename)
                print(str(n) + "-" + str(k+2) + "-" + str(idx))

if __name__ == '__main__':
    main()

import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def print_error(n, k, r, i, name):
    print("Cannot find " + name + " with")
    print("n: " + str(n))
    print("k: " + str(k))
    print("r: " + str(r))
    if i != -1:
        print("i: " + str(i))
    sys.exit()

def find_int(df, n, k, r, i, name):
    idx = df.loc[((df['n'] == n) & (df['k'] == k)
                & (df['r'] == r) & (df['id'] == i)),name]
    if (len(idx) <= 0):
        print_error(n, k, r, i, name)
    return sum(idx.astype(int))

def find_avg(df, n, k, r, name):
    idx = df.loc[(df['n'] == n) & (df['k'] == k) & (df['r'] == r),name]
    if (len(idx) <= 0):
        print_error(n, k, r, -1, name)
    return sum(idx) / len(idx)

def find_set(df, n, k, r, i, name):
    idx = df.loc[(df['n'] == n) & (df['k'] == k)
                & (df['r'] == r) & (df['index'] == i),name]
    if (len(idx) <= 0):
        print_error(n, k, r, i, name)
    #Convert from dtype object to list of ints
    idxStr = idx.iloc[0]
    if idxStr == '[]':
        return list()
    ret = idxStr.split(' ')
    ret = [x[1:len(x)-2] for x in ret]
    ret[0] = ret[0][1:]
    ret = list(map(int, ret))
    return ret

def set_comp(df, n, k, r, i, name1, name2):
    set1 = find_set(df, n, k, r, i, name1)
    set2 = find_set(df, n, k, r, i, name2)
    set1only = []
    set2only = []
    setboth = []
    for i in set1:
        if i in set2:
            setboth.append(i)
        else:
            set1only.append(i)
    for i in set2:
        if i not in set1:
            set2only.append(i)
    return [set1only, set2only, setboth]

def IS_partial(df, n, k, r, i):
    idx = find_set(df, n, k, r, i, names[18])
    IS_partial_set = []
    for i in range(n):
        if i not in idx:
            IS_partial_set.append(i)
    return IS_partial_set;

def IS_partial_size(df, n, k, r, i = -1):
    if i == -1:
        avg = find_avg(df, n, k, r, names[19])
    else:
        avg = find_int(df, n, k, r, i, names[19])
    return n - avg

def IS_full(n, k, r, i):
    idx = find_set(df, n, k, r, i, names[20])
    IS_full_set = []
    for i in range(n):
        if i not in idx:
            IS_full_set.append(i)
    return IS_full_set;

def IS_full_size(df, n, k, r, i = -1):
    if i == -1:
        avg = find_avg(df, n, k, r, names[21])
    else:
        avg = find_int(df, n, k, r, i, names[21])
    return n - avg

def edit_set_ratio(df, n, k, r, i = -1):
    if i == -1:
        approx = find_avg(df, n, k, r, names[11])
        opt = find_avg(df, n, k, r, names[24])
    else:
        approx = find_avg(df, n, k, r, i, names[11])
        opt = find_avg(df, n, k, r, i, names[24])
    if opt == 0:
        return approx
    return approx / opt

def IS_partial_ratio(df, n, k, r, i = -1):
    if i == -1:
        approx = find_avg(df, n, k, r, names[13])
        opt = IS_partial_size(df, n, k, r, i)
    else:
        approx = find_avg(df, n, k, r, i, names[13])
        opt = IS_partial_size(df, n, k, r, i)
    return approx / opt;

def IS_lifting_ratio(df, n, k, r, i = -1):
    if i == -1:
        approx = find_avg(df, n, k, r, names[15])
        opt = IS_full_size(df, n, k, r, i)
    else:
        approx = find_avg(df, n, k, r, i, names[15])
        opt = IS_full_size(df, n, k, r, i)
    return approx / opt;

def IS_full_ratio(df, n, k, r, i = -1):
    if i == -1:
        approx = find_avg(df, n, k, r, names[17])
        opt = IS_full_size(df, n, k, r, i)
    else:
        approx = find_avg(df, n, k, r, i, names[17])
        opt = IS_full_size(df, n, k, r, i)
    return approx / opt;

def k_diff(df, n, k, r, i = -1):
    if i == -1:
        target_k = find_avg(df, n, k, r, names[3])
        new_k = find_avg(df, n, k, r, names[22])
    else:
        target_k = find_int(df, n, k, r, i, names[3])
        new_k = find_int(df, n, k, r, i, names[22])
    return target_k - new_k;

def edit_set_vertex_diff(df, n, k, r, i):
    return set_comp(df, n, k, r, i, names[10], names[23])

# TODO
def IS_partial_set_vertex_diff(df, n, k, r, i):
    return list([],[],[])

def println():
    for i in range(100):
        print('-', end='')
    print()

def print_table(table1, table2, var, varr, r):
    print("%5s" % 'r', end='')
    for v in var:
        print("%25s" % names[v], end='')
    for v in varr:
        print("%25s" % namess[v], end='')
    print()
    #println()

    for rr in r:
        print("%5s" % rr, end='')
        for v in range(len(var)):
            print("%25s" % table1[v][rr-1], end='')
        for v in range(len(varr)):
            print("%25s" % table2[v][rr-1], end='')
        print()

def print_sets(set_table, varrr, r):
    print()
    for v in range(len(varrr)):
        print(set_names[varrr[v]])
        for rr in r:
            println()
            print("r = %s" % rr)
            println()
            print("{0:>10}{1:>10}{2:>10}".format('approx', 'exact', 'both'))
            println()
            sets = set_table[v][rr-1]
            len1 = len(sets[0])
            len2 = len(sets[1])
            len3 = len(sets[2])
            max_len = max(len1, len2, len3)
            for i in range(max_len):
                if (len1 > i):
                    print("%10s" % sets[0][i], end='');
                else:
                    print("          ", end='');
                if (len2 > i):
                    print("%10s" % sets[1][i], end='');
                else:
                    print("          ", end='');
                if (len3 > i):
                    print("%10s" % sets[2][i], end='');
                else:
                    print("          ", end='');
                print()

def heat_map(matrix,n):
    fig, ax = plt.subplots()
    heatmap = plt.imshow(matrix, cmap='hot', interpolation='nearest')
    plt.colorbar(heatmap)
    plt.clim(0,2)
    # plt.title('IS size ratio on the edited graph: k = 5')
    xticks = [x+2 for x in range(7)]
    yticks = [x+10 for x in range(n)]
    # yticks = [20,40,60,80,100,120]
    ax.set_xticks(np.arange(n))
    ax.set_yticks(np.arange(len(yticks)))
    ax.set_xticklabels(xticks)
    ax.set_yticklabels(yticks)
    plt.xlabel('r')
    plt.ylabel('k')
    plt.show()

#straight from the csv
names = ['file',
        'n',
        'k',
        'r',
        'e',
        'actual_r', #5
        'time_vertex_deletion',
        'time_IS_partial_approx',
        'time_lifting',
        'time_IS_full_approx',
        'edit_set',
        'edit_set_size', #11
        'IS_partial_approx',
        'IS_partial_approx_size', #13
        'IS_lifting_approx',
        'IS_lifting_approx_size', #15
        'IS_full_approx',
        'IS_full_approx_size', #17
        'VC_partial',
        'VC_partial_size', #19
        'VC_full',
        'VC_full_size', #21
        'new_k', #22
        'opt_edit', #23
        'opt_edit_size', #24
        'index', #25
        'id', #26
        'ds_approx_size', #27
        'ds_approx_time', #28
        'ds_approx', #29
        'ds_fpt_size', #30
        'ds_fpt_time', #31
        'ds_fpt', #32
        'ds_fpt_lifting_size', #33
        'ds_fpt_lifting_time', #34
        'ds_fpt' #35
        ];
names2 = ['file',
        'n',
        'k',
        'r',
        'e',
        'actual_k', #5
        'time_vertex_deletion',
        'time_kernelization',
        'time_lifting',
        'time_DS_full_approx',
        'edit_set',
        'edit_set_size', #11
        'DS_partial_approx',
        'DS_partial_approx_size', #13
        'DS_lifting_approx',
        'DS_lifting_approx_size', #15
        'DS_full_approx',
        'DS_full_approx_size', #17
        'DS_partial',
        'DS_partial_size', #19
        'DS_full',
        'DS_full_size', #21
        ];
namess = ['IS_partial',
            'IS_partial_size',
            'IS_full',
            'IS_full_size',
            'edit_set_ratio', #4
            'IS_partial_ratio', #5
            'IS_lifting_ratio',
            'IS_full_ratio',
            'k_diff'];
set_names = ['edit_set_vertex_diff',
            'IS_partial_set_vertex_diff'];
more_names = [IS_partial,
            IS_partial_size,
            IS_full,
            IS_full_size,
            edit_set_ratio,
            IS_partial_ratio,
            IS_lifting_ratio,
            IS_full_ratio,
            k_diff,
            edit_set_vertex_diff,
            IS_partial_set_vertex_diff];

"""
Make a table of relevant data
TODO: find a good way to display sets
"""
def main():
    # df = pd.read_csv('output-Jan-14.csv')
    df = pd.read_csv('outputDSlarge-Feb-27.csv')
    #let the input args be the value of n/k/r/etc
    #the variable we want to graph over
    #and the type of graph

    var = [11,27,30,33]#[4,22,11,24]#[11,13,15,17]
    varr = []#[4]#[3,5,6,7]
    varrr = []
    n = 200
    k = 8
    r = list(range(1,k+1))
    # i = 1
    table1 = [[0 for rr in r] for v in var]
    table2 = [[0 for rr in r] for v in varr]
    set_table = [[0 for rr in r] for v in varrr]
    print()
    print("     Table with n = {0:>2}, k = {1:>2}".format(n, k))
    print()

    # #Heat map
    # n1 = [20,40,60,80,100,120]
    # k1 = 5
    # matrix = [[1 for ki in range(k1)] for kj in range(len(n1))]
    # for i in range(len(n1)):
    #     for j in range(k1):
    #         matrix[i][j] = more_names[5](df,n1[i],k1,j+1,-1)
    # for i in range(len(n1)):
    #     print(matrix[i])
    # heat_map(matrix,k1)

    # #Heat map
    k = 11
    matrix = [[0 for ki in range(k)] for kj in range(k)]
    for i in range(k):
        # print(str(i))
        for j in range(7):
            matrix[i][j] = find_avg(df, n, i+10, j+2, names2[17]) / find_avg(df, n, i+10, j+2, names2[21])
    heat_map(matrix,k)
    matrix2 = [[0 for ki in range(k)] for kj in range(k)]
    for i in range(k):
        # print(str(i))
        for j in range(7):
            matrix2[i][j] = find_avg(df, n, i+10, j+2, names2[15]) / find_avg(df, n, i+10, j+2, names2[21])
    heat_map(matrix2,k)

    # matrix = [[0 for ki in range(k)] for kj in range(k)]
    # for i in range(k):
    #     for j in range(min(i-1,7)):
    #         matrix[i][j] = find_avg(df, n, i+10, j+2, names2[17]) / find_avg(df, n, i+1, j+1, names2[21])
    # heat_map(matrix,k)
    #
    # matrix2 = [[0 for ki in range(k)] for kj in range(k)]
    # for i in range(k):
    #     for j in range(min(i-1,7)):
    #         matrix2[i][j] = find_avg(df, n, i+10, j+2, names2[15]) / find_avg(df, n, i+1, j+1, names2[21])
    # heat_map(matrix2,k)


    #averages straight from the csv
    # for v in range(len(var)):
    #     for rr in r:
    #         #invalid for sets
    #         table1[v][rr-1] = find_avg(df, n, k, rr, names[var[v]])
    #
    # #non set calculations
    # for v in range(len(varr)):
    #     for rr in r:
    #         table2[v][rr-1] = more_names[varr[v]](df, n, k, rr, i)
    #
    #set calculations
    # for v in range(len(varrr)):
    #     for rr in r:
    #         set_table[v][rr-1] = more_names[varrr[v]+9](df, n, k, rr, i)
    #
    # print_table(table1, table2, var, varr, r)
    # print()
    # if len(varrr) > 0:
    #     print_sets(set_table, varrr, r)

    # graph_the_thing1(x, y, x_label, y_label);

if __name__ == '__main__':
    main()

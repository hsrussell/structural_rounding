"""Solve VC or DS by reducing to an ILP.

This module relies on IBM CPLEX. They are not on PyPi
and must be installed independently of `requirements.txt`.
"""

# Imports
from cplex_solver.solver import solve_with_cplex
from solution import Solution
import getopt
import networkx as nx
import os
import sys

# Constants
FORMULATIONS = ('VC','DS')
FORMULATION = '--formulation'
MIPGAP = '--mipgap'
THREADS = '--threads'
TIMELIMIT = '--timelimit'
MEMLIMIT = '--memlimit'
USAGE = (
    'Usage: python -m solver '
    '[--formulation=] [--mipgap=] '
    '[--threads=] [--timelimit=] [--memlimit=] '
    '<edgelist-file>'
)


def read_edgelist(filename):
    """Read edgelist.

    Parameters
    ----------
    filename : string
        Edgelist filename

    Returns
    -------
    Networkx Graph
        Networkx Graph formed from input file
    """

    # Split file extension
    name, extension = os.path.splitext(filename)

    # Read file
    G = nx.read_adjlist(filename)

    return G


def solve(G, formulation='VC', mipgap=0,
          threads=1, timelimit=None, memlimit=None):
    """Solve an ILP problem instance with CPLEX.

    Parameters
    ----------
    G : Networkx Graph
        Graph for which VC will be computed
    formulation : string
        How problem should be solved. Either VC or DS.
    mipgap : float
        Allowed gap in tolerance
    threads : int
        Number of threads to use.
    timelimit : float
        Time limit for computation, in seconds.
    memlimit : int
        Memory limit in Mb.
    """

    # Solve
    if not G.nodes():
        return Solution(
            G=G,
            threads=threads,
            mipgap=mipgap,
            certificate=[],
            time=0,
            cuts=[]
        )
    else:
        solution = solve_with_cplex(
            G,
            formulation=formulation,
            mipgap=mipgap,
            threads=threads,
            timelimit=timelimit,
            memlimit=memlimit
        )

    # Return
    return solution


def main():
    """Parse call, solve, and report solution."""

    # Default options
    opts_dict = {
        FORMULATION: 'VC',
        MIPGAP: '0',
        THREADS: '1',
        TIMELIMIT: None,
        MEMLIMIT: None
    }

    # Validate options
    try:

        # Get and parse options
        opts, args = getopt.getopt(sys.argv[1:], None, [
            'formulation=', 'mipgap=',
            'threads=', 'timelimit=', 'memlimit='
        ])

        # Error if input file is not specified
        if not args:
            raise Exception(USAGE)

        # Override options with any specified
        for opt in opts:
            opts_dict[opt[0]] = opt[1]

        # Cast datatypes
        opts_dict[MIPGAP] = float(opts_dict[MIPGAP])
        opts_dict[THREADS] = int(opts_dict[THREADS])
        if opts_dict[TIMELIMIT] is not None:
            opts_dict[TIMELIMIT] = float(opts_dict[TIMELIMIT])
        if opts_dict[MEMLIMIT] is not None:
            opts_dict[MEMLIMIT] = int(opts_dict[MEMLIMIT])

        # Validate enumeration options
        if (opts_dict[FORMULATION] not in FORMULATIONS):
            raise Exception(USAGE)

    except getopt.GetoptError:

        # Except if unable to parse
        raise Exception(USAGE)

    # Get filename and extension
    filename = os.path.abspath(args[0])

    # Get graph
    G = read_edgelist(filename)

    # Find and print solution
    print(solve(
        G,
        formulation=opts_dict[FORMULATION],
        mipgap=opts_dict[MIPGAP],
        threads=opts_dict[THREADS],
        timelimit=opts_dict[TIMELIMIT],
        memlimit=opts_dict[MEMLIMIT]
    ))


if __name__ == '__main__':
    main()

"""CPLEX ILP solver."""
from itertools import chain
from solution import Solution
import cplex
import math
import os

def _formulate_as_vc(G, mipgap=0, threads=1, timelimit=None, memlimit=None):
    """Construct a Vertex Cover problem with CPLEX.

    Paramters
    ---------
    G : Networkx Graph
        Graph to solve Vertex Cover on.
    mipgap : float
        Allowed tolerance.
    threads : int
        Number of threads to use.
    timelimit : float
        Time limit for computation, in seconds.
    memlimit : int
        Memory limit in Mb.
    """

    # Calculate number of nodes
    n = len(G.nodes())

    # Create a new cplex problem and set parameters
    prob = cplex.Cplex()
    prob.parameters.mip.tolerances.mipgap.set(mipgap)
    prob.parameters.emphasis.numerical.set(True)
    prob.parameters.parallel.set(
        prob.parameters.parallel.values.opportunistic
    )
    prob.parameters.threads.set(threads)
    if timelimit is not None:
        prob.parameters.timelimit.set(timelimit)
    if memlimit is not None:
        prob.parameters.workmem.set(memlimit)

    # Throw away default cplex output
    prob.set_results_stream(os.devnull)

    # Construct node variables
    node_vars = {
        node: 'c{}'.format(node)
        for node in G.nodes()
    }
    node_vars_values = list(node_vars.values())

    # Add vertices as variables to the LP
    prob.variables.add(
        names=node_vars_values,
        types=([prob.variables.type.integer] * n)
    )

    # Add constraints for edges
    constraints = [
        cplex.SparsePair(
            ind=(node_vars[e[0]], node_vars[e[1]]),
            val=[1, 1]
        )
        for e in G.edges()
    ]
    num_constraints = len(constraints)
    prob.linear_constraints.add(
        lin_expr=constraints,
        senses=['G'] * num_constraints,
        rhs=[1] * num_constraints
    )

    # Set LP objective
    prob.objective.set_sense(prob.objective.sense.minimize)
    prob.objective.set_linear(list(zip(node_vars_values, [1] * n)))

    # Return
    return prob, node_vars_values

def _formulate_as_ds(G, mipgap=0, threads=1, timelimit=None, memlimit=None):
    """Construct a Dominating Set problem with CPLEX.

    Paramters
    ---------
    G : Networkx Graph
        Graph to solve Vertex Cover on.
    mipgap : float
        Allowed tolerance.
    threads : int
        Number of threads to use.
    timelimit : float
        Time limit for computation, in seconds.
    memlimit : int
        Memory limit in Mb.
    """

    # Calculate number of nodes
    n = len(G.nodes())

    # Create a new cplex problem and set parameters
    prob = cplex.Cplex()
    prob.parameters.mip.tolerances.mipgap.set(mipgap)
    prob.parameters.emphasis.numerical.set(True)
    prob.parameters.parallel.set(
        prob.parameters.parallel.values.opportunistic
    )
    prob.parameters.threads.set(threads)
    if timelimit is not None:
        prob.parameters.timelimit.set(timelimit)
    if memlimit is not None:
        prob.parameters.workmem.set(memlimit)

    # Throw away default cplex output
    prob.set_results_stream(os.devnull)

    # Construct node variables
    node_vars = {
        node: 'c{}'.format(node)
        for node in G.nodes()
    }
    node_vars_values = list(node_vars.values())

    # Add vertices as variables to the LP
    prob.variables.add(
        names=node_vars_values,
        types=([prob.variables.type.integer] * n)
    )

    # Add constraints for edges
    constraints = [
        cplex.SparsePair(
            ind=([node_vars[neigh] for neigh in list(G.neighbors(node))]+[node_vars[node]]),
            val=[1] * (len(list(G.neighbors(node)))+1)
        )
        for node in G.nodes()
    ]
    num_constraints = len(constraints)
    prob.linear_constraints.add(
        lin_expr=constraints,
        senses=['G'] * num_constraints,
        rhs=[1] * num_constraints
    )

    # Set LP objective
    prob.objective.set_sense(prob.objective.sense.minimize)
    prob.objective.set_linear(list(zip(node_vars_values, [1] * n)))

    # Return
    return prob, node_vars_values

def solve_with_cplex(G, formulation='VC', mipgap=0,
                     threads=1, timelimit=None, memlimit=None):
    """Solve an ILP problem instance with CPLEX.

    Parameters
    ----------
    G : Networkx Graph
        Graph for which the problem will be computed
    formulation : string
        How the problem should be solved. Either VC or DS.
    mipgap : float
        Allowed gap in tolerance
    threads : int
        Number of threads to use
    timelimit : float
        Time limit for computation, in seconds.
    memlimit : int
        Memory limit in Mb.
    """

    # Typecast numeric types for safety
    mipgap = float(mipgap)
    threads = int(threads)
    if timelimit is not None:
        timelimit = float(timelimit)
    if memlimit is not None:
        memlimit = int(memlimit)

    # Get problem formulation
    if formulation == 'VC':
        problem, node_vars = _formulate_as_vc(
            G, mipgap=mipgap, threads=threads,
            timelimit=timelimit, memlimit=memlimit
        )
    elif formulation == 'DS':
        problem, node_vars = _formulate_as_ds(
            G, mipgap=mipgap, threads=threads,
            timelimit=timelimit, memlimit=memlimit
        )
    else:
        raise Exception('Unknown Formulation')

    # Optimize
    start = problem.get_time()
    problem.solve()
    end = problem.get_time()

    # Calculate total time
    time = round(end - start, 1)

    # Construct certificate from solution values
    solution = problem.solution.get_values(node_vars)
    certificate = [
        node_vars[i][1:]
        for i in range(len(solution))
        if math.isclose(solution[i], 1, abs_tol=1e-10)
        and node_vars[i][0] == 'c'
    ]

    # Return solution
    return Solution(
        G=G,
        threads=threads,
        mipgap=mipgap,
        certificate=certificate,
        time=time
    )

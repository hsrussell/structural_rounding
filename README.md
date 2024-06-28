# structural rounding

A practical evaluation of the structural rounding framework for approximation algorithms. Focused on the Vertex Cover problem and edited to the class of bipartite graphs.

## Environment Configuration

Structural rounding is implemented in Python and C/C++ (using g++17). In addition, it makes use of 
ILP solver, IBM CPLEX.

To build and run structural rounding, the ILP solver and the language toolchains for Python and C/C++
must be available. It is known to support the following language and build tool versions.

| Tool | Version |
| ---- | ------- |
| Python | 3.5 |
| g++ | g++17 |
| CPLEX | 12.8 |

To configure a development environment, first run `make`. The `Makefile` will

* Compile all C/C++

Afterwards, activate the virtual environment with `source env/bin/activate` and install the
CPLEX bindings for Python with `python setup.py install`.

## Command line usage

To generate random graphs currently: 

./output -s
//TODO: replace with script

All generated graphs are in /rounding/generated-graphs
labeled
```
<n> - <k> - <r> - <i> -graph.txt
```
where:

n = number of vertices in the graph

k = degeneracy of graph

r = smallest nonempty core of graph

i = graph id number for this set of parameters

If any graphs failed to be generated (i.e. took too long to generate a random set of edges that ensured a valid set of edges between cores) the parameters are noted in failure.txt and the total number of failures is printed to the console once all graphs have been generated 

### ILP Solver

The general ILP solver can be used to solve VertexCover and OCT from snap and edgelist files.

```
python -m src.ilp.solver [option [option ...]] <edgelist-file|snap-file>
```

| Flag | Values | Description |
| -------- | ------ | ----------- |
| `threads` | `[0, INF]` | Number of threads used to compute solution. Defaults to `1`. |
| `timelimit` | `(0, INF)` | Time limit for computation in seconds. Defaults to `INF`. |

Output will be in the form

```
n = int
m = int
opt = int
time = float
threads = int
mipgap = float
certificate = int int int ...
```

## Data Formats

All preprocessed data conforms to one of the following data formats.

### Edgelist

A graph file format. The first line is `n m`, where `n` is the number of vertices and `m` is the
number of edges. All following lines are of the format `u v`, representing an edge between vertices
`u` and `v`.

Edgelist files contain the preprocessed version of the original graph.

class Solution():
    """VC Solution Class.

    Attributes
    ----------
    n : int
        Number of nodes in the graph.
    m : int
        Number of edges in the graph.
    g : Networkx Graph
        An instance of the original graph.
    threads : int
        Number of threads used to find the solution. Zero indicates
        solver was run in auto mode.
    time : double
        Length of time taken to find the solution.
    certificate : list
        List of nodes in the solution.
    opt : int
        Size of the solution.

    """

    def __init__(self, *args, **kwargs):
        """Initialize Solution.

        Parameters
        ----------
        G : Networkx Graph
            Graph for solution.
        threads : int
            Number of threads used to find the solution.
        mipgap : float
            MIP relative tolerance on the gap from the best integer objective.
        certificate : list
            List of vertex names in the solution.
        time : float
            Time taken to find solution.
        cuts : list<tuple>
            List of all cuts made.
        """


        # Assign variables
        try:
            self.g = kwargs.pop('G')
            self.n = len(self.g.nodes())
            self.m = len(self.g.edges())
            self.threads = kwargs.pop('threads')
            self.mipgap = kwargs.pop('mipgap')
            self.certificate = kwargs.pop('certificate')
            self.time = kwargs.pop('time')
            self.opt = len(self.certificate)
        except KeyError as e:
            raise Exception('Missing required key word argument.') from e

    def __str__(self):
        """Return Solution info as a string."""

        return (
            'n = {}\n'
            'm = {}\n'
            'opt = {}\n'
            'time = {:.3f}\n'
            'threads = {}\n'
            'mipgap = {}\n'
            'certificate = {}\n'
        ).format(
            self.n, self.m, self.opt, self.time, self.threads, self.mipgap,
            ' '.join(map(str, self.certificate))
        )

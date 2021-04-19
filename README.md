# LexBFS-GraphAlgorithmsProject

Project description can be found at https://faliszew.github.io/algograf/2020.project2 (in Polish).

Basically - it is known that the graph is either a tree or a chordal graph.
If it's chordal, then the program finds maximum clique in this graph and returns a number equal to max_clique - 1./

I implemented a structure described in https://www.sciencedirect.com/science/article/pii/S0304397597002417 (2 double linked lists), which allows to search through the graph using LexBFS in O (V + E) time.

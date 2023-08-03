#ifndef _GRAPH_H
#define _GRAPH_H

#include "linked_list.h"

/* a graph represented as an adjacency list */
typedef LinkedList Graph;

/* each node in the adjacency list stores a vertex */
/* a vertex has a unique numerical id */
/* and stores a list of edges from the vertex */
typedef struct Vertex {
    int id;
    LinkedList *edges;
} Vertex;

/* each node in the list of edges for a vertex store the edge weight */
/* and the destination vertex */
typedef struct Edge {
    double weight;
    Vertex *vertex;
} Edge;

/* function prototypes */

Graph *init_graph(void);
void free_graph(Graph *);
void print_graph(Graph *);
Vertex *find_vertex(Graph *, int);
Vertex *add_vertex(Graph *, int);
void remove_vertex(Graph *, int);
Edge *add_edge(Graph *, int, int, double);
void add_edge_undirected(Graph *, int, int, double);
void remove_edge(Graph *, int, int);
void remove_edges(Graph *, int);

Vertex *init_vertex(int);
void free_vertex(Vertex *);

Edge *init_edge(void);
void free_edge(Edge *);

int *get_vertices(Graph *, int *);
Edge **get_edges(Graph *, Vertex *, int *);
Edge *get_edge(Graph *, int, int);
int edge_destination(Edge *);
double edge_weight(Edge *);

#endif


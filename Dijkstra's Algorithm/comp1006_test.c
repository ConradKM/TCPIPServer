/*
 *  comp1006_test.c
 *  ProgrammingPortfolio
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "graph.h"
#include "dijkstra.h"
#include "utility.h"

void PrintTable(Graph *g, Path *paths, int n);

int main(int argc, char **argv)
{
    Graph *graph;
    Path *table;
    int n;
    int i, num_vert, v1, v2, src;
    double w;

    /* parse command line */
    if(argc < 3)
    {
        fprintf(stderr, "usage: %s SOURCE GRAPH_ENTRY [GRAPH_ENTRY ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    num_vert = argc - 1;
    src = convert_integer(argv[1]);
    if(src == INT_MAX)
        return 1;

    /* initialise graph */
    if(!(graph = init_graph()))
    {
        return 1;
    }

    /* add edges to graph */
    for(i = 2; i <= num_vert; i++)
    {
        /* parse edge */
        if(!parse_edge(argv[i], &v1, &v2, &w)) {
            fprintf(stderr, "error: unable to parse spec \"%s\"\n", argv[i]);
            free_graph(graph);
            exit(EXIT_FAILURE);
        }

        /* add edge to graph */
        add_edge_undirected(graph, v1, v2, w);
    }

    /* run dijkstras */
    table = dijkstra(graph, src, &n);


    /* print table */
    if(table != NULL)
    {
        PrintTable(graph, table, n);

        free(table);
    }
    
    /* free memory for graph */
    free_graph(graph);

    exit(EXIT_SUCCESS);
}

void PrintTable(Graph *g, Path *paths, int n)
{
    int i = 0;
    int *vertices, nVertices;
    
    if(g == NULL || paths == NULL || n <= 0)
        return;
    
    vertices = get_vertices(g, &nVertices);
    printf("%s %8s %8s\n", "Net", "Weight", "Next Hop");
    for(i = 0; i < nVertices; i++)
    {
        Path *p = paths + vertices[i];
        
        printf("%d ", vertices[i]);
        if(p->weight == DBL_MAX)
        {
            printf("%8s", "INF");
        }
        else
        {
            printf("%8.2f",p->weight);
        }
        printf(" %8d\n", p->next_hop);
        
    }
    
    free(vertices);
}


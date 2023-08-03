/*
 *  dijkstra.c
 *  ProgrammingPortfolio
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "graph.h"
#include "dijkstra.h"

void delete(int * s, int u, int vertexCount);
void getEdgedata(int * dest, double * weight, Edge * edge);


/* find shortest paths between source node id and all other nodes in graph. */
/* upon success, returns an array containing a table of shortest paths.  */
/* return NULL if *graph is uninitialised or an error occurs. */
/* each entry of the table array should be a Path */
/* structure containing the path information for the shortest path between */
/* the source node and every node in the graph. If no path exists to a */
/* particular desination node, then next should be set to -1 and weight */
/* to DBL_MAX in the Path structure for this node */

Path *dijkstra(Graph *graph, int id, int *pnEntries)
{
    Path *table = NULL;
    Edge ** edges;
    Edge * edge;
    int *s, *r;
    double *d, uWeight, newUWeight, vWeight, c;
    int elementFound = -1, edgeFound, vFound = -1;
    int i, x;
    int u, newU, v;
    int vCount, vertexCount, edgeCount;
    

   if(!(s = get_vertices(graph, &vertexCount)))
    {
        fprintf(stderr,"error: no vertices in graph");
        return NULL;
    }

    if(!(graph))
    {
        fprintf(stderr,"error: graph not found");
        return NULL;
    }

    if(!(find_vertex(graph,id)))
    {
        fprintf(stderr,"error: vertex not found");
        return NULL;
    }

    *pnEntries = 0;
    for(i=0;i<vertexCount;i++)
    {
        if (s[i] >= *pnEntries)
        {
            *pnEntries = s[i] + 1;
        }
    }


    if(!(table = (Path*) calloc(*pnEntries, sizeof(Path))))
    {
        fprintf(stderr,"error: unable to initialise table");
        exit(EXIT_FAILURE);
    }

    if(!(d = (double*) calloc(*pnEntries, sizeof(double))))
    {
        fprintf(stderr,"error: unable to initialise d");
        exit(EXIT_FAILURE);
    }

    for(vCount=0;vCount<*pnEntries;vCount++)
    {
        d[vCount] = DBL_MAX;
    }

    if(!(r = (int*) calloc(*pnEntries, sizeof(int))))
    {
        fprintf(stderr,"error: unable to initialise r");
        exit(EXIT_FAILURE);
    }

    for(vCount=0;vCount<*pnEntries;vCount++)
    {
        r[vCount] = -1;
    }


    for(i=0;i<vertexCount;i++)
    {
        if((edge = get_edge(graph,id,s[i])))
        {
            edgeFound = 0;
        }
        if(edgeFound == 0)
        {
            getEdgedata(&r[s[i]],&d[s[i]],edge);
            edgeFound = -1;
        }

    }

    for(i=0; i<vertexCount; i++)
    {
        if(s[i] == id)
        {
            elementFound = 0;
            continue;
        }
        if(elementFound == 0)
        {
            s[i-1] = s[i];
        }
    }

    elementFound = -1;   
    s[vertexCount-1] = -1;

    
    while(s[0] != -1)
    {
        u = s[0];
        uWeight= d[s[0]];

        for(i=1;i<vertexCount;i++)
        {
            newU = s[i];

            if(newU == -1)
            {
               break;
            }

            newUWeight = d[s[i]];

            if(newU != -1  && newUWeight < uWeight)
            {
                u = newU;
                uWeight = newUWeight;
                uWeight = -1;
                newU = -1;
            }
        }

        if(uWeight == DBL_MAX)
        {
            break;
        }
        
        delete(s, u, vertexCount);
        
        if(!(edges = get_edges(graph, find_vertex(graph,u), &edgeCount)))
        {
            fprintf(stderr, "error: edges not found");
            exit(EXIT_FAILURE);
        }

        for(i=0; i<edgeCount; i++){
            getEdgedata(&v,&vWeight,edges[i]);
            for(x=0;x<vertexCount;x++)
            {
                if(s[x] == v)
                {
                    vFound = 0;
                }
                if (vFound == 0)
                {
                    c = d[u] + vWeight;
                    if(c < d[v])
                    {
                        r[v] = r[u];
                        d[v] = c;
                    }
                    vFound = -1;
                }
                
            }
        }
        free(edges);
    }

    for(i=0;i<*pnEntries;i++)
    {
        table[i].weight = d[i];
        table[i].next_hop = r[i];
    }

    free(s);
    free(d);
    free(r);

    return table;
}


void delete(int * s, int u, int vertexCount)
{
    int i;
    int elementFound = -1;

    for(i=0; i<vertexCount; i++)
    {

        if(s[i] == u)
        {
            elementFound = 0;
            continue;
        }
        if(elementFound == 0)
        {
            s[i-1] = s[i];
        }
    }
    
}

void getEdgedata(int * dest, double * weight, Edge * edge)
{
    *dest = edge_destination(edge);
    *weight = edge_weight(edge);
}



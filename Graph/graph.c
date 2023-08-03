#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

/* initialise an empty graph */
/* return pointer to initialised graph */
Graph *init_graph(void)
{
    Graph * graph;

    if (!(graph = (Graph *) malloc(sizeof(Graph))))
    {
        fprintf(stderr, "-ERR: Unable to initalise graph (no free memory)\n");
        exit(EXIT_FAILURE);
    }
    
    graph->head = graph->tail = NULL;
    return graph;
}

/* release memory for graph */
void free_graph(Graph *graph)
{
    Node * node;
    Vertex * vertex;

    if(!(graph))
    {
        printf("warning: unable to free graph\n");
        return;
    }

    node = graph->head;

    if(!(node))
    {
        printf("-WAR: Graph freed with no nodes");
        free_linked_list(graph);
        return;
    }

    while(node)
    {
        vertex = node->data;
        free_vertex(vertex);

        node = node->next;

    }
   

    

    free_linked_list(graph);

}

/* initialise a vertex */
/* return pointer to initialised vertex */
Vertex *init_vertex(int id)
{
    Vertex *vertex;

    if(!(vertex = (Vertex *) malloc (sizeof(Vertex))))
    {
        fprintf(stderr, "error: unable to initalise vertex.\n");
        exit(EXIT_FAILURE);
    }

    vertex->id = id;
    vertex->edges = initialise_linked_list();

    return vertex;
}

/* release memory for initialised vertex */
void free_vertex(Vertex *vertex)
{
    Node * node;
    Edge * edge;

    if(!(vertex))
    {
        printf("warning: unable to free vertex\n");
        return;
    }
    node=vertex->edges->head;
    
    while(node){
        edge = node->data;
        free_edge(edge);
        node = node->next;
	}
   
    free_linked_list(vertex->edges);
    free(vertex);

}

/* initialise an edge. */
/* return pointer to initialised edge. */
Edge *init_edge(void)
{
    Edge *edge;

    if(!(edge = (Edge *) malloc (sizeof(Edge))))
    {
        fprintf(stderr, "warning: unable to initalise edge\n");
        exit(EXIT_FAILURE);
    }
    edge->weight = 0;
    edge->vertex = NULL;
    
    return edge;
}

/* release memory for initialised edge. */
void free_edge(Edge *edge)
{
    if (!(edge))
    {
        printf("warning: unable to free edge\n");
        return;
    }

    free(edge);
}

/* remove all edges from vertex with id from to vertex with id to from graph. */
void remove_edge(Graph *graph, int from, int to)
{
    
    Node * node;
    Vertex * fromVertex;
    Vertex * toVertex;
    LinkedList * edges;
    Edge * edge;
    int fromFound = -1;
    int toFound = -1;
    
    
    
    if (!(graph) || !(find_vertex(graph,from)) || !(find_vertex(graph,to)))
    {  
        if (!(find_vertex(graph,from)) || !(find_vertex(graph,to)))
        {}
        printf("warning: unable to remove edge\n");
        return;
    }

    node = graph->head;

    if((fromVertex = find_vertex(graph,from)))
    {
        fromFound = 0;
    }
    edges = fromVertex->edges;
    node = edges->head;
    while(node && fromFound == 0)
    {
        edge = node->data;
        if(edge->vertex->id == to)
        {
            if(node->next)
            {
                node->next->prev = node->prev;
            }
            if(node->prev)
            {
                node->prev->next = node->next;
            }
            if(edges->head == node)
            {
                fromVertex->edges->head = node->next;
            }
            if(edges->tail == node)
            {
                fromVertex->edges->tail = node->prev;
            }
            free_edge(edge);
            free_node(node);
            break;
        }
        node = node->next;
    }
   
    
    node = graph->head;
    if((toVertex = find_vertex(graph,to)))
    {
        toFound = 0;
    }
    edges = toVertex->edges;
    node = edges->head;
    while(node && toFound == 0)
    {
        edge = node->data;
        if(edge->vertex->id == from)
        {
            if(node->next)
            {
                node->next->prev = node->prev;
            }
            if(node->prev)
            {
                node->prev->next = node->next;
            }
            if(edges->head == node)
            {
                toVertex->edges->head = node->next;
            }
            if(edges->tail == node)
            {
                toVertex->edges->tail = node->prev;
            }
            free_edge(edge);
            free_node(node);
            break;
        }
        node = node->next;
    }

	
}

/* remove all edges from vertex with specified id. */
void remove_edges(Graph *graph, int id)
{
    Node * node;
    Vertex * vertex;
    Edge * edge;
    int vertexFound = -1;

    if(!graph)
    {
        printf("warning: unable to remove edge\n");
        return;
    }

    if((vertex = find_vertex(graph,id)))
    {
        vertexFound = 0;
    }

    if(vertexFound == -1)
    {
        printf("warning: unable to find vertex\n");
        return;
    }

    node = vertex->edges->head;
  
    while(node)
    {
        edge = node->data;
        node = node->next;
        remove_edge(graph, id, edge->vertex->id);
    }



    
}

/* output all vertices and edges in graph. */
/* each vertex in the graphs should be printed on a new line */
/* each vertex should be printed in the following format: */
/* vertex_id: edge_to_vertex[weight] edge_to_vertex[weight] ... */
/* for example: */
/* 1: 3[1.00] 5[2.00] */
/* indicating that vertex id 1 has edges to vertices 3 and 5 */
/* with weights 1.00 and 2.00 respectively */
/* weights should be output to two decimal places */
void print_graph(Graph *graph)
{
    int * vertices;
    int count, count2, i,j;
    Edge **edges;

    if (!(graph))
    {
        printf("warning: unable to print graph\n");
        return;
    }

    /*from task1_test*/
    vertices = get_vertices(graph, &count);
	for(i = 0; i < count; i++) {
		edges = get_edges(graph, find_vertex(graph, vertices[i]), &count2);
		printf("%d:", vertices[i]);
		for(j = 0; j < count2; j++)
			printf(" %d[%0.2f]", edge_destination(edges[j]), edge_weight(edges[j]));
		printf("\n");
		free(edges);
	}
	free(vertices);

}

/* find vertex with specified id in graph. */
/* return pointer to vertex, or NULL if no vertex found. */
Vertex *find_vertex(Graph *graph, int id)
{
    Node * node;
    Vertex * vertex;

    if (!(graph))
    {
        printf("warning: unable to find vertex\n");
        return NULL;
    }

    node = graph->head;
    while(node)
    {
        vertex = node->data;
        if (id == vertex->id)
        {
            return vertex;
        }
        node = node->next;
    }

    return NULL;
    
}

/* create and add vertex with specified id to graph. */
/* return pointer to vertex or NULL if an error occurs. */
/* if vertex with id already exists, return pointer to existing vertex. */
Vertex *add_vertex(Graph *graph, int id)
{
    Vertex * vertex;
    Vertex * newVertex;


    if(!(graph))
    {
        printf("warning: unable to add vertex\n");
        return NULL;
    }

    vertex = find_vertex(graph,id);

    if(vertex)
    {
        return vertex;
    }

    newVertex = init_vertex(id);
    append_linked_list(graph,newVertex);

    return newVertex;
}

/* remove vertex with specified id from graph. */
/* remove all edges between specified vertex and any other vertices in graph. */
void remove_vertex(Graph *graph, int id)
{
    Node * node;
    Vertex * vertex;
    int vertexFound = -1;

    if (!(graph))
    {
        printf("warning: unable to remove vertex\n");
        return;
    }

    node = graph->head;

    while(node)
    {
        vertex = node->data;
        if(vertex->id == id)
        {
            vertexFound = 0;
            break;
        }
        node = node->next;
    }

    if(vertexFound == -1)
    {
        printf("warning: unable to find vertex\n");
        return;
    }

    if(node->next)
    {
        node->next->prev = node->prev;
    }
    else
    {
        graph->tail = node->prev;
    }

    if(node->prev)
    {
        node->prev->next = node->next;
    }
    else
    {
        graph->head = node->next;
    }

    remove_edges(graph, id);
    free_vertex(vertex);
    free_node(node);

}

/* add directed edge with specified weight between vertex with id from */
/* to vertex with id to. */
/* if no vertices with specified ids (from or to) exist */
/* then the vertices will be created. */
/* multiple edges between the same pair of vertices are allowed. */
/* return pointer to edge, or NULL if an error occurs found. */
Edge *add_edge(Graph *graph, int from, int to, double weight)
{
    Node * node;
    Edge * edge;
    Vertex * fromVertex;
    Vertex * toVertex;
    Vertex * testVertex;
    int fromFound = -1;
    int toFound = -1;

    if (!(graph) || from < 0 || to < 0 || weight < 0)
    {
        printf("warning: unable to add edge\n");
        return NULL;
    }


    node = graph->head;

    while(node)
    {
        testVertex = node->data;
        if(testVertex->id == from)
        {
            fromVertex = testVertex;
            fromFound = 0;
        }
        if(testVertex->id == to)
        {
            toVertex = testVertex;
            toFound = 0;
        }

        node = node->next;
    }


    if (fromFound == -1)
    {
        fromVertex = add_vertex(graph,from);
    }
    if (toFound == -1)
    {
        toVertex = add_vertex(graph,to);
    }


    edge = init_edge();
    edge->vertex = toVertex;
    edge->weight = weight;
    append_linked_list(fromVertex->edges, edge);

    return edge;

}

/* add two edges to graph, one from vertex with id from to vertex with id to, */
/* and one from vertex with id to to vertex with id from. */
/* both edges should have the same weight */
/* if no vertices with specified ids (from or to) exist */
/* then the vertices will be created. */
/* multiple vertices between the same pair of vertices are allowed. */
void add_edge_undirected(Graph *graph, int from, int to, double weight)
{
    if (!(graph) || from < 0 || to < 0 || weight < 0)
    {
        printf("warning: unable to add undirected edge\n");
        return;
    }
   
    add_edge(graph,from,to,weight);
    add_edge(graph,to,from,weight);
    

}

/* return array of node ids in graph. */
/* array of node ids should be dynamically allocated */
/* set count to be the number of nodes in graph */
/* return NULL if no vertices in graph */
int *get_vertices(Graph *graph, int *count)
{
    int * array;
    int i = 0;
    Vertex * vertex;
    Node * node;

    

    if (!(graph))
    {
        printf("warning: unable to get vertex\n");
        return NULL;
    }

    if (!(graph->head))
    {
        return NULL;
    }

    node = graph->head;

    *count = 0;

    while(node)
    {
        *count = *count +1;
        node = node->next;
    }

    node = graph->head;
    if (!(array = (int *) calloc(*count, sizeof(int))))
    {
        printf("warning: Unable to initalise array \n");
        return NULL;
    }

    while(node && i<(*count))
    {
        vertex = node->data;
        array[i] = vertex->id;
        node = node->next;
        i++;
    
    }
  
    

    return array;

}

/* return array of pointers to edges for a given vertex. */
/* array of edges should be dynamically allocated */
/* set count to be number of edges of vertex */
/* return NULL if no edges from/to vertex */
Edge **get_edges(Graph *graph, Vertex *vertex, int *count)
{
    Edge ** array;
    Node * node;
    LinkedList * edges;
    int i = 0;
    
    if(!(graph) || !(find_vertex(graph, vertex->id)))
    {
        printf("warning: unable to get vertex\n");
        return NULL;
    }

     
    *count = 0;
    node = vertex->edges->head;
    while(node)
    {
        *count = *count+1;
        node = node->next;
    }

    if (!(array = (Edge **) calloc ((*count) , sizeof(Edge))))
    {
        printf("warning: unable to initialise array");
        return NULL;
    }

    edges = vertex->edges;
    node = edges->head;


    while(node && i < *count)
    {
        array[i] = node->data;
        node = node->next;
        i++;
    }

    return array;

    

}

/* return pointer to edge from vertex with id from, to vertex with id to. */
/* return NULL if no edge */
Edge *get_edge(Graph *graph, int from, int to)
{
    Node * node;
    Vertex * fromVertex;
    Vertex * toVertex;
    Edge * edge;

    if (!(fromVertex = find_vertex(graph,from)) || !(toVertex = find_vertex(graph,from)))
    {
        printf("warning: unable to find vertex\n");
        return NULL;
    }

    node = fromVertex->edges->head;

    while(node)
    {
        edge = node->data;
        if(edge->vertex->id == to)
        {
            return node->data;
        } 
    }

    return NULL;
}

/* return id of destination node of edge. */
int edge_destination(Edge *edge)
{
    return edge->vertex->id;
}

/* return weight of edge. */
double edge_weight(Edge *edge)
{
    return edge->weight;
}


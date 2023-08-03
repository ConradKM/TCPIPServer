#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "graph.h"
#include "utility.h"

int main(int argc, char **argv)
{
	Graph *graph;
	Edge **edges;
	int i, j, num_vert, v1, v2, *vertices, count, count2;
	double w;

	/* parse command line */
	if(argc < 2) {
		fprintf(stderr, "usage: %s GRAPH_ENTRY [GRAPH_ENTRY ...]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	num_vert = argc - 1;

	/* TEST 1 - check function error handling */
	/* should print some error messages but program should continue running */
	printf("Performing Test 1...\n");
	remove_vertex(NULL, 0);
	remove_edge(NULL, 0, 0);
	remove_edges(NULL, 0);
	if(add_vertex(NULL, 0)) {
		fprintf(stderr, "error: able to add vertex to nonexistent graph.\n");
		exit(EXIT_FAILURE);
	}
	if(add_edge(NULL, 0, 0, 0.0)) {
		fprintf(stderr, "error: able to add edge to nonexistent graph.\n");
		exit(EXIT_FAILURE);
	}
	add_edge_undirected(NULL, 0, 0, 0.0);
	find_vertex(NULL, 0);
	print_graph(NULL);
	free_edge(NULL);
	free_vertex(NULL);
	free_graph(NULL);
	printf("Completed Test 1.\n");

	/* TEST 2 - add all entries to graph then free */
	printf("Performing Test 2...\n");
	/* initialise graph */
	printf("initialising graph...\n");
	graph = init_graph();

	/* add edges to graph */
	printf("adding edges...\n");
	for(i = 1; i <= num_vert; i++) {
		/* parse edge */
		if(!parse_edge(argv[i], &v1, &v2, &w)) {
			fprintf(stderr, "error: unable to parse spec \"%s\"\n", argv[i]);
			free_graph(graph);
			exit(EXIT_FAILURE);
		}

		/* add edge to graph */
		add_edge_undirected(graph, v1, v2, w);
	}

	printf("removing graph...\n");
	free_graph(graph);
	printf("Completed Test 2.\n");

	/* TEST 3 - add all entries to graph, remove then free */
	printf("Performing Test 3...\n");
	/* initialise graph */
	printf("initialising graph...\n");
	graph = init_graph();

	/* add edges to graph */
	printf("adding edges...\n");
	for(i = 1; i <= num_vert; i++) {
		/* parse edge */
		if(!parse_edge(argv[i], &v1, &v2, &w)) {
			fprintf(stderr, "error: unable to parse spec \"%s\"\n", argv[i]);
			free_graph(graph);
			exit(EXIT_FAILURE);
		}

		/* add edge to graph */
		add_edge(graph, v1, v2, w);
	}

	printf("removing edges...\n");
	for(i = 1; i <= num_vert; i++) {
		/* parse edge */
		if(!parse_edge(argv[i], &v1, &v2, &w)) {
			fprintf(stderr, "error: unable to parse spec \"%s\"\n", argv[i]);
			free_graph(graph);
			exit(EXIT_FAILURE);
		}

		/* remove edges from graph */
		remove_edge(graph, v1, v2);
	}

	printf("removing graph...\n");
	free_graph(graph);
	printf("Completed Test 3.\n");

	/* TEST 4 - add all entries to graph, remove vertices then free */
	printf("Performing Test 4...\n");
	/* initialise graph */
	printf("initialising graph...\n");
	graph = init_graph();

	/* add edges to graph */
	printf("adding edges...\n");
	for(i = 1; i <= num_vert; i++) {
		/* parse edge */
		if(!parse_edge(argv[i], &v1, &v2, &w)) {
			fprintf(stderr, "error: unable to parse spec \"%s\"\n", argv[i]);
			free_graph(graph);
			exit(EXIT_FAILURE);
		}

		/* add edge to graph */
		add_edge(graph, v1, v2, w);
	}

	printf("removing vertices...\n");
	for(i = 1; i <= num_vert; i++) {
		/* parse edge */
		if(!parse_edge(argv[i], &v1, &v2, &w)) {
			fprintf(stderr, "error: unable to parse spec \"%s\"\n", argv[i]);
			free_graph(graph);
			exit(EXIT_FAILURE);
		}

		/* remove vertex from graph */
		remove_vertex(graph, v1);
	}

	printf("removing graph...\n");
	free_graph(graph);
	printf("Completed Test 4.\n");

	/* TEST 5 - add all entries to graph, print then free */
	printf("Performing Test 5...\n");
	/* initialise graph */
	printf("initialising graph...\n");
	graph = init_graph();

	/* add edges to graph */
	printf("adding edges...\n");
	for(i = 1; i <= num_vert; i++) {
		/* parse edge */
		if(!parse_edge(argv[i], &v1, &v2, &w)) {
			fprintf(stderr, "error: unable to parse spec \"%s\"\n", argv[i]);
			free_graph(graph);
			exit(EXIT_FAILURE);
		}

		/* add edge to graph */
		add_vertex(graph, v1);
		add_vertex(graph, v2);
		add_edge_undirected(graph, v1, v2, w);
	}

	printf("printing graph...\n");
	print_graph(graph);

	printf("removing graph...\n");
	free_graph(graph);
	printf("Completed Test 5.\n");

	/* TEST 6 - add all entries to graph, remove, re-add, print then free */
	printf("Performing Test 6...\n");
	/* initialise graph */
	printf("initialising graph...\n");
	graph = init_graph();

	/* add edges to graph */
	printf("adding edges...\n");
	for(i = 1; i <= num_vert; i++) {
		/* parse edge */
		if(!parse_edge(argv[i], &v1, &v2, &w)) {
			fprintf(stderr, "error: unable to parse spec \"%s\"\n", argv[i]);
			free_graph(graph);
			exit(EXIT_FAILURE);
		}

		/* add edge to graph */
		add_vertex(graph, v1);
		add_vertex(graph, v2);
		add_edge_undirected(graph, v1, v2, w);
	}

	/* remove edges from graph */
	printf("removing edges...\n");
	for(i = 1; i <= num_vert; i++) {
		/* parse edge */
		if(!parse_edge(argv[i], &v1, &v2, &w)) {
			fprintf(stderr, "error: unable to parse spec \"%s\"\n", argv[i]);
			free_graph(graph);
			exit(EXIT_FAILURE);
		}

		/* add edge to graph */
		remove_edges(graph, v1);
		remove_edges(graph, v2);
	}

	/* add edges to graph */
	printf("adding edges...\n");
	for(i = 1; i <= num_vert; i++) {
		/* parse edge */
		if(!parse_edge(argv[i], &v1, &v2, &w)) {
			fprintf(stderr, "error: unable to parse spec \"%s\"\n", argv[i]);
			free_graph(graph);
			exit(EXIT_FAILURE);
		}

		/* add edge to graph */
		add_edge_undirected(graph, v1, v2, w);
	}

	printf("printing graph...\n");
	print_graph(graph);

	printf("removing graph...\n");
	free_graph(graph);
	printf("Completed Test 6.\n");

	/* TEST 7 - add all entries to graph, print all vertices, edges then free */
	printf("Performing Test 7...\n");
	/* initialise graph */
	printf("initialising graph...\n");
	graph = init_graph();

	/* add edges to graph */
	printf("adding edges...\n");
	for(i = 1; i <= num_vert; i++) {
		/* parse edge */
		if(!parse_edge(argv[i], &v1, &v2, &w)) {
			fprintf(stderr, "error: unable to parse spec \"%s\"\n", argv[i]);
			free_graph(graph);
			exit(EXIT_FAILURE);
		}

		/* add edge to graph */
		add_vertex(graph, v1);
		add_vertex(graph, v2);
		add_edge_undirected(graph, v1, v2, w);
	}

	printf("printing graph...\n");
	vertices = get_vertices(graph, &count);
	for(i = 0; i < count; i++) {
		edges = get_edges(graph, find_vertex(graph, vertices[i]), &count2);
		printf("%d:", vertices[i]);
		for(j = 0; j < count2; j++)
			printf(" %d[%0.2f]", edge_destination(edges[j]), \
				edge_weight(edges[j]));
		printf("\n");
		free(edges);
	}

	printf("removing graph...\n");
	free(vertices);
	free_graph(graph);
	printf("Completed Test 7.\n");

	return 0;
}

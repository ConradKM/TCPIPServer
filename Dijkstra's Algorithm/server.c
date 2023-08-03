/*
 *  NetworkServer.c
 *  ProgrammingPortfolio Skeleton
 *
 */

/* You will need to include these header files to be able to implement the TCP/IP functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>

/* You will also need to add #include for your graph library header files */
#include "graph.h"
#include "dijkstra.h"

/* Definitions */
#define MAX_SIZE 512

/* Function Declarations */
void serverConnection(int sd, Graph * graph);
void ReadLineFromNetwork(int sd, char *buf, int size);
void printClient(int sd, char * string);


int main(int argc, const char * argv[])
{
	int serverSocket = -1, clientConnection;
	int port = -1;
	struct sockaddr_in server;
	struct sockaddr_in client;
	unsigned int alen;

	Graph * graph;

	
	printf("Programming Portfolio 2022 Implementation\n");
	printf("=========================================\n\n");
	
	/* Insert your code here to create, and the TCP/IP socket for the Network Server */


	/* Creating port */
	port = atoi(argv[1]);

	if (argc != 2 && port != -1)
	{
		fprintf(stderr, "Invalid Port");
		exit(4);
	}

	
	/* Creating Socket */
	serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket == -1)
	{
		fprintf(stderr, "Cannot create socket \n");
		exit(1);
	}

	/* Setup Socket Address */
	memset(&server,0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (struct sockaddr * ) &server, sizeof(server)) < 0)
	{
		fprintf(stderr, "bind() failed \n");
		exit(2);
	}
	
	if (listen(serverSocket,15) < 0)
	{
		fprintf(stderr, "listen() failed \n");
		exit(3);
	}

	printf("Socket and Port were created\n");

	graph = init_graph();
	
	/* Then once a connection has been accepted implement protocol described in the coursework description.*/
	while(1)
	{
		alen = sizeof(client);
		clientConnection = accept(serverSocket, (struct sockaddr *) &client, &alen);

		printf("Connection from %x on port %d\n", ntohl(client.sin_addr.s_addr), ntohs(client.sin_port));

		/* Protocols */
		serverConnection(clientConnection, graph);
		
		/* close connection */
		printf("Disconnection from %x on port %d\n", ntohl(client.sin_addr.s_addr), ntohs(client.sin_port));
		close(clientConnection);
	}

	free_graph(graph);
	
	
	return 0;
}

void serverConnection(int sd, Graph * graph)
{
	Path * paths;
	Path path;
	Edge ** edges;
	Edge * edge;
	Node * node;
	Vertex * vertex;
	char outBuffer[MAX_SIZE];
	char inBuffer[MAX_SIZE];
	char confirm[MAX_SIZE] = {'\0'};
	char endMessage[MAX_SIZE] = {'\0'};
	char quitstring[MAX_SIZE] = "+OK";
	char str[10];
	char * token;
	char * ptr;
	int * networks;
	int quit = -1;
	int i = 0;
	int count = 0;
	int entries = 0;
	int networkCount = 0;
	int src, dst, hop;
	int successAdd = 1, successDel = 1;
	double weight;

	/* Server Greeting */
	sprintf(outBuffer, "+OK 2022 Programming Portfolio Route Server\r\n");
	write(sd, outBuffer, strlen(outBuffer));

	/* Main */
	while (quit == -1)
	{
		ReadLineFromNetwork(sd, inBuffer, MAX_SIZE);
		printf("[%s] received \n", inBuffer);
		token = strtok(inBuffer," ");

		if(0 == strcmp(inBuffer,"QUIT"))
		{
			printClient(sd,quitstring);
			quit = 0;
			break;
		}
		else if (0 == strcmp(token,"NET-ADD"))
		{
			token = strtok(NULL," ");

			/* Check if Network is in Graph */
			if (find_vertex(graph, atoi(token)) != NULL)
			{
				strcpy(endMessage, "-ERR Network already added");
				printClient(sd,endMessage);
				successAdd = 1;
				continue;
			}

			/* Add Network to Graph */
			add_vertex(graph, atoi(token));
			successAdd = 0;

			/* Confirmation */
			strcpy(endMessage, "+OK Added ");
			strcat(endMessage, token);
			printClient(sd, endMessage);
		}
		else if (0 == strcmp(token,"NET-DELETE"))
		{
			token = strtok(NULL," ");

			/* Check if Network is in Graph */
			
			if (!(find_vertex(graph, atoi(token))))
			{
				strcpy(endMessage,"-ERR Network does not exist");
				printClient(sd,endMessage);
				continue;
			}

			/* Add Network to Graph */
			remove_vertex(graph, atoi(token));

			/* Confirmation */
			strcpy(endMessage, "+OK Deleted ");
			strcat(endMessage, token);
			printClient(sd, endMessage);
			successDel = 0;
		}
		else if (0 == strcmp(token,"NET-LIST"))
		{
			networks = get_vertices(graph, &count);

			if (count == 0)
			{
				continue;
			}

			strcpy(endMessage,"+OK ");
			sprintf(str,"%d",count);
			strcat(endMessage,str);
			printClient(sd, endMessage);
			

			for(i=0;i<count;i++)
			{
				strcpy(endMessage,"+OK ");
				sprintf(str,"%d",networks[i]);
				strcat(endMessage,str);
				printClient(sd, endMessage);
			}

			free(networks);
		}
		
		else if (0 == strcmp(token,"ROUTE-ADD"))
		{
			
			token = strtok(NULL, " ");
			src = atoi(token);
			
			token = strtok(NULL, " ");
			dst = atoi(token);
			
			token = strtok(NULL, " ");
			weight = strtod(token, &ptr);
			

			if(!(find_vertex(graph,src)) || !(find_vertex(graph,dst)))
			{
				strcpy(endMessage, "-ERR Invalid Networks");
				printClient(sd,endMessage);
				continue;
			}
			if(!(get_edge(graph,src,dst)))
			{
				add_edge_undirected(graph,src,dst,weight);
			}
			else
			{
				remove_edge(graph,src,dst);
				add_edge_undirected(graph,src,dst,weight);
			}
			
			strcpy(endMessage, "+OK Route Added");
			printClient(sd,endMessage);
		}
		
		else if (0 == strcmp(token,"ROUTE-DELETE"))
		{
			
			token = strtok(NULL, " ");
			src = atoi(token);
			token = strtok(NULL, " ");
			dst = atoi(token);
			
			
			if(!(find_vertex(graph,src)) || !(find_vertex(graph,dst)))
			{
				strcpy(endMessage, "-ERR Invalid Networks");
				printClient(sd,endMessage);
				continue;
			}

			if(!get_edge(graph,src,dst))
			{
				strcpy(endMessage, "-ERR Route does not exist");
				printClient(sd,endMessage);
				continue;
			}

			remove_edge(graph,src,dst);
			remove_edge(graph,dst,src);
			strcpy(endMessage, "+OK Route Deleted");
			printClient(sd,endMessage);
		}
		
		else if (0 == strcmp(token,"ROUTE-SHOW"))
		{
			token = strtok(NULL, " ");

			vertex = find_vertex(graph,atoi(token));

			node = vertex->edges->head;

			count = 0;

			while(node)
			{
				count++;
				node = node->next;
			}

			if (count = 0)
			{
				strcpy(endMessage,"+OK 0");
				printClient(sd,endMessage);
			}

			
			edges = get_edges(graph,vertex,&count);

			for(i=0;i<count;i++)
			{
				edge = edges[i];
				vertex = edge->vertex;
				strcpy(endMessage,"+OK ");
				sprintf(str,"%d", vertex->id);
				strcat(endMessage,str);
				printClient(sd, endMessage);
			}

			free(edges);
		}
		
		else if (0 == strcmp(token,"ROUTE-HOP"))
		{
			token = strtok(NULL," ");
			src = atoi(token);
			token = strtok(NULL," ");
			dst = atoi(token);
			
			if (src == dst || !(find_vertex(graph,src)) || !(find_vertex(graph,dst)))
			{
				strcpy(endMessage, "-ERR Invalid Networks");
				printClient(sd,endMessage);
				continue;
			}
			
			paths = dijkstra(graph,src,&networkCount);

			path = paths[dst];

			hop = path.next_hop;

			if(!(hop))
			{
				strcpy(endMessage,"+OK -1");	
				printClient(sd,endMessage);
			}	

			strcpy(endMessage,"+OK ");
			sprintf(str,"%d", hop);
			strcat(endMessage,str);
			printClient(sd, endMessage);
		}
		
		
		else if (0 == strcmp(token,"ROUTE-TABLE"))
		{
			token = strtok(NULL, " ");
			src = atoi(token);

			if(!(find_vertex(graph,src)))
			{
				strcpy(endMessage, "-ERR Invalid Networks");
				printClient(sd,endMessage);
				continue;
			}
			
			
			paths = dijkstra(graph,src,&count);

			for(i=0;i<count;i++)
			{
				if(paths[i].next_hop != -1)
				{
					entries++;
				}
			}

			sprintf(str, "+OK %d", entries);
			printClient(sd,str);

			entries = 0;
			
			for(i=0;i<count;i++)
			{
				if(paths[i].next_hop != -1)
				{
					sprintf(endMessage, "%d -> %d, next-hop %d, weight %.0f", src, i, paths[i].next_hop, paths[i].weight);
					printClient(sd,endMessage);
				}
			}

		}
		
		else
		{
			strcpy(endMessage, "-ERR Not implemented");
			printClient(sd,endMessage);
		}

	}

}


void ReadLineFromNetwork(int sd, char *buf, int size)
 {
    
    char l[size];
    int n, i, j=0, cline=0;
    
    do
    {
	 n = read(sd, l, size);
	  
   for(i = 0; i < n; i++){
	    buf[j] = l[i];
	    if(buf[j] == 10 && buf[j-1] == 13){
			buf[j-1] = '\0';
			cline = 1;
			break;
	    }
	    j++;
	}
    } while(cline == 0 && n>0);
}

void printClient(int sd, char * string)
{
	char outBuffer[MAX_SIZE];
	
	strcat(string,"\r\n");
	sprintf(outBuffer, string);
	write(sd, outBuffer, strlen(outBuffer));
}
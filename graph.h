// graph.h
/*
General purpose graph structure.
Depth first search and breadth first search algorithms.
By Anton Babkin.
*/

#ifndef __GRAPH_H
#define __GRAPH_H

#include <stdbool.h>
#include <stdio.h>

#include "stack.h"

typedef struct Graph *GraphPtr;

/* Return pointer to new empty graph with specified number of vertices. */
GraphPtr graphCreate(int nVertices, bool directed);

/* 
Return pointer to new graph read from input stream.
Input format:
num_vertices num_edges
tail_1 head_1
tail_2 head_2
...
Vertices must be 0-indexed.
*/
GraphPtr graphCreateFromInputStream(FILE *in, bool directed);

/* Return pointer to new graph read from file. */
GraphPtr graphCreateFromFile(char const *file, bool directed);

/* Free allocated memory. */
void graphDestroy(GraphPtr g);

/* Insert edge tail->head of given length. 
For undirected graphs, use oneway=false to insert pair of edges tail->head and head->tail. */
void graphInsertEdge(GraphPtr g, int tail, int head, int length, bool oneway);

void graphPrint(GraphPtr g);

/* Depth first search. 
------------------------ */
typedef struct GraphDFS *GraphDFSPtr;

/* Run DFS from source and return results handle. */
GraphDFSPtr graphDFSCreate(GraphPtr g, int source);

/* Free allocated memory. */
void graphDFSDestroy(GraphDFSPtr d);

/* Return true if path from source vertex to v exists. */
bool graphDFSHasPathTo(GraphDFSPtr d, int v);

/* Return new stack handle with path from source to v, with source on top of the stack and v on bottom.
Returns NULL if path does not exist. */
StackPtr graphDFSPathTo(GraphDFSPtr d, int v);

/* Breadth first search. 
------------------------ */
typedef struct GraphBFS *GraphBFSPtr;

/* Run BFS from source and return results handle. */
GraphBFSPtr graphBFSCreate(GraphPtr g, int source);

/* Free allocated memory. */
void graphBFSDestroy(GraphBFSPtr d);

/* Return true if path from source vertex to v exists. */
bool graphBFSHasPathTo(GraphBFSPtr d, int v);

/* Return new stack handle with path from source to v, with source on top of the stack and v on bottom.
Returns NULL if path does not exist. */
StackPtr graphBFSPathTo(GraphBFSPtr d, int v);

void graphUnitTest(bool verbose);

#endif /* __GRAPH_H */
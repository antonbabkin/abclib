// graph.c
/*
Adjacency list representation.
By Anton Babkin.
*/

#include <stdlib.h>
#include <assert.h>

#include "graph.h"
#include "queue.h"

/* Adjacent edge - linked list node. */
typedef struct EdgeNode *EdgeNodePtr;
struct EdgeNode
{
    int head;
    int length;
    EdgeNodePtr next;
};

struct Graph
{
    int nVertices;
    int nEdges;
    bool directed;
    int *outDegree;
    /* edges[tail] is a linked list of edges adjacent to tail vertex. */
    EdgeNodePtr *edges;
};

GraphPtr graphCreate(int nVertices, bool directed)
{
    GraphPtr g = malloc(sizeof *g);
    g->nVertices = nVertices;
    g->nEdges = 0; /* will be dynamically incremented when edges are inserted */
    g->directed = directed;
    g->outDegree = calloc(nVertices, sizeof *(g->outDegree));
    g->edges = calloc(nVertices, sizeof *(g->edges));
    return g;
}

GraphPtr graphCreateFromInputStream(FILE *in, bool directed)
{
    int const DUMMY_LEN = 1;
    int nv, ne;
    fscanf(in, "%d %d", &nv, &ne);
    GraphPtr g = graphCreate(nv, directed);

    for (int i = 0; i < ne; i++)
    {
        int tail, head;
        fscanf(in, "%d %d", &tail, &head);
        graphInsertEdge(g, tail, head, DUMMY_LEN, directed);
    }

    assert(g->nEdges == ne);

    return g;
}

GraphPtr graphCreateFromFile(char const *file, bool directed)
{
    FILE *in = fopen(file, "r");
    GraphPtr g = graphCreateFromInputStream(in, directed);
    fclose(in);
    return g;
}

void graphDestroy(GraphPtr g)
{
    for (int tail = 0; tail < g->nVertices; tail++)
    {
        EdgeNodePtr edge = g->edges[tail];
        while (edge != NULL)
        {
            EdgeNodePtr nextEdge = edge->next;
            free(edge);
            edge = nextEdge;
        }
    }
    free(g->outDegree);
    free(g->edges);
    free(g);
}

/* Insert edge (x,y) into the graph. 
Use oneway==false to insert edges (x->y) and (y->x) for undirected graphs. */
void graphInsertEdge(GraphPtr g, int tail, int head, int length, bool oneway)
{
    EdgeNodePtr edge = malloc(sizeof *edge);
    edge->head = head;
    edge->length = length;
    edge->next = g->edges[tail];
    g->edges[tail] = edge; /* insert at head of list */
    g->outDegree[tail]++;
    if (!oneway)
    {
        graphInsertEdge(g, head, tail, length, true);
    }
    else
    {
        g->nEdges++;
    }
}

/* Print graph edges in order of storage.
Format:
tail: head_1(length_1), head_2(length_2) ... */
void graphPrint(GraphPtr g)
{
    printf("%sirected graph with %d vertices and %d edges:\n", g->directed ? "D" : "Und", g->nVertices, g->nEdges);
    for (int tail = 0; tail < g->nVertices; tail++)
    {
        printf("%d:", tail);
        for (EdgeNodePtr e = g->edges[tail]; e != NULL; e = e->next)
        {
            printf(" %d(%d)", e->head, e->length);
        }
        printf("\n");
    }
}

/* Depth first search. */
struct GraphDFS
{
    int s;
    bool *marked;
    int *edgeTo;
};

static void dfs(GraphPtr g, GraphDFSPtr d, int t)
{
    d->marked[t] = true;
    for (EdgeNodePtr e = g->edges[t]; e != NULL; e = e->next)
    {
        int h = e->head;
        if (!d->marked[h])
        {
            dfs(g, d, h);
            d->edgeTo[h] = t;
        }
    }
}

GraphDFSPtr graphDFSCreate(GraphPtr g, int source)
{
    GraphDFSPtr d = malloc(sizeof *d);
    d->s = source;
    int nv = g->nVertices;
    d->marked = malloc(nv * (sizeof *d->marked));
    d->edgeTo = malloc(nv * (sizeof *d->edgeTo));
    for (int i = 0; i < nv; i++)
    {
        d->marked[i] = false;
        d->edgeTo[i] = -1;
    }

    dfs(g, d, source);
    return d;
}

void graphDFSDestroy(GraphDFSPtr d)
{
    free(d->marked);
    free(d->edgeTo);
    free(d);
}

bool graphDFSHasPathTo(GraphDFSPtr d, int v)
{
    return d->marked[v];
}

StackPtr graphDFSPathTo(GraphDFSPtr d, int v)
{
    if (!graphDFSHasPathTo(d, v))
    {
        return NULL;
    }

    StackPtr s = stackCreate();
    for (int w = v; w != d->s; w = d->edgeTo[w])
    {
        stackPush(s, w);
    }
    stackPush(s, d->s);
    return s;
}

/* Breadth first search. */
struct GraphBFS
{
    int s;
    bool *marked;
    int *edgeTo;
};

GraphBFSPtr graphBFSCreate(GraphPtr g, int source)
{
    GraphBFSPtr d = malloc(sizeof *d);
    d->s = source;
    int nv = g->nVertices;
    d->marked = malloc(nv * (sizeof *d->marked));
    d->edgeTo = malloc(nv * (sizeof *d->edgeTo));
    for (int i = 0; i < nv; i++)
    {
        d->marked[i] = false;
        d->edgeTo[i] = -1;
    }

    QueuePtr q = queueCreate();
    queueAdd(q, source);
    d->marked[source] = true;
    while (!queueIsEmpty(q))
    {
        int t = queueRemove(q);
        for (EdgeNodePtr e = g->edges[t]; e != NULL; e = e->next)
        {
            int h = e->head;
            if (!d->marked[h])
            {
                d->marked[h] = true;
                d->edgeTo[h] = t;
                queueAdd(q, h);
            }
        }
    }

    queueDestroy(q);

    return d;
}

void graphBFSDestroy(GraphBFSPtr d)
{
    free(d->marked);
    free(d->edgeTo);
    free(d);
}

bool graphBFSHasPathTo(GraphBFSPtr d, int v)
{
    return d->marked[v];
}

StackPtr graphBFSPathTo(GraphBFSPtr d, int v)
{
    if (!graphBFSHasPathTo(d, v))
    {
        return NULL;
    }

    StackPtr s = stackCreate();
    for (int w = v; w != d->s; w = d->edgeTo[w])
    {
        stackPush(s, w);
    }
    stackPush(s, d->s);
    return s;
}

static void testDFS(GraphPtr g)
{
    /* Input graph13.txt has 3 connected components: {0,1,2,3,4,5,6}, {7,8}, {9,10,11,12}. */
    GraphDFSPtr d0 = graphDFSCreate(g, 0);
    GraphDFSPtr d7 = graphDFSCreate(g, 7);
    GraphDFSPtr d9 = graphDFSCreate(g, 9);

    for (int v = 0; v < 13; v++)
    {
        if (v < 7)
        {
            assert(graphDFSHasPathTo(d0, v));
            assert(!graphDFSHasPathTo(d7, v));
            assert(!graphDFSHasPathTo(d9, v));
        }
        else if (v < 9)
        {
            assert(!graphDFSHasPathTo(d0, v));
            assert(graphDFSHasPathTo(d7, v));
            assert(!graphDFSHasPathTo(d9, v));
        }
        else
        {
            assert(!graphDFSHasPathTo(d0, v));
            assert(!graphDFSHasPathTo(d7, v));
            assert(graphDFSHasPathTo(d9, v));
        }
    }

    assert(graphDFSPathTo(d0, 10) == NULL);
    assert(graphDFSPathTo(d7, 10) == NULL);
    assert(graphDFSPathTo(d9, 0) == NULL);

    /* Path: 0 -> 6 -> 4 -> 5. */
    StackPtr path = graphDFSPathTo(d0, 5);
    assert(stackPop(path) == 0);
    assert(stackPop(path) == 6);
    assert(stackPop(path) == 4);
    assert(stackPop(path) == 5);
    assert(stackIsEmpty(path));
    stackDestroy(path);

    /* Path: 9 -> 11 -> 12. */
    path = graphDFSPathTo(d9, 12);
    assert(stackPop(path) == 9);
    assert(stackPop(path) == 11);
    assert(stackPop(path) == 12);
    assert(stackIsEmpty(path));
    stackDestroy(path);

    graphDFSDestroy(d0);
    graphDFSDestroy(d7);
    graphDFSDestroy(d9);
}

static void testBFS(GraphPtr g)
{
    /* Input graph13.txt has 3 connected components: {0,1,2,3,4,5,6}, {7,8}, {9,10,11,12}. */
    GraphBFSPtr d0 = graphBFSCreate(g, 0);
    GraphBFSPtr d7 = graphBFSCreate(g, 7);
    GraphBFSPtr d9 = graphBFSCreate(g, 9);

    for (int v = 0; v < 13; v++)
    {
        if (v < 7)
        {
            assert(graphBFSHasPathTo(d0, v));
            assert(!graphBFSHasPathTo(d7, v));
            assert(!graphBFSHasPathTo(d9, v));
        }
        else if (v < 9)
        {
            assert(!graphBFSHasPathTo(d0, v));
            assert(graphBFSHasPathTo(d7, v));
            assert(!graphBFSHasPathTo(d9, v));
        }
        else
        {
            assert(!graphBFSHasPathTo(d0, v));
            assert(!graphBFSHasPathTo(d7, v));
            assert(graphBFSHasPathTo(d9, v));
        }
    }

    assert(graphBFSPathTo(d0, 10) == NULL);
    assert(graphBFSPathTo(d7, 10) == NULL);
    assert(graphBFSPathTo(d9, 0) == NULL);

    /* Path: 0 -> 5 -> 3. */
    StackPtr path = graphBFSPathTo(d0, 3);
    assert(stackPop(path) == 0);
    assert(stackPop(path) == 5);
    assert(stackPop(path) == 3);
    assert(stackIsEmpty(path));
    stackDestroy(path);

    /* Path: 9 -> 12. */
    path = graphBFSPathTo(d9, 12);
    assert(stackPop(path) == 9);
    assert(stackPop(path) == 12);
    assert(stackIsEmpty(path));
    stackDestroy(path);

    graphBFSDestroy(d0);
    graphBFSDestroy(d7);
    graphBFSDestroy(d9);
}

void graphUnitTest(bool verbose)
{

    GraphPtr g = graphCreateFromFile("graph13.txt", false);
    assert(g != NULL);

    if (verbose)
        graphPrint(g);

    testDFS(g);

    testBFS(g);

    graphDestroy(g);

    printf("Graph unit test completed successfully.\n");
}
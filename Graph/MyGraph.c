// Made By Sheng Du
// Z5171466
// COMP 9024 Assignment 4

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>


// A vertex is a 2D point
typedef struct Vertex {
	int x; // x-coordinate
	int y; // y-coordinate
} Vertex;
// each edge is a pair of vertices (end-points)
typedef struct Edge {
	Vertex* p1; // first end point
	Vertex* p2; // second end point
} Edge;
typedef struct VertexNode {
	Vertex* v;
	float lenth;
	int edge_index;
	struct VertexNode* next;
	int node_flag;
	int edge_falg;
	// other information
} VertexNode;
typedef struct GraphRep* Graph;
typedef struct GraphRep { // graph header
	VertexNode** vertices; // an array of vertices or a linked list of vertices  
	int nV; // #vertices
	int nE; // #edges
} GraphRep;
// A vertex node stores a vertex and other information, and you need to expand this type

// create a deque of vertexNode
typedef struct Node
{
	VertexNode* vertex;
	struct Node* next;
}Node;

typedef struct DequeLL
{
	Node* Head;
	Node* Tail;
	int num;
}DequeLL;

DequeLL* newDequeLL()
{
	DequeLL* Q;
	Node* NodeH;
	Node* NodeT;
	Q = malloc(sizeof(DequeLL));
	assert(Q != NULL);
	NodeH = malloc(sizeof(Node));
	assert(NodeH != NULL);
	NodeT = malloc(sizeof(Node));
	assert(NodeT != NULL);
	Q->Head = NodeH;
	Q->Head->vertex = NULL;
	Q->Head->next = NULL;
	Q->Tail = NodeT;
	Q->Tail->vertex = NULL;
	Q->Tail->next = NULL;
	Q->num = 0;
	return Q;
}

// create a binomial heap as priority queue
typedef struct heap_Node 
{
	VertexNode* vertex;
	double h;
}heap_Node;


typedef struct Heap 
{
	struct heap_Node** Node;
	int size;
}Heap;


Heap* newHeap(int n)
{
	Heap* Heap;
	Heap = malloc(sizeof(struct Heap));
	assert(Heap != NULL);
	Heap->size = 0;
	heap_Node* Nodes = (heap_Node*)calloc(n + 1, sizeof(heap_Node));
	Heap->Node = Nodes;
	return Heap;
}


heap_Node* new_heap_node(VertexNode *Node, Vertex *v)
{
	heap_Node* newHeapNode;
	newHeapNode = malloc(sizeof(heap_Node));
	assert(newHeapNode != NULL);
	newHeapNode->h = 0.0;
	VertexNode* newNode;
	newHeapNode->vertex = malloc(sizeof(VertexNode));
	assert(newHeapNode->vertex != NULL);
	newHeapNode->vertex = Node;
	return newHeapNode;
}


typedef struct Stack {
	Vertex* Node[20];
	int top;
}Stack;

Stack* CreateStack()
{
	Stack* stack;
	stack = malloc(sizeof(Stack));
	assert(stack != NULL);
	stack->top = 0;
	for (int i = 0; i < 20; i++) stack->Node[i] = NULL;
	return stack;
}

void push(Stack* stack, Vertex * Node)
{
	stack->Node[stack->top] = Node;
	stack->top++;
	return;
}

Vertex* pop(Stack* stack)
{
	if (!stack->top) return NULL;
	stack->top--;
	return stack->Node[stack->top];
}

void release(Stack* stack)
{
	stack->Node[stack->top] = NULL;
	return;
}

//The above types serve as a starting point only. You need to expand them and add more types. 
// Watch the lecture video between 7:50pm-8:20 or so for my discussion about this assignment    

// functions declaration
double get_lenth(Vertex* x, Vertex* y);
void set_edge_flag(VertexNode* Node, int x, int y);
void append_Deque(DequeLL* Deque, VertexNode* Node);
void pop_Deque(DequeLL* Deque);
VertexNode* find_Node(Graph g, Vertex* v);
void free_Nodes(VertexNode* Node);
void append_heap(Heap* H, VertexNode* start, VertexNode* Node, Vertex* v, double distance);
void check_up(heap_Node** Nodes, int size);
void Remove_min(Heap* H);
void find_path(Graph g, VertexNode* Vertex_s, VertexNode* Vertex_e, VertexNode* Node, Stack* stack, Heap* heap);

VertexNode* createVertexNode(Vertex *e_v)
{
	VertexNode* newNode;
	newNode = malloc(sizeof(VertexNode));
	assert(newNode != NULL);
	newNode->v = e_v;
	newNode->next = NULL;
	newNode->lenth = 0;
	newNode->edge_index = NULL;
	newNode->edge_falg = 0;
	newNode->node_flag = 0;
	return newNode;
}


// Add the time complexity analysis of CreateEmptyGraph() here
// CreateEmptyGraph runs in O(1), because it is an empty graph
Graph CreateEmptyGraph()
{
	Graph g;
	VertexNode *newVertexNode;
	g = malloc(sizeof(GraphRep));
	assert(g != NULL);
	g->nE = 0; g->nV = 0;
	g->vertices = (struct VertexNode*)calloc(100, sizeof(VertexNode));
	assert(g->vertices != NULL);
	for (int i = 0; i < 100; i++)
	{
		newVertexNode = malloc(sizeof(VertexNode));
		g->vertices[i] = newVertexNode;
		g->vertices[i]->lenth = 0;
		g->vertices[i]->edge_index = NULL;
		g->vertices[i]->next = NULL;
		g->vertices[i]->v = NULL;
		g->vertices[i]->edge_falg = 0;
		g->vertices[i]->node_flag = 0;
	}
	return g;
}

// Add the time complexity analysis of InsertEdge() here
// for n-Vertexes graph g ,InsertEdge runs in O(n + max_degree_of_Vertexs) 
// checking edge in graph g takes O(n + max_degree_of_Vertexs), n is the number of Vertexes
// insertion takes O(1) time.
int InsertEdge(Graph g, Edge* e)
{
	double lenth = get_lenth(e->p1, e->p2);
	VertexNode* Node;
	VertexNode* Node_j;
	// check if edge is in g
	for (int i = 0; i < g->nV; i++)			// for n-Vertexes graph g, worst case runs in O(n) n is the number of Vertexes
	{
		if (g->vertices[i]->v->x == e->p1->x && g->vertices[i]->v->y == e->p1->y)
		{
			Node = g->vertices[i];
			while (Node->next)				// runs in degree(Vertex)
			{
				Node = Node->next;
				if (Node->v->x == e->p2->x && Node->v->y == e->p2->y)
					return 0;
				if (Node->next) continue;
				// edge not in g, insert edge
				Node->next = createVertexNode(e->p2);
				Node->lenth = lenth;
				g->nE += 1;
				for (int j = 0; j < g->nV; j++)
				{
					if (g->vertices[j]->v->x == e->p2->x && g->vertices[j]->v->y == e->p2->y)
					{
						Node_j = g->vertices[j];
						while (Node_j->next) Node_j = Node_j->next;
						Node_j->next = createVertexNode(e->p1);
						Node_j->lenth = lenth;
						Node_j->edge_index = i;
						Node->edge_index = j;
						return 1;
					}
				}
				g->vertices[g->nV]->v = e->p2;
				g->vertices[g->nV]->lenth = lenth;
				g->vertices[g->nV]->next = createVertexNode(e->p1);
				g->vertices[g->nV]->edge_index = i;
				Node->edge_index = g->nV;
				g->nV += 1;
				return 1;
			}
		}
		if (g->vertices[i]->v->x == e->p2->x && g->vertices[i]->v->y == e->p2->y)
		{
			Node = g->vertices[i];
			while (Node->next)
			{
				Node = Node->next;
				if (Node->v->x == e->p1->x && Node->v->y == e->p1->y)
					return 0;
				if (Node->next) continue;
				// edge not in g, insert edge
				Node->next = createVertexNode(e->p1);
				Node->lenth = lenth;
				g->nE += 1;
				for (int j = 0; j < g->nV; j++)
				{
					if (g->vertices[j]->v->x == e->p1->x && g->vertices[j]->v->y == e->p1->y)
					{
						Node_j = g->vertices[j];
						while (Node_j->next) Node_j = Node_j->next;
						Node_j->next = createVertexNode(e->p2);
						Node_j->lenth = lenth;
						Node_j->edge_index = i;
						Node->edge_index = j;
						return 1;
					}
				}
				g->vertices[g->nV]->v = e->p1;
				g->vertices[g->nV]->lenth = lenth;
				g->vertices[g->nV]->next = createVertexNode(e->p2);
				g->vertices[g->nV]->edge_index = i;
				Node->edge_index = g->nV;
				g->nV += 1;
				return 1;
			}
		}
	}
	// if edge not in g, insert edge, return 1
	g->vertices[g->nV]->v = e->p1;
	g->vertices[g->nV]->lenth = lenth;
	g->vertices[g->nV]->next = createVertexNode(e->p2);
	g->vertices[g->nV]->edge_index = g->nV + 1;
	g->nV += 1;
	g->vertices[g->nV]->v = e->p2;
	g->vertices[g->nV]->lenth = lenth;
	g->vertices[g->nV]->next = createVertexNode(e->p1);
	g->vertices[g->nV]->edge_index = g->nV - 1;
	g->nV += 1;
	g->nE += 1;
	return 1;
}

// Add the time complexity analysis of DeleteEdge() here
// DeleteEdge runs in O(n + max_degree_of_Vertexs), n is the number of Vertexes
// finding edge in graph g takes O(n + max_degree_of_Vertexs) time
// Deletion takes O(1) time
void DeleteEdge(Graph g, Edge* e)
{
	VertexNode* Node;
	VertexNode* temp;
	int j;
	// check if edge in g
	for (int i = 0; i < g->nV; i++)
	{
		if (g->vertices[i]->v->x == e->p1->x && g->vertices[i]->v->y == e->p1->y)
		{
			Node = g->vertices[i];
			while (Node->next)
			{
				if (Node->next->v->x == e->p2->x && Node->next->v->y == e->p2->y)
				{
					temp = Node->next;
					Node->next = temp->next;
					j = Node->edge_index;
					Node->edge_index = temp->edge_index;
					Node->lenth = temp->lenth;
					free(temp);
					// delete edge copy
					Node = g->vertices[j];
					while (Node->next)
					{
						if (Node->next->v->x == e->p1->x && Node->next->v->x == e->p1->y)
						{
							temp = Node->next;
							Node->next = temp->next;
							Node->edge_index = temp->edge_index;
							Node->lenth = temp->lenth;
							g->nE -= 1;
							if (g->vertices[j]->next == NULL)
							{
								// delete Vertex
								;
							}
							return;
						}
						Node = Node->next;
					}
				}
				Node = Node->next;
			}
			continue;
		}
		if (g->vertices[i]->v->x == e->p2->x && g->vertices[i]->v->y == e->p2->y)
		{
			Node = g->vertices[i];
			while (Node->next)
			{
				if (Node->next->v->x == e->p1->x && Node->next->v->y == e->p1->y)
				{
					temp = Node->next;
					Node->next = temp->next;
					j = Node->edge_index;
					Node->edge_index = temp->edge_index;
					Node->lenth = temp->lenth;
					free(temp);
					// delete edge copy
					Node = g->vertices[j];
					while (Node->next)
					{
						if (Node->next->v->x == e->p2->x && Node->next->v->x == e->p2->y)
						{
							temp = Node->next;
							Node->next = temp->next;
							Node->edge_index = temp->edge_index;
							Node->lenth = temp->lenth;
							g->nE -= 1;
							if (g->vertices[j]->next == NULL)
							{	
								//delete_Vertex(g, j);
								; 
							}
							return;
						}
						Node = Node->next;
					}
				}
				Node = Node->next;
			}
			continue;
		}
	}
	return;
}

// Add the time complexity analysis of ReachableVertices() here
// for n-Vertexes graph g, ReachableVertices runs in O(n + max(degree(Vertex)))
void ReachableVertices(Graph g, Vertex* v)
{
	VertexNode* Node;
	DequeLL* Deque = newDequeLL();
	Node = find_Node(g, v);
	Node->node_flag = 1;
	// print Vertexs which links to Node and does not expended before
	// runs in degree(Vertex)
	while (Node->next)
	{
		printf("(%d,%d) ", Node->next->v->x, Node->next->v->y);
		append_Deque(Deque, g->vertices[Node->edge_index]);
		Node = Node->next;
	}
	// runs in O(n) n is the number of Vertexes 
	while (Deque->num)
	{
		Node = Deque->Head->vertex;
		Node->node_flag = 1;
		while (Node->next)
		{
			if (g->vertices[Node->edge_index]->node_flag == 1)
			{
				Node = Node->next;
				continue;
			}
			append_Deque(Deque, g->vertices[Node->edge_index]);
			g->vertices[Node->edge_index]->node_flag = 1;
			printf("(%d,%d) ", Node->next->v->x, Node->next->v->y);
			Node = Node->next;
			continue;
		}
		pop_Deque(Deque);
		continue;
	}
	printf("\n");
	printf("\n");
	// clear flag ,runs in O(Vertexes)
	for (int i = 0; i < g->nV; i++) g->vertices[i]->node_flag = 0;
	return;
}

// Add the time complexity analysis of ShortestPath() here
// for n-Vertexes and m-Edges graph g
// ShortestPath runs in O((m+n)*logn) time
// the worst case could be expended all Vertexes and Edges
// and any basic operations of heap takes O(logn) time
void ShortestPath(Graph g, Vertex* u, Vertex* v)
{
	Heap* heap;
	Stack *stack = CreateStack();
	heap = newHeap(g->nE);
	VertexNode* Vertex_s = find_Node(g, u);
	VertexNode* Vertex_e = find_Node(g, v);
	VertexNode* Node;
	Node = Vertex_s;
	// find path runs in O()
	find_path(g, Vertex_s, Vertex_e, Node, stack, heap);
	// runs in size(stack), 
	// stack contains the shoerest path
	while (stack->top)
	{
		Vertex * Vertex = pop(stack);
		printf("(%d,%d)", Vertex->x, Vertex->y);
		release(stack);
	}
	printf("\n");
	printf("\n");
	// clear flags
	for (int i = 0; i < g->nV; i++)
	{
		g->vertices[i]->edge_falg = 0; g->vertices[i]->node_flag = 0;
		Node = g->vertices[i];
		while (Node->next)
		{
			Node = Node->next;
			Node->edge_falg = 0;
			Node->node_flag = 0;
		}
	}
	return;
}

// find_path runs in O((n+m)logn)time
void find_path(Graph g, VertexNode *Vertex_s, VertexNode *Vertex_e, VertexNode*Node, Stack *stack, Heap* heap)
{
	// end condition, if Node riches the destination
	if (Node->v->x == Vertex_e->v->x && Node->v->y == Vertex_e->v->y)
	{
		Node->node_flag = 1;
		push(stack, Node->v);
		return;
	}
	VertexNode* tempV;
	tempV = Node;
	double distance = 0.0;
	if (Node->v->x == Vertex_s->v->x && Node->v->y == Vertex_s->v->y);
	else 
	{
		distance = heap->Node[1]->h - get_lenth(Node->v, Vertex_e->v);
		// operation of heap takes O(logn) time
		Remove_min(heap);
	}
	while (tempV->next)
	{
		if (tempV->edge_falg) { tempV = tempV->next; continue; };
		append_heap(heap, tempV, tempV->next, Vertex_e->v, distance);
		tempV->edge_falg = 1;
		set_edge_flag(g->vertices[tempV->edge_index],Node->v->x, Node->v->y);
		tempV = tempV->next;
	}
	while (heap->size)
	{
		tempV = g->vertices[heap->Node[1]->vertex->edge_index];
		find_path(g, Vertex_s, Vertex_e, tempV, stack, heap);
		tempV = Node;
		while(tempV->next)
		{
			if (g->vertices[tempV->edge_index]->node_flag == 1)
			{
				push(stack, Node->v);
				Node->node_flag = 1;
				return;
			}
			tempV = tempV->next;
		}
		return;
	}
}


// Add the time complexity analysis of FreeGraph() here
// for n-Vertexes and m-Edges graph g
// free Graph takes O(n+m)time
// for every Vertexes and Edges runs in O(1)
void FreeGraph(Graph g)
{
	for (int i = 0; i < g->nV; i++)
	{
		free_Nodes(g->vertices[i]);
		free(g->vertices[i]);
		g->vertices[i] = NULL;
	}
	for (int i = g->nV; i < 100; i++)
	{
		free(g->vertices[i]);
		g->vertices[i] = NULL;
	}
	free(g);
	g = NULL;
	return;
}

// Add the time complexity analysis of ShowGraph() here
// for n-Vertexes and m-Edges graph g
// ShowGraph runs in O(n+m) time
// for every Vertex and Edge, runs in O(1)
void ShowGraph(Graph g)
{
	if (g->nE == 0) return;
	VertexNode* Node;
	// creat deque Link List
	DequeLL* Deque = newDequeLL();
	append_Deque(Deque, g->vertices[0]);
	while (Deque->num)
	{
		Deque->Head->vertex->node_flag = 1;
		Node = Deque->Head->vertex;
		while (Node->next)
		{
			if (Node->edge_falg == 0)
			{
				printf("(%d,%d),(%d,%d)\n", Deque->Head->vertex->v->x, Deque->Head->vertex->v->y, Node->next->v->x, Node->next->v->y);
				Node->edge_falg = 1;
				set_edge_flag(g->vertices[Node->edge_index], Deque->Head->vertex->v->x, Deque->Head->vertex->v->y);
				if (g->vertices[Node->edge_index]->node_flag == 0) append_Deque(Deque, g->vertices[Node->edge_index]);
			}
			Node = Node->next;
		}
		pop_Deque(Deque);
	}
	for (int i = 1; i < g->nV; i++)
	{
		if (g->vertices[i]->node_flag == 1) continue;
		Deque = newDequeLL();
		append_Deque(Deque, g->vertices[i]);
		while (Deque->num)
		{
			Deque->Head->vertex->node_flag = 1;
			Node = Deque->Head->vertex;
			while (Node->next)
			{
				if (Node->edge_falg == 0)
				{
					printf("(%d,%d),(%d,%d)\n", Deque->Head->vertex->v->x, Deque->Head->vertex->v->y, Node->next->v->x, Node->next->v->y);
					Node->edge_falg = 1;
					set_edge_flag(g->vertices[Node->edge_index], Deque->Head->vertex->v->x, Deque->Head->vertex->v->y);
					if (g->vertices[Node->edge_index]->node_flag == 0) append_Deque(Deque, g->vertices[Node->edge_index]);
				}
				Node = Node->next;
			}
			pop_Deque(Deque);
		}
	}
	printf("\n");
	// clear flag
	for (int i = 0; i < g->nV; i++)
	{
		g->vertices[i]->edge_falg = 0; g->vertices[i]->node_flag = 0;
		Node = g->vertices[i];
		while (Node->next)
		{
			Node = Node->next;
			Node->edge_falg = 0;
			Node->node_flag = 0;
		}
	}
	return;
}

// Additional functions
// calculate the length of edge
double get_lenth(Vertex* x, Vertex* y)
{
	double d_x = x->x - y->x;
	double d_y = x->y - y->y;
	return sqrt(pow(d_x, 2) + pow(d_y, 2));
}


void set_edge_flag(VertexNode *Node, int x, int y)
{
	if (Node->next->v->x == x && Node->next->v->y == y)
	{
		Node->edge_falg = 1;
		return;
	}
	else set_edge_flag(Node->next, x, y);
	return;
}


void append_Deque(DequeLL *Deque, VertexNode *Node)
{
	if (Deque->num == 0)
	{
		Deque->Head->vertex = Node;
		Deque->Tail = Deque->Head;
		Deque->num = 1;
		return;
	}
	struct Node* newNode;
	newNode = malloc(sizeof(struct Node));
	newNode->vertex = Node;
	newNode->next = NULL;
	Deque->Tail->next = newNode;
	Deque->Tail = Deque->Tail->next;
	Deque->num += 1;
	return;
}


void pop_Deque(DequeLL *Deque)
{
	struct Node* Node;
	Node = Deque->Head;
	Deque->Head = Deque->Head->next;
	Deque->num -= 1;
	free(Node);
	Node = NULL;
	return;
}


VertexNode* find_Node(Graph g, Vertex *v)
{
	VertexNode* Node;
	for (int i = 0; i < g->nV; i++)
	{
		if (g->vertices[i]->v->x == v->x && g->vertices[i]->v->y == v->y)
		{
			Node = g->vertices[i];
			return Node;
		}
	}
	return NULL;
}


void free_Nodes(VertexNode *Node)
{
	while (Node->next->next)
	{
		free_Nodes(Node->next);
		Node->next->next = NULL;
	}
	free(Node->next);
	Node->next = NULL;
	return;
}


double heuristic(VertexNode *start,VertexNode *Node, Vertex* end)
{
	double d = get_lenth(Node->v, end);
	d += start->lenth;
	return d;
}


void append_heap(Heap* H,VertexNode* start, VertexNode* Node, Vertex* v, double distance)
{
	heap_Node* newNode = new_heap_node(start, v);
	if (H->size == 0)
	{
		H->Node[0] = NULL;
		H->Node[1] = newNode;
		H->Node[1]->h = heuristic(start, Node, v) + distance;
		H->size += 1;
		return;
	}
	H->size += 1;
	H->Node[H->size] = newNode;
	H->Node[H->size]->h = heuristic(start, Node, v) + distance;
	check_up(H->Node, H->size);
	return;
}


void check_up(heap_Node** Nodes, int size)
{
	if (size == 1) return;
	if (Nodes[size]->h < Nodes[size / 2]->h)
	{
		heap_Node* Node = Nodes[size];
		Nodes[size] = Nodes[size / 2];
		Nodes[size / 2] = Node;
		check_up(Nodes, size / 2);
	}
	return;
}


void check_down(heap_Node** Nodes, int size)
{
	int i = 1;
	while (i <= size)
	{
		if (Nodes[2 * i + 1]) // Node has right child and left child
		{
			if (Nodes[2 * i + 1]->h < Nodes[2 * i]->h)		// right child smaller
			{
				heap_Node* Node = Nodes[i];
				Nodes[i] = Nodes[2 * i + 1];
				Nodes[2 * i + 1] = Node;
				i = 2 * i + 1;
				continue;
			}
		}
		if (Nodes[2 * i])				// Node only has left child
		{
			if (Nodes[2 * i]->h < Nodes[i]->h)
			{
				heap_Node* Node = Nodes[i];
				Nodes[i] = Nodes[2 * i];
				Nodes[2 * i] = Node;
				i = 2 * i;
				continue;
			}
		}
		return;
	}
	return;
}


void Remove_min(Heap* H)
{
	heap_Node* Node = H->Node[1];
	if (H->size == 1)
	{
		H->Node[1] = NULL;
		H->size -= 1;
		return;
	}
	H->Node[1] = H->Node[H->size];
	H->Node[H->size] = Node;
	H->size -= 1;
	H->Node[H->size + 1] = NULL;
	check_down(H->Node, H->size);	// runs in O(logn)
	return;
}


int main() //sample main for testing 
{
	Graph g1;
	Edge* e_ptr;
	Vertex* v1, * v2;

	// Create an empty graph g1;
	g1 = CreateEmptyGraph();

	// Create first connected component 
	// Insert edge (0,0)-(0,10)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 0;
	v2->y = 10;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (0,0)-(5,6)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 5;
	v2->y = 6;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (0, 10)-(10, 10)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 10;
	v2->x = 10;
	v2->y = 10;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (0,10)-(5,6)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 10;
	v2->x = 5;
	v2->y = 6;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (0,0)-(5,4)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 5;
	v2->y = 4;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (5, 4)-(10, 4)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 5;
	v1->y = 4;
	v2->x = 10;
	v2->y = 4;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (5,6)-(10,6)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 5;
	v1->y = 6;
	v2->x = 10;
	v2->y = 6;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (10,10)-(10,6)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 10;
	v1->y = 10;
	v2->x = 10;
	v2->y = 6;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (10, 6)-(10, 4)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 10;
	v1->y = 6;
	v2->x = 10;
	v2->y = 4;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Create second connected component
	// Insert edge (20,4)-(20,10)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 20;
	v1->y = 4;
	v2->x = 20;
	v2->y = 10;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (20,10)-(30,10)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 20;
	v1->y = 10;
	v2->x = 30;
	v2->y = 10;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	// Insert edge (25,5)-(30,10) 	
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 25;
	v1->y = 5;
	v2->x = 30;
	v2->y = 10;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	if (InsertEdge(g1, e_ptr) == 0) printf("edge exists\n");

	//Display graph g1
	ShowGraph(g1);

	// Find the shortest path between (0,0) and (10,6) 
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 10;
	v2->y = 6;
	ShortestPath(g1, v1, v2);
	free(v1);
	free(v2);

	// Delete edge (0,0)-(5, 6)
	e_ptr = (Edge*)malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 5;
	v2->y = 6;
	e_ptr->p1 = v1;
	e_ptr->p2 = v2;
	DeleteEdge(g1, e_ptr);
	free(e_ptr);
	free(v1);
	free(v2);

	// Display graph g1
	ShowGraph(g1);

	// Find the shortest path between (0,0) and (10,6) 
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 10;
	v2->y = 6;
	ShortestPath(g1, v1, v2);
	free(v1);
	free(v2);

	// Find the shortest path between (0,0) and (25,5)
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2 = (Vertex*)malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x = 0;
	v1->y = 0;
	v2->x = 25;
	v2->y = 5;
	ShortestPath(g1, v1, v2);
	free(v1);
	free(v2);

	// Find reachable vertices of (0,0)
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v1->x = 0;
	v1->y = 0;
	ReachableVertices(g1, v1);
	free(v1);

	// Find reachable vertices of (20,4)
	v1 = (Vertex*)malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v1->x = 20;
	v1->y = 4;
	ReachableVertices(g1, v1);
	free(v1);

	// Free graph g1
	FreeGraph(g1);

	return 0;
}

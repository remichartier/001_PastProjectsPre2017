/*
 * 20131026_Week2Assignment.cpp
 * 
 * Copyright 2013 Remi <remi@remi-Latitude-D630>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


/*
Objective
Implement a Monte Carlo simulation that calculates the average shortest path in a graph. 
* The graph can be generated using a pseudo-random number generator to produce edges and their costs. 
* The shortest path algorithm will be Dijkstra’s.

Follow the style guidelines in the text, 
DONE --> * proper indention, 
DONE --> * one-line per statement, 
DONE -->? * good choice of identifiers will figure prominently in grading.


Graph algorithms and graph representation are a critical tool in CS. We want to create a graph as an ADT 
* (Abstract Data Type) using C++ classes. The basic problem will be to write Dijkstra’s algorithm as a class 
* member function (method in OO speak). 
* You should already know Dijkstra’s algorithm for the shortest path problem from prior experience, 
* but it will be reviewed in class. It is the basis for many route calculations and optimizations programs. 

There are 2 basic implementations used for graphs – one is edge lists, and the other is connectivity matrices. 
* You can decide which to use, but comment on your choice. 

Basic problem:  Write a set of constructors for declaring and initializing a graph. 
* DONE ---> An edge will have a positive cost that is its distance. 
* DONE ----> Have a procedure that produces a randomly generated set of edges with positive distances.  
* 				Assume the graphs are undirected. 
* DONE ---> The random graph procedure should have edge density as a parameter and distance range as a parameter. 
* DONE ---> So a graph whose density is 0.1 would have 10% of its edges picked at random and its edge distance 
* 			would be selected at random from the distance range. 
* DONE --> The procedure should run through all possible undirected edges, 
* 			say (i,j) and place the edge in the graph if a random probability 
* 			calculation is less than the density. 
* Compute for a set of randomly generated graphs an average shortest path.

Turn in:  
* 
* Printout of program, 
* 200 words on what you learned, 
* and output showing the average path length calculation. 
* Use densities: 20% and 40% on a graph of 50 nodes 
* with a distance range of 1.0 to 10.0.   
* To get an average path length, compute the 49 paths:

1 to 2, 1 to 3, 1 to 4, …, 1 to 50. 

[In an instance where there is no path between 1 and n, 
* omit that value from the average. 
* This should be very rare for the chosen density and size in this homework.]

Keep in mind: 
* good style – choice of identifiers, short functions, good documentation, 
* correctness and efficiency. Cite any references in creating this program.

Tips:  Hand-simulate your algorithm on a small graph.

Grading:

Submit the program for peer grading.

Nota Bene:  
* Keep in mind that there is a wide array of backgrounds in students taking this course. 
* From this perspective the student who is the intended target has a C background 
* and some computer science. 
* At this point in the class it is not expected that the student 
* know the detailed uses of STL and iterator classes. 
* When peer grading be generous, and read the 200 word explanation. 
* It is certainly okay for an experienced C++ programmer to use the most advanced elements 
* of the libraries and the language if properly explained in her code. 
* Conversely, it is also okay for the beginner to use a much simpler approach.  
* Problem 3 will continue with graph algorithms and will allow participants 
* to program in a more advanced and richer style.

 

Reference Abstractions:  
* 
* Implementing Dijkstra’s algorithm requires thinking about at least three basic abstractions: 
* Graph (G = (V, E), PriorityQueue, and ShortestPath algorithm.  
* Additionally, deciding on a scheme for naming vertices (V) is an important first step 
* in implementation design. 
* By convention, vertices are generally mapped onto the set of Integers in the range from 0 : |V| -1.  
* This provides an effective Key into sequential containers 
* (like ARRAY) to access vertex records in constant time - Θ(1). 
* Also, in models where vertex names are not represented with integers, 
* the use of a symbol table could be used to provide a 1-to-1 mapping 
* to associate V arbitrary names with V integers in the proper range.

A potential partial interface definition for a Graph could be:

    Class Graph:

        DONE --> V (G): returns the number of vertices in the graph
        DONE --> E (G): returns the number of edges in the graph
        DONE --> adjacent (G, x, y): tests whether there is an edge from node x to node y.
        DONE --> neighbors (G, x): lists all nodes y such that there is an edge from x to y.
        DONE --> add (G, x, y): adds to G the edge from x to y, if it is not there.
        DONE --> delete (G, x, y): removes the edge from x to y, if it is there.
        DONE --> get_node_value (G, x): returns the value associated with the node x.
        DONE --> set_node_value( G, x, a): sets the value associated with the node x to a.
        DONE --> get_edge_value( G, x, y): returns the value associated to the edge (x,y).
        DONE --> set_edge_value (G, x, y, v): sets the value associated to the edge (x,y) to v.

One important consideration for the Graph class is how to represent the graph as a member ADT. 
* Two basic implementations are generally considered: 
* adjacency list and adjacency matrix depending on the relative edge density. 
* For sparse graphs, the list approach is typically more efficient, 
* but for dense graphs, the matrix approach can be more efficient 
* (reference an Algorithm’s source for space and time analysis). 
* Note in some cases such as add(G, x, y) you may also want to have the edge carry along its cost. 
* Another approach could be to use (x, y) to index a cost stored in an associated array or map.

The value of the PriorityQueue is to always have access to the vertex 
* with the next shortest link in the shortest path calculation 
* at the top of the queue. A typically implementation is a minHeap:

DONE -->    Class PriorityQueue

DONE -->        chgPrioirity(PQ, priority): changes the priority (node value) of queue element.
DONE -->        minPrioirty(PQ): removes the top element of the queue.
DONE -->        contains(PQ, queue_element): does the queue contain queue_element.
DONE -->        insert(PQ, queue_element): insert queue_element into queue
DONE -->               top(PQ):returns the top element of the queue.
DONE -->        size(PQ): return the number of queue_elements.

Finally, the class: ShortestPathAlgo - implements the mechanics of Dijkstra’s algorithm. 
* Besides having member fields (has a relationship) of Graph and Priority Queue, 
* an additional ADT maybe required to maintain the parent relationship of the shortest path.

    Class ShortestPath

        vertices(List): list of vertices in G(V,E).
        path(u, w): find shortest path between u-w and returns the sequence of vertices representing shorest path u-v1-v2-…-vn-w.
        path_size(u, w): return the path cost associated with the shortest path.

The class implementing your Monte Carlo simulation is the workflow manager for this assignment, 
* but other helper classes may be necessary depending on your particular implementation

 Notes and Reminders:

    Write an appropriate set of constructors for each of your classes ensuring proper initialization 
    * – especially think about the process for declaring and initializing a graph. 
    * 
DONE :    In this implementation, assume that an edge will have a positive cost function like distance (no negative edge cost). 
    Assume the graph edges (E)  are undirected.
 DONE -->   Ensure that your ADTs support a graph of at least size 50.
DONE -->    The random graph procedure should have edge density as a parameter and distance range as a parameter.
DONE -->    Random graph generator should generate a sufficient set of edges 
DONE -->    * to satisfy the edge density parameter, and each edge should be assigned a randomly generated cost 
DONE -->    * based on the distance range parameter.
DONE -->    So a graph whose density is 0.1 would have 10% of its edges picked at random 
DONE -->    * and its edge distance would be selected at random from the distance range. 
    Compute for a set of randomly generated graphs an average shortest path.

    URL’s

    http://en.wikipedia.org/wiki/Dijkstra's_algorithm

    The time allotted for this problem is two weeks. 
    * Beginners should spend the first week creating a graph class with appropriate constructors. 
    * The second week can be used to implement and test Dijkstra’s algorithm. 

	Peer grading standards will be based on documentation, good style, correctness, and elegance. 
* 
* */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
using namespace std;

// Ensure that your ADTs support a graph of at least size 50.
const int SIZE_GRAPH = 50;

// Implementation chosen : Matrixes as we deal with a lot of Vertex (up to 50 here)...
class Vertice{ // For Vertice
	public :
		Vertice(int i):v(i){} 	//	Constructor
		Vertice():v(0){}		// Constructor overloading
		
		int v; // should be range 0 to SIZE_GRAPH-1
};

class Cost {
	
	public :
		Cost(double d):c(d){}
		Cost():c(0.0){}
		double c;
		
		// Method CostRange : returns random Cost between 2 ranges.
};


Cost randomCost(double min, double max) 	// edge distance would be selected at random from the distance range.
		{
			Cost r = Cost(0.0);
			r.c= min + (rand() * (max - min) / RAND_MAX);
			return(r);
		}

class QueueElement{

		public :
			QueueElement(Vertice a, Cost b):node(a),costNode(b){}
			Vertice node;
			Cost costNode;
};
		
/*
 * The value of the PriorityQueue is to always have access to the vertex 
* with the next shortest link in the shortest path calculation 
* at the top of the queue. A typically implementation is a minHeap:

    Class PriorityQueue

        chgPrioirity(PQ, priority): changes the priority (node value) of queue element.
        minPrioirty(PQ): removes the top element of the queue.
        contains(PQ, queue_element): does the queue contain queue_element.
        Insert(PQ, queue_element): insert queue_element into queue
        top(PQ):returns the top element of the queue.
        size(PQ): return the number of queue_elements.
 * */
 

class PriorityQueue{
	
	public:
	
		PriorityQueue() {	queue.clear();}// Constructor
		//~PriorityQueue(){~vector(queue)} // Destructor
		
		void insert(QueueElement q);	//        Insert(PQ, queue_element): insert queue_element into queue
		 
		bool chgPriority(Vertice vertice, Cost priority);  // chgPrioirity(PQ, priority): changes the priority (node value) of queue element.
		
		bool minPriority() ;  // minPriority(PQ): removes the top element of the queue.
		
		bool contains(Vertice vertice, Cost& cost);  // contains(PQ, queue_element): does the queue contain queue_element.
		
		QueueElement top();  // top(PQ):returns the top element of the queue.
		
		size_t size();  // size(PQ): return the number of queue_elements.
	
	private :
	
		vector<QueueElement> queue;
	
}; // End classe Priority Queue


//        Insert(PQ, queue_element): insert queue_element into queue
void PriorityQueue::insert(QueueElement q)
{
		queue.push_back(q);
}

// chgPrioirity(PQ, priority): changes the priority (node value) of queue element.
bool PriorityQueue::chgPriority(Vertice vertice, Cost priority)
{
	if(!queue.empty())
	{
		for(auto& element: queue)
		{
			if (element.node.v == vertice.v)
			{
				element.costNode.c = priority.c ;
				return true;
			}
		}
	}
	return false;
}

// minPriority(PQ): removes the top element of the queue.

bool PriorityQueue::minPriority() {
	
	if(queue.empty())
		return false;
	else
	{
		queue.pop_back();
		return true;
	}
}

// contains(PQ, queue_element): does the queue contain queue_element.
bool PriorityQueue::contains(Vertice vertice, Cost& cost)
{
	if(!queue.empty())
	{
		for(auto& element: queue)
		{
			if (element.node.v == vertice.v)
			{
				cost = element.costNode;
				return true;
			}
		}
	}
	return false;		
}

// top(PQ):returns the top element of the queue.
QueueElement PriorityQueue::top()
{
	return queue.back();
}

// size(PQ): return the number of queue_elements.
size_t PriorityQueue::size()
{
	return queue.size();
}


					
class Graph{
	
	public :
		// constructor : The random graph procedure should have edge density as a parameter and distance range as a parameter.
		// The random graph procedure should have edge density as a parameter and distance range as a parameter. 
		// Graph G = (V, E)
		// * The graph can be generated using a pseudo-random number generator to produce edges and their costs. 

		Graph(double density, double costmin, double costmax){

			/* The procedure should run through all possible undirected edges, 
			* * say (i,j) and place the edge in the graph if a random probability 
			* calculation is less than the density. 
			*/
			
			for(int i=0;i<SIZE_GRAPH; i++)
			{
				for(int j=0;j<i; j++)								// Graph undirected ie G[i][j] = G[j][i] with G[i][i]=0; ==> only scroll j<i and set the opposite side to equal.
				{													// say (i,j) and place the edge in the graph if a random probability
					if ((rand()/ RAND_MAX < density) &&  				// calculation is less than the density.
						(i!=j) )										// if i=j, Vertice to itself ==> cost should be 0.0.
						G[i][j] = randomCost(costmin, costmax);	// its edge distance would be selected at random from the distance range. 
					else
						G[i][j].c= 0.0;	
					
					G[j][i].c = G[i][j].c;							// Graph undirected ie G[i][j] = G[j][i] with G[i][i]=0;						
				} // end for(j) ...
					G[i][i].c= 0.0;								// Vertice to itself ==> cost should be 0.0.
					
			} // end for(i)...


		}; //end description constructor graph()
				
		int V();	// V (G): returns the number of vertices in the graph
					
		int E(); // E (G): returns the number of edges in the graph
				
		bool adjacent(Vertice x, Vertice y);	// adjacent (G, x, y): tests whether there is an edge from node x to node y.
			
		void neighbors(Vertice x, vector<Vertice>& nodes); // neighbors (G, x): lists all nodes y such that there is an edge from x to y.
			
		void add(Vertice x, Vertice y, Cost cost); // add (G, x, y, cost): adds to G the edge from x to y, if it is not there.
				
		void delete_edge(Vertice x, Vertice y);	// delete (G, x, y): removes the edge from x to y, if it is there.
				
		int get_node_value(Vertice x);	// get_node_value (G, x): returns the value associated with the node x.
				
		void set_node_value(Vertice& x, int a);	// set_node_value( G, x, a): sets the value associated with the node x to a
					
		Cost get_edge_value (Vertice x, Vertice y);	// get_edge_value( G, x, y): returns the value associated to the edge (x,y).
				
		void set_edge_value (Vertice x, Vertice y, Cost v); // set_edge_value (G, x, y, v): sets the value associated to the edge (x,y) to v.
	
	private:	
		
		//Cost G[SIZE_GRAPH][SIZE_GRAPH]; // matrix or vertices including distance between vertices.
		array<array<Cost,SIZE_GRAPH>, SIZE_GRAPH> G;
	
 }; // end class graph

		// V (G): returns the number of vertices in the graph
int Graph::V()
{
	int n=0;
	// nb or Vertices means any Vertice at least connected to another vertice ie 
	// if line i or column j has any values !=0 ==> connected and should check that no connected to itself 
	// ==> it is a Vertice to count.
	for(int i=0;i<SIZE_GRAPH; i++)
	{
		for(int j=0;j<i; j++)								// Graph undirected ie G[i][j] = G[j][i] with G[i][i]=0; ==> only scroll j<i and set the opposite side to equal.
		{													// say (i,j) and place the edge in the graph if a random probability
			if (G[i][j].c != 0.0)
				n++;									
		} // end for(j) ...
	} // end for(i)...
	return(n);
} 
	
	// E (G): returns the number of edges in the graph
int Graph::E()
{
	int n=0;
	for(int i=0;i<SIZE_GRAPH; i++)
	{
		for(int j=0;j<SIZE_GRAPH; j++)
		{			
			if (G[i][j].c!=0.0)
				n++;
		} // end for(j) ...	
	} // end for(i)...
	return(n);
} 

// adjacent (G, x, y): tests whether there is an edge from node x to node y.
bool Graph::adjacent(Vertice x, Vertice y) 
{
	if (x.v == y.v) 				// no edge to a vertice/node to itself
		return(false);
	else
	{
		if (G[x.v][y.v].c != 0.0)
			return(true);
		else
			return(false);
	} // end else
		
}	

// neighbors (G, x): lists all nodes y such that there is an edge from x to y.
void Graph::neighbors(Vertice x, vector<Vertice>& nodes)
{
	// scroll line or column from vertice x - excluding vertice x- and create list nodes if Vertice x to vertice y has a cost !=0).
	for(int i=0;i<SIZE_GRAPH;i++)
	{
			if ((i != x.v) && (G[x.v][i].c !=  0.0 ))
			{
				Vertice vertice = Vertice(i);
				//vertice.v=i;
				nodes.push_back(vertice);
			}
	}
	
} 

// add (G, x, y, cost): adds to G the edge from x to y, if it is not there.
void Graph::add(Vertice x, Vertice y, Cost cost)
{
	if (G[x.v][y.v].c == 0.0)
	{
		G[x.v][y.v].c = cost.c; // add at (x,y)
		G[y.v][x.v].c = cost.c; // and also at (y,x), matrix effect - mirroring row,column and column, row
	}
}

// delete (G, x, y): removes the edge from x to y, if it is there.
void Graph::delete_edge(Vertice x, Vertice y)
{
	if (G[x.v][y.v].c != 0.0)
	{
		G[x.v][y.v].c = 0.0; // add at (x,y)
		G[y.v][x.v].c = 0.0; // and also at (y,x), matrix effect - mirroring row,column and column, row
	}
}

// get_node_value (G, x): returns the value associated with the node x.
int Graph::get_node_value(Vertice x)
{
	return x.v;
}

// set_node_value( G, x, a): sets the value associated with the node x to a.
void Graph::set_node_value(Vertice& x, int a)
{
	x.v = a ;
}

// get_edge_value( G, x, y): returns the value associated to the edge (x,y).
Cost Graph::get_edge_value (Vertice x, Vertice y)
{
	return G[x.v][y.v];
}

// set_edge_value (G, x, y, v): sets the value associated to the edge (x,y) to v.
void Graph::set_edge_value (Vertice x, Vertice y, Cost v)
{
	G[x.v][y.v].c = G[y.v][x.v].c = v.c; // set (row,Column) and (Column,row) for symetrical matrix ....
}

/*
 * an additional ADT maybe required to maintain the parent relationship of the shortest path.

    Class ShortestPath

        vertices(List): list of vertices in G(V,E).
        path(u, w): find shortest path between u-w and returns the sequence of vertices representing shortest path u-v1-v2-…-vn-w.
        path_size(u, w): return the path cost associated with the shortest path.

 * 
 */

class ShortestPath{
	
	public :
	
		ShortestPath(Graph g) {vertices.clear();};	// constructor
		
		// path(u, w): find shortest path between u-w and returns the sequence of vertices representing shortest path u-v1-v2-…-vn-w.
		void path(Vertice u, Vertice v, vector<Vertice>& shortestpath_u_v) {};
		
		// path_size(u, w): return the path cost associated with the shortest path.
		void path_size(Vertice u, Vertice w, Cost& c_u_w) {};
		
	private :
	
		vector<Vertice> vertices;
	
};

 /*
 * Finally, the class: ShortestPathAlgo - implements the mechanics of Dijkstra’s algorithm. 
* Besides having member fields (has a relationship) of Graph and Priority Queue, 
*/

	

class ShortestPathAlgo{
	
	
	public :
		void set_graph(Graph gr);
		void set_start(Vertice st);
		void set_dest(Vertice dest);
		Cost computeShortestPath();
	private : 	
		Graph g;
		PriorityQueue pq; // ClosedSet
		PriorityQueue oq; // OpenSet
		Vertice S; // origine
		Vertice T; // target/destination
		
		
};

//ShortestPathAlgo::ShortestPathAlgo(Graph gr, Vertice a, Vertice b){this->g=gr;this->S=a;this->T=b;}		// makes plenty of compilations errors ....
void ShortestPathAlgo::set_graph(Graph gr) {this->g=gr;}
void ShortestPathAlgo::set_start(Vertice st) {this->S=st;}
void ShortestPathAlgo::set_dest(Vertice dest) {this->T=dest;}

Cost ShortestPathAlgo::computeShortestPath()
{
	Cost shortest, tempCost;
	Vertice currentNode (S.v);  // initialise currentNode to Start Vertice - S.
	Cost currentCost(0.0);			// initialise currentCost to 0 For StartVertice S.
	// I will take Start Node and will put it in PriorityQueue ...
	// QueueElement Elem=QueueElement(S, Cost(0.0)); // start by Start Node S at cost 0 and put in the Priority Queue.
	// Now I put it in Priority Queue -->
	// void insert(QueueElement q);	//        Insert(PQ, queue_element): insert queue_element into queue
	pq.insert(QueueElement(S, Cost(0.0)));			// --> yes, work !
	
	
	// now, I need to study all the nodes connected to S, and put them in the OpenSet, until Note T target gets into Priority List ...
	while (pq.contains(T, tempCost) == false)
	{
		// study all the nodes connected to current Vertice S, and put them in the OpenSet
		// neighbors (G, x): lists all nodes y such that there is an edge from x to y.
		// void Graph::neighbors(Vertice x, vector<Vertice>& nodes)
		// all nodes directly connected to S will be given by g.neighbors(S, vector of vertices)
		vector<Vertice> listNodes;
		g.neighbors(S, listNodes);
		// is there neighboors ? yes if listNodes.size() !=0. If yes, will fill open list with Cost. if not ===> no solution for ShortestPath ....
		int n(listNodes.size()) ;
		if (n != 0)
		{
				// Exisiting neighbors to the current node --> add them in Open List if not yet existing, 
				// if already in Open List, only update the cost is new accumulated cost from S < existing cost ....
				for(auto node:listNodes) // ie scroll for all nodes into neighboor list
				{
					// Search if current node exists in OpenList. If not, add to open list with calculation of cost. (ie cost of S + cost to this Node ...)
					if(oq.contains(node, tempCost) == false)
					{
						oq.insert(QueueElement(node, Cost(currentCost.c + tempCost.c)));
					}
					else // contained already in OpenList ===> only update the cost is new accumulated cost from S < existing cost ....
					{	
						double temp=currentCost.c + (g.get_edge_value(S,node)).c;
						if(tempCost.c > temp)
						{
							// 	chgPrioirity(PQ, priority): changes the priority (node value) of queue element.
							//	bool PriorityQueue::chgPriority(Vertice vertice, Cost priority)
							oq.chgPriority(node, Cost(temp));
						}
					}
				}
		} // if neighboors exists.
		
		else // no neighboors to current node ....
		{
			// in this case it should study from priority list where it can go, but here I have absolutely to program a recursive approach ...
		}
		
		
	}
	
	
	
	return shortest;
}

double shortestPathAverage(ShortestPath s)
{
		double shortestPathAverage(0.0);
		int n(0);
		Cost c;
		double sum(0);

		for (int i=1; i<SIZE_GRAPH-1;i++)
		{
			s.path_size(Vertice(0), Vertice(i), c)	;
			if (c.c!= 0.0)
			{
					sum += c.c;
					n++;
			}
			
			if (n!=0)
			{
				shortestPathAverage = sum / n ;
			}
			
		}
		
		return shortestPathAverage ;
}

int main(int argc, char **argv)
{
		
				
		srand(clock());
					
		// Use densities: 20% and 40% on a graph of 50 nodes 
		Graph g1=Graph(0.2, 1.0, 10.0);
		
		Graph g2=Graph(0.4, 1.0, 10.0);
		
		ShortestPath s1=ShortestPath(g1);
		ShortestPath s2=ShortestPath(g2);
		
		// Compute for a set of randomly generated graphs an average shortest path.
		// with a distance range of 1.0 to 10.0.   
		//	* To get an average path length, compute the 49 paths:
		//	1 to 2, 1 to 3, 1 to 4, …, 1 to 50. 
		// [In an instance where there is no path between 1 and n, 
		//	* omit that value from the average. 
		//	* This should be very rare for the chosen density and size in this homework.]
		
		// Calculation average shortest path for g1 and g2
		cout << "Calculation average shortest path for g1 density 20% : ";
		cout << shortestPathAverage(s1) << endl;
		cout << "Calculation average shortest path for g2 density 40% : ";
		cout << shortestPathAverage(s2) << endl;

		
		
	return 0;
}



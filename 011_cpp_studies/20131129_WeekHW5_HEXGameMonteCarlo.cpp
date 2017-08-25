

/* 
Objective

Homework 5 expectations:

    The computer should be able to play Hex intelligently against a human on an 11 by 11 board.
    Efficiently determine within no more than 2 minutes what the computer's move is.

This program will evaluate a position using a Monte Carlo selection of moves until the board is filled up. 
* 
* Then using work of Homework 4 you determine who won.  
* 
* The program takes turns. 
* 
* It inputs the human (or machine opponent if playing against another program) move. 
* 
* When it is the “AI”’s turn, it is to evaluate all legal available next moves and select a “best” move.  
* 
* Each legal move will be evaluated using  ~1000 or more trials. 
* 
* Each trial winds the game forward by randomly selecting successive moves until there is a winner. 
* 
* The trial is counted as a win or loss. The ratio: wins/trials are the AI’s metric for picking which next move to make. 

*/




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




#include <iostream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#include <string>
#include <chrono>		// for see and shuffle
#include <random>
#include <algorithm>    // std::minmax_element
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */


using namespace std;

// Ensure that your ADTs support a graph of at least size 50.

const int DEBUG_ON = false ;
const int DEBUG_ON1 = false ;
const int DEBUG_isPlayerWining = false ;
const int DEBUG_computerPicks = false ;
const int DEBUG_compileHW5 = false ;
const int MIN_SIZE_BOARD = 2 ;
const int MAX_SIZE_BOARD = 11;
const int SIZE_GRAPH = MAX_SIZE_BOARD * MAX_SIZE_BOARD;
// added for Hw5 Montecarlo hexBoard stat loop
const int MONTECARLO_LOOP = 1000 ;

enum class Content{EMPTY, BLUE, RED, POSITION}; 
enum class Player{COMPUTER,PLAYER};
enum class ComputerChoice{RANDOM, MONTECARLO}; 

// Implementation chosen : Matrixes as we deal with a lot of Vertex (up to 50 here)...
class Vertice{ // For Vertice
	public :
		// Vertice(int i){v=i} 	//	Constructor
		Vertice():v(0){}		// Constructor overloading
		Vertice(int i):v(i){}		// Constructor overloading
		int v; // should be range 0 to SIZE_GRAPH-1
};

class Cost {
	
	public :
		Cost(double d):c(d){}
		Cost():c(0.0){}
		double c;
		
		// Method CostRange : returns random Cost between 2 ranges.
};


Cost randomCost(double min, double max) {	// edge distance would be selected at random from the distance range.
		
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
	
		~PriorityQueue(){queue.~vector();} // Destructor

		PriorityQueue(){	queue.clear();}// Constructor

		
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
void PriorityQueue::insert(QueueElement q){
		queue.push_back(q);
}

// chgPrioirity(PQ, priority): changes the priority (node value) of queue element.
bool PriorityQueue::chgPriority(Vertice vertice, Cost priority){
	if(!queue.empty()){
		for(auto& element: queue){
			if (element.node.v == vertice.v){
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
	else{
		queue.pop_back();
		return true;
	}
}

// contains(PQ, queue_element): does the queue contain queue_element.
bool PriorityQueue::contains(Vertice vertice, Cost& cost) {
	if(!queue.empty()){
		for(auto& element: queue){
			if (element.node.v == vertice.v){
				cost = element.costNode;
				return true;
			}
		}
	}
	return false;		
}

// top(PQ):returns the top element of the queue.
QueueElement PriorityQueue::top() {
	return queue.back();
}

// size(PQ): return the number of queue_elements.
size_t PriorityQueue::size() {
	return queue.size();
}

/********************
*					*
* 	Graph CLASS		*
* 					*
*********************/ 

					
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
			
			for(int i=0;i<SIZE_GRAPH; i++){
				for(int j=0;j<i; j++){								// Graph undirected ie G[i][j] = G[j][i] with G[i][i]=0; ==> only scroll j<i and set the opposite side to equal.
																	// say (i,j) and place the edge in the graph if a random probability
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
		
		Graph(){

			/* The procedure should run through all possible undirected edges, 
			* * say (i,j) and place the edge in the graph if a random probability 
			* calculation is less than the density. 
			*/
			
			for(int i=0;i<SIZE_GRAPH; i++){
				for(int j=0;j<SIZE_GRAPH; j++){								// Graph undirected ie G[i][j] = G[j][i] with G[i][i]=0; ==> only scroll j<i and set the opposite side to equal.
																	// say (i,j) and place the edge in the graph if a random probability
					G[i][j].c= 0.0;					
				} // end for(j) ...
	
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
		
		void print();
		void print(int depth);
	
	private:	
		
		//Cost G[SIZE_GRAPH][SIZE_GRAPH]; // matrix or vertices including distance between vertices.
		array<array<Cost,SIZE_GRAPH>, SIZE_GRAPH> G;
	
 }; // end class graph

		// V (G): returns the number of vertices in the graph
int Graph::V() {
	int n=0;
	// nb or Vertices means any Vertice at least connected to another vertice ie 
	// if line i or column j has any values !=0 ==> connected and should check that no connected to itself 
	// ==> it is a Vertice to count.
	for(int i=0;i<SIZE_GRAPH; i++){
		for(int j=0;j<i; j++){								// Graph undirected ie G[i][j] = G[j][i] with G[i][i]=0; ==> only scroll j<i and set the opposite side to equal.
															// say (i,j) and place the edge in the graph if a random probability
			if (G[i][j].c != 0.0)
				n++;									
		} // end for(j) ...
	} // end for(i)...
	return(n);
} 
	
	// E (G): returns the number of edges in the graph
int Graph::E() {
	int n=0;
	for(int i=0;i<SIZE_GRAPH; i++){
		for(int j=0;j<SIZE_GRAPH; j++){			
			if (G[i][j].c!=0.0)
				n++;
		} // end for(j) ...	
	} // end for(i)...
	return(n);
} 

// adjacent (G, x, y): tests whether there is an edge from node x to node y.
bool Graph::adjacent(Vertice x, Vertice y)  {
	if (x.v == y.v) 				// no edge to a vertice/node to itself
		return(false);
	else{
		if (G[x.v][y.v].c != 0.0)
			return(true);
		else
			return(false);
	} // end else
		
}	

// neighbors (G, x): lists all nodes y such that there is an edge from x to y.
void Graph::neighbors(Vertice x, vector<Vertice>& nodes) {
	// scroll line or column from vertice x - excluding vertice x- and create list nodes if Vertice x to vertice y has a cost !=0).
	for(int i=0;i<SIZE_GRAPH;i++) {
			if ((i != x.v) && (G[x.v][i].c !=  0.0 )) {
				Vertice vertice = Vertice(i);
				//vertice.v=i;
				nodes.push_back(vertice);
			}
	}
	
} 

// add (G, x, y, cost): adds to G the edge from x to y, if it is not there.
void Graph::add(Vertice x, Vertice y, Cost cost) {
	if (G[x.v][y.v].c == 0.0){
		G[x.v][y.v].c = cost.c; // add at (x,y)
		G[y.v][x.v].c = cost.c; // and also at (y,x), matrix effect - mirroring row,column and column, row
	}
}

// delete (G, x, y): removes the edge from x to y, if it is there.
void Graph::delete_edge(Vertice x, Vertice y) {
	if (G[x.v][y.v].c != 0.0){
		G[x.v][y.v].c = 0.0; // add at (x,y)
		G[y.v][x.v].c = 0.0; // and also at (y,x), matrix effect - mirroring row,column and column, row
	}
}

// get_node_value (G, x): returns the value associated with the node x.
int Graph::get_node_value(Vertice x) {
	return x.v;
}

// set_node_value( G, x, a): sets the value associated with the node x to a.
void Graph::set_node_value(Vertice& x, int a){
	x.v = a ;
}

// get_edge_value( G, x, y): returns the value associated to the edge (x,y).
Cost Graph::get_edge_value (Vertice x, Vertice y){
	return G[x.v][y.v];
}

// set_edge_value (G, x, y, v): sets the value associated to the edge (x,y) to v.
void Graph::set_edge_value (Vertice x, Vertice y, Cost v){
	G[x.v][y.v].c = G[y.v][x.v].c = v.c; // set (row,Column) and (Column,row) for symetrical matrix ....
}

// print graph to cout
void Graph::print() {
		Graph::print(SIZE_GRAPH);	
}

// print graph to cout
void Graph::print(int depth) {
		cout << "PRINTOUT OF GRAPH :" << endl << "===================" << endl << endl ;

		for(int i=0;i<depth; i++){
				for(int j=0;j<depth; j++){								// Graph undirected ie G[i][j] = G[j][i] with G[i][i]=0; ==> only scroll j<i and set the opposite side to equal.
																	// say (i,j) and place the edge in the graph if a random probability
					printf("%3d ", (int)G[i][j].c);					
				} // end for(j) ...
				cout << endl;
			} // end for(i)...
	
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
		
		// Destructors PriorityQueue
//		pq.~PriorityQueue();
//		PriorityQueue::~vector(pq);
//		oq.~PriorityQueue();
		
		
};

//ShortestPathAlgo::ShortestPathAlgo(Graph gr, Vertice a, Vertice b){this->g=gr;this->S=a;this->T=b;}		// makes plenty of compilations errors ....
void ShortestPathAlgo::set_graph(Graph gr) {this->g=gr;}
void ShortestPathAlgo::set_start(Vertice st) {this->S=st;}
void ShortestPathAlgo::set_dest(Vertice dest) {this->T=dest;}

Cost ShortestPathAlgo::computeShortestPath() {
	Cost shortest, tempCost;
	Vertice currentNode (S.v);  // initialise currentNode to Start Vertice - S.
	Cost currentCost(0.0);			// initialise currentCost to 0 For StartVertice S.
	// I will take Start Node and will put it in PriorityQueue ...
	// QueueElement Elem=QueueElement(S, Cost(0.0)); // start by Start Node S at cost 0 and put in the Priority Queue.
	// Now I put it in Priority Queue -->
	// void insert(QueueElement q);	//        Insert(PQ, queue_element): insert queue_element into queue
	pq.insert(QueueElement(S, Cost(0.0)));			// --> yes, work !
	
	
	// now, I need to study all the nodes connected to S, and put them in the OpenSet, until Note T target gets into Priority List ...
	while (pq.contains(T, tempCost) == false){
		// study all the nodes connected to current Vertice S, and put them in the OpenSet
		// neighbors (G, x): lists all nodes y such that there is an edge from x to y.
		// void Graph::neighbors(Vertice x, vector<Vertice>& nodes)
		// all nodes directly connected to S will be given by g.neighbors(S, vector of vertices)
		vector<Vertice> listNodes;
		g.neighbors(S, listNodes);
		// is there neighboors ? yes if listNodes.size() !=0. If yes, will fill open list with Cost. if not ===> no solution for ShortestPath ....
		int n(listNodes.size()) ;
		if (n != 0){
				// Exisiting neighbors to the current node --> add them in Open List if not yet existing, 
				// if already in Open List, only update the cost is new accumulated cost from S < existing cost ....
				for(auto node:listNodes){ // ie scroll for all nodes into neighboor list
				
					// Search if current node exists in OpenList. If not, add to open list with calculation of cost. (ie cost of S + cost to this Node ...)
					if(oq.contains(node, tempCost) == false){
						oq.insert(QueueElement(node, Cost(currentCost.c + tempCost.c)));
					}
					else {// contained already in OpenList ===> only update the cost is new accumulated cost from S < existing cost ....
						
						double temp=currentCost.c + (g.get_edge_value(S,node)).c;
						if(tempCost.c > temp){
							// 	chgPrioirity(PQ, priority): changes the priority (node value) of queue element.
							//	bool PriorityQueue::chgPriority(Vertice vertice, Cost priority)
							oq.chgPriority(node, Cost(temp));
						}
					}
				}
		} // if neighboors exists.
		
		else {// no neighboors to current node ....
			// in this case it should study from priority list where it can go, but here I have absolutely to program a recursive approach ...
		}
		
#if 0
		// release vectors memory ...
		listNodes.~vector();	
#endif
	}
	
	
	
	return shortest;
}

double shortestPathAverage(ShortestPath s) {
		double shortestPathAverage(0.0);
		int n(0);
		Cost c;
		double sum(0);

		for (int i=1; i<SIZE_GRAPH-1;i++){
			s.path_size(Vertice(0), Vertice(i), c)	;
			if (c.c!= 0.0){
					sum += c.c;
					n++;
			}
			
			if (n!=0){
				shortestPathAverage = sum / n ;
			}
			
		}
		
		return shortestPathAverage ;
}

/********************
*					*
* 	HexBoard CLASS	*
* 					*
*********************/ 

class HexBoard{
	
	public :


		HexBoard( int sizeboard ); 				// declaration constructor.
		void print(); 							// print an HexBoard.
		bool isMoveLegal(int x, int y);			// Method analysing move.
		bool movePlayer(Content color,int x, int y);	// Method to move a player
		bool isPlayerWining(Content color, int x, int y); // Method to check if player is winning.
		
		// added for HW5 (HexMonteCarlo) :
		void GetListEmptyPositions(vector<int>&);
		void getVerticeHexBoardPoint(Vertice ver, int& x, int& y) { 
			x = ver.v / size;
			y = ver.v % size;
		};
		int getSize() {return size;}
		
	private :
	
		Graph g;
		int size;
		array<array<Content,MAX_SIZE_BOARD>, MAX_SIZE_BOARD> contentMatrix;
		

		// method to check if point part of border Line ==> non-corner edge has 4 neighbors.
		bool isBorderLineUp(int x, int y){
					if( (x==0) ) {return true;}
			return false;	
		};

		bool isBorderLineUp(Vertice ver){
			int x,y;
			getVerticeHexBoardPoint(ver, x, y);
			return(	isBorderLineUp(x,y) );
		};
				
		bool isBorderLineDown(int x, int y){
			if( (x==size-1) ) {return true;}
			return false;	
		};

		bool isBorderLineLeft(int x, int y){
			if( (y==0) ) {return true;}
			return false;	
		};

		bool isBorderLineRight(int x, int y){
			if( (y==size-1) ) {return true;}
			return false;	
		};

		bool isCornerUpLeft(int x, int y){
			if( (x==0) && (y==0) ) {return true;}
			return false;	
		};

		bool isCornerUpRight(int x, int y){
			if( (x==0) && (y==size-1) ) {return true;}
			return false;	
		};

		bool isCornerDownRight(int x, int y){
			if( (x==size-1) && (y==size-1) ) {return true;}
			return false;	
		};

		bool isCornerDownLeft(int x, int y){
			if( (x==size-1) && (y==0) ) {return true;}
			return false;	
		};
		
		// Method set vertice number according to position in HEXBoard
		Vertice setVerticeHexBoard(int x, int y) { 
			return Vertice( (x*size) + y);
		};
		

			
		bool isVerticeInList(Vertice ver, vector<Vertice> List){
			
			for (vector<Vertice>::iterator p = List.begin(); p != List.end(); ++p) {
				
				if( p->v == ver.v  ) {return true;}

			}
			return false;
		}
		
}; // end class HexBoard

// Constructor HexBoard
HexBoard::HexBoard( int sizeboard ){   // constructor
	
	size=sizeboard;
	if(DEBUG_ON) {cout << "Build the connections between Vertices ie build the neighboors."  << endl;} 
	if(DEBUG_ON) {cout << "Sizeboard : "  << size << " meaning " << size*size << "points/nodes. " << endl;} 
	// should build the connections between Vertices ie build the neighboors.
		for(int i=0; i<size; i++){
			for(int j=0; j<size; j++){
				
									
				// Corners has either two or three neighbors 
				if(isCornerUpLeft(i,j)){
					// this one has connect with (0,1) and (1,0)
					if(DEBUG_ON1) {cout << "Build CornerUpLeft. " ;} 
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(0,j+1), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i+1,j), Cost(1.0));

				}
				else if( isCornerDownLeft(i,j)){
					// this one has connect with up and right and diagonal up 
					if(DEBUG_ON1) {cout << "Build CornerDownLeft. " ;} 
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i-1,j), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i,j+1), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i-1,j+1), Cost(1.0));
				}
				else if( isCornerUpRight(i,j)){
					if(DEBUG_ON1) {cout << "Build CornerUpRight. " ;} 
					// this one has connect with left and down and diagonal down left 
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i,j-1), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i+1,j), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i+1,j-1), Cost(1.0));					
				}					
				else if( isCornerDownRight(i,j)){
					// this one has connect with up and left 
					if(DEBUG_ON) {cout << "Build CornerDownRight. " ;} 
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i-1,j), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i,j-1), Cost(1.0));
					// for bug
//							g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i,j+1), Cost(0.0));
					
				}
				else if(isBorderLineUp(i,j)){ // if not a corner but border up : non-corner edge has 4 neighbors.
				
					if(DEBUG_ON1) {cout << "Build BorderLineUp. " ;} 
					// this one has connect with left, right, down left, down 
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i,j-1), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i,j+1), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i+1,j-1), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i+1,j), Cost(1.0));
				}
				else if(isBorderLineDown(i,j)) { // if not a corner but border down : non-corner edge has 4 neighbors.
				
					// this one has connect with left, right, up, up right
					if(DEBUG_ON1) {cout << "Build BorderLineDown. " ;} 
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i,j-1), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i,j+1), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i-1,j), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i-1,j+1), Cost(1.0));
				}
				else if(isBorderLineLeft(i,j)) {// if not a corner but border left : non-corner edge has 4 neighbors.
				
					if(DEBUG_ON1) {cout << "Build BorderLineLeft. " ;} 
					// this one has connect with up, down, up right, right
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i-1,j), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i+1,j), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i-1,j+1), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i,j+1), Cost(1.0));

				}
				else if(isBorderLineRight(i,j)) { // if not a corner but border right : non-corner edge has 4 neighbors.
				
					// this one has connect with up, down, left, left down
					if(DEBUG_ON1) {cout << "Build BorderLineRight. " ;} 
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i-1,j), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i+1,j), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i,j-1), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i+1,j-1), Cost(1.0));
				}				
				else { // internal node ==> has connect with left, right, up, down, diagonale righ up, diag left down.
					if(DEBUG_ON1) {cout << "Build InternalNode. ";} 
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i-1,j), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i+1,j), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i,j-1), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i,j+1), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i-1,j+1), Cost(1.0));
					g.set_edge_value(setVerticeHexBoard(i,j), setVerticeHexBoard(i+1,j-1), Cost(1.0));
				}

		}
	}	

	// should fill the vertices contents with Content::EMPTY.
	if(DEBUG_ON) {cout << "Fill the vertices contents with Content::EMPTY." << endl;} 	
			for(int i=0; i<size; i++){
				for(int j=0; j<size; j++){
					contentMatrix[i][j] = Content::EMPTY;
				}			
			}
			
			// positions first player with a X on (0,0)
			contentMatrix[0][0] = Content::POSITION;
		
}; //end constructor HexBoard()

// Method to check if required move is legal 
bool HexBoard::isMoveLegal(int x, int y) {

		// logically prior to calling the method, main have already checked x and y within range and boarders of the board.
		// but can check it again ...
		if ( (x <0) || (y < 0) || (x>= this->size) || (y >= this-> size) ) {return false;}
		else {
			if ((contentMatrix[x][y] == Content::EMPTY) || (contentMatrix[x][y] == Content::POSITION)) {return true;}
			else return false;
		}
};

// method to move on a spot on the HexBoard
bool HexBoard::movePlayer(Content color,int x, int y){	// Method to move a player

	if( (color==Content::RED) || (color == Content::BLUE)){
		this->contentMatrix[x][y] = color ;
		return true;
	}
	else
		return false;

};
		
// method to print the HexBoard
void HexBoard::print(void){
	
	// Will start by first line,
	// then each even line will study down and diag link and print them.
	// then each odd lines, will print the nodes, offsets by columns/lines numbers ...
	string columnOffset="";
	
	cout << "Current picture of HEX Board : size : " << size << endl << "==============================" << endl ;
	// First line : print node then print link ... second line, add offset and links ...
	// The offset will be : i or i+1
	for( int i=0; i<size; i++){
		// print offset first
		cout << columnOffset ;
		for(int j=0;j<size;j++){
			
			// scroll matrix of vertices, print content and line in between ....
			if(contentMatrix[i][j] == Content::EMPTY) {cout << ". ";}
			else if(contentMatrix[i][j] == Content::RED) {cout << "R ";}
			else if(contentMatrix[i][j] == Content::BLUE) {cout << "B ";}
			else if(contentMatrix[i][j] == Content::POSITION) {cout << "X ";}
			
			// Now study link to the right
			// Cost  get_edge_value (Vertice x, Vertice y)
			// int get_node_value(Vertice x);	// get_node_value (G, x): returns the value associated with the node x.				
			// void set_node_value(Vertice& x, int a);	// set_node_value( G, x, a): sets the value associated with the node x 	to a
			//	bool adjacent(Vertice x, Vertice y);	// adjacent (G, x, y): tests whether there is an edge from node x to node y.
			if(j!=size-1) { if (g.adjacent( setVerticeHexBoard(i,j),setVerticeHexBoard(i,j+1)  ) ){cout << "- ";}}
			}
		
		// end of line --> insert end of line, and increase the offset for following line.
		cout << endl;
		columnOffset = columnOffset + " ";
		cout << columnOffset ;
		// now study adjacent links of previous line and display them.
		for(int j=0;j<size;j++){
			// only study the down neighboor for first item do not study down left neighboor for first item in line
			if(j!=0){
				if(g.adjacent( setVerticeHexBoard(i,j),setVerticeHexBoard(i+1,j-1)  ) ){cout << "/ ";}
			}
			// now study if neighboor down --> print a "/ "
			if(g.adjacent( setVerticeHexBoard(i,j),setVerticeHexBoard(i+1,j)  ) ){cout << "\\ ";}
			//cout << " ";
		}
		// end of row --> cout endl + offset for the following line.
		cout << endl ;
		columnOffset = columnOffset + " ";
	}
	
} // end method HexBoard::print()

// method to check if there is a winner
bool HexBoard::isPlayerWining(Content color, int currentx, int currenty){
	
	// here based on current latest position we check :
	// for the reds, if (x,y) belong to a path connecting up and down points.
	// for the blues, if (x,y) belong to a path connecting west and east points.
	vector<Vertice> verticesNeighboors;
	vector<Vertice> verticesKnown;
	bool hasNeighboorBlueEast(false);
	bool hasNeighboorBlueWest(false);
	bool hasNeighboorRedTop(false);
	bool hasNeighboorRedBottom(false);
	bool isRedWinner(false);
	bool isBlueWinner(false);
	Vertice currentVertice(setVerticeHexBoard(currentx, currenty));
	bool conditionExit(false) ;
	vector<Vertice> foundVerNeighboors;
	//static int counter(0);
	
	string keywait;
		
	//1. Add current (x,y) in list of VerticeKnow.
	
	verticesNeighboors.push_back(currentVertice);
	
//	if(DEBUG_isPlayerWining) { cout << "verticesNeighboors.back() : " << verticesNeighboors.back() << "  " ; }
	
	do{
		//counter++;
		//if (counter % 10000 == 0) {cout << "*" ;}
		// identify neighboors 
		// take last neighboor.
		currentVertice = verticesNeighboors.back();
//		if(DEBUG_isPlayerWining) { cout << "currentVertice = " << currentVertice.v << endl; }
		// add to verticesKnown list if not in this list yet.
		if(!isVerticeInList(currentVertice, verticesKnown)){
			verticesKnown.push_back(currentVertice);
//			if(DEBUG_isPlayerWining) {cout << "add currentVertice to verticesKnown " << currentVertice.v << endl; }
		}
		// check if from borders or corners ..., if yes, update the bool flags accordingly.
		int x,y;
		getVerticeHexBoardPoint(currentVertice, x, y);
		if(color==Content::RED) {
			hasNeighboorRedTop |= isBorderLineUp(x,y);
			hasNeighboorRedBottom |= isBorderLineDown(x,y);
		}
		
		if(color==Content::BLUE) {
			hasNeighboorBlueWest |= isBorderLineLeft(x,y);
			hasNeighboorBlueEast |= isBorderLineRight(x,y);
		}
		// remove current from verticesNeighboors list
		verticesNeighboors.pop_back();

		// search its identical color neighboors and add to Neighboor lists while updating the flags as well.
		// Will use the graph methods to search neighboors of current vertices, then will search if matches the color.
		// if matches the color, check if in the Knownlist, if not, add to neighboorlist ...
		
		// Graph::neighbors(Vertice x, vector<Vertice>& nodes)
		g.neighbors(currentVertice, foundVerNeighboors)	;	
		
		// now, identify identical color neighboors and add them to neighboor list if needed and if not yet in know list.
		for (vector<Vertice>::iterator p = foundVerNeighboors.begin(); p != foundVerNeighboors.end(); ++p) {
				int xf,yf;
				getVerticeHexBoardPoint(*p, xf, yf);
				if(( contentMatrix[xf][yf] ==  color ) && (!isVerticeInList(*p, verticesKnown)) && (!isVerticeInList(*p, verticesNeighboors))) {
					// then add to Neighboor list.
					verticesNeighboors.push_back(*p);
					//if(DEBUG_isPlayerWining) {cout << "add vertice to verticesNeighboors because did not find it in VerticesKnown " << currentVertice.v << endl; }
				}
		}
		
		// empty foundVerNeighboors because do not use anymore for this loop.
		foundVerNeighboors.clear();
		
		isRedWinner = ((color==Content::RED) & hasNeighboorRedTop & hasNeighboorRedBottom) ;
		
		isBlueWinner = ((color==Content::BLUE) & hasNeighboorBlueEast & hasNeighboorBlueWest) ;
		
		conditionExit = isBlueWinner |  isRedWinner | verticesNeighboors.empty() ;
		
		
	} while (!conditionExit) ;	

#if 0	
	if(DEBUG_isPlayerWining) { 
		cout << " isRedWinner " << isRedWinner << " hasNeighboorRedTop " << hasNeighboorRedTop << " hasNeighboorRedBottom " << hasNeighboorRedBottom << " ; ";
		
		cout << " isBlueWinner " << isBlueWinner << " hasNeighboorBlueEast " << hasNeighboorBlueEast << " hasNeighboorBlueWest " << hasNeighboorBlueWest << " ; ";
		
		//cin >> keywait ;
		
		}
#endif	
#if 0
	// empty vectors because do not use anymore for this loop.
	//verticesKnown.clear();
	verticesKnown.~vector();
	//verticesNeighboors.clear();
	verticesNeighboors.~vector();
	foundVerNeighboors.~vector();
#endif	
	if (isRedWinner || isBlueWinner) return true;
	else
		return false;
		
		
	
} // end method HexBoard::isPlayerWining()

// method to get list of Empty positions in the HexBoard. // added for HW5 (HexMonteCarlo) :
void HexBoard::GetListEmptyPositions(vector<int>&  ListEmptyPos){
	ListEmptyPos.clear() ;
	
	// Scroll matrix contentMatrix over sizeBoard and add empty positions to the list.
	for( int i=0 ; i < this->size ; i++){
		for(int j=0 ; j < this->size ; j++)
		{
			if( this->isMoveLegal(i,j) ){
				ListEmptyPos.push_back( (i*this->size) + j   );
			}
		}
	}

} // end GetListEmptyPositions()

/********************************
*								*
* 	prepareGame()	HW5			*
* 								*
*********************************/ 

int prepareGame(int& size_board, HexBoard& hexBoard, Content& colorPlayer, Content& colorComputer,Player& nextPlayer){
	
	string colorPicked ;
	string playAgain ;
	
	do{
		cout << "Please specify board size X * X. X= ? ; X should be >= " << MIN_SIZE_BOARD << " and <= " << MAX_SIZE_BOARD << " : " << endl ;
		cin >> size_board ;	
	} while ( (size_board < MIN_SIZE_BOARD) || (size_board > MAX_SIZE_BOARD) ) ;

	if(DEBUG_ON) {cout << "Size board chosen : " << size_board << endl;} 
	
	// Now should build the graph representing the board.
	
	if(DEBUG_ON) {cout << "Create HexBoard " << endl;} 
	hexBoard = HexBoard(size_board);
	
	// Now, should print exemple of HexBoard.
		
	if(DEBUG_ON) {cout << "hexBoard.print() " << endl;} 
	hexBoard.print();
		
	// Ask if wants to play
	do {
		cout << "Do you want to play Y/N ? " ;
		cin >> playAgain;
	} while( (playAgain[0]!='Y') && (playAgain[0]!='y') && (playAgain[0]!='n') && (playAgain[0]!='N')    );
	
	if( (playAgain[0] == 'n') || (playAgain[0] == 'N') ) return 0;
	
	cout << "HEX GAME RULES : Blue player must make a connected set of blue hexagons from east to west. The red player must do the same from north to south." << endl << endl;
	// Ask pick color
	do {
		cout << "Please pick your color Blue or Red (Blue starting first) : B/R ? " ;
		cin >> colorPicked;
	} while( (colorPicked[0]!='B') && (colorPicked[0]!='b') && (colorPicked[0]!='r') && (colorPicked[0]!='R')    );
	
	
	cout << "You chose color " ;
	if( (colorPicked[0]=='B') || (colorPicked[0]=='b') ){
		cout << "BLUE, so you will play first. ";
		colorPlayer = Content::BLUE ;
		colorComputer = Content::RED;
		nextPlayer = Player::PLAYER ;
	}
	else {
		cout << "RED, so the computer will play first. ";
		colorPlayer = Content::RED ;
		colorComputer = Content::BLUE;
		nextPlayer = Player::COMPUTER;
	}
	
	string key;
	cout << "Enter any key then ENTER when ready. ";
	cin >> key;

	return 1 ;
	
} // end prepareGame() funciont


/************************************
*									*
* 	positionAnalysis()	HW5			*
* 									*
*************************************/ 

int positionAnalysis(Content color, string splayer, string oponent, HexBoard& hexBoard, int x, int y, Player& nextPlayer){
	
	string playAgain; 
	
	// determine if move is legal ....
	if (hexBoard.isMoveLegal(x,y)) {
	
		// fill the spot.
		if( hexBoard.movePlayer(color,x,y) ) {
				
			hexBoard.print();
			cout << "Computer is analysing the move. Is it a Win move ? " ;

			if (hexBoard.isPlayerWining(color,x,y)){
				// splayer won
				cout << splayer << " WON - WON - WON - "<< endl << " GAME OVER - GAME OVER - GAME OVER" << endl ;
				return 0;
			}
			else{
				cout << "...... Answer : Not yet. Next is for " << oponent << " to play." << endl ;
			}	

		}
		else
			cout << "Not identified Error moving choice from " << splayer << endl ;
		
		if ( splayer == "COMPUTER") { nextPlayer = Player::PLAYER ;}
		else {
			nextPlayer = Player::COMPUTER ;
			cout << "Continue Playing ? Y/N : " << endl;
			cin >> playAgain ;
			if 	((playAgain[0]=='N') || (playAgain[0]=='n'))
				return 0 ;
		}

	}
	else
	{
		cout << "Move not legal, (x,y) not empty, " << splayer << " should select another position. " ;
/*		if ( splayer == "COMPUTER") { nextPlayer = Player::PLAYER ;}
		else {
			nextPlayer = Player::COMPUTER ;
		}
*/
	}

	return 1 ;
}

/****************************
*							*
* 	playerPicks() HW5		*
* 							*
*****************************/ 

void playerPicks(int size_board, int& x, int& y) {	

	cout << "Enter position to move with 2 numbers (x,y) : " ;
	do {
		cout << "Enter x : ";
		cin >> x;
		if(x<0) { cout << x << " not valid for x, x should be >= 0.";}
		else if(x>=size_board) {cout << x << " not valid for x, x should be < boardsize(" << size_board << "). ";} 
	} while( (x<0) || (x>=size_board) );
	do {
		cout << "Enter y : ";
		cin >> y;
		if(y<0) { cout << y << " not valid for x, x should be >= 0.";}
		else if(y>=size_board) {cout << y << " not valid for y, y should be < boardsize(" << size_board << "). ";} 
	} while( (y<0) || (y>=size_board) );

	
} // end function PlayerPicks()

/****************************************
*										*
* 	isColorWinning()		HW5			*
* 										*
*****************************************/

bool isColorWinning(Content color, Content oponent, HexBoard board){
	
	int x, y ;
	bool result = false;
	int sizeb = board.getSize() ;
	
	// increment counter if Computer has winning path.
					// if Computer wins, WincountEmptyPos[i] ++;
					// but how to determine if it is a win for Computer ?  --> 
					// depending of color computer :
					// for the reds, if (x,y) belong to a path connecting up and down points.
					// for the blues, if (x,y) belong to a path connecting west and east points.
					// For Reds, scroll all bottom positions. if Computer has, then call  // HexBoard::isPlayerWining(Content color, int currentx, int currenty)
					// For Blues, scroll all east positions, if computer has then call HexBoard::isPlayerWining(Content color, int currentx, int currenty). Count number of wins for this position.
					
	// re-use previous HW4 function : bool HexBoard::isPlayerWining(Content color, int currentx, int currenty){
	
	// for reds, check all bottom line positions, and if for each red, is it connected to top. if find one, return true immediately.
	if (color == Content::RED){
		x = sizeb -1 ;
		y = 0;
		while( (result == false) && (y < sizeb ) ){
			result = board.isPlayerWining(color, x, y)	;
			y ++ ;
		}
	}
	else // color = Content::BLUE
	{	// for blues, check all right border positions, and each for each blue, it is connected to left border. If find on, return true immediately.
		y = sizeb -1 ;
		x = 0;
		while( (result == false) && (x < sizeb) ){
			result = board.isPlayerWining(color, x, y)	;
			x ++ ;
		}
	}
	
	return result ;

} // end function isColorWinning()	

/****************************************
*										*
* 	ComputeMonteCarloStat()	HW5			*
* 										*
*****************************************/
int ComputMonteCarloStat(HexBoard hexBoardTest, vector<int> sublistEmptyPos, Content colorComputer, Content colorPlayer){
	
	int count(0);
	
	vector<Content> listContent(sublistEmptyPos.size(), Content::EMPTY) ;
	vector<Content> testList(sublistEmptyPos.size(), Content::EMPTY) ;
	Content currentColor = colorPlayer ;					
		
	// obtain a time-based seed:
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		
	// populate rest of the board equally with Computer or Player's move.
		// distribute equally possible moves of Computer and Player.
	for(size_t j=0;j<listContent.size();j++){
			listContent[j] = currentColor ;
			if (currentColor == colorComputer) { currentColor = colorPlayer; } else { currentColor = colorComputer; }
	}
	
	// statistic MonteCarlo loop : MONTECARLO_LOOP
	for(size_t mc=0; mc < MONTECARLO_LOOP; mc++)
	{
		// copy listContent to reshuffle from same list each time.
		testList = listContent ;
		string s;
		
		
		// shuffle the test list of content
		shuffle (testList.begin(), testList.end(), std::default_random_engine(seed));

					// fill the virtual testhexBoard empty cells with this shuffle list of content
		for(size_t j=0;j<testList.size();j++){
			int x,y;
			hexBoardTest.getVerticeHexBoardPoint(Vertice(sublistEmptyPos[j]), x, y) ;
			hexBoardTest.movePlayer(listContent[j],x, y) ; 
		}
		if(DEBUG_isPlayerWining) { 
			//hexBoardTest.print();
		}
		// Now needs to identify if computer wins or not.
		if( isColorWinning(colorComputer, colorPlayer, hexBoardTest) ) {
			count ++ ;
//			if(DEBUG_isPlayerWining) {cout << " position wining " ;  }
		}
							
	} // end for 		for(size_t mc=0; mc < MONTECARLO_LOOP, mc++)	

#if 0	
	// release vectors memory ...
	listContent.~vector() ;
	testList.~vector();
#endif	
	if(DEBUG_isPlayerWining) {
		cout << endl << " ComputMonteCarloStat : ComputMonteCarloStat counts " << count << "number of wins for this position. " << endl ;
	}
	return count ;
} // end function ComputMonteCarloStat


/************************************************
*												*
* 	ComputeMonteCarloStatOnPosition() HW5		*
* 												*
*************************************************/
void ComputeMonteCarloStatOnPosition( size_t indexpos, vector<int> listEmptyPos, HexBoard hexBoardTest, vector<int>& WincountEmptyPos, Content colorComputer, Content colorPlayer){

	if(DEBUG_compileHW5) {cout << "Enter ComputeMonteCarloStatOnPosition() function." << endl ; cout.flush(); }
	// Build hexBoardTest with current evaluated position filled.
	int a, b ;
	hexBoardTest.getVerticeHexBoardPoint(Vertice(listEmptyPos[indexpos]), a, b) ;
	hexBoardTest.movePlayer(colorComputer,a, b) ; 

	// Construct SubList of Empty position without current evaluated position
	vector<int> sublistEmptyPos = listEmptyPos;			
	sublistEmptyPos.erase(sublistEmptyPos.begin() + indexpos);
				
	// Compute Win stat for this configuration
	// vector<int> WincountEmptyPos[indexpos] = ComputMonteCarloStat (HexBoard hexBoardTest, vector<int> sublistEmptyPos, Content colorComputer, Content colorPlayer) ;
	WincountEmptyPos[indexpos] = ComputMonteCarloStat( hexBoardTest, sublistEmptyPos, colorComputer, colorPlayer) ;
	if(DEBUG_isPlayerWining) {
		cout << endl << " ComputeMonteCarloStatOnPosition : receive wining counts for this position " << indexpos << " : " << WincountEmptyPos[indexpos] << endl ;
	}
#if 0	
	// release vectors memory ...
	sublistEmptyPos.~vector() ;
#endif	
		
} // end ComputeMonteCarloStatOnPosition()



/****************************
*							*
* 	computerPicks()	HW5		*
* 							*
*****************************/ 
void computerPicks(int size_board, int& x, int& y,ComputerChoice choice, HexBoard hexBoard, Content colorComputer, Content colorPlayer){
	
	time_t timer;
	struct tm y2k;
	double seconds, start, stop;

	y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
	y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	time(&timer);  /* get current time; same as: timer = time(NULL)  */

	start = difftime(timer,mktime(&y2k));
	
	// hexBoard in input so computer can analyse current situation and choose its best pick.
	
	cout << endl << "NOW COMPUTER PLAYS ... timer in seconds going through each positions possibilities + winning chance indicator over "<< MONTECARLO_LOOP << " trials : " << endl ;
	
	if ( choice == ComputerChoice::RANDOM ) {
		x = rand() % (size_board - 1);
		y = rand() % (size_board - 1);
	}
	else // ComputerChoice=MONTECARLO
	{
		// Homework 5 : IA using Algo MonteCarlo to choose next computer move.
		
		// Algo MONTECARLO : 
		// For each empty postion :
		// Analyse % of Win of occupy this position. By filling randomly/shuffle equally between Player and computer, then creating the graph and // analysing who wins ...making % over 1000 shuffles. then picks highest % position to win ...
		// Fill rest of empty positions equally between color but shuffle those choice to do it random.
		
		// 1. Look if neighboor choices triggers direct Win. If yes, then go for it.
		// taking the hexBoard, for each computer's current positions, is a neighboor's direct winner ?

		
		// 2. Otherwise Look if Oponent direct neighboors would lead directly to a win for him. If yes, choose this position.
		
		// 3. Otherwise Can I extend my longuest partial path ?
		
		// 4. Otherwise can I block my oponentś longues partial path ?
		
		// 5. Otherwise Can I extend or make a bridge move from a path ?
		
		// 6. Otherwise Can I move to a row (column) that I do not occupy ?
		
		// 7. Othersise I use MonteCarloMetrics to choose computer's best next move.
			// 7.1 Make list of empty positions based on current graph :

			vector<int> listEmptyPos ;
			hexBoard.GetListEmptyPositions(listEmptyPos) ;
			
			if (DEBUG_computerPicks) { 	
				cout << "computerPicks() : listEmptyPos " ;	for (vector<int>::iterator p = listEmptyPos.begin(); p != listEmptyPos.end(); ++p) 	{	cout <<  *p << "; ";	} cout << endl;	
			}
			
			// Counter counting computer wins for each possible positions.
			vector<int> winCountEmptyPos (listEmptyPos.size(),0); 
			
			// create hexBoardTest - to test all the MonteCarlo stats.
			HexBoard hexBoardTest = hexBoard ;
							
			// 7.2 For each empty position, 
			for (size_t i=0 ; i<listEmptyPos.size(); i++){	
				
				time(&timer);  /* get current time; same as: timer = time(NULL)  */
				stop = difftime(timer,mktime(&y2k));
				seconds = stop - start;
				cout << "." << seconds << "s";cout.flush();
				// ComputeMonteCarloStatOnPosition( size_t indexpos, vector<int> listEmptyPos, HexBoard hexBoardTest, vector<int>, vector<int> WincountEmptyPos) ;
				ComputeMonteCarloStatOnPosition( i, listEmptyPos, hexBoardTest, winCountEmptyPos, colorComputer, colorPlayer) ;
				if(DEBUG_isPlayerWining) {
					cout << endl << " computerPicks : receive wining counts for this position " << i << " : " << winCountEmptyPos[i] << endl ;
				}
				cout << winCountEmptyPos[i] ;
				
			} 	
				
			// End of For each empty position : pick computer's position by looking at higher winning count position.
			
			// Now, winCountEmptyPos should have stats. We need to take the position with maximum stat and make it the computer choice and move !
			auto presult = std::max_element(winCountEmptyPos.begin(),winCountEmptyPos.end());
			int posMax = presult -winCountEmptyPos.begin() ;
			// bool HexBoard::movePlayer(Content color,int x, int y){	// Method to move a player
			// // Method to check if required move is legal bool HexBoard::isMoveLegal(int x, int y) {
			// 		void getVerticeHexBoardPoint(Vertice ver, int& x, int& y) { 
			hexBoard.getVerticeHexBoardPoint( Vertice(listEmptyPos[posMax]),x,y);
			
#if 0			
		// release memory allocated, vectors, etc ...:
		// winCountEmptyPos.clear() ;
		winCountEmptyPos.~vector() ;
		
		// listEmptyPos.clear() ;
		listEmptyPos.~vector() ;
#endif
		
	} // end // ComputerChoice=MONTECARLO
	
	cout << "COMPUTER CHOSE (" << x << "," << y << "). " << endl ;
	
} // end function computerPicks()


/************************
*						*
* 	MAIN()	HW5			*
* 						*
*************************/ 

int main(int argc, char **argv)
{
	srand(clock());

	int size_board, x, y, ret;
	Content colorPlayer=Content::EMPTY, colorComputer=Content::EMPTY;
	Player nextPlayer;
	HexBoard hexBoard = HexBoard( 0 ) ;
	
	ret = prepareGame(size_board, hexBoard, colorPlayer, colorComputer, nextPlayer) ;
	
	while (ret == 1) {
		x = -1 ; y = -1 ;

		if(nextPlayer == Player::COMPUTER) {
			computerPicks(size_board, x, y, ComputerChoice::MONTECARLO, hexBoard, colorComputer, colorPlayer);
			ret = positionAnalysis(colorComputer, "COMPUTER", "YOU", hexBoard, x, y, nextPlayer) ;			
		}		
		else // player YOU
		{

			playerPicks(size_board, x, y);

			ret = positionAnalysis(colorPlayer, "YOU", "COMPUTER", hexBoard, x, y, nextPlayer ) ;			

		}
		
	} // end while(playAgain)
	
	
	return 0;
}



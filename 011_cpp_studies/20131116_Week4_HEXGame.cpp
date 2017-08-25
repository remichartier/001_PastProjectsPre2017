

/* 
Objective

The objective of this assignment together with Homework 5  is to make a playable version of the game of HEX.  Homework 4 is about drawing a HEX board and determining a legal position and a winning position. Homework 5 will be to add an AI that can play HEX well. In Homework 5 we will use ideas from the video lectures that implement a Monte Carlo determined best move that will let your final program possibly exceed human ability.
* 
* Homework4 expectations :
* ======================
* 
*   1. Be able to draw the board using ASCII symbols and a given size, such as 7 by 7 or 11 by 11.
    
    2. Input a move and determine if a move is legal.
    
    3. Determine who won.

*/


// Your program should use a graph representation and treat the game as a path finding problem.

// Each internal node (hexagon) has six neighbors – so each would have 6 edges.

// The corners and the edges are special. A corner has either two or three neighbors and a non-corner edge has 4 neighbors.

// The player should be able to interact with the program and choose its “color” with blue going first.

// The program should have a convenient interface for entering a move, displaying the board, and then making its own move.

// The program should determine when the game is over and announce the winner.

// A simple board display would be to have an 11 x 11 printout with B, R, or a blank in each position. 

// A simple way to input a move would be to have the player enter an (i,j) coordinate corresponding to a currently empty hexagon and have the program check that this is legal and if not ask for another choice.

// Here X indicates a move in the corner. The dots are empty cells. A  O indicates the second player. 


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

using namespace std;

// Ensure that your ADTs support a graph of at least size 50.

const int DEBUG_ON = false ;
const int DEBUG_ON1 = false ;
const int DEBUG_isPlayerWining = false ;
const int MIN_SIZE_BOARD = 4 ;
const int MAX_SIZE_BOARD = 11;
const int SIZE_GRAPH = MAX_SIZE_BOARD * MAX_SIZE_BOARD;

enum class Content{EMPTY, BLUE, RED, POSITION}; 
enum class Player{COMPUTER,PLAYER};

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
		
		void getVerticeHexBoardPoint(Vertice ver, int& x, int& y) { 
			x = ver.v / size;
			y = ver.v % size;
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
		
// method to pring the HexBoard
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
	
	string keywait;
		
	//1. Add current (x,y) in list of VerticeKnow.
	
	verticesNeighboors.push_back(currentVertice);
	
//	if(DEBUG_isPlayerWining) { cout << "verticesNeighboors.back() : " << verticesNeighboors.back() << "  " ; }
	
	do{
		cout << "*" ;
		// identify neighboors 
		// take last neighboor.
		currentVertice = verticesNeighboors.back();
		if(DEBUG_isPlayerWining) {cout << "currentVertice = " << currentVertice.v << endl; }
		// add to verticesKnown list if not in this list yet.
		if(!isVerticeInList(currentVertice, verticesKnown)){
			verticesKnown.push_back(currentVertice);
			if(DEBUG_isPlayerWining) {cout << "add currentVertice to verticesKnown " << currentVertice.v << endl; }
		}
		// check if from borders or corners ..., if yes, update the bool flags accordingly.
		int x,y;
		getVerticeHexBoardPoint(currentVertice, x, y);
		if(color==Content::RED) {
			hasNeighboorRedTop |= isBorderLineUp(x,y);
			hasNeighboorRedTop |= isBorderLineDown(x,y);
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
					if(DEBUG_isPlayerWining) {cout << "add vertice to verticesNeighboors because did not find it in VerticesKnown " << currentVertice.v << endl; }
				}
		}
		
		// empty foundVerNeighboors because do not use anymore for this loop.
		foundVerNeighboors.clear();
		
		isRedWinner = (color==Content::RED) & hasNeighboorRedTop & hasNeighboorRedBottom ;
		isBlueWinner = (color==Content::BLUE) & hasNeighboorBlueEast & hasNeighboorBlueWest ;
		conditionExit = isBlueWinner |  isRedWinner | verticesNeighboors.empty() ;
		if(DEBUG_isPlayerWining) { cin >> keywait ;}
		
	} while (!conditionExit) ;	
	
	// empty vectors because do not use anymore for this loop.
	verticesKnown.clear();
	verticesNeighboors.clear();
	if (isRedWinner || isBlueWinner) return true;
	else
		return false;
	
} // end method HexBoard::isPlayerWining()



/********************
*					*
* 	MAIN()			*
* 					*
*********************/ 

int main(int argc, char **argv)
{
		
				
		srand(clock());
					
//		Graph g1=Graph(0.2, 1.0, 10.0);

	int size_board;
	string playAgain, colorPicked, key;
	Content colorPlayer=Content::EMPTY, colorComputer=Content::EMPTY;
	Player nextPlayer;
	
	do{
		cout << "Please specify board size X * X. X= ? ; X should be >= " << MIN_SIZE_BOARD << " and <= " << MAX_SIZE_BOARD << " : " << endl ;
		cin >> size_board ;	
	} while ( (size_board < MIN_SIZE_BOARD) || (size_board > MAX_SIZE_BOARD) ) ;

	if(DEBUG_ON) {cout << "Size board chosen : " << size_board << endl;} 
	
	// Now should build the graph representing the board.
	
	if(DEBUG_ON) {cout << "Create HexBoard " << endl;} 
	HexBoard hexBoard = HexBoard(size_board);
	
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
	cout << "Enter any key then ENTER when ready. ";
	cin >> key;

	
	
	while ( (playAgain[0]!='N') && (playAgain[0]!='n')    ) {
		if(nextPlayer == Player::COMPUTER) {
			
			cout << endl << "NOW COMPUTER PLAYS ..." << endl ;
			int x(-1),y(-1);
			
			x = rand() % (size_board - 1);
			y = rand() % (size_board - 1);
			
			cout << "COMPUTER CHOSE (" << x << "," << y << "). " << endl ;
			
			// position analysis
			// determine if move is legal ....
			if (hexBoard.isMoveLegal(x,y)) {
				
				// fill the spot.
				if( hexBoard.movePlayer(colorComputer,x,y) ) {
					hexBoard.print();
					cout << "Computer is analysing the move. Is it a Win move ? " ;
					if (hexBoard.isPlayerWining(colorComputer,x,y)){
						// Computer won
						cout << "COMPUTER WON - GAME OVER - GAME OVER - GAME OVER" << endl ;
						return 0;
					}
					else{
						cout << "...... Answer : Not yet. Next is for you to play." << endl ;
					}
				}
				else
					cout << "Not identified Error moving Computer choice." << endl ;
				
				nextPlayer = Player::PLAYER ;
				
				cout << "Continue Playing ? Y/N : " << endl;
				cin >> playAgain ;
			}
			else
			{
				cout << "Move not legal, (x,y) not empty, COMPUTER should select another position. " ;
				nextPlayer = Player::COMPUTER ;
			}
			
		}
		
		else // player you
		{
			string position;
			int x(-1),y(-1);
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
			
			// position analysis
			// determine if move is legal ....
			if (hexBoard.isMoveLegal(x,y)) {
				
				// fill the spot.
				if( hexBoard.movePlayer(colorPlayer,x,y) ) {
					
					hexBoard.print();
					cout << "Computer is analysing the move. Is it a Win move ? " ;
					if (hexBoard.isPlayerWining(colorPlayer,x,y)){
						// PLAYER won
						cout << "YOU WON - YOU WON - YOU WON - "<< endl << "GAME OVER - GAME OVER - GAME OVER" << endl ;
						return 0;
					}
					else{
						cout << "...... Answer : Not yet. Next is for COMPUTER to play." << endl ;
					}	
				}
				else
					cout << "Error moving player." << endl ;
				
				nextPlayer = Player::COMPUTER ;
			}
			else
			{
				cout << "Move not legal, (x,y) no empty, please select another position. " ;
				nextPlayer = Player::PLAYER ;
			}
			
		}
		
	}
	
	return 0;
}



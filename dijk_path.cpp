// C program for Dijkstra's single 
// source shortest path algorithm. 
// The program is for adjacency matrix 
// representation of the graph. 
#include <stdio.h> 
#include <limits.h> 
#include <fstream>
#include <iostream>
using namespace std;

// Number of vertices 
// in the graph 
#define V 900 
std::fstream outfile("/home/prachi/PycharmProjects/untitled1/srcdest.txt", std::ios_base::out); 

// A utility function to find the 
// vertex with minimum distance 
// value, from the set of vertices 
// not yet included in shortest 
// path tree 
long minDistance(long dist[], 
				bool sptSet[]) 
{ 
	
	// Initialize min value 
	int min = INT_MAX, min_index; 

	for (int v = 0; v < V; v++) 
		if (sptSet[v] == false && 
				dist[v] <= min) 
			min = dist[v], min_index = v; 

	return min_index; 
} 

// Function to print shortest 
// path from source to j 
// using parent array 
void printPath(long parent[], int j, int grid[30][30]) 
{ 
	
	// Base Case : If j is source 
	if (parent[j] == - 1) 
		return; 

	printPath(parent, parent[j], grid); 

	printf("%d ", j);
	outfile<<j<<" "; 
	int d = j/30;
	int r = j%30;
	grid[d][r] = 5;
} 

// A utility function to print 
// the constructed distance 
// array 
int printSolution(int src, long dist[], long parent[], int dest, int grid[30][30]) 
{  
	int i = dest;
	printf("Vertex\t Distance\tPath"); 
	//for (int i = 1; i < V; i++) 
	{ 	cout<<endl<<src<<"->"<<i<<"\t\t"<<dist[i]<<"\t\t"<<src<<" ";
		//printf("\n%d -> %d \t\t" + dist[i]+ "\t\t%d ", 
		//			src, i, src);
		outfile<<src<<" ";
		printPath(parent, i, grid); 
		//outfile<<printPath(parent, i, grid); 
	} 
} 

// Funtion that implements Dijkstra's 
// single source shortest path 
// algorithm for a graph represented 
// using adjacency matrix representation 
void dijkstra(long graph[V][V], int src, int dest, int grid[30][30]) 
{ 
	
	// The output array. dist[i] 
	// will hold the shortest 
	// distance from src to i 
	long dist[V]; 

	// sptSet[i] will true if vertex 
	// i is included / in shortest 
	// path tree or shortest distance 
	// from src to i is finalized 
	bool sptSet[V]; 

	// Parent array to store 
	// shortest path tree 
	long parent[V]; 

	// Initialize all distances as 
	// INFINITE and stpSet[] as false 
	for (int i = 0; i < V; i++) 
	{ 
		parent[0] = -1; 
		dist[i] = INT_MAX; 
		sptSet[i] = false; 
	} 

	// Distance of source vertex 
	// from itself is always 0 
	dist[src] = 0; 

	// Find shortest path 
	// for all vertices 
	for (int count = 0; count < V - 1; count++) 
	{ 
		// Pick the minimum distance 
		// vertex from the set of 
		// vertices not yet processed. 
		// u is always equal to src 
		// in first iteration. 
		long u = minDistance(dist, sptSet); 

		// Mark the picked vertex 
		// as processed 
		sptSet[u] = true; 

		// Update dist value of the 
		// adjacent vertices of the 
		// picked vertex. 
		for (int v = 0; v < V; v++) 

			// Update dist[v] only if is 
			// not in sptSet, there is 
			// an edge from u to v, and 
			// total weight of path from 
			// src to v through u is smaller 
			// than current value of 
			// dist[v] 
			if (!sptSet[v] && graph[u][v] && 
				dist[u] + graph[u][v] < dist[v]) 
			{ 
				parent[v] = u; 
				dist[v] = dist[u] + graph[u][v]; 
			} 
	} 

	// print the constructed 
	// distance array 
	printSolution(src, dist, parent, dest, grid); 
} 

// Driver Code 
int main() 
{ 
	std::fstream myfile("/home/prachi/PycharmProjects/untitled1/grids.txt", std::ios_base::in);

	    float a;
	    int src, dest,p,q;
	    myfile >> src;
	    myfile >> dest;
	    int grid[30][30];
	    int i=0, j=0;
	    while (myfile >> a)
	    {	 
	//    	cout << (int)a;
	//    	cout<< endl;
	        grid[i][j] = (int)a;
	        cout << grid[i][j] << " " ;
			j++;
	        
	        if(j==30){
	            
				j=0;
	            i++;
	            cout << endl;  
	        }
	        
	        if (i==30){
	            break;
	        }
	    }
	    for(int i=0; i<30; i++){
	     grid[i][0] = 0;
	     grid[i][29] = 0;
}/* Let us create the example graph discussed above */
/*int graph[V][V] = {{0, 4, 0, 0, 0, 0, 0, 8, 0}, 
					{4, 0, 8, 0, 0, 0, 0, 11, 0}, 
					{0, 8, 0, 7, 0, 4, 0, 0, 2}, 
					{0, 0, 7, 0, 9, 14, 0, 0, 0}, 
					{0, 0, 0, 9, 0, 10, 0, 0, 0}, 
					{0, 0, 4, 14, 10, 0, 2, 0, 0}, 
					{0, 0, 0, 0, 0, 2, 0, 1, 6}, 
					{8, 11, 0, 0, 0, 0, 1, 0, 7}, 
					{0, 0, 2, 0, 0, 0, 6, 7, 0} 
					}; 

  */
	    long graph[V][V];
	    int v, c=0;
	    for(int i=0; i<30; i++){
	    	for(int j=0; j<30; j++){
	    		v = (int) grid[i][j];

	    		for(int k = 0; k<900; k++) graph[c][k]=0;
    			if(v == 1){
    				if(grid[i][j-1] == 1 && j>0) graph[c][c-1] = 1;
    				if(grid[i][j+1] == 1 && j<30) graph[c][c+1] = 1;
    				if(grid[i-1][j] == 1 && i>0) graph[c][c-30] = 1;
    				if(grid[i+1][j] == 1 && i<30) graph[c][c+30] = 1;
    			}
    			c++;

	    	}
	    }

	p = dest/30;
	q = dest%30;
	if(grid[p][q] == 0){
		q++;
		if(grid[p][q] == 1) break;
	}
	dest = p*30 + q;
	dijkstra(graph, src, dest, grid);
	cout<<endl; 
	for(int i=0; i<=30; i++){
		for(int j=0; j<=30; j++){
			cout<< grid[i][j]<<" ";

		}
		cout<<endl;
	}
	//outfile<<"hello";

	return 0; 
} 

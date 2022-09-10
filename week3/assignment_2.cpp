// CSCI 411 - Fall 2022
// Assignment 2 Skeleton
// Author: Wataru Oshima
// Feel free to use all, part, or none of this code for the coding problem on assignment 2.

#include <iostream>
#include <vector>
#include <deque>
#include <climits>
#include <algorithm>
using namespace std;

/******************************************************************************
 * A simple Node struct                                                       *
 * id - int - the id or name of the node                                      *
 * dist - int - the distance from some given node to this node                *
 * inI - bool - true if the node is a member of the set I and false otherwise *
 * visited - bool - whether or not this node has been visited                 *
 * ****************************************************************************/
struct Node {
    int id;
    int dist;
    bool inI;
    bool visited;
};

/**************************************************************************
 * A simple Edge struct                                                   *
 * from - Node* - the node where this edge starts                         *
 * to - Node* - the node where this edge ends                             *
 * weight - int - the weight of this edge                                 *
 * ************************************************************************/
struct Edge {
    Node* from;
    Node* to;
    int weight;
};

/*****************************************************************************************************************
 * Given a graph, find the set of nodes that belong to the set I, that is, the set of vertices v such that there *
 * is at least one path of length negative infinity ending at v.                                                 *
 * A - vector<vector<Edge*>> - an adjacency list representation of a graph where each element is a weighted edge *
 * return - vector<int> - the integer ids of nodes in the set I                                                  *
 * ***************************************************************************************************************/
vector<int> findSetI(vector<vector<Edge*>> A){
    //YOUR CODE HERE
    vector<int> L;
    for (int i = 1; i < A[0].size(); i++) //Iterate |V|-1
    {
        
       if (!A[i].empty() &&A[i][0]->from->dist ==INT_MAX){
            A[i][0]->from->dist=0;
            
       }
        
       for (int j = 0; j < A[i].size(); j++){ //Iterate |E|
            // std::cout << " from " << A[i][j]->from->id << ": dist " << A[i][j]->from->dist
            // << " to " << A[i][j]->to->id << ": dist " << A[i][j]->to->dist
            // << std::endl;
            if (A[i][j]->to->dist > (A[i][j]->from->dist + A[i][j]->weight)){
                A[i][j]->to->dist = A[i][j]->from->dist + A[i][j]->weight;
                // std::cout << "updated" << std::endl;
            }
       }
    }
    // std::cout << "" << std::endl;
    for (int i = 1; i < A[0].size(); i++) //Iterate |V|-1
    { 
       for (int j = 0; j < A[i].size(); j++){ //Iterate |E|
            if(!A[i][j]->to->visited || !A[i][j]->from->visited){
                if  (A[i][j]->to->inI || A[i][j]->from->inI){
                    L.push_back(A[i][j]->to->id);
                    L.push_back(A[i][j]->from->id);
                    continue;
                }
                // std::cout << " from " << A[i][j]->from->id << ": dist " << A[i][j]->from->dist
                // << " to " << A[i][j]->to->id << ": dist " << A[i][j]->to->dist
                // << std::endl;
                if (A[i][j]->to->dist > (A[i][j]->from->dist + A[i][j]->weight)){
                    // A[i][j]->to->dist = A[i][j]->from->dist + A[i][j]->weight;
                    L.push_back(A[i][j]->to->id);
                    L.push_back(A[i][j]->from->id);
                    A[i][j]->to->inI = true;
                    A[i][j]->from->inI = true;
                    // std::cout << "inserted" << std::endl;
                }
            }
            A[i][j]->from->visited =true;                
       }
    }
    sort( L.begin(), L.end() );
    L.erase(unique( L.begin(), L.end() ), L.end() );
    // for (auto l:L){
    //     std::cout << l <<std::endl;
    // }
        
    return L;
}

int main(){
    //get the number of nodes and number of edges from cin separated by a space
    int n = -1, m = -1;
    cin >> n >> m;

    //add the nodes to an adjacency list
    //in this case, A[i] is a vector of all edges leaving A[i]
    //note that A[0] is a list of self loops representing all nodes in the graph
    //these are not actual edges in the graph, just a way to keep track of all nodes
    //Furthermore, A[0][0] is a dummy edge with a dummy node
    //this means that A[i] is the node with id i where ids start at 1
    vector<vector<Edge*>> A(n+1);
    A[0].push_back(new Edge());
    for (int i=1; i<n+1; i++){
        Node* v = new Node();
        v->id = i;
        v->dist = INT_MAX;
        v->inI = false;
        v->visited = false;
        Edge* e = new Edge();
        e->from = v;
        e->to = v;
        e->weight = 0;
        A[0].push_back(e);
    }

    //get edges from cin and add them to the adjacency list
    //the start, end, and weight of a single edge are on the same line separated by spaces
    int u = -1, v = -1, w = -1;
    for (int i=0; i<m; i++){
        cin >> u >> v >> w;
        Edge* e = new Edge();
        e->from = A[0][u]->from;
        e->to = A[0][v]->to;
        e->weight = w;
        A[u].push_back(e);
    }

    //find nodes belonging to the set I and print them out in ascending order
    vector<int> I = findSetI(A);
    sort(I.begin(), I.end());
    for (int i=0; i<(int)I.size()-1; i++){
        cout << I[i] << " ";
    }
    if (I.size() > 1){ cout << I[I.size()-1] << endl; }

    return 0;
}

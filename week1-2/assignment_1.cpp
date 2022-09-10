// CSCI 411 - Fall 2022
// Assignment 1 Skeleton
// Author: Carter Tillquist
// Feel free to use all, part, or none of this code for the coding problem on assignment 1.

#include <iostream>
#include <vector>
using namespace std;

/**************************************************************************
 * A simple Node struct                                                   *
 * id - int - the id or name of the node                                  *
 * SCC - int - the strongly connected component that this node belongs to *
 * visited - bool - whether or not this node has been visited             *
 * ************************************************************************/
struct Node {
    int id;
    int SCC;
    bool visited;
};

/**************************************************************************************************
 * A simple struct of strongly connected component (SCC) graph nodes                              *
 * id - int - the id or name of the node (this corresponds to the SCC id)                         *
 * size - int - the number of nodes from the original graph that belong to this SCC               *
 * hasInEdges - bool - true if there are edges with end points in this SCC and false otherwise    *
 * hasOutEdges - bool - true if there are edges with start points in this SCC and false otherwise *
 * ************************************************************************************************/
struct SCCNode {
    int id;
    int size;
    bool hasInEdges;
    bool hasOutEdges;
};

/*********************************************************
 * A simple struct to hold the sizes of sets A, B, and C *
 * A - int - the size of set A                           *
 * B - int - the size of set B                           *
 * C - int - the size of set C                           *
 * *******************************************************/
struct Result {
    int A;
    int B;
    int C;
};


//for debug, pritn the adjucent list
void printAdjcentList(vector<vector<Node*>>& A){
    int counter =0;
    for(auto a:A){
        std::cout << counter << ": ";
        for( auto v:a){
            std::cout << v->id << " ";
        }
        std::cout <<std::endl;
        counter++;
    }
}


/***************************************************************************************
 * Given the adjacency list of a graph, generate a new adjacency list with the         *
 * same nodes but with edges reversed                                                  *
 * A - vector<vector<Node*>> - an adjacency list representation of a graph. Note that  * 
 *   A[0] is a list of all the Nodes in the graph with an additional dummy Node at     * 
 *   A[0][0]. As a result, A[i] are the neighbors of the Node with id i where these    *
 *   ids go from 1 to n, the size of the graph.                                        *
 * return - vector<vector<Node*>> - an adjacency list of a new graph equivalent to the *
 *        original but with edges reversed.                                            *
 * *************************************************************************************/


vector<vector<Node*>> reverseEdges(vector<vector<Node*>> A){
    vector<vector<Node*>> Ta(A[0].size());
    Ta[0] = A[0];
    // for (int i=1; i<A[0].size(); i++){
    //     Node* v = new Node();
    //     v->id = i;
    //     v->SCC = -1;
    //     v->visited = false;
    //     Ta[0].push_back(v);
    // }
    //YOUR CODE HERE
    for (int i=1; i<A.size(); i++){
        for(auto v:A[i]){
            Ta[v->id].push_back(Ta[0][i]);
        }
    }
    // std::cout << "Original" << std::endl;
    // printAdjcentList(A);
    // std::cout <<" Transposed "<< std::endl;
    // printAdjcentList(Ta);
    return Ta;
}

/********************************************************************************************************
 * A variation of DFS for the first pass over a graph looking for strongly connected components.        *
 * The goal is to fill the vector L with nodes in decreasing order with respect to finishing time       *
 * A - vector<vector<Node*>> - an adjacency list                                                        *
 * v - Node* - the start node for the DFS                                                               *
 * &L - vector<Node*> - a list of nodes to be filled in decreasing order with respect to finishing time *
 * ******************************************************************************************************/
void DFSSCC(vector<vector<Node*>> A, Node* v, vector<Node*> &L){
    //YOUR CODE HERE
    v->visited = true;
    for (auto u: A[v->id]){
        if (u->visited == false){
            DFSSCC(A,u,L); //call the function again
            
        }
    }    
    //adding the L at the front
    L.insert(L.begin(),v); 
}

/******************************************************************************************************************
 * A variation of DFS for the second pass over a graph looking for strongly connected components.                 *
 * There are three goals (1) to label nodes with a SCC id (2) to generate nodes of a SCC metagraph (3) and to     *
 * determine which nodes in this metagraph have incoming and outgoing edges.                                      *
 * A - vector<vector<Node*>> - an adjacency list representing the transpose or edge reverse of the original graph *
 * v - Node* - the start node for the DFS                                                                         *
 * scc - int - the current strongly connected component id                                                        *
 * &SCCs - vector<SCCNode*> - the nodes of a SCC metagraph                                                        *
 ******************************************************************************************************************/
void DFSAssign(vector<vector<Node*>> A, Node* v, int scc, vector<SCCNode*> &SCCs){
    // YOUR CODE HERE
    v->SCC = scc;
    v->visited = true;
    for (auto u:A[v->id]){
        if (u->SCC == -1){
            DFSAssign(A,u,scc, SCCs);
            SCCs[scc]->size +=1;
        }else if(u->SCC != -1 && u->SCC != scc){
            SCCs[scc]->hasInEdges =true;
            SCCs[u->SCC]->hasOutEdges = true;
        }
    }
    
}

/******************************************************************************************************
 * Find the strongly connected components (SCCs) of a graph. The SCC of each Node is added to the SCC *
 * member of the Node struct. In addition, a vector of SCCNode* is returned.                          *
 * A - vector<vector<Node*>> - an adjacency list                                                      *
 * return - vector<SCCNode*> - a vector of nodes in the SCC metagraph of A                            *
 * ****************************************************************************************************/
vector<SCCNode*> SCC(vector<vector<Node*>> A){
    //YOUR CODE HERE
    vector<Node*> L;
    vector<SCCNode*> sccs;
    for (int i=1; i<A[0].size(); i++){
        if(A[0][i]->visited==false){
            DFSSCC(A,A[0][i],L);
        }
    }

    // std::cout <<std::endl;
    vector<vector<Node*>> Ta = reverseEdges(A);
    int counter = 0;
    for (auto v:L){
        if(Ta[0][v->id]->SCC == -1){
            // std::cout << "here" << std::endl;
            SCCNode* sc = new SCCNode();
            sc->id = counter;
            sc->size =1; //node itself
            sc->hasInEdges = false;
            sc->hasOutEdges = false;
            sccs.push_back(sc);
            // std::cout << sccs[counter]->id << std::endl;
            DFSAssign(Ta,Ta[0][v->id],counter,sccs);
            // std::cout << sccs[counter]->size << std::endl;
            counter ++;
        }
    }


    // for (auto &a:Ta[0]){
    //     std::cout << a->id << " ";
    //     std::cout << a->SCC << std::endl;
    // }
    
    // std::cout << "sccs size: " <<sccs.size() << std::endl;
    // for (int i=0; i<sccs.size();i++){
    //     std::cout << "id: "<<sccs[i]->id << " ";
    //     std::cout << "size: "<<sccs[i]->size << std::endl;
    //     std::cout << "In: "<<sccs[i]->hasInEdges << std::endl;
    //     std::cout << "Out " <<sccs[i]->hasOutEdges << std::endl;
    // }
    return sccs;
}

/************************************************************************************************
 * Given the adjacency list representation of a graph, fill and return a Result struct with the *
 * number of nodes that belong to the three sets A, B, and C as described in assignment 1       *
 * A - vector<vector<Node*>> - an adjacency list                                                *
 * return - Result - a Result struct holding the sizes of sets A, B, and C                      *
 * **********************************************************************************************/
Result* getSetSizes(vector<vector<Node*>> A){
    //YOUR CODE HERE
    vector<SCCNode*> sccs = SCC(A);
    static Result* result = new Result();
    result->A=0;
    result->B=0;
    result->C=0;
    for (auto scc:sccs){
       if (!scc->hasInEdges && scc->hasOutEdges){
            result->A += scc->size;
        }else if (scc->hasInEdges && !scc->hasOutEdges){
            result->B += scc->size;
        }else{
            result->C += scc->size;
        }
    }
    return result;
}

int main(){
    //get the number of nodes and number of edges from cin separated by a space
    cout << "Number of nodes and number of edges: " << endl;
    int n = -1, m = -1;
    cin >> n >> m;

    //add the nodes to an adjacency list
    //note that A[0] is a list of nodes with a dummy node in A[0][0]
    //this means that A[i] is the node with id i where ids start at 1
    vector<vector<Node*>> A(n+1);
    A[0].push_back(new Node());
    for (int i=1; i<n+1; i++){
        Node* v = new Node();
        v->id = i;
        v->SCC = -1;
        v->visited = false;
        A[0].push_back(v);
    }

    //get edges from cin and add them to the adjacency list
    //the start and end of a single edge are on the same line separated by a space
    cout << "Add " << m << " edges: " << endl;
    int u = -1, v = -1;
    for (int i=0; i<m; i++){
        cin >> u >> v;
        A[u].push_back(A[0][v]);
    }

    //call getSetSizes to determine the size of the sets A, B, and C and print the results
    Result* R = getSetSizes(A);
    cout << "|A| = " << R->A << ", |B| = " << R->B << ", |C| = " << R->C;

    return 0;
}
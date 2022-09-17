// CSCI 411 - Fall 2022
// Assignment 3 Skeleton
// Author: Wataru Oshima
// Feel free to use all, part, or none of this code for the coding problem on assignment 3.

#include <iostream>
#include <vector>
using namespace std;

/*****************************************************************************************
 * A simple struct to keep track of subproblem solutions                                 *
 * numCoins - int - the total number of coins required to solve a subproblem             *
 * lastCoin - int - the denomination of the last node added to arrive at this solution   *
 *                  notice that, in the context of an array of subproblems R where loc   *
 *                  is the index of this ChangeCell, loc-lastCoin is the position of the * 
 *                  subproblem on which this solution is based                           *
 * ***************************************************************************************/
struct ChangeCell{
  int numCoins;
  int lastCoin;
};

void findIndex(vector<int> &result, vector<int> &coins, int lastCoin){
    for (int i=0; i<coins.size();i++){
        if (coins[i] == lastCoin){
            result[i] +=1;
            break;
        }
    }
}

/*****************************************************************************************************
 * Given a target amount of money and a vector of available coin denominations, determine the number *
 * of each coin denomination required to reach the target such that the total number of coins used   *
 * is minimized                                                                                      *
 * target - int - the target amount of money                                                         *
 * coins - vector<int> - a vector of integers representing available coin denominations. Assume that *
 *                       these denominations are sorted from least to greatest                       *
 * return - vector<int> - a vector of integers of the same size as coins representing the number of  *
 *                        each type of coin required to reach the target                             *
 * ***************************************************************************************************/
vector<int> makeChange(int target, vector<int> coins){
    // YOUR CODE HERE
    int max = 10000000;
    vector<int> L;
    vector<ChangeCell> C;
    for (int i=0; i<target+1;i++){
        ChangeCell _cc;
        _cc.lastCoin=0;
        _cc.numCoins=0;
        L.push_back(max);
        C.push_back(_cc);
    }

    
    L[0]=0;
    for (int i=1; i<target+1; i++){
        int prev =0;
        for (int j=coins.size()-1; j>-1;j--){   
            
            if (i<coins[j])
                continue;
            else{
                L[i] = std::min(L[i],L[i-coins[j]]+1); //number of coins
                if (L[i] != max && prev != L[i]){
                    C[i].numCoins = L[i];
                    C[i].lastCoin = coins[j];  
                    prev = L[i];  
                }
            
            }
        }
        // cout << temp << endl;
    }
    vector<int> result;
     for (int i=0; i<coins.size();i++)
        result.push_back(0);

    int index=target;
    // cout << index << endl;
    // cout << "num of coins " << C[index].numCoins << endl;
    // cout << "last coin " << C[index].lastCoin << endl;
    for(int i=0; i<C[target].numCoins;i++){
        findIndex(result,coins, C[index].lastCoin);
        // cout << index << endl;
        // cout << "num of coins " << C[index].numCoins << endl;
        // cout << "last coin " << C[index].lastCoin << endl;
        index -= C[index].lastCoin;
    }
    
    return result;
    
}

int main(){
  //get the number of coin denominations and the number of target amounts
  int n = -1, m = -1;
  cin >> n >> m;

  //collect the available coin denominations in a vector
  vector<int> coins;
  int v = -1;
  for (int i = 0; i < n; i++){
    cin >> v;
    coins.push_back(v);
  }

  //determine the number of each type of coin required for each target amount
  for (int i = 0; i < m; i++){
    int target = -1;
    cin >> target;
    vector<int> results = makeChange(target, coins);
    for (int j = 0; j < results.size()-1; j++){
      cout << results[j] << " ";
    }
    cout << results[results.size()-1] << endl;
  }

  return 0;
}
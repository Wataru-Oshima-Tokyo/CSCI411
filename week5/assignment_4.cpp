// CSCI 411 - Fall 2022
// Assignment 4 Skeleton
// Author: Carter Tillquist
// Feel free to use all, part, or none of this code for the coding problem on assignment 4

#include <iostream>
#include <vector>
using namespace std;

/*******************************************************************************************
 * A simple struct to describe alignments. Insertions are denoted with an underscore ("_") *
 * a, b - strings - two strings representing an alignment. The strings being aligned may   *
 *                - be extracted by removing underscores                                   *
 * score - float - the score of the alignment given costs associated with insertions,      *
 *                 deletions, and substitutions                                            *
 * *****************************************************************************************/
struct Alignment{
  string a;
  string b;
  float score;
};

/***************************************************************************************
 * A simple struct to keep track of subproblem solutions                               *
 * act - string - the action taken to reach this Cell: "match", "ins", "del", or "sub" *
 * score - float - the score of the alignment of prefixes up to this point given costs *
 *                 associated with insertions, deletions, and substitutions            *
 * *************************************************************************************/
struct Cell{
  string act;
  float score;
};


//compare 3 possibilities and reutunr the lowest one and act
Cell comp(int &ins, int &del, int &sub){
  Cell r_cell;
  if ((sub<=del && sub<=ins)){ //if all the cost is the same
    r_cell.score = sub;
    r_cell.act ="sub";
  }else if (ins<sub && sub<=del){ // if the cost of insertion is smaller than substition and deletion
    r_cell.score = ins;
    r_cell.act ="ins";
  }else if(del<sub && sub<=ins){ //if the cost of deletion is samller than substion and insertion
    r_cell.score = del;
    r_cell.act ="del";
  }else if (del<ins){ //if the deletion is smaller than insertion
    r_cell.score = del;
    r_cell.act ="del";
  }else if (ins<del){ //if the insertion is smaller than deletion
    r_cell.score = ins;
    r_cell.act ="ins";
  }else if(ins==del && del<sub){ // if the insertion and deletion are the same cost and substition is bigger than them
    r_cell.score = del;
    r_cell.act ="del";
  }
  return r_cell;
}



/*************************************************************************************
 * Given two strings a and b along with costs associated with insertions, deletions, *
 * and substitutions, find an optimal alignment (of minimum cost) of a and b         *
 * a, b - strings - two strings for which we want an alignment                       *
 * ins, del, sub - floats - the cost of performing insertion, deletion, and          *
 *                          substitution opertations respectively on string a        *
 * return - Alignment - an optimal alignment of a and b given the matrix T along     *
 *                      with the score or cost of the alignment                      *
 * ***********************************************************************************/
Alignment editDistance(string a, string b, float ins, float del, float sub){
  //YOUR CODE HERE
  Alignment alignment_;
  std::string act_[4] = {"ins", "del", "sub", "stay"};
  Cell D[a.length()+1][b.length()+1];
  D[0][0].score=0;
  D[0][0].act=act_[3];
  //making basecases
  for (int i=1; i< a.length()+1;i++){
    D[i][0].score = i* del; // set the cost for all the first column
    D[i][0].act = act_[1];  //set del for all the first column
  }
  for (int j=1; j<b.length()+1; j++){
    D[0][j].score = j* ins; // set the cost for all the first row
    D[0][j].act = act_[0]; // set ins for all the first row
  }
  for (int i=1; i<a.length()+1;i++){
    for (int j=1; j<b.length()+1;j++){
       if (a[i-1]==b[j-1]){
        D[i][j].score = D[i-1][j-1].score; //set the same value as the previous cost
        D[i][j].act = act_[3]; //set stay for the cell
       }
      else{
        int ins_ = D[i][j-1].score + ins; //get the cost of insertion
        int del_ = D[i-1][j].score + del; //get the cost of deletion
        int sub_ = D[i-1][j-1].score + sub; //get the cost of substition
        D[i][j] = comp(ins_,del_,sub_);
      }
    }
  }
  //Get the string;
  int i = a.length();
  int j = b.length();
  std::string _a;
  std::string _b;
  std::string type;
  while(i>0 || j>0){ // keep going until i and j become 0
    type=D[i][j].act;
    if(type == act_[3] || type == act_[2]){ // if the act is sub or stay
      _a = a[i-1] +_a;
      _b = b[j-1]+_b;
      i--;
      j--;
    }else if(type == act_[0]){ // if the act is ins
      _a ="_" +_a;
      _b = b[j-1]+_b;
      j--;
    }else if (type == act_[1]){ //if the act is del
      _a = a[i-1] +_a;
      _b = "_"+_b;
      i--;
    }

  }
  alignment_.a = _a;
  alignment_.b = _b;
  alignment_.score = D[a.length()][b.length()].score;
  return alignment_;
}

int main(){
  //get the number of tests
  int n = -1;
  cin >> n;

  //get the costs associated with insertions, deletions, and substitutions
  int ins = -1, del = -1, sub = -1;
  cin >> ins >> del >> sub;

  //for each test, print the resulting alignment along with its score
  for (int i = 0; i < n; i++){
    string a = "", b = "";
    cin >> a >> b;

    Alignment align = editDistance(a, b, ins, del, sub);
    cout << align.a << endl;
    cout << align.b << endl;
    cout << align.score << endl;
  }

  return 0;
}
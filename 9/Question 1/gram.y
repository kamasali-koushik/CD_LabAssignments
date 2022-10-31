%{
#include <bits/stdc++.h>
using namespace std;

int yyerror(char *s);

class Node{

public:
  bool isop;
  string op;
  double val;
  int label;
  Node* left;
  Node* right;

};

void dfs(Node* root){
  if(!root)
  return;
  
  if(root->isop)
  cout<<root->op<<" ";
  else
  cout<<root->val<<" ";

  dfs(root->left);
  dfs(root->right);
}

void makedot(Node *root)
{

    string s = "digraph G  {  \n";

    queue<Node *> q;

    q.push(root);
   root->label = 0;
    int i=1;
    while (!q.empty())
    {
        Node *temp = q.front();
        q.pop();

      //label
        s += to_string(temp->label) + "[label= \"";
        if (temp->isop)
           s += temp->op + "\"];\n";
        else
            s += to_string((int)temp->val) + "\"];\n";

        string parent = to_string(temp->label);

        if (temp->left)
        {

            temp->left->label = i; 
            s += parent + "->" + to_string(i++)+"\n";
            q.push(temp->left);
        }
        if (temp->right)
        {
            temp->right->label = i;
            s += parent + "->" + to_string(i++)+"\n";
            q.push(temp->right);
        }

  }
    s +="\n}";
    ofstream outFile("edges.dot");
    outFile << s;
    outFile.close();
    system("dot -Tpng edges.dot -o image.png");

    system("xdg-open image.png");

}

#include "y.tab.h"
#include "lex.yy.c"

%}



%union{
  double val;
  Node* x;
}


%token NUMBER EQUALS ADD SUBTRACT MULTIPLY DIVIDE

%left ADD SUBTRACT
%left MULTIPLY DIVIDE

%start S


  
%%

S: S Input 
   | ;

Input: E                             {    Node* root = $<x>1;
                                          printf("\nValid Expression\n"); printf("Result of Expression: %lf\n", root->val);                                          
                                        
                                          makedot(root);
                                          // dfs(root);
                                           }
      | EQUALS                      {exit(0);} ;

E:    NUMBER                         {          
                                                  Node* root = new Node();
                                                  root->val = $<val>1;
                                                  root->isop = 0;
                                                  $<x>$ = root;}	

      | E ADD E                      {           
                                                  Node* root2 = new Node();
                                                  root2->op = "+";
                                                  root2->isop = 1;
                                                  root2->left = $<x>1;
                                                  root2->right = $<x>3;
                                                  root2->val = root2->left->val + root2->right->val;
                                                  $<x>$ = root2;
                                                  }

      | E SUBTRACT E                 {           
                                                  Node* root2 = new Node();
                                                  root2->op = "-";
                                                  root2->isop = 1;
                                                  root2->left = $<x>1;
                                                  root2->right = $<x>3;
                                                  root2->val = root2->left->val-root2->right->val;
                                                  $<x>$ = root2;}

      | E MULTIPLY E                 {            
                                                  Node* root2 = new Node();
                                                  root2->op = "*";
                                                  root2->isop = 1;
                                                  root2->left = $<x>1;
                                                  root2->right = $<x>3;
                                                  root2->val = root2->left->val*root2->right->val;
                                                  $<x>$ = root2;}
      | E DIVIDE E                   {           
                                                  Node* root2 = new Node();
                                                  root2->op = "/";
                                                  root2->isop = 1;
                                                  root2->left = $<x>1;
                                                  root2->right = $<x>3;
                                                  root2->val = root2->left->val/root2->right->val;
                                                  $<x>$ = root2;}

%%
extern int yylex();

int yyerror(char *s) {
  fprintf(stderr, "%s\n", s);
  return 0;
}

int main() {
  printf("\nEnter an arithmetic operation ending with =:\n");
  yyparse();
  return 0;
}

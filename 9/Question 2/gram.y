%{
#include <bits/stdc++.h>
using namespace std;

int yyerror(char *s);

int i = 0;

class Node{

public:
	bool isop;
	string op;
	double val;
	string p;
	int id;
	Node* left;
	Node* right;

};

map<string,Node*> m;


Node* createNode(string opr,Node* temproot1, Node* temproot2)
{
	
	string temp = opr + temproot1->p + temproot2->p;

	if(m.find(temp) == m.end())
	{           
		Node* root = new Node();
		root->id = ++i;
		root->p = temp;
		root->op = opr;
		root->isop = 1;
		root->left = temproot1;
		root-> right = temproot2;

		if(opr == "+") 
			root->val = root->left->val + root->right->val;
		else if(opr == "-")
			root->val = root->left->val - root->right->val;
		else if(opr == "*")
			root->val = root->left->val * root->right->val;
		else if(opr == "/")
			root->val = root->left->val / root->right->val;

		m[temp] = root;
	}
	
	return m[temp];
}

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



void dagdfs(Node *root, map<string,int>&mp,string &ans){
    if(root==NULL){
        return;
    }
            // fout << id << " [label=\"" << name << "\",style=filled,color=lightgrey];" << endl;
    string id = to_string(root->id);
	string name;
    if(root->isop)
	name = root->op;
	else
	name = to_string(root->val);

    string pre = root->p;

    if(mp.find(pre)==mp.end()){
        mp[pre]=root->id;
        ans+=id+" [label=\""+name+"\",style=filled,color=lightgrey];\n";
    }else {
        return ;
    }
    ans+=id+" [label=\""+name+"\",style=filled,color=lightgrey];\n";
    if(root->left!=NULL){
        string left = to_string(root->left->id);
        Node * l = root->left;
        if(mp.find(l->p)==mp.end()){
            ans+=id+" -> "+left+" ;\n";
            dagdfs(root->left,mp,ans);
        }else {
            left = to_string(mp[l->p]);
            ans+=id+" -> "+left+" ;\n";
        }
    }
    if(root->right!=NULL){
        Node * r = root->right;
        string right = to_string(root->right->id);
        if(mp.find(r->p)==mp.end()){
            ans+=id+" -> "+right+" ;\n";
            dagdfs(root->right,mp,ans);
        } else {
            right = to_string(mp[r->p]);
            ans+=id+" -> "+right+" ;\n";
        }
    }
}

void printdag(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    ofstream fout("edges.dot");
    fout << "digraph dag {" << endl;
    // fout << "rankdir=\"LR\";" << endl;
    fout << "node [shape=circle];" << endl;
    string ans;
    map<string,int>mp;
    dagdfs(root,mp,ans);
    fout<<ans;
    fout << "}" << endl;
    // system("dot -Tpng ./out/dag.dot -o ./out/dag.png"); 
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

Input: E                              {
										Node* root = $<x>1;
										printf("\nValid Expression\n"); printf("Result of Expression: %lf\n", root->val);                                          
										// makedot(root);
										printdag(root); 
									}
			| EQUALS                      {exit(0);} ;

E:    NUMBER                                  {   
												string temp = to_string($<val>1);
												if(m.find(temp) == m.end())
												{       
												Node* root = new Node();
												root->id = ++i;
												root->val = $<val>1;
												root->isop = 0;
												root->p = temp;
												$<x>$ = root;
												m[temp] = root;
												}
												else
												$<x>$ = m[temp];
											}	

			| E ADD E                               { $<x>$ = createNode("+",$<x>1,$<x>3); }

			| E SUBTRACT E                          { $<x>$ = createNode("-",$<x>1,$<x>3);}

			| E MULTIPLY E                         { $<x>$ = createNode("*",$<x>1,$<x>3);}
			| E DIVIDE E                            { $<x>$ = createNode("/",$<x>1,$<x>3);}

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

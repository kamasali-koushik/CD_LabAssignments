#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;

char tcharr[100];

struct trieNode{
  bool isEnd;
  unordered_map<string, trieNode*> children;
  trieNode(){
    isEnd = false;
  }
};

void insert(trieNode* root, string s){
  trieNode* curr = root;
  for(int i = 0; i < s.size(); i++){
    if(curr->children.find(s.substr(i, 1)) == curr->children.end()){
      curr->children[s.substr(i, 1)] = new trieNode();
    }
    curr = curr->children[s.substr(i, 1)];
  }
  curr->isEnd = true;
}

void fillAns(trieNode* root, vector<string> &ans, string &s, int idx){
  int dash = 1;
  if(root->isEnd && root->children.size()>0){
    s += "ε|";
  }
  for(auto it: root->children ){
    s += it.first;
    trieNode *curr = it.second;
    while(curr->children.size() == 1 && !curr->isEnd){
      s += curr->children.begin()->first;
      curr = curr->children.begin()->second;
    }
    if(curr->children.size() == 0){
      s += "|";
      continue;
    }
    string temp = "";
    sscanf(s.c_str(), "%[^-]s", tcharr);
    temp = tcharr;
    for(int i=0;i<dash;i++){
      temp += '\'';
    }
    dash++;
    s = s + temp + "|";
    temp += "->";
    ans.push_back(to_string(idx+1));
    fillAns(curr, ans, temp, idx+1);
  }
  ans[idx] = s.substr(0, s.size()-1);
}

void makeTrie(string str, vector<string> &ans){
  trieNode *root = new trieNode();
  string s="";
  int i=0;
  sscanf(str.c_str()+i,"%[^-]s",tcharr);
  s = tcharr;
  s = s + "->";
  i+=s.length();

  for(;i<str.length();i++){
    string temp;
    sscanf(str.c_str()+i,"%[^|]s",tcharr);
    temp = tcharr;
    insert(root, temp);
    i += temp.length();
  }
  ans.push_back("0");
  fillAns(root, ans, s, 0);
}

int main()
{
  int n;
  cout<<"Enter the number of relations: ";
  cin>>n;
  vector<string> ans;
  cout<<endl;
  for(int i=0;i<n;i++){
    string prod;
    cin>>prod;
    makeTrie(prod, ans);
    for(string s: ans){
      cout<<s<<endl;
    }
    ans.clear();
  }
  return 0;
}
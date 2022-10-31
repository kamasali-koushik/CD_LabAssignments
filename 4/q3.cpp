#include <stdio.h>
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
using namespace std;

char tcharr[100];
unordered_map<string, string> prods;
set<string> nts, ts;

struct node
{
  string val;
  vector<node *> children;
  node(string s)
  {
    val = s;
  }
};

string getCprod(string prod, int i)
{
  string ret = "";
  ret += prod[i++];
  if (i < prod.length() && (prod[i] == '\'' || prod[i] == 'd'))
    ret += prod[i];
  return ret;
}

string nextProd(string prod, int i)
{
  if (i >= prod.length())
    return "nothinghere";
  sscanf(prod.c_str() + i, "%[^|]s", tcharr);
  string ret = tcharr;
  return ret;
}

int findGraph(node *root, string str, int &cur, string nont)
{
  int i = 0;
  string prod = nextProd(prods[nont], i);
  i += prod.length();
  i++;
  bool ins = false;
  int j = 0;
  while (j < prod.length())
  {
    string tempProd = getCprod(prod, j);
    j += tempProd.length();
    root->children.push_back(new node(tempProd));
  }
  int vit=0;
  for (;vit<root->children.size();vit++)
  {
    string cprod = root->children[vit]->val;

    if (nts.find(cprod) != nts.end())
    {
      int ret = findGraph(root->children[vit], str, cur, cprod);
      if (ret == -1)
      {
        root->children.clear();
        prod = nextProd(prods[nont], i);
        if (prod == "nothinghere")
          return -1;
        i += prod.length();
        i++;
        j = 0;
        while (j < prod.length())
        {
          string tempProd = getCprod(prod, j);
          j += tempProd.length();
          root->children.push_back(new node(tempProd));
        }
        vit=-1;
        continue;
      }
    }
    else if (str[cur] == cprod[0])
    {
      cur++;
      if(str[cur] == 'd') cur++;
    }
    else if (cprod == "e")
    {
      continue;
    }
    else
    {
      root->children.clear();
      prod = nextProd(prods[nont], i);
      if (prod == "nothinghere")
        return -1;
      i += prod.length();
      i++;
      j = 0;
      while (j < prod.length())
      {
        string tempProd = getCprod(prod, j);
        j += tempProd.length();
        root->children.push_back(new node(tempProd));
      }
      vit=-1;
    }
  }
  return 0;
}
int main()
{
  prods.insert({"E", "TE'"});
  prods.insert({"E'", "+TE'|e"});
  prods.insert({"T", "FT'"});
  prods.insert({"T'", "*FT'|e"});
  prods.insert({"F", "(E)|id"});

  nts.insert("E");
  nts.insert("T");
  nts.insert("E'");
  nts.insert("F");
  nts.insert("T'");

  cout << "Enter the string: ";
  string str;
  cin >> str;

  node *root = new node("E");
  int cur = 0;
  cout << endl;
  int ret = findGraph(root, str, cur, "E");
  if (ret == 0 && cur == str.length())
  {
    cout << "String is accepted\n";
    return 0;
  }
  else
  {
    cout << "String is not accepted\n";
    return 1;
  }
}
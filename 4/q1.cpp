#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

char tcharr[100];

void replaceFinal(string a, string b2, string &ans)
{
  int i = 0;
  while (i < a.length() && a[i] != '-')
    i++;
  i += 2;
  while (i < a.length())
  {
    string temp = "";
    sscanf(a.c_str() + i, "%[^|]s", tcharr);
    temp = tcharr;
    i += (temp.length() + 1);
    ans = ans + temp + b2 + '|';
  }
}

bool doesMatch(string nt, string prod, int j)
{
  int i = 0;
  while (i < nt.length() && j < prod.length())
  {
    if (nt[i] == prod[j])
    {
      i++;
      j++;
    }
    else
    {
      return false;
    }
  }
  return true;
}

void replaceUtil(string nt, string a, string b, string &ans)
{
  int j = 0, flag = 0;
  string temp = "";
  int i = 0;
  sscanf(b.c_str(), "%[^-]s", tcharr);
  ans = tcharr;
  ans = ans + '-' + '>';
  i += ans.length();
  for (; i < b.length(); i++)
  {
    if (doesMatch(nt, b, i))
    {
      i += nt.length();
      sscanf(b.c_str() + i, "%[^|]s", tcharr);
      temp = tcharr;
      i += temp.length();
      replaceFinal(a, temp, ans);
    }
    else
    {
      sscanf(b.c_str() + i, "%[^|]s", tcharr);
      temp = tcharr;
      ans = ans + temp + '|';
      i += temp.length();
    }
  }
  ans = ans.substr(0, ans.length() - 1);
}

void replace(string &a, string &b)
{
  string nt;
  sscanf(a.c_str(), "%[^-]s", tcharr);
  nt = tcharr;
  string b2 = "";
  replaceUtil(nt, a, b, b2);
  b = b2;
}

void removeLeftRecursion(vector<string> &start, int cur, vector<string> &ans)
{
  for (int j = 0; j < cur; j++)
  {
    replace(start[j], start[cur]);
  }
  string s1 = "", s2 = "", nt;
  int i = 0;
  sscanf(start[cur].c_str(), "%[^-]s", tcharr);
  nt = tcharr;
  s1 = nt + "->";
  s2 = nt + "'->";
  i += (nt.length() + 2);
  int flag = 0;
  while (i < start[cur].length())
  {
    if (doesMatch(nt, start[cur], i))
    {
      flag = 1;
      i += nt.length();
      string temp;
      sscanf(start[cur].c_str() + i, "%[^|]s", tcharr);
      temp = tcharr;
      i += (temp.length() + 1);
      s2 = s2 + temp + nt + "'|";
    }
    else
    {
      string temp;
      sscanf(start[cur].c_str() + i, "%[^|]s", tcharr);
      temp = tcharr;
      s1 = s1 + temp + nt + "'|";
      i += (temp.length() + 1);
    }
  }
  if (flag == 1)
  {
    s2 += "ε";
    s1 = s1.substr(0, s1.length() - 1);
    start[cur] = s1;
    ans.push_back(s1);
    ans.push_back(s2);
  }
  else
  {
    ans.push_back(start[cur]);
  }
}

int main()
{
  vector<string> start, ans;
  int n;
  cout << "Enter the number of relations: ";
  cin >> n;
  for (int i = 0; i < n; i++)
  {
    string prod;
    cin >> prod;
    start.push_back(prod);
    string temp;
    sscanf(prod.c_str(), "%[^-]s", tcharr);
    temp = tcharr;
    removeLeftRecursion(start, i, ans);
  }
  cout << endl;
  for (int i = 0; i < ans.size(); i++)
  {
    cout << ans[i] << endl;
  }
  return 0;
}
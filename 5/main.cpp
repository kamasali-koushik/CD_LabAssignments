#include <bits/stdc++.h>
using namespace std;

#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GRE "\033[0;32m"
#define YEL "\033[0;33m"
#define BLUE "\033[0;34m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"
#define CYAN "\033[0;36m"

string start;
int cnt = 0;
string getnext(string var)
{
	string newvar = var + to_string(cnt);
	cnt++;
	return newvar;
}
struct node
{
	int count = 0;
	bool isEnd = false;
	map<string, node *> nxt;
};
struct Production
{
	string lhs;
	vector<vector<string>> rhs;
};
class Trie
{
public:
	node *root;
	vector<Production> graph;
	bool leftfactor = false;
	void insert(vector<string> &term)
	{
		node *curr = root;
		int sz = term.size();
		for (int i = 0; i < sz; i++)
		{
			if (curr->nxt.find(term[i]) == curr->nxt.end())
			{
				curr->nxt[term[i]] = new node();
				curr->count++;
			}
			else
			{
				leftfactor = true;
			}
			curr = curr->nxt[term[i]];
		}
		curr->isEnd = true;
		return;
	}
	void inserts(vector<vector<string>> &terms)
	{
		for (vector<string> &term : terms)
		{
			insert(term);
		}
	}
	vector<vector<string>> getall(Trie &c)
	{
		vector<vector<string>> ans;
		return ans;
	}
	void dfs(node *curr, vector<string> prefix, string parent, bool isEps = false)
	{
		// cout << curr->count << " " << parent;
		// if (prefix.size() > 0)
		// 	cout << " " << prefix.back();

		// cout << " " << curr->isEnd << endl;

		if (curr->count == 0)
		{
			if (prefix.size() > 0)
			{
				Production p;
				p.lhs = parent;
				p.rhs.push_back(prefix);
				if (isEps)
					p.rhs.push_back({"#"});
				graph.push_back(p);
			}
			return;
		}
		if (curr->count == 1 && !curr->isEnd)
		{
			for (auto it : curr->nxt)
			{
				prefix.push_back(it.first);
				dfs(it.second, prefix, parent);
			}
			return;
		}
		if (prefix.size() == 0&&!curr->isEnd)
		{
			for (auto it : curr->nxt)
			{
				vector<string> nxtprefix;
				nxtprefix.push_back(it.first);
				dfs(it.second, nxtprefix, parent );
			}
			return;
		}
		string nxtparent = getnext(parent);
		Production p;
		p.lhs = parent;
		p.rhs.push_back(prefix);
		if (isEps)
			p.rhs.push_back({"#"});
		p.rhs.back().push_back(nxtparent);
		graph.push_back(p);
		for (auto it : curr->nxt)
		{
			vector<string> nxtprefix;
			nxtprefix.push_back(it.first);
			dfs(it.second, nxtprefix, nxtparent, curr->isEnd);
		}
	}

	Trie()
	{
		root = new node();
	}
};
vector<string> splitter(string s, string delim)
{
	vector<string> ans;
	while (s.size() > 1 && s[0] == ' ')
	{
		s.erase(0, 1);
	}
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delim)) != std::string::npos)
	{
		token = s.substr(0, pos);
		ans.push_back(token);
		s.erase(0, pos + delim.length());
		while (s.size() > 1 && s[0] == ' ')
		{
			s.erase(0, 1);
		}
	}
	if (s.size() > 0)
		ans.push_back(s);
	return ans;
}
void removeLeft(vector<vector<string>> &ip, string start, map<string, vector<vector<string>>> &ans)
{
	vector<vector<string>> nonrec, rec;
	string startdash = start + "`";
	for (auto it : ip)
	{
		if (it[0] == start)
		{
			vector<string> temp = vector<string>(it.begin() + 1, it.end());
			temp.push_back(startdash);
			rec.push_back(temp);
		}
		else
		{

			nonrec.push_back(it);
			nonrec.back().push_back(startdash);
		}
	}
	if (rec.size() == 0)
	{
		ans[start] = ip;
		return;
	}
	rec.push_back(vector<string>(1, "#"));
	ans[start] = nonrec;
	ans[startdash] = rec;
	return;
}
class Grammar
{
	vector<Production> gr;
	string start;
	set<string> nonTerminals;
	set<string> terminals;
	map<string, vector<string>> first;
	map<string, vector<string>> follow;
	map<pair<string, string>, Production *> ptable;

public:
	vector<string> input;
	void printGrammar()
	{
		for (Production p : gr)
		{
			cout << p.lhs << " -> ";
			int i = 0;
			for (vector<string> terms : p.rhs)
			{
				if (i != 0)
					cout << " | ";
				int j = 0;
				for (auto chars : terms)
				{
					if (j > 0)
						cout << " ";
					cout << chars;
					j++;
				}
				i++;
			}
			cout << endl;
		}
	}
	void readGrammar(string name)
	{
		// input3 or input4
		ifstream *fin = new ifstream(name, ios_base::in);
		// cout<<"Reading grammar from "<<name<<endl;
		string s;
		try
		{
			if (!fin->is_open())
				throw "File not found";
			while (getline(*fin, s))
			{
				// cout<<s<<endl;
				vector<string> v = splitter(s, "->");
				if (v.size() != 2)
					throw "Invalid input";
				Production p;
				p.lhs = v[0];
				if (start.size() == 0)
					start = v[0];
				string lhs = v[0];
				string rhs = v[1];
				vector<string> v1 = splitter(rhs, "|");
				for (auto &it : v1)
				{
					// cout<<it<<endl;
					vector<string> temp1 = splitter(it, " ");
					p.rhs.push_back(temp1);
				}
				gr.push_back(p);
			}
		}
		catch (exception e)
		{
			cout << e.what();
		}
	}
	bool removeLeftRec()
	{
		bool flag = false;
		map<string, vector<vector<string>>> ans;
		int sz = gr.size();
		for (Production p : gr)
		{
			vector<vector<string>> newit2;
			for (auto &terms : p.rhs)
			{
				if (ans.find(terms[0]) != ans.end())
				{
					vector<string> rem = vector<string>(terms.begin() + 1, terms.end());
					vector<vector<string>> temp = ans[terms[0]];
					for (auto &it3 : temp)
					{
						it3.insert(it3.end(), rem.begin(), rem.end());
					}
					newit2.insert(newit2.end(), temp.begin(), temp.end());
					flag = true;
				}
				else
				{
					newit2.push_back(terms);
				}
			}
			removeLeft(newit2, p.lhs, ans);
		}
		gr.clear();
		for (auto it : ans)
		{
			Production p;
			p.lhs = it.first;
			p.rhs = it.second;
			gr.push_back(p);
		}
		return flag;
	}
	bool removeLeftFactor()
	{
		int sz = gr.size();
		bool leftfactor = false;
		vector<Production> nans;
		for (Production p : gr)
		{
			Trie *tr = new Trie();
			tr->inserts(p.rhs);
			vector<string> prefix;
			if (tr->leftfactor)
				leftfactor = true;

			tr->dfs(tr->root, prefix, p.lhs);
			for (Production p : tr->graph)
			{
				nans.push_back(p);
			}
		}
		if (leftfactor == false)
			return false;
		gr.clear();
		gr = nans;
		return true;
	}
	void fillnont()
	{
		map<string, vector<vector<string>>> grammar;
		for (auto it : gr)
		{
			nonTerminals.insert(it.lhs);
			for (auto t : it.rhs)
			{
				grammar[it.lhs].push_back(t);
			}
		}
		gr.clear();
		for (auto it : grammar)
		{
			Production p;
			p.lhs = it.first;
			p.rhs = it.second;
			gr.push_back(p);
		}
	}
	bool dfs(string curr, set<string> &first, set<string> &visited)
	{
		// find first using dfs
		bool containsEpsilon = false;
		visited.insert(curr);
		for (auto it : gr)
		{
			if (it.lhs == curr)
			{
				int sz = it.rhs.size();
				for (int i = 0; i < sz; i++)
				{
					if (it.rhs[i][0] == curr)
					{
						// left recursion
						// err
						// not possible
						continue;
					}
					set<string> newFirst;
					for (int j = 0; j < it.rhs[i].size(); i++)
					{
						if (nonTerminals.find(it.rhs[i][j]) == nonTerminals.end())
						{
							//
							if (it.rhs[i][j] == "#")
								containsEpsilon = true;
							first.insert(it.rhs[i][j]);
							break;
						}
						else
						{
							if (dfs(it.rhs[i][j], newFirst, visited))
							{
								containsEpsilon = true;
								if (j == it.rhs[i].size() - 1)
								{
									first.insert("#");
								}
							}
							else
							{
								break;
							}
						}
					}
					for (auto it : newFirst)
					{
						first.insert(it);
					}
				}
			}
		}
		return containsEpsilon;
	}
	void findFirst()
	{
		// use dfs to find first;
		set<string> fr;
		set<string> visited;
		for (auto it : nonTerminals)
		{
			fr.clear();
			visited.clear();
			dfs(it, fr, visited);
			first[it] = vector<string>(fr.begin(), fr.end());
		}
	}
	void printfirst()
	{
		for (auto it : first)
		{
			cout << it.first << " -> ";
			for (auto t : it.second)
			{
				cout << t << " ";
			}
			cout << endl;
		}
	}
	void findFollow()
	{
		set<string> fw;
		vector<string> ntrm;
		ntrm.push_back(start);
		map<string,vector<string>> incl;
		for (auto it : nonTerminals)
		{
			if (it != start)
			{
				ntrm.push_back(it);
			}
		}
		for (auto it : ntrm)
		{
			fw.clear();
			if (it == start)
			{
				fw.insert("$");
			}
			for (Production t : gr)
			{
				for (auto it2 : t.rhs)
				{
					int i;
					for (i = 0; i < it2.size(); i++)
					{
						if (it2[i] == it)
						{
							break;
						}
					}
					if (i == it2.size())
					{
						continue;
					}
					// found
					for (i += 1; i < it2.size(); i++)
					{
						if (nonTerminals.find(it2[i]) == nonTerminals.end())
						{
							fw.insert(it2[i]);
							break;
						}
						else
						{
							if (it2[i] == it)
							{
								continue;
							}
							bool epsilon = false;
							for (auto it3 : first[it2[i]])
							{
								if (it3 == "#")
								{
									epsilon = true;
									continue;
								}
								fw.insert(it3);
							}
							if (epsilon == false)
							{
								break;
							}
						}
					}
					if (i == it2.size())
					{
						// add follow of lhs
						incl[it].push_back(t.lhs);
						for (auto it3 : follow[t.lhs])
						{
							fw.insert(it3);
						}

					}
				}
			}
			//

			follow[it] = vector<string>(fw.begin(), fw.end());
		}
		// cout<<RED<<"follow includes"<<endl;
		for(auto it:incl)
		{
			fw.clear();
			set<string> visited;
			fw.insert(follow[it.first].begin(),follow[it.first].end());
			followdfs(it.first,incl,fw,visited);
			follow[it.first] = vector<string>(fw.begin(),fw.end());
		}
		// printFollow();

	}
	void followdfs(string s, map<string, vector<string>> &incl, set<string> &fw, set<string> &visited)
	{
		if (visited.find(s) != visited.end())
		{
			return;
		}
		visited.insert(s);
		for (auto it : incl[s])
		{
			if (visited.find(it) == visited.end())
			{
				followdfs(it, incl, fw, visited);
			}
		}
		for (auto it : follow[s])
		{
			fw.insert(it);
		}
	}
	void printFollow()
	{
		for (auto it : follow)
		{
			cout << it.first << " -> ";
			for (auto t : it.second)
			{
				cout << t << " ";
			}
			cout << endl;
		}
	}
	void findterminals()
	{
		for (auto it : gr)
		{
			for (auto it2 : it.rhs)
			{
				for (auto it3 : it2)
				{
					if (nonTerminals.find(it3) == nonTerminals.end())
					{
						terminals.insert(it3);
					}
				}
			}
		}
	}
	vector<string> getFirst(string a)
	{
		// cout << "first of " << a << endl;
		if (terminals.find(a) != terminals.end())
		{
			// cout << "terminal" << endl;
			return vector<string>(1, a);
		}
		return first[a];
	}
	void collectGrammar(){
		map<string, vector<vector<string>>> grammar;
		for (auto it : gr)
		{
			for (auto t : it.rhs)
			{
				grammar[it.lhs].push_back(t);
			}
		}
		gr.clear();
		for (auto it : grammar)
		{
			Production p;
			p.lhs = it.first;
			p.rhs = it.second;
			gr.push_back(p);
		}
	}
	bool makeParseTable()
	{
		//
		int n = nonTerminals.size();
		int m = terminals.size() + 1; // for $
		set<string> tnew = terminals;
		tnew.insert("$");
		// make table
		map<pair<string, string>, Production *> prs;
		// fill table
		for (auto it : gr)
		{
			// cout<<it.lhs<<" -> ";
			for (auto ti : it.rhs)
			{
				// cout<<ti[0]<<" ";
				for (auto ti2 : getFirst(ti[0]))
				{
					// cout<<ti2<<" ti2";
					if (ti2 == "#")
					{

						for (auto ti3 : follow[it.lhs])
						{
							Production *p = new Production();
							p->lhs = it.lhs;
							p->rhs.push_back(ti);
							if (prs.find({it.lhs, ti3}) != prs.end())
							{
								cout << RED << "Error in grammar(#)" << RESET << endl;
								return false;
							}
							prs[{it.lhs, ti3}] = p;
						}
					}
					else
					{
						// cout<<"something"<<it.lhs<<" "<<ti2<<endl;
						Production *p = new Production();
						p->lhs = it.lhs;
						p->rhs.push_back(ti);
						if (prs.find({it.lhs, ti2}) != prs.end())
						{
							cout << RED << "error in grammar()" << RESET << endl;
							return false;
						}
						prs[{it.lhs, ti2}] = p;
					}
				}
			}
		}
		ptable = prs;
		return true;
	}
	void printParseTable()
	{
		for (auto it : ptable)
		{
			cout << it.first.first << " " << it.first.second << " -> ";
			for (auto it2 : it.second->rhs)
			{
				for (auto it3 : it2)
				{
					cout << it3 << " ";
				}
			}
			cout << endl;
		}
	}
	void readString(string file)
	{
		ifstream *fin = new ifstream(file);
		string s;
		try
		{
			if (!fin->is_open())
				throw "File not found";

			string tmp = "";
			while (getline(*fin, s))
			{
				if (tmp != "")
					tmp += " ";
				tmp += s;
			}
			cout << RED << tmp << endl;
			vector<string> v = splitter(tmp, " ");
			input = v;
			parseString();
		}
		catch (exception e)
		{
			cout << e.what();
		}
	}
	void parseString()
	{
		stack<string> st;
		// ll1 parsing
		st.push("$");
		input.push_back("$");
		st.push(start);
		int idx = 0;
		while (st.top() != "$")
		{
			// cout << st.top() << " " << input[idx] << endl;
			string p = st.top();
			if (idx < input.size() && p == input[idx])
			{
				st.pop();
				if (input[idx] != "$")
					idx++;
			}
			else
			{
				if (terminals.find(p) != terminals.end())
				{
					cout<<BLUE << "ERROR parsing" <<p<< endl;
					break;
				}
				if (ptable.find({p, input[idx]}) == ptable.end())
				{
					cout<<YEL << "ERROR parsing"<<p <<" "<<input[idx]<< endl;
					break;
				}
				Production *pr = ptable[{p, input[idx]}];
				st.pop();
				cout << pr->lhs << " -> ";
				int rhsz = pr->rhs[0].size();
				for (int i = rhsz - 1; i >= 0; i--)
				{
					cout << pr->rhs[0][rhsz - i - 1] << " ";
					if (pr->rhs[0][i] != "#")
					{
						st.push(pr->rhs[0][i]);
					}
				}
				cout << endl;
			}
		}
		if (st.top() != "$")
		{
			cout << RED << "ERROR parsing $" << endl;
			while (!st.empty())
			{
				cout << st.top() << " ";
				st.pop();
			}
		}
		else
		{
			cout << "String parsed successfully" << endl;
		}
	}
};
// take name of file as arg
int main(int argc, char *argv[])
{
	string filename;
	string stringfile;
	stringfile = "out.txt";
	if (argc != 3)
	{
		cout << RED << "invalid name using default input3.txt" << RESET << endl;
		string nfile = "input3.txt";
		filename = nfile;
	}
	else
	{
		filename = argv[1];
		stringfile = argv[2];
	}
	Grammar *gram = new Grammar();
	gram->readGrammar(filename);
	cout << GRE << "Given Grammar:\n";
	// gram->printGrammar();
	cout << RED << "after removing left rec\n";
	bool leftrec = gram->removeLeftRec();
	// gram->printGrammar();
	cout << CYAN << "after removing left factor\n";
	bool leftfac = gram->removeLeftFactor();
	// print 80 -
	cout << RED;
	printf("-----------------\n");
	gram->collectGrammar();
	gram->printGrammar();
	gram->fillnont();
	gram->findterminals();
	printf("-----------------\n");
	cout << GRE;
	printf("-----------------\nFirst of grammar\n");
	gram->findFirst();
	gram->printfirst();
	cout << BLUE;
	printf("-----------------\nFollow of grammar\n");
	gram->findFollow();
	gram->printFollow();
	cout << YEL;
	printf("-----------------\nParse Table\n");
	if (!gram->makeParseTable())
		return -1;
	gram->printParseTable();
	cout << CYAN;
	printf("-----------------\nleftmost derivation\n");
	gram->readString(stringfile);
	cout << RESET;
	return 0;
}
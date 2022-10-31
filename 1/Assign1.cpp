#include <bits/stdc++.h>
using namespace std;

class NFANode
{
public:
    int c;
    int type;

    unordered_map<char, vector<NFANode *>> transitions;
    NFANode(int typ)
    {
        type = typ;
    }
    void setType(int typ)
    {
        this->type = typ;
    }
};

class NFA
{
public:
    NFANode *start;
    vector<NFANode *> end;
    NFA(char c)
    {
        start = new NFANode(0);
        start->c = 0;
        NFANode *end = new NFANode(1);
        end->c = 1;
        this->end.push_back(end);
        start->transitions[c].push_back(end);
    }
};

int prec(char c)
{

    if (c == '.')
    {
        return 2;
    }
    else if (c == '|' || c == '+')
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

NFA *eval(NFA *a, char op, NFA *b = NULL)
{
    NFA *res;
    switch (op)
    {
    case '*':
    {
        NFANode *new_state = new NFANode(2);
        a->start->setType(-1);
        new_state->transitions['e'].push_back(a->start);

        for (auto node : a->end)
        {
            node->transitions['e'].push_back(a->start);
        }

        a->end.push_back(new_state);
        a->start = new_state;

        return a;
    }
    break;
    case '+':
    case '|':
    {
        NFANode *new_state = new NFANode(0);
        a->start->setType(-1);
        b->start->setType(-1);

        new_state->transitions['e'].push_back(a->start);
        new_state->transitions['e'].push_back(b->start);

        a->start = new_state;

        NFANode *new_end_state = new NFANode(1);
        for (auto node : a->end)
        {
            node->setType(-1);
            node->transitions['e'].push_back(new_end_state);
        }
        for (auto node : b->end)
        {
            node->setType(-1);
            node->transitions['e'].push_back(new_end_state);
        }

        a->end = {new_end_state};

        return a;
    }
    break;
    case '.':
    {
        for (auto node : a->end)
        {
            node->transitions['e'].push_back(b->start);
            node->setType(-1);
        }
        b->start->setType(-1);

        a->end = b->end;

        return a;
    }
    break;
    default:
        cout << "Invalid operation \"" << op << "\" on NFAs!\n";
        exit(-1);
        break;
    }

    return res;
}

void printNFA(NFA *A)
{
    unordered_map<NFANode *, bool> visit, assign;
    queue<NFANode *> q;
    int final = 0;

    string s = "digraph G  {  \nstart [shape=\"plaintext\"]\n";
    A->start->c = final++;
    assign[A->start] = true;
    s = s + "start ->  0\n";

    q.push(A->start);

    while (!q.empty())
    {
        NFANode *temp = q.front();
        visit[q.front()] = true;
        q.pop();
        for (auto x : temp->transitions)
        {
            for (auto node : x.second)
            {

                if (!assign[node])
                {
                    node->c = final++;
                    assign[node] = true;
                }
                if (temp->type == 0)
                    cout << "(start state)";
                else if (temp->type == 1)
                    cout << "(end state)";
                else if (temp->type == 2)
                    cout << "(start state and end state)";
                cout << temp->c << " ---" << x.first << "--> ";

                if (node->type == 1 || node->type == 2)
                    cout << "(end state)";
                cout << node->c << endl;

                if (temp->type >= 1)
                    s = s + to_string(temp->c) + " [shape=\"doublecircle\"];\n";

                s = s + to_string(temp->c) + " -> ";

                if (node->c == 'e')
                    s = s + to_string(node->c) + "[label : \"&epsilon;\"];\n ";
                else
                {
                    s = s + to_string(node->c);
                    s = s + +" [label=\"";
                    if (x.first == 'e')
                        s = s + "&epsilon;";
                    else
                        s = s + x.first;
                    s = s + "\"];\n";
                }

                if (!visit[node])
                {
                    q.push(node);
                    visit[node] = true;
                }
            }
        }
    }
    s = s + "}";
    cout << endl
         << s;

    ofstream outFile("edges.dot");
    outFile << s;
    outFile.close();
    system("dot -Tpng edges.dot -o image.png");

    system("xdg-open image.png");
}

void Solve(string s)
{

    stack<NFA *> nfa_stack;
    stack<char> st;

    int l = s.length();

    for (int i = 0; i < l; i++)
    {
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))
        {

            nfa_stack.push(new NFA(s[i]));
        }

        else if (s[i] == '(')
        {
            st.push('(');
        }
        else if (s[i] == ')')
        {
            while (st.top() != '(')
            {
                char c = st.top();
                st.pop();
                NFA *temp = nfa_stack.top();
                nfa_stack.pop();
                NFA *temp1 = nfa_stack.top();
                nfa_stack.pop();

                nfa_stack.push(eval(temp1, c, temp));
            }
            if (st.top() == '(')
            {
                char c = st.top();
                st.pop();
            }
        }
        else
        {
            if (s[i] == '*')
            {
                NFA *temp = nfa_stack.top();
                nfa_stack.pop();
                nfa_stack.push(eval(temp, s[i], NULL));

                continue;
            }

            while (prec(s[i]) <= prec(st.top()))
            {
                char c = st.top();
                st.pop();
                NFA *temp = nfa_stack.top();
                nfa_stack.pop();
                NFA *temp1 = nfa_stack.top();
                nfa_stack.pop();

                nfa_stack.push(eval(temp1, c, temp));
            }

            st.push(s[i]);
        }
    }

    printNFA(nfa_stack.top());
}

string preprocessor(string s)
{
    char x[5000];
    int l = s.length();
    int j = 0;
    x[j] = '(';
    j += 1;
    for (int i = 0; i < l; i++)
    {
        x[j] = s[i];
        j += 1;
        if (s[i] >= 97 && s[i] <= 122 && s[i + 1] >= 97 && s[i + 1] <= 122)
        {
            x[j] = '.';
            j += 1;
        }
        else if (s[i] == ')' && s[i + 1] == '(')
        {
            x[j] = '.';
            j += 1;
        }
        else if (s[i] >= 97 && s[i] <= 122 && s[i + 1] == '(')
        {
            x[j] = '.';
            j += 1;
        }
        else if (s[i] == ')' && s[i + 1] >= 97 && s[i + 1] <= 122)
        {
            x[j] = '.';
            j += 1;
        }
        else if (s[i] == '*' && (s[i + 1] == '(' || (s[i + 1] >= 97 && s[i + 1] < 122)))
        {
            x[j] = '.';
            j += 1;
        }
    }
    x[j] = ')';
    j += 1;
    string p;
    for (int i = 0; i < j; i++)
        p += x[i];
    return p;
}

int main()
{

    cout << "enter the expression :" << endl;
    cout << "ex : (a|b)*d" << endl;

    string s;
    cin >> s;

    s = preprocessor(s);
    cout << "Converting the expression into : ";
    cout << s << endl;

    Solve(s);
}


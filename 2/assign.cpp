#include<bits/stdc++.h>
using namespace std;

bool check_digit(char ip){
    if(ip >= '0' && ip <= '9')
        return true;
    return false;
}
bool check_number(string ip){
    int start = 0,count_e = 0,count_dot = 0,count_op = 0,idx_e,idx_dot;
    if(ip[0] == '-' && ip.length()>1 ){
        start++;
    }
    if(ip[0] == '.'){
        if(ip[1] == 'e')
            return false;
        start++;
        count_dot++;
    }
    if(ip[start]=='e')
        return false;
    
    for(int i = start; i < ip.length();i++){
        if(count_e > 1 || count_dot > 1 || count_op > 1)
            return false;
        if(ip[i]=='.'){
            count_dot++;
            if(count_e == 1)
                return false;
        }else if(ip[i] == 'e'){
            count_e++;
            if((ip[i+1] == '+' || ip[i+1] == '-')){
                if(i>ip.length()-3)
                    return false;
                if(check_digit(ip[i+2]))
                    count_op++;
                else
                    return false;
                i=i+2;
            }    
        }else if(check_digit(ip[i]))
            continue;
        else 
            return false;
    }
    return true;
}
bool check_id(string ip){
    if(!(ip[0]>='a' && ip[0]<='z' )&& !(ip[0]>='A' && ip[0]<='Z' ))
        return false;
    for(int i = 1;i<ip.size();i++){
        if(!(ip[0]>='a' && ip[0]<='z' ) && !(ip[0]>='A' && ip[0]<='Z' ) && !(ip[0]>='0' && ip[0]<='9' ))
            return false;
    }
    return true;
}
bool check_relop(string ip){
    if(ip == "<" || ip == ">" || ip == "<=" || ip == ">=" || ip == "==" || ip == "!=")
        return true;
    return false;
}
bool check_ariop(string ip){
    if(ip == "+" || ip == "-" || ip == "*" || ip == "/" || ip == "%" || ip == "++" || ip == "--")
        return true;
    return false;
}

bool check_asgop(string ip){
    if(ip == "=" || ip == "+=" || ip == "-=" || ip == "/=" || ip == "*=" || ip == "%=")
        return true;
    return false;
}

bool check_logop(string ip){
    if(ip == "||" || ip == "&&" || ip == "!")  
        return true;
    return false;
}

void generator(string ip){
    if(check_number(ip)){
        cout << "(number , "<<ip<<")"<<endl;
    }else if(ip == "if"){
        cout << "(if , )"<<endl;
    }else if(ip=="then"){
        cout << "(then , )"<<endl;
    }else if(ip=="else"){
        cout << "(else , )"<<endl;
    }else if(check_id(ip)){
        cout << "(id , "<<ip<<")"<<endl;
    }else if(check_relop(ip)){
        cout << "(relop , "<<ip<<")"<<endl;
    }else if(check_ariop(ip)){
        cout << "(relop , "<<ip<<")"<<endl;
    }else if(check_asgop(ip)){
        cout << "(relop , "<<ip<<")"<<endl;
    }else if(check_logop(ip)){
        cout << "(loglop , "<<ip<<")"<<endl;
    }else{
        cout<<"(invalid , "<<ip<<")"<<endl;
    }
}



string addspace(string ip)
{
    string temp;
  for(int i=0;i<ip.size();i++) 
  {
    if((ip[i] == '>' && ip[i+1] == '=') || (ip[i] == '=' && ip[i+1] == '=') || (ip[i] == '!' && ip[i+1] == '=')  )
    {
        temp.push_back(' ');
        temp.push_back(ip[i]);
        temp.push_back(ip[i+1]);
        temp.push_back(' ');
        i++;
    } 
    else if( ip[i] == '<' || ip[i] == '>'|| ip[i] == '=')
    {
        temp.push_back(' ');
         temp.push_back(ip[i]);
        temp.push_back(' ');
    }
    else
    {
        temp.push_back(ip[i]);
    }
  }

  return temp;  
}




int main(){
    char ch;
    cout<< "Enter the statement"<<endl;
    string ip,dum="";
    getline(cin,ip);
   
    cout << endl<< "Tuples of (tokens,values)"<<endl;

   ip=addspace(ip);
int len = ip.length();

    for(int i = 0; i < len;i++){
        if(ip[i]==' '){
            generator(dum);
            dum = "";
        }else
            dum+=ip[i];
    }
    if(dum.length())
        generator(dum);
    return 0;
    
}
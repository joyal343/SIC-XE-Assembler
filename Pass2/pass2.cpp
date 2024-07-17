#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>//has overloaded getline
#include <vector>
using namespace std;

class oprec{
    public:
    string opcode;
    int type;
    oprec(int t,string cd){
        opcode=cd;
        type=t;
    }
};

class symrec{
    public:
    string addr;
    symrec(string a){
        addr=a;
    }
};

class modrec{
    public:
    string addr;
    modrec(string a){
        addr=a;
    }
};

string decToHex(int num1){
    if (num1 == 0)
        return "0";
    int num = num1;
    string s = "";
    while (num) { 
        int temp = num % 16;
        if (temp <= 9){//
            s+=48 + temp;//ASCII code 0 is 48
        }
        else
            s+=55 + temp;//ASCII code A 65 
        num = num / 16;
    }
    int n=s.length();
    for (int i = 0; i < n / 2; i++){// reverse the string
        char temp= s[i];
        s[i]=s[n-i-1];
        s[n-i-1]=temp;
    }
    return s;
}
int hexToDecimal(string hexVal)
{
    int len = hexVal.size();
    int base = 1;
    int dec_val = 0;
    for (int i = len - 1; i >= 0; i--) {
        if (hexVal[i] >= '0' && hexVal[i] <= '9') {
            dec_val += (int(hexVal[i]) - 48) * base;// increment dec val with base
            base = base * 16;// increment base
        }

        else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
            dec_val += (int(hexVal[i]) - 55) * base;
            base = base * 16;
        }
    }
    return dec_val;
}


string reg(string r){// number correspond register 
    if (r=="A")
        return "0";
    else if (r=="X")
        return "1";
    else if (r=="L")
        return "2";
    else if (r=="B")
        return "3";
    else if (r=="S")
        return "4";
    else if (r=="T")
        return "5";
    else if (r=="F")
        return "6";
}

string optobi(char a){// object code 2nd hex number in binary
    if (a=='0')        // start with format 3 & 4
        return "00";
    else if(a=='4')
        return "01";
    else if(a=='8')
        return "10";
    else 
        return "11";
}

string bitohex(string s){
    if(s=="0000")
        return "0";
    if(s=="0001")
        return "1";
    if(s=="0010")
        return "2";
    if(s=="0011")
        return "3";
    if(s=="0100")
        return "4";
    if(s=="0101")
        return "5";
    if(s=="0110")
        return "6";
    if(s=="0111")
        return "7";
    if(s=="1000")
        return "8";
    if(s=="1001")
        return "9";
    if(s=="1010")
        return "A";
    if(s=="1011")
        return "B";
    if(s=="1100")
        return "C";
    if(s=="1101")
        return "D";
    if(s=="1110")
        return "E";
    if(s=="1111")
        return "F";
}

int chkInd(string opr){
    char ch;
    for(int i=0;i<opr.length();i++){
        ch=opr[i];
        if(ch==',')
            if(opr[i+1]=='X')
                return 1;
    }
    return 0;
}

string obj_instr(string opsym ,string opr,string loc,unordered_map <string,oprec*> optab,unordered_map <string,symrec*> symtab,string b){
    string objcd="";
    string r1,r2;
    stringstream ss(opr);
    string s1="",s2="";// what happens if + infront - not in optab
    string temp="";
    int pc,ea,disp;
    int addr;
    if (opsym[0]=='+'){
        opsym=opsym.substr(1,opsym.length()-1);
        objcd+=optab[opsym]->opcode[0];
        s1+=optobi(optab[opsym]->opcode[1]);
        s1+="11";
        s2+="0001";
        objcd+=bitohex(s1);
        objcd+=bitohex(s2);
        objcd+="0";
        objcd+=symtab[opr]->addr;
    }
    else if(opsym=="BYTE"){
        string x=decToHex(stoi(opr));
        if (x.length()==1){
            objcd+="0";
            objcd+=x;
        }
        else 
            objcd+=x;
        return objcd;
    }
    else if(opsym=="WORD"){
        string x=decToHex(stoi(opr));
        if (x.length()==1){
            objcd+="00000";
            objcd+=x;
        }
        else if (x.length()==2){
            objcd+="0000";
            objcd+=x;
        }
        else if (x.length()==3){
            objcd+="000";
            objcd+=x;
        }
        else if (x.length()==4){
            objcd+="00";
            objcd+=x;
        }
        else if (x.length()==5){
            objcd+="0";
            objcd+=x;
        }
        else 
            objcd+=x;
        return objcd;
    }
    else if(optab[opsym]->type==1){
        objcd+=optab[opsym]->opcode;
        return objcd;
    }
    else if(optab[opsym]->type==2){
        objcd+=optab[opsym]->opcode;
        getline(ss, r1, ',');
        getline(ss, r2, ',');
        objcd+=reg(r1);
        objcd+=reg(r2);
        return objcd;
    }
    else if(opsym[0]!='+'){
        objcd+=optab[opsym]->opcode[0];  
        s1+=optobi(optab[opsym]->opcode[1]);
        if(opr[0]=='#'){
            int num;
            string hex;
            s1+="01";
            s2+="0000";
            objcd+=bitohex(s1);
            objcd+=bitohex(s2);
            if (opr[1]>='0'&&opr[1]<='9'){// to check if int operand supplied
                opr=opr.substr(1,opr.length()-1);
                stringstream ss(opr);// used ss to convert string to int
                ss>>num;
                temp=decToHex(num);
                if(temp.length()==0)
                    objcd+="000";
                else if(temp.length()==1){
                    objcd+="00";
                    objcd+=temp;
                }
                else if(temp.length()==2){
                    objcd+="0";
                    objcd+=temp;
                }
                else if(temp.length()==3){
                    objcd+=temp;
                }
                return objcd;
            }
            opr=opr.substr(1,opr.length()-1);// if symbol operand supplied
            temp=symtab[opr]->addr;
            temp=temp.substr(1,temp.length()-1);
            objcd+=temp;           
        }
        else {
            s1+="11";//n=1 i=1
            if(chkInd(opr)==1){
                stringstream ss(opr);
                getline(ss,opr,',');
                s2+="1";
                }
            else 
                s2+="0";
            pc=hexToDecimal(loc)+3;
            ea=hexToDecimal(symtab[opr]->addr);
            disp=ea-pc;
            if(-2048<=disp&& disp<=2047){
                s2+="010";
                objcd+=bitohex(s1);
                objcd+=bitohex(s2);
                if (disp>=0){
                    temp=decToHex(disp);
                    if(temp.length()==0)
                        objcd+="000";
                    else if(temp.length()==1){
                        objcd+="00";
                        objcd+=temp;
                    }
                    else if(temp.length()==2){
                        objcd+="0";
                        objcd+=temp;
                    }
                    else if(temp.length()==3){
                        objcd+=temp;
                    }
                    return objcd;
                }
                else{
                    disp=-disp;
                    temp=decToHex(disp);
                    string temp2;char ch;
                    int val;
                    for (int i=0;i<temp.length();i++){
                        ch=temp[i];
                        if(i<temp.length()-1){
                        if (ch>='0'&&ch<='9'){
                            val=(15-(ch-48));
                            temp2+=decToHex(val);
                        }
                        else if(ch>='A'&&ch<='F'){
                            val=(15-(ch-55));
                            temp2+=decToHex(val);
                        }
                        }
                        else{
                        if (ch>='0'&&ch<='9'){
                            val=(16-(ch-48));
                            temp2+=decToHex(val);
                        }
                        else if(ch>='A'&&ch<='F'){
                            val=(16-(ch-55));
                            temp2+=decToHex(val);
                        }
                        }
                    }
                    if(temp.length()==0)
                        objcd+="FFF";
                    else if(temp.length()==1){
                        objcd+="FF";
                        objcd+=temp2;
                    }
                    else if(temp.length()==2){
                        objcd+="F";
                        objcd+=temp2;
                    }
                    else if(temp.length()==3){
                        objcd+=temp2;
                    }
                    return objcd;
                }

            }
            disp=ea-hexToDecimal(b);
            if(0<=disp&&disp<=4095){
                s2+="100";
                objcd+=bitohex(s1);
                objcd+=bitohex(s2);
                if (disp>=0){
                    temp=decToHex(disp);
                    if(temp.length()==0)
                        objcd+="000";
                    else if(temp.length()==1){
                        objcd+="00";
                        objcd+=temp;
                    }
                    else if(temp.length()==2){
                        objcd+="0";
                        objcd+=temp;
                    }
                    else if(temp.length()==3){
                        objcd+=temp;
                    }
                    return objcd;
                }

            }

        }
    }
}
vector <string> split_st(string st){
    string a="";
    vector <string> stl;
    for (int i=0;i<st.length();i++){
        if(st[i]==' ' || st[i]=='\n'){
            cout<<a<<endl;
            if(a!=""){
                stl.push_back(a);
            }
            a="";
        }
        else {
            a+=st[i];
        }
    }
    return stl;
}

int count(string s, char c){// 2 spaces --> 3 words 
    int res = 0;
    for (int i=0;i<s.length();i++)
        if (s[i] == c)
            res++;
    res++;
    return res;
}

int main(){
    unordered_map <string,oprec*> optab;
    unordered_map <string,symrec*> symtab;
    optab["ADD"]=new oprec(3,"18");
    optab["ADDF"]=new oprec(3,"58");
    optab["ADDR"]=new oprec(2,"90");
    optab["CLEAR"]=new oprec(2,"B4");
    optab["FIX"]=new oprec(1,"C4");
    optab["SUB"]=new oprec(3,"1C");
    optab["FLOAT"]=new oprec(1,"C0");
    optab["STL"]=new oprec(3,"14");
    optab["STA"]=new oprec(3,"0C");
    optab["STX"]=new oprec(3,"10");
    optab["LDS"]=new oprec(3,"6C");
    optab["LDT"]=new oprec(3,"74");
    optab["LDX"]=new oprec(3,"04");
    optab["LDA"]=new oprec(3,"00");
    optab["COMPR"]=new oprec(2,"A0");
    optab["JLT"]=new oprec(3,"38");
    optab["JEQ"]=new oprec(3,"30");
    optab["JSUB"]=new oprec(3,"48");
    optab["J"]=new oprec(3,"3C");
    symtab["ONE"]=new symrec("0015");
    symtab["ALPHA"]=new symrec("0018");
    symtab["BETA"]=new symrec("001E");
    symtab["GAMMA"]=new symrec("0021");
    symtab["DELTA"]=new symrec("0024");
    symtab["INCR"]=new symrec("0027");
    symtab["ARR"]=new symrec("002A");
    symtab["ANUM"]=new symrec("102A");
    vector <string> modtab;
//obj_instr(string opsym ,string opr,string loc,unordered_map <string,oprec*> optab,unordered_map <string,symrec*> symtab,string b)
    string pgmsize="002A";
    fstream file1;
    ofstream file2;
    file1.open("interm.txt",ios::in);
    string inputline="";
    int x;
    string b="0000";//BASE REGISTER
    string opr,opsym,loc,label;
    string tr="",temp="",ln="",trtemp="";
    while(getline(file1,inputline)){
        x=count(inputline,' ');// count function line 378
        cout<<x<<endl;
        stringstream ss(inputline);// breaks a sentence based on ' '
        //division based on x bec. no of outputs from string stream///////////////////////////////////////////////////////
        if(x==2){//To update base register  
            ss>>opsym;
            ss>>opr;
            b=symtab[opr]->addr;
        }
        if(x==3){
            ss>>loc;// loc given
            ss>>opsym;
            ss>>opr;
            if(opsym[0]=='+'){
                modtab.push_back(loc);
            }
            if(opsym=="END"){
                file2.open("output.txt",ios::app);
                int x;string y,op;
                if(trtemp.length()!=0){// trtemp is temporary text record
                    tr="T^";
                    tr+=("00"+loc+"^");
                    ln=decToHex(trtemp.length());
                    if (ln.length()==1){
                        tr+="0";
                        tr+=ln;
                        tr+="^";
                    }
                    else{
                        tr+=ln;
                        tr+="^";
                    }
                    tr+=trtemp;
                    file2<<tr<<"\n";
                    trtemp="";
                }
                for(auto i=modtab.begin();i!=modtab.end();i++){// resolving modification records
                    x=hexToDecimal(*i);
                    x+=1;// dont know hexadecimal addition
                    y=decToHex(x);
                    if(y.length()==0)
                        op="0000";
                    else if(y.length()==1){
                        op="000";
                        op+=y;
                    }
                    else if(y.length()==2){
                        op="00";
                        op+=y;
                    }
                    else if(y.length()==3){
                        op="0";
                        op+=y;
                    }
                    else 
                        op=y;
                    file2<<"M^"<<"00"<<op<<"^05\n";
                }
                file2<<"E^000000\n";
                file2.close();
            }
            else{
                temp=obj_instr(opsym,opr,loc,optab,symtab,b);
                if((temp.length()+trtemp.length())>60){// triggers if overfilled
                    tr="T^";
                    tr+=("00"+loc+"^");
                    ln=decToHex(trtemp.length());
                    if (ln.length()==1){
                        tr+="0";
                        tr+=ln;
                        tr+="^";
                    }
                    else{
                        tr+=ln;
                        tr+="^";
                    }
                    tr+=trtemp;
                    file2.open("output.txt",ios::app);
                    file2<<tr<<"\n";
                    file2.close();
                    trtemp="";
                }
                trtemp.append(temp);
            }
        }
        else if(x==4){
            ss>>loc;
            ss>>label;
            ss>>opsym;
            ss>>opr;
            if(opsym[0]=='+'){
                modtab.push_back(loc);
            }
            if (opsym=="START"){
                file2.open("output.txt",ios::app);
                file2<<"H^"<<label<<"^"<<"000000^"<<"00"<<pgmsize<<"\n";
                file2.close();
            }
            else{
                if (opsym!="RESW" && opsym!="RESB"){
                temp=obj_instr(opsym,opr,loc,optab,symtab,b);
                if((temp.length()+trtemp.length())>60){
                    tr="T^";
                    tr+=("00"+loc+"^");//loc is last loc
                    ln=decToHex(trtemp.length());
                    if (ln.length()==1){
                        tr+="0";
                        tr+=ln;
                        tr+="^";
                    }
                    else{
                        tr+=ln;
                        tr+="^";
                    }
                    tr+=trtemp;
                    file2.open("output.txt",ios::app);
                    file2<<tr<<"\n";
                    file2.close();
                    trtemp="";
                }
                trtemp.append(temp);
                }
                else if(trtemp!=""){// if it is RESW or RESB
                    tr="T^";
                    tr+=("00"+loc+"^");
                    ln=decToHex(trtemp.length());
                    if (ln.length()==1){
                        tr+="0";
                        tr+=ln;
                        tr+="^";
                    }
                    else{
                        tr+=ln;
                        tr+="^";
                    }
                    tr+=trtemp;
                    file2.open("output.txt",ios::app);
                    file2<<tr<<"\n";
                    file2.close();
                    trtemp="";
                }
            }
        }
    }
    file1.close();
    
}


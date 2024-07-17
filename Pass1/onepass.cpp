#include<iostream>
#include<map>
#include<vector>
#include<fstream>
#include<sstream>
#include<vector>
#include<math.h>
#include<string>
#include<cstring>
using namespace std;
string deci_hex(int n){
    int quotient=n/16;
    int remain;
     string ren="";
vector<string>arr3;
    while(quotient>1){
     remain=n%16;
       n=quotient;
       quotient=quotient/16;
if(remain<=9){
    arr3.push_back(to_string(remain));
}
else {
  string x;
   switch (remain)
   {
   case 10:
    x="A";
     arr3.push_back(x);
    break;
      case 11:
    x="B";
    arr3.push_back(x);
    break;
      case 12:
    x="C";
    arr3.push_back(x);
    break;
      case 13:
    x="D";
    arr3.push_back(x);
    break;
     case 14:
    x="E";
    arr3.push_back(x);
    break;
      case 15:
    x="F";
    arr3.push_back(x);
    break;
   default:
    break;
   }
     
 }
    }
    for(int i=arr3.size();i>=0;i--){
        ren=ren+arr3[i];
    }

    return ren;

}
string formate(string sstr){
  fstream optab("OPTAB.txt");
  string ope,ope1,ope2,formi1;
  int formate1;
  vector<string>optab1;
  vector<string>opsym;
  vector<string>forma;
  vector<string>opco;
  while(!optab.eof()){
    getline(optab,ope);
    stringstream rr(ope);
    while(getline(rr,ope1,' ')){
        optab1.push_back(ope1);
    }
        opsym.push_back(optab1[0]);
        forma.push_back(optab1[1]);
        opco.push_back(optab1[2]);
        optab1.clear();
  }
  for(int j=0;j<opsym.size();j++){
    string k="+"+opsym[j];
     if(opsym[j]==sstr){
         formi1=forma[j];
        formate1= stoi(formi1);
     }else if(k==sstr){
        formate1=4;
     }
  }
      string formate11;
      formate11=to_string(formate1);
      return formate11;
}
int hex_to_deci(char t[],int y){
    int size1=y;
    //sizeof(t)/sizeof(t[0]);
   //int k; 
   int power;
     int sum2;
    vector<int>arr1;
for(int i=0;i<size1;i++){
    string g;
    
    char tt=t[i];
    g=tt;
    int k;
    if(stoi(g)<=9){
    k=stoi(g);
      arr1.push_back(k);
    }else{
     if(g=="A"){
         k=10;
        arr1.push_back(k);
     }else if(g=="B"){
        k=11;
        arr1.push_back(k);
     }else if(g=="C"){
        k=12;
        arr1.push_back(k);
     }else if(g=="D"){
        k=13;
        arr1.push_back(k);
     }else if(g=="E"){
        k=14;
        arr1.push_back(k);
     }
     else if(g=="F"){
        k=11;
        arr1.push_back(k);
     }
    }
}
for(int i=0;i<arr1.size();i++){
    int size2=arr1.size();
    
    while(size2>=0){
       power=pow(16,size2);
       size2=size2-1;
     sum2=power*arr1[i]+sum2;
    }
}
return sum2;
}

string addition_of_hex(string hex,string forma2){
        int lenght1=hex.length();
        int lenght2=forma2.length();
        char hexarry[lenght1+1];
        strcpy(hexarry,hex.c_str());
        char formaarry[lenght2+1];
        strcpy(formaarry,forma2.c_str());
      int intofhex=hex_to_deci(hexarry,lenght1+1);
      int intofforma2=hex_to_deci(formaarry,lenght2+1);
      int sum3=intofforma2+intofhex;
    string res=deci_hex(sum3);
    return res;
}

int main(){
    int count,num=1;
string str1;
fstream inputfile("input1.txt");
fstream symtab("symtab.txt");
fstream opcode("opcode.txt");
fstream operand("operand.txt");
fstream optab("OPTAB.txt");
fstream new1inputfile("input1.txt");
fstream new1inputfile1("intput2.txt");
vector<string>inputline1;
vector<string>opecod1;
string str;
if(inputfile.is_open()){ 

    cout<<"input file is open"<<endl;
    while(!inputfile.eof()){

    getline(inputfile,str);
    stringstream ss(str);
  
  while(getline(ss,str1,' ')){

     inputline1.push_back(str1);
    }

   if(inputline1.size()==3){
        if(inputline1[1]=="START"){

            string prgname=inputline1[0];
            cout<<"the programme name is: "<<prgname<<endl;
        }
        else{

        symtab<<inputline1[0]<<endl;
        opcode<<inputline1[1]<<endl;
        operand<<inputline1[2]<<endl;
        }
    }
    else if(inputline1.size()==2){
          if(inputline1[0]=="END"){
            string startaddress=inputline1[1];
            cout<<"the starting adress of the programme is: "<<startaddress<<endl;
          }else{
        opcode<<inputline1[0]<<endl;
        operand<<inputline1[1]<<endl;
          }
    }
   inputline1.clear();
}
    }
  //  inputfile.close();
   
  // fstream new1inputfile("intput2.txt");
   if (new1inputfile.is_open())
   {string newline,newline1,resultant;
   string starting_adress="0000";
   vector<string>newopcode;
   cout<<"see ya germans"<<endl;
     while(!new1inputfile.eof()){

         getline(new1inputfile,newline);
         stringstream kk(newline);
         
          while (getline(kk,newline1,' ')){
               newopcode.push_back(newline1);
          }
          if(new1inputfile1.is_open()){
          if(newopcode.size()==3){
            cout<<"hi howdy how aaaa"<<endl;
          string hh=formate(newopcode[1]);
          cout<<hh;
           resultant=addition_of_hex(starting_adress,hh);
           starting_adress=resultant;
            new1inputfile1<<resultant;
          newopcode.clear();
          }
          else if(newopcode.size()==2){
            cout<<"nice people huh"<<endl;
            string hh1=formate(newopcode[0]);
            cout<<hh1<<endl;
           resultant=addition_of_hex(starting_adress,hh1);
           cout<<resultant<<endl;
           starting_adress=resultant;
            new1inputfile1<<resultant;
          newopcode.clear();
          }
          }
       }
      
   }
   


    return 0;
}


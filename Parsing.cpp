#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<cstring>
#include<set>
#include<queue>
using namespace std;

string start,VN[10],VT[10],EXP_F[100];
int VN_num,VT_num,EXP_num;
bool check_first[30],check_exp_first[100],check_space_first[100];
map<string,int> GV;
map<string,int> GT;
map<string,int> EXP;
vector<string> GV_alpha[10];
vector<string> VN_F[10];
vector<string> EXP_first[100];
vector<string> VN_first[10];
vector<string> VN_follow[10];
vector<string> FA[20][20];

void read(){
    string s,t,k;
    bool check;
    while(1){
        t="";
        k="";
        check=false;
        cin>>s;
        if(s=="-1") break;
        int l=s.length();
        for(int i=0;i<l;i++){
            if(s[i]=='='){
                GV[t]=VN_num;
                VN[VN_num++]=t;
                check=true;
            }
            else if(s[i]=='|'){
                GV_alpha[GV[t]].push_back(k);
                k="";
            }
            else{
                if(!check){
                    t=t+s[i];
                }
                else{
                    k=k+s[i];
                }
            }
        }
        GV_alpha[GV[t]].push_back(k);
    }
    start=VN[0];
    return;
}

bool isLetter(char x){
    return (x>='a'&&x<='z');
}

bool isNum(char x){
    return x>='0'&&x<='9';
}

void initG(){
    string s=" ",t="";
    for(int i=0;i<VN_num;i++){
        int l=GV_alpha[GV[VN[i]]].size();
        for(int j=0;j<l;j++){
            EXP_F[EXP_num]=GV_alpha[GV[VN[i]]][j];
            EXP[GV_alpha[GV[VN[i]]][j]]=EXP_num++;
        }
        s=s+VN[i]+' ';
    }
    for(int i=0;i<EXP_num;i++){
        int l=EXP_F[i].length();
        for(int j=0;j<l;j++){
            if(EXP_F[i][j]!='`'&&s.find(EXP_F[i][j])==-1){
                if(isLetter(EXP_F[i][j])){
                    string st="";
                    for(;j<l;j++){
                        if(isLetter(EXP_F[i][j])){
                            st=st+EXP_F[i][j];
                        }
                        else{
                            j--;
                            break;
                        }
                    }
                    if(t.find(st)==-1&&st!="space"){
                        GT[st]=VT_num;
                        VT[VT_num++]=st;
                        t=t+st+' ';
                    }
                }
                else if(t.find(EXP_F[i][j])==-1){
                    string gt="";
                    gt=gt+EXP_F[i][j];
                    GT[gt]=VT_num;
                    VT[VT_num++]=EXP_F[i][j];
                    t=t+EXP_F[i][j]+' ';
                }
                // string t="";
                // t=t+EXP_F[i][j];
                // for(int k=j;k<l;k++){
                //     if(EXP_F[i][k]=='`'){
                //         t=t+EXP_F[i][k];
                //     }
                //     else break;
                // }
                // if(s.find(' '+t+' ')==-1){
                //     VT[VT_num++]=EXP_F[i][j];
                // }
            }
        }
    }
    return;
}

void leftRecure(){
    vector<string> s1,s2;
    for(int i=0;i<VN_num;i++){
        s1.clear();
        s2.clear();
        int l=GV_alpha[GV[VN[i]]].size();
        for(int j=0;j<l;j++){
            if(GV_alpha[GV[VN[i]]][j].find(VN[i])==0){
                s1.push_back(GV_alpha[GV[VN[i]]][j]);
            }
            else{
                s2.push_back(GV_alpha[GV[VN[i]]][j]);
            }
        }
        if(!s1.empty()){
            string t=VN[i]+'`';
            int l1=s1.size(),l2=s2.size(),lt=VN[i].length();
            GV_alpha[GV[VN[i]]].clear();
            GV[t]=VN_num;
            VN[VN_num++]=t;
            for(int j=0;j<l2;j++){
                GV_alpha[GV[VN[i]]].push_back(s2[j]+t);
            }
            for(int j=0;j<l1;j++){
                string st=s1[j].substr(lt);
                GV_alpha[GV[t]].push_back(st+t);
            }
            GV_alpha[GV[t]].push_back("space");
        }
    }
    return;
}

bool isVT(string x){
    for(int i=0;i<VT_num;i++){
        if(x.find(VT[i])!=-1){
            return true;
        }
    }
    return false;
}

bool isVN(string x){
    for(int i=0;i<VN_num;i++){
        if(x.find(VN[i])!=-1){
            return true;
        }
    }
    return false;
}

bool forecastFirstSpace(string x){
    bool check=true;
    if(isVT(x)){
        return false;
    }
    else{
        for(int i=0;i<VN_num;i++){
            int t=x.find(VN[i]);
            if(t!=-1&&((t+VN[i].length())==x.length()||x[t+VN[i].length()]!='`')&&!check_space_first[GV[VN[i]]]){
                int l=GV_alpha[GV[VN[i]]].size();
                check_space_first[GV[VN[i]]]=true;
                for(int j=0;j<l;j++){
                    if(GV_alpha[GV[VN[i]]][j]=="space"){
                        break;
                    }
                    else if(forecastFirstSpace(GV_alpha[GV[VN[i]]][j])){
                        break;
                    }
                    else if(j==(l-1)){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

vector<string> forecastFirst(string x){
    vector<string> s;
    s.clear();
    check_exp_first[EXP[x]]=true;
    if(x=="space"){
        s.push_back("space");
        return s;
    }
    for(int i=0;i<VN_num;i++){
        int t=x.find(VN[i]);
        if(t==0&&((t+VN[i].length())==x.length()||x[t+VN[i].length()]!='`')&&!check_first[GV[VN[i]]]){
            int l=GV_alpha[GV[VN[i]]].size();
            check_first[GV[VN[i]]]=true;
            for(int j=0;j<l;j++){
                vector<string> st;
                st.clear();
                if(GV_alpha[GV[VN[i]]][j]=="space"){
                    st.push_back("space");
                    check_exp_first[EXP[GV_alpha[GV[VN[i]]][j]]]=true;
                    EXP_first[EXP[GV_alpha[GV[VN[i]]][j]]]=st;
                    memset(check_space_first,0,sizeof(check_space_first));
                    if(forecastFirstSpace(x)){
                        s.push_back("space");
                    }
                    st.clear();
                    string s1=x.substr(t+VN[i].length());
                    st=forecastFirst(s1);
                    s.insert(s.end(),st.begin(),st.end());
                }
                else{
                    st=forecastFirst(GV_alpha[GV[VN[i]]][j]);
                    EXP_first[EXP[GV_alpha[GV[VN[i]]][j]]]=st;
                    s.insert(s.end(),st.begin(),st.end());
                }
            }
        }
    }
    for(int i=0;i<VT_num;i++){
        if(x.find(VT[i])==0&&!check_first[i+VN_num]){
            check_first[i+VN_num]=true;
            s.push_back(VT[i]);
        }
    }
    return s;
}

void findFirst(){
    set<string> s;
    for(int i=0;i<EXP_num;i++){
        memset(check_first,0,sizeof(check_first));
        if(!check_exp_first[EXP[EXP_F[i]]]){
            EXP_first[EXP[EXP_F[i]]]=forecastFirst(EXP_F[i]);
        }
    }
    memset(check_exp_first,0,sizeof(check_exp_first));
    for(int i=0;i<EXP_num;i++){
        s.clear();
        s.insert(EXP_first[EXP[EXP_F[i]]].begin(),EXP_first[EXP[EXP_F[i]]].end());
        EXP_first[EXP[EXP_F[i]]].assign(s.begin(),s.end());
    }
    for(int i=0;i<VN_num;i++){
        int l=GV_alpha[GV[VN[i]]].size();
        s.clear();
        for(int j=0;j<l;j++){
            s.insert(EXP_first[EXP[GV_alpha[GV[VN[i]]][j]]].begin(),EXP_first[EXP[GV_alpha[GV[VN[i]]][j]]].end());
        }
        VN_first[GV[VN[i]]].assign(s.begin(),s.end());
    }
    return;
}

void findFollow(){
    set<string> s[10];
    bool check=true;
    s[GV[start]].insert("$");
    while(check){
        check=false;
        for(int i=0;i<VN_num;i++){
            int l=GV_alpha[GV[VN[i]]].size();
            for(int j=0;j<l;j++){
                for(int k=0;k<VN_num;k++){
                    int t=GV_alpha[GV[VN[i]]][j].find(VN[k]);
                    bool check_st=false;
                    if(t!=-1&&(t+VN[k].length())==GV_alpha[GV[VN[i]]][j].length()){
                        if(s[GV[VN[k]]].find("$")==s[GV[VN[k]]].end()){
                            s[GV[VN[k]]].insert("$");
                            check=true;
                        }
                        check_st=true;
                    }
                    else if(t!=-1&&GV_alpha[GV[VN[i]]][j][t+VN[k].length()]!='`'){
                        memset(check_first,0,sizeof(check_first));
                        string st=GV_alpha[GV[VN[i]]][j].substr(t+VN[k].length());
                        vector<string> st_first=forecastFirst(st);
                        int st_l=st_first.size();
                        for(int c=0;c<st_l;c++){
                            if(st_first[c]=="space"){
                                if(s[GV[VN[k]]].find("$")==s[GV[VN[k]]].end()){
                                    s[GV[VN[k]]].insert("$");
                                    check=true;
                                }
                                check_st=true;
                            }
                            else{
                                if(s[GV[VN[k]]].find(st_first[c])==s[GV[VN[k]]].end()){
                                    s[GV[VN[k]]].insert(st_first[c]);
                                    check=true;
                                }
                            }
                        }
                    }
                    if(check_st){
                        int sk_l_before=s[GV[VN[k]]].size();
                        s[GV[VN[k]]].insert(s[GV[VN[i]]].begin(),s[GV[VN[i]]].end());
                        int sk_l_after=s[GV[VN[k]]].size();
                        if(sk_l_before!=sk_l_after) check=true;
                    }
                }
            }
        }
    }
    for(int i=0;i<VN_num;i++){
        VN_follow[GV[VN[i]]].assign(s[GV[VN[i]]].begin(),s[GV[VN[i]]].end());
    }
    return;
}

void print(){
    cout<<endl<<"Eliminate Left Recursion:"<<endl;
    for(int i=0;i<VN_num;i++){
        cout<<VN[i]<<'=';
        int l=GV_alpha[GV[VN[i]]].size();
        for(int j=0;j<l;j++){
            cout<<GV_alpha[GV[VN[i]]][j];
            if(j!=(l-1)) cout<<'|';
        }
        cout<<endl;
    }
    return;
}

void printV(){
    cout<<endl<<"VN:"<<endl;
    for(int i=0;i<VN_num;i++){
        cout<<VN[i]<<' ';
    }
    cout<<endl<<"VT:"<<endl;
    for(int i=0;i<VT_num;i++){
        cout<<VT[i]<<' ';
    }
    cout<<endl;
    return;
}

void printFirst(){
    cout<<endl<<"First Set:"<<endl;
    for(int i=0;i<VN_num;i++){
        cout<<"First("<<VN[i]<<"): ";
        int l=VN_first[GV[VN[i]]].size();
        for(int j=0;j<l;j++){
            cout<<VN_first[GV[VN[i]]][j]<<' ';
        }
        cout<<endl;
    }
    for(int i=0;i<EXP_num;i++){
        cout<<"First("<<EXP_F[i]<<"): ";
        int l=EXP_first[EXP[EXP_F[i]]].size();
        for(int j=0;j<l;j++){
            cout<<EXP_first[EXP[EXP_F[i]]][j]<<' ';
        }
        cout<<endl;
    }
    return;
}

void printFollow(){
    cout<<endl<<"Follow Set:"<<endl;
    for(int i=0;i<VN_num;i++){
        cout<<"Follow("<<VN[i]<<"): ";
        int l=VN_follow[GV[VN[i]]].size();
        for(int j=0;j<l;j++){
            cout<<VN_follow[GV[VN[i]]][j]<<' ';
        }
        cout<<endl;
    }
}

void isLL1(){
    cout<<endl<<"Judging LL(1)..."<<endl;
    bool check=false,check_sq[100];
    memset(check_sq,0,sizeof(check_sq));
    for(int i=0;i<VN_num;i++){
        int l=GV_alpha[GV[VN[i]]].size();
        if(l>1){
            queue<int> sq;
            while(!sq.empty()){
                sq.pop();
            }
            for(int j=0;j<l;j++){
                int lf=EXP_first[EXP[GV_alpha[GV[VN[i]]][j]]].size();
                for(int k=j+1;k<l;k++){
                    set<string> s(EXP_first[EXP[GV_alpha[GV[VN[i]]][k]]].begin(),EXP_first[EXP[GV_alpha[GV[VN[i]]][k]]].end());
                    for(int c=0;c<lf;c++){
                        if(s.find(EXP_first[EXP[GV_alpha[GV[VN[i]]][j]]][c])!=s.end()){
                            check=true;
                            break;
                        }
                    }
                    if(check) break;
                    else{
                        cout<<"First("<<GV_alpha[GV[VN[i]]][j]<<") intersect First("<<GV_alpha[GV[VN[i]]][k]<<") = space"<<endl;
                    }
                }
                for(int k=0;k<lf;k++){
                    if(EXP_first[EXP[GV_alpha[GV[VN[i]]][j]]][k]=="space"){
                        sq.push(j);
                        break;
                    }
                }
                if(check) break;
            }
            while(!sq.empty()){
                int t=sq.front();
                sq.pop();
                set<string> s(VN_follow[GV[VN[i]]].begin(),VN_follow[GV[VN[i]]].end());
                for(int j=0;j<l;j++){
                    if(!check_sq[EXP[GV_alpha[GV[VN[i]]][j]]]&&j!=t){
                        check_sq[EXP[GV_alpha[GV[VN[i]]][j]]]=true;
                        int lf=EXP_first[EXP[GV_alpha[GV[VN[i]]][j]]].size();
                        for(int k=0;k<lf;k++){
                            if(s.find(EXP_first[EXP[GV_alpha[GV[VN[i]]][j]]][k])!=s.end()){
                                check=true;
                                break;
                            }
                        }
                        if(check) break;
                        else{
                            cout<<"Follow("<<VN[i]<<") intersect First("<<GV_alpha[GV[VN[i]]][j]<<") = space"<<endl;
                        }
                    }
                }
                if(check) break;
            }
        }
        if(check) break;
    }
    if(check){
        cout<<"It is not LL(1) grammar."<<endl;
        system("pause");
    }
    else cout<<"It is LL(1) grammar."<<endl;
    return;
}

void buildForecastAnalysisTable(){
    for(int i=0;i<VN_num;i++){
        int l=GV_alpha[GV[VN[i]]].size();
        for(int j=0;j<l;j++){
            bool check=false;
            int lf=EXP_first[EXP[GV_alpha[GV[VN[i]]][j]]].size();
            for(int k=0;k<lf;k++){
                if(EXP_first[EXP[GV_alpha[GV[VN[i]]][j]]][k]=="space") check=true;
                FA[GV[VN[i]]][GT[EXP_first[EXP[GV_alpha[GV[VN[i]]][j]]][k]]].push_back(GV_alpha[GV[VN[i]]][j]);
            }
            if(check){
                int lfo=VN_follow[GV[VN[i]]].size();
                for(int k=0;k<lfo;k++){
                    if(VN_follow[GV[VN[i]]][k]=="$") FA[GV[VN[i]]][VT_num].push_back(GV_alpha[GV[VN[i]]][j]);
                    else FA[GV[VN[i]]][GT[VN_follow[GV[VN[i]]][k]]].push_back(GV_alpha[GV[VN[i]]][j]);
                }
            }
        }
    }
    return;
}

void printFA(){
    cout<<endl<<"Forecast Analysis Table:"<<endl<<"          ";
    for(int i=0;i<VT_num;i++){
        int l=VT[i].length();
        cout<<VT[i];
        for(int j=0;j<(10-l);j++) cout<<" ";
    }
    cout<<"$"<<endl;
    for(int i=0;i<VN_num;i++){
        int l=VN[i].length();
        cout<<VN[i];
        for(int j=0;j<(10-l);j++) cout<<" ";
        for(int j=0;j<VT_num;j++){
            if(!FA[GV[VN[i]]][GT[VT[j]]].empty()){
                int lfa=FA[GV[VN[i]]][GT[VT[j]]].size();
                for(int k=0;k<lfa;k++){
                    int  lf=FA[GV[VN[i]]][GT[VT[j]]][k].length()+VN[i].length()+1;
                    cout<<VN[i]<<"="<<FA[GV[VN[i]]][GT[VT[j]]][k];
                    for(int c=0;c<(10-lf);c++) cout<<" ";
                }
            }
            else{
                cout<<"x         ";
            }
        }
        if(!FA[GV[VN[i]]][VT_num].empty()){
            int lfa=FA[GV[VN[i]]][VT_num].size();
            for(int k=0;k<lfa;k++){
                int  lf=FA[GV[VN[i]]][VT_num][k].length()+VN[i].length()+1;
                cout<<VN[i]<<"="<<FA[GV[VN[i]]][VT_num][k];
                for(int c=0;c<(10-lf);c++) cout<<" ";
            }
        }
        else{
            cout<<"x         ";
        }
        cout<<endl;
    }
    return;
}

vector<string> initS(string x){
    vector<string> s;
    s.clear();
    int l=x.length();
    bool check_num=false;
    for(int i=0;i<l;i++){
        if(isNum(x[i])){
            check_num=true;
        }
        else{
            if(check_num) s.push_back("num");
            check_num=false;
            string st="";
            st=st+x[i];
            s.push_back(st);
        }
    }
    if(check_num) s.push_back("num");
    return s;
}

void printError(){
    cout<<endl<<"Error!This can not match."<<endl;
    system("pause");
}

vector<string> initL(string x){
    vector<string> s;
    string t="";
    s.clear();
    int l=x.length();
    for(int i=0;i<l;i++){
        t=t+x[i];
        if(isVN(t)&&(i==(l-1)||x[i+1]!='`')){
            s.push_back(t);
            t="";
        }
        else if(isVT(t)){
            s.push_back(t);
            t="";
        }
    }
    return s;
}

void forecastAnalyze(){
    vector<string> faz;
    vector<string> fas;
    string x,otp="";
    bool check=false;
    int pointer_s=0,pointer_z=0;
    cout<<endl<<"Waiting for the sentence..."<<endl;
    cin>>x;
    faz.clear();
    faz.push_back("$");
    faz.push_back(start);
    pointer_z=1;
    fas.clear();
    fas=initS(x);
    fas.push_back("$");
    int l=fas.size();
    cout<<endl<<"Forecast Analysis Process:"<<endl;
    cout<<"stack               input               output              left sentence pattern"<<endl;
    do{
        int lz=0,ls=0;
        for(int i=0;i<=pointer_z;i++){
            cout<<faz[i];
            lz+=faz[i].length();
        }
        for(int i=0;i<(20-lz);i++) cout<<" ";
        for(int i=pointer_s;i<l;i++){
            cout<<fas[i];
            ls+=fas[i].length();
        }
        for(int i=0;i<(20-ls);i++) cout<<" ";
        if(!check) cout<<"                    ";
        else{
            cout<<otp;
            int lt=otp.length();
            for(int i=0;i<(20-lt);i++) cout<<" ";
        }
        check=false;
        for(int i=0;i<pointer_s;i++) cout<<fas[i];
        for(int i=pointer_z;i>0;i--) cout<<faz[i];
        string xz=faz[pointer_z],xs=fas[pointer_s];
        if(xz=="$"||isVT(xz)){
            if(xz==xs){
                faz.pop_back();
                pointer_z--;
                pointer_s++;
            }
            else{
                printError();
                break;
            }
        }
        else{
            if(!FA[GV[xz]][GT[xs]].empty()){
                faz.pop_back();
                pointer_z--;
                vector<string> fae;
                fae.clear();
                fae=initL(FA[GV[xz]][GT[xs]][0]);
                int le=fae.size();
                for(int i=le-1;i>=0;i--){
                    faz.push_back(fae[i]);
                    pointer_z++;
                }
                otp=xz+"="+FA[GV[xz]][GT[xs]][0];
                check=true;
            }
            else{
                printError();
                break;
            }
        }
        cout<<endl;
    }while(pointer_z>=0);
}

int main(){
    read();
    leftRecure();
    initG();
    findFirst();
    findFollow();
    printV();
    print();
    printFirst();
    printFollow();
    isLL1();
    buildForecastAnalysisTable();
    printFA();
    forecastAnalyze();
    return 0;
}
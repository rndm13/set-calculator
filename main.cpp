#include <ostream>
#include <istream>
#include <map>
#include <iostream>
#include <set>
#include <string>
#include <algorithm>
#include <regex>
#include <vector>
#include <queue>
#include <iterator>
using namespace std;
struct info_manager {
    bool print_info=0;
    ostream &os = cout;
    template <typename T>
    info_manager &operator<<(T msg)
    {
        if (print_info)
            os << msg;
        return *this;
    }
};
namespace set_math {

    template <typename T,typename R>  
    set<pair<T, R>> multiplication(set<T> set_a,set<R> set_b) {
        set<pair<T,R>> set_result;
        for (T a : set_a) {
            for (R b : set_b) {
                set_result.insert({a,b});
            }
        }
        return set_result;
    }
    template <typename T>
    set<T> _union(set<T> set_a,set<T> set_b) {
        set<T> set_result;
        set_union(  set_a.begin(),set_a.end(),
                    set_b.begin(),set_b.end(),
                    inserter(set_result,set_result.end()));
        return set_result;
    }
    template <typename T>
    set<T> difference(set<T> set_a,set<T> set_b) {
        set<T> set_result;
        set_difference( set_a.begin(),set_a.end(),
                        set_b.begin(),set_b.end(),
                        inserter(set_result,set_result.end()));
        return set_result;
    }
    template <typename T>
    set<T> intersection(set<T> set_a,set<T> set_b) {
        set<T> set_result;
        set_intersection(   set_a.begin(),set_a.end(),
                            set_b.begin(),set_b.end(),
                            inserter(set_result,set_result.end()));
        return set_result;
    }
    template <typename T>
    set<T> sym_difference(set<T> set_a,set<T> set_b) {
        set<T> set_result;
        set_symmetric_difference(   set_a.begin(),set_a.end(),
                                    set_b.begin(),set_b.end(),
                                    inserter(set_result,set_result.end()));
        return set_result;
    }
    template <typename T>
    set<T> not_operation(set<T> set_a,set<T> Universal) {
        return difference(Universal,set_a);
    }
}
namespace set_io {

    template <typename T>
    void insert(set<T> &set_a,const size_t size_a) {
        for (size_t i = 0;i<size_a;i++) {
            string x;
            cin >> x;
            set_a.insert(x);
        }
    }
    set<string> get_set(string name, ostream &os ,istream &is) {
        set<string> new_set;
        os << "size_"<<name<<": ";
        size_t size_set;
        is >> size_set;
        os << "set_"<<name<<": ";
        insert(new_set,size_set);
        return new_set;
    }
    template <typename T,typename R>
    ostream& operator<<(ostream &os,const pair<T,R> &x) {
        os << '{'<<x.first<<", "<<x.second<<'}';
        return os;
    }

    template <typename T>
    void print(set<T> set_a, string end = "\n") {
        for (T a : set_a)
            cout << a << end;
    }
}
bool comparator_result(string a,string b) {
    int an=0,bn=0;
    bool ab=0,bb=0;
    if (isdigit(a[0])) {
        an=stoi(a);
        ab=1;
    }
    if (isdigit(b[0])) {
        bn=stoi(b);
        bb=1;
    }
    if (ab && bb)
        return an < bn;
    if (ab)
        return 1;
    if (bb)
        return 0;
    return a<b;
}
int string_type(string s) {
    if (s.size()>1 || (s[0]>='A' && s[0]<='Z'))
        return 1;
    if (s[0]=='(' || s[0]==')')
        return 3;
    else
        return 2;
}
ostream& operator<<(ostream &os, pair<int,size_t> p) {
    os <<'{'<< p.first << ' '<<p.second<<"}\n";
    return os;
}
template <typename T,class C,class M>
void print_priority_queue(priority_queue<T,C,M> priorq) {
    while (!priorq.empty()){
        cout << priorq.top();
        priorq.pop();
    }
    cout <<'\n';
}
int main(int argc,char *argv[]) {
    const string universal_name = "UNIVERSAL";
    info_manager IM;
    bool sort_result=0;
    for (int i = 1;i<argc;i++) {
        string option = argv[i];
        if (option == "-s" || option == "--sort"){
            sort_result=1;
            IM <<"[INFO]   sort option set (argument = "<<i<<")\n"; 
        }
        else if (option == "-i" || option == "--info"){
            IM.print_info=1;
            IM <<"[INFO]   info option set (argument = "<<i<<")\n";
        }
    }
    map<char,int> operation_priority_lookup;
    operation_priority_lookup['o']=0;
    operation_priority_lookup['/']=1;
    operation_priority_lookup['u']=2;
    operation_priority_lookup['n']=3;
    operation_priority_lookup['~']=4;
    string expression;
    getline(cin,expression);
    regex regex_filter(R"abc(([A-Z]+)|([un\/o~])|(\(|\)))abc"); 
    //TODO: make operation list autogenerate from map of operations
    //TODO: make regex operations autogenerate from map of operations
    vector <string> filter {
        sregex_token_iterator(expression.begin(),expression.end(),regex_filter,0),
        sregex_token_iterator()
    };
    IM << "[INFO]   filtered out the expression like so:";
    for (size_t i = 0;i<filter.size();i++)
        IM << filter[i] << ' ';
    IM <<'\n';
    auto comp = [](pair<int,size_t> left, pair<int,size_t> right) {
            if (left.first!=right.first)
                return left.first<right.first;
            else return left.second>right.second;
        };
    priority_queue<
        pair<int,size_t>,
        vector<pair<int,size_t>>,
        decltype(comp)> operation_order(comp);
    map<string,set<string>> name_set_lookup;
    int prior_stack = 0;
    for (size_t i = 0;i<filter.size();i++) {
        if (filter[i]=="(")
            prior_stack+=9;
        else if (filter[i]==")")
            if (prior_stack>=9)
                prior_stack-=9;
            else {
                cerr << "[ERROR]   NOT BALANCED BRACES\n";
                exit(1);
            }
        else if (string_type(filter[i])==2) {
            operation_order.push({operation_priority_lookup[filter[i][0]]+prior_stack,i});
            IM <<"[INFO]   pushed operation '"<<filter[i][0]<<"' with priority "<<operation_priority_lookup[filter[i][0]]+prior_stack<<" in position "<<i<<'\n';
            if (filter[i][0]=='~' && name_set_lookup.find(universal_name) == name_set_lookup.end()) {
                IM <<"[INFO]   UNIVERSAL set not found. Creating new one\n";
                name_set_lookup[universal_name]=set_io::get_set("UNIVERSAL",cout,cin);
            }
        }
        else if (string_type(filter[i])==1 && name_set_lookup.find(filter[i]) == name_set_lookup.end()) {
            IM << "[INFO]   found new set '"<< filter[i] <<"' at position "<<i<<'\n';
            name_set_lookup[filter[i]]=set_io::get_set(filter[i],cout,cin);
        }
    }
    if (prior_stack) {
        cerr << "[ERROR]   NOT BALANCED BRACES\n";
        exit(1);
    }
    IM << "[INFO]   finished making priority queue\n";
    vector<pair<size_t,size_t>> index_deletions;
    while (!operation_order.empty()) {
        size_t original_ind = operation_order.top().second;
        size_t ind=original_ind;
        for (auto x : index_deletions) {
            if(original_ind>x.first)
                ind-=x.second;
        }
        IM <<"[INFO]   solving operator '"<<filter[ind]<<"' at the original position "<<original_ind<<" with the current being "<<ind<<'\n';
        operation_order.pop();
        IM <<"[INFO]   poped queue\n";
        bool two_argument_operation=1;
        char operation = filter[ind][0];
        if (filter[ind][0]=='~')
            two_argument_operation=0;
        if (two_argument_operation) {
            string arg_left  = filter[ind-1]; 
            string arg_right = filter[ind+1]; 
            IM << "            it is a two argument function with arguments '"<<arg_left<<"' as left argument and '"<<arg_right<<"' as right argument\n";
            bool braces = (ind>=2 && ind<=filter.size()-3 && filter[ind-2]=="(" && filter[ind+2]==")");
            if (braces)
            IM << "            function has braces on both sides\n";
            IM << "            created new name: "<<(braces?"(":"")+arg_left+filter[ind]+arg_right+(braces?")":"")<<'\n';
            filter[ind-1-braces] = (braces?"(":"")+arg_left+filter[ind]+arg_right+(braces?")":"");
            IM << "            executing operation\n";
            try {
                switch (operation) {
                    case 'u':
                        name_set_lookup[filter[ind-1-braces]] = set_math::_union(name_set_lookup[arg_left],name_set_lookup[arg_right]);
                    break;
                    case 'n':
                        name_set_lookup[filter[ind-1-braces]] = set_math::intersection(name_set_lookup[arg_left],name_set_lookup[arg_right]);
                    break;
                    case '/':
                        name_set_lookup[filter[ind-1-braces]] = set_math::difference(name_set_lookup[arg_left],name_set_lookup[arg_right]);
                    break;
                    case 'o':
                        name_set_lookup[filter[ind-1-braces]] = set_math::sym_difference(name_set_lookup[arg_left],name_set_lookup[arg_right]);
                    break;
                }
            }
            catch(runtime_error &err) {
                cerr<<"[ERROR]   "<<err.what()<<'\n';
            }
            IM << "            removing left overs ["<<ind-braces+1<<'-'<<ind+2+braces<<")\n";
            try {
            filter.erase(filter.begin()+ind-braces,filter.begin()+ind+2+braces);
            }
            catch (runtime_error &err) {
                cerr <<"[ERROR]   "<<err.what()<<'\n';
            }
            index_deletions.push_back({original_ind,2+braces*2});
        }
        else
        {
            string arg_right = filter[ind+1]; 
            IM << "            it is a single argument function with argument '"<<arg_right<<"' as right argument\n";
            bool braces = (ind>=1 && ind<=filter.size()-3 && filter[ind-1]=="(" && filter[ind+2]==")");
            IM << "            function has braces on both sides\n";
            IM << "            creating new name: "<<(braces?"(":"")+filter[ind]+arg_right+(braces?")":"")<<'\n';
            filter[ind-braces] = (braces?"(":"")+filter[ind]+arg_right+(braces?")":"");
            IM << "            executing operation\n";
            try { 
                switch (operation) {
                    case '~':
                        name_set_lookup[filter[ind-braces]] = set_math::not_operation(name_set_lookup[arg_right],name_set_lookup[universal_name]);
                    break;
                }
            }
            catch (const runtime_error &err) {
                cerr <<"[ERROR]   "<<err.what()<<'\n';
            }
            IM << "            removing left overs ["<<ind-braces+1<<'-'<<ind+2+braces<<")\n";
            try {
                filter.erase(filter.begin()+ind-braces+1,filter.begin()+ind+2+braces);
            }
            catch (const runtime_error &err) {
                cerr <<"[ERROR]   "<<err.what()<<'\n';
            }
            index_deletions.push_back({original_ind,1+braces*2});
        }
        IM <<"[INFO]   Solved this operation\n";
        IM <<"[INFO]   new filter:\n";
        for (size_t i = 0;i<filter.size();i++)
            IM << filter[i] << ' ';
        IM <<'\n';
    }
    vector<string> result;
    for (auto x : name_set_lookup[filter[0]])
        result.push_back(x);
    if (sort_result) {
        IM <<"[INFO]   Sorting result\n";
        sort(result.begin(),result.end(),comparator_result);
        IM <<"[INFO]   Sorting finished\n";
    }
    cout << filter[0]<<" = {";
    for (size_t i = 0;i<result.size();i++)
        cout << result[i]<<' ';
    cout <<"}\n";
    return 0;
}


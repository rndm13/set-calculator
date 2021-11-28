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
namespace set_math {

    template <typename T,typename R>  
    set<pair<T, R>> multiplication(set<T> set_a,set<R> set_b)
    {
        set<pair<T,R>> set_result;
        for (T a : set_a)
        {
            for (R b : set_b)
            {
                set_result.insert({a,b});
            }
        }
        return set_result;
    }
    template <typename T>
    set<T> _union(set<T> set_a,set<T> set_b)
    {
        set<T> set_result;
        set_union(  set_a.begin(),set_a.end(),
                    set_b.begin(),set_b.end(),
                    inserter(set_result,set_result.end()));
        return set_result;
    }
    template <typename T>
    set<T> difference(set<T> set_a,set<T> set_b)
    {
        set<T> set_result;
        set_difference( set_a.begin(),set_a.end(),
                        set_b.begin(),set_b.end(),
                        inserter(set_result,set_result.end()));
        return set_result;
    }
    template <typename T>
    set<T> intersection(set<T> set_a,set<T> set_b)
    {
        set<T> set_result;
        set_intersection(   set_a.begin(),set_a.end(),
                            set_b.begin(),set_b.end(),
                            inserter(set_result,set_result.end()));
        return set_result;
    }
    template <typename T>
    set<T> sym_difference(set<T> set_a,set<T> set_b)
    {
        set<T> set_result;
        set_symmetric_difference(   set_a.begin(),set_a.end(),
                                    set_b.begin(),set_b.end(),
                                    inserter(set_result,set_result.end()));
        return set_result;
    }
}
namespace set_io {

    template <typename T>
    void insert(set<T> &set_a,const size_t size_a)
    {
        for (size_t i = 0;i<size_a;i++)
        {
            string x;
            cin >> x;
            set_a.insert(x);
        }
    }

    template <typename T,typename R>
    ostream& operator<<(ostream &os,const pair<T,R> &x)
    {
        os << '{'<<x.first<<", "<<x.second<<'}';
        return os;
    }

    template <typename T>
    void print(set<T> set_a, string end = "\n")
    {
        for (T a : set_a)
            cout << a << end;
    }
}
int string_type(string s)
{
    if (s.size()>1 || (s[0]>='A' && s[0]<='Z'))
        return 1;
    if (s[0]=='(' || s[0]==')')
        return 3;
    else
        return 2;
}
int main()
{
    map<char,int> operation_priority_lookup;
    operation_priority_lookup['o']=0;
    operation_priority_lookup['/']=1;
    operation_priority_lookup['u']=2;
    operation_priority_lookup['n']=3;
    string expression;
    cin >> expression;
    regex regex_filter(R"abc(([A-Z]+)|([un\/ox])|(\(|\)))abc"); 
    //TODO: make operation list autogenerate from map of operations
    vector <string> filter {
        sregex_token_iterator(expression.begin(),expression.end(),regex_filter,0),
        sregex_token_iterator()
    };
    auto comp = [](pair<int,size_t> left, pair<int,size_t> right){
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
    for (size_t i = 0;i<filter.size();i++)
    {
        //cout << "PROCESSING "<<filter[i]<<'\n';
        if (filter[i]=="(")
            prior_stack+=9;
        else if (filter[i]==")")
            if (prior_stack>=9)
                prior_stack-=9;
            else
            {
                cerr << "NOT BALANCED BRACKETS\n";
                exit(1);
            }
        else if (string_type(filter[i])==2)
            operation_order.push({operation_priority_lookup[filter[i][0]]+prior_stack,i});

        else if (string_type(filter[i])==1 && name_set_lookup.find(filter[i]) == name_set_lookup.end()) {
            set<string> new_set;
            cout << "size_"<<filter[i]<<": ";
            size_t size_set;
            cin >> size_set;
            cout << "set_"<<filter[i]<<": ";
            set_io::insert(new_set,size_set);
            name_set_lookup[filter[i]]=new_set;
        }
    }
    vector<pair<size_t,size_t>> index_deletions;
    while (!operation_order.empty())
    {
//        for (auto x : filter)
//            cout <<x <<' ';
//        cout <<'\n';
        size_t original_ind = operation_order.top().second;
        size_t ind=original_ind;
        for (auto x : index_deletions)
        {
            if(original_ind>x.first)
                ind-=x.second;
        }
        operation_order.pop();
        string arg_left  = filter[ind-1]; 
        string arg_right = filter[ind+1]; 
        bool brackets = (filter[ind-2]=="(" && filter[ind+2]==")");
        filter[ind-1-brackets] = (brackets?"(":"")+arg_left+filter[ind]+arg_right+(brackets?")":"");
        char operation = filter[ind][0];
//        cout <<"[ "<<arg_left<<" ]=";set_io::print(name_set_lookup[arg_left]);cout<<'\n'<<operation;cout <<"\n[ "<<arg_right<<" ]=";set_io::print(name_set_lookup[arg_right]);cout << '\n';
        filter.erase(filter.begin()+ind-brackets,filter.begin()+ind+2+brackets);
        index_deletions.push_back({original_ind,2+brackets*2});
        switch (operation)
        {
            case 'u':
                name_set_lookup[filter[ind-1-brackets]] = set_math::_union(name_set_lookup[arg_left],name_set_lookup[arg_right]);
            break;
            case 'n':
                name_set_lookup[filter[ind-1-brackets]] = set_math::intersection(name_set_lookup[arg_left],name_set_lookup[arg_right]);
            break;
            case '/':
                name_set_lookup[filter[ind-1-brackets]] = set_math::difference(name_set_lookup[arg_left],name_set_lookup[arg_right]);
            break;
            case 'o':
                name_set_lookup[filter[ind-1-brackets]] = set_math::sym_difference(name_set_lookup[arg_left],name_set_lookup[arg_right]);
            break;
        }
    }
    cout << filter[0]<<" = {";
    set_io::print(name_set_lookup[filter[0]]," ");
    cout <<"}\n";
}

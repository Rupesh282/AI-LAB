#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<bitset>
#include<utility>
#include<stack>
using namespace std;

int n, m, no_of_states_explored = 0, length_of_path = 0;          // For Results 
const int N = 1e3+10;                                             // Large Number for Vector Size
vector<string>a;                                                  // To store our Input
vector<vector<int>> v(N, vector<int>(N, 0));
vector<vector<pair<int, int>>> par(N, vector<pair<int, int>>(N, {-1, -1}));
int t;                                                            

const int dx[] = {-1, 0, 1, 0}, dy[] = {0, -1, 0, 1};              // Preference Order for adding nodes 
          
bool validState(int i, int j) {                                    // Check if the State is valid or not 
	return i>=0 && i<n && j>=0 && j<m && v[i][j] == 0 && a[i][j] != '|' && a[i][j] != '+' && a[i][j] != '-';     
} 

bool GoalTest(int i, int j) {                                     // Check is this State is a Goal State 
    return a[i][j] == '*';
}

vector<pair<int, int>> MoveGen(int i, int j) {                   // To generate the neighbours of this State
    vector<pair<int, int>> res;
    for(int k=0;k<4;++k) {
        int x = i + dx[k], y = j + dy[k];
        if(validState(x, y))
            res.push_back({x, y});
    }
    return res;
}

void markPath(pair<int, int> end) {                              // To put '0' in our followed path
    length_of_path = 1;
    while(end.first != 0 || end.second != 0) {
        ++ length_of_path;
        a[end.first][end.second] = '0';
        end = par[end.first][end.second];
    }
    a[end.first][end.second] = '0';
    return;
}

void printAns() {                                                  // To print out the solution 
    cout << no_of_states_explored << "\n" << length_of_path << "\n";
    for(string& s : a)
        cout << s << "\n";
    return;
}

bool bfs(pair<int, int> start = {0, 0}) {                        // Function for Breadth First Search
    queue<pair<int, int>> st;
    st.push(start);
    v[start.first][start.second] = 1;
    while(st.size()) {                                          
        pair<int, int> t = st.front();
        st.pop();
        no_of_states_explored ++;
        if(GoalTest(t.first, t.second)) {                         // If Goal State is reached , mark path then print answer and return 
            markPath(t);                          
            printAns();
            return 1;
        }
        for(pair<int, int> node : MoveGen(t.first, t.second))        
            st.push(node), v[node.first][node.second]=1, par[node.first][node.second] = t;
    }
    return 0;
}

bool dfs(pair<int, int> start = {0, 0}) {                         // Function for Depth First Search                     
    stack<pair<int, int>> st;
    st.push(start);
    v[start.first][start.second] = 1;
    while(st.size()) {
        pair<int, int> t = st.top();
        st.pop();
        ++ no_of_states_explored;
        if(GoalTest(t.first, t.second)) {                     // If Goal State is reached , mark path then print answer and return
            markPath(t);
            printAns();
            return 1;
        }

        vector<pair<int, int>> sideNodes = MoveGen(t.first, t.second); 
        reverse(sideNodes.begin(), sideNodes.end());

        for(pair<int, int> node : sideNodes) 
            st.push(node), v[node.first][node.second] = 1, par[node.first][node.second] = t;
    }
    return 0;
}

vector<vector<int>> Dep(N, vector<int>(N, 1e9));
int maxDepth = -1;

bool dfsn(pair<int, int> start, int depth) {             // Recursive Depth First Search for DFID

    if(depth > maxDepth)
        return 0;
    
    Dep[start.first][start.second] = depth;

    if(GoalTest(start.first, start.second)) {                     // If Goal State is reached , mark path then print answer and return
        markPath(start);
        printAns();
        return 1;
    }
    no_of_states_explored ++;

    for(pair<int, int> node : MoveGen(start.first, start.second)) {
        if(depth + 1 < Dep[node.first][node.second]) {              // IF the depth is lower than earlier value , we continue again
            par[node.first][node.second] = start;
            if(dfsn(node, depth + 1))
                return 1;
        }
    }
   
    return 0;
}


void dfid() {                                          // Function for Depth First Iterative Deepening 
    for(maxDepth=0; maxDepth<=n*m; ++maxDepth) { 
        if(dfsn({0, 0}, 0)) 
            break;
        Dep.assign(n, vector<int>(m, 1e9));
    }
}



int main() {
    int T;                                        // Variable to store user choice 
	string s;
    getline(cin, s);
    T = stoi(s);
    while(getline(cin, s))                                  //  Taking input into our data structure
        if((int)s.size() == 0)
            break;
        else 
            a.push_back(s);
    n = (int)a.size();
    m = (int)a[0].size();

    if(T == 0) {                   // Else If ladder that calls the appropriate function according to input 0,1,2
        bfs();
    } else if(T == 1) {
        dfs();
    } else {
        dfid();
    }
	return 0;
}


#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<bitset>
#include<utility>
#include<stack>
using namespace std;

int n, m, no_of_states_explored = 0, length_of_path = 0;
const int N = 1e3+10;
vector<string>a;
vector<vector<int>> v(N, vector<int>(N, 0));
vector<vector<pair<int, int>>> par(N, vector<pair<int, int>>(N, {-1, -1}));
int t;

// const int dx[] = {0, 1, -1, 0}, dy[] = {1, 0, 0, -1}; //right first
//const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1}; //down first
//const int dx[] = {0, 0, 1, -1}, dy[] = {1, -1, 0, 0}; //down first
// DOWN > UP > RIGHT > LEFT
//int rot_x[] = {0, 0, 1, -1};
//int rot_y[] = {1, -1, 0, 0};



//TODO:
//  clean code
//  do dfid 

const int dx[] = {1, -1, 0, 0}, dy[] = {0, 0, 1, -1};

bool validState(int i, int j) {
	return i>=0 && i<n && j>=0 && j<m && v[i][j] == 0 && a[i][j] != '|' && a[i][j] != '+' && a[i][j] != '-';
}

bool GoalTest(int i, int j) {
    return a[i][j] == '*';
}

vector<pair<int, int>> MoveGen(int i, int j) {
    vector<pair<int, int>> res;
    for(int k=0;k<4;++k) {
        int x = i + dx[k], y = j + dy[k];
        if(validState(x, y))
            res.push_back({x, y});
    }
    return res;
}

void markPath(pair<int, int> end) {
    length_of_path = 1;
    while(end.first != 0 || end.second != 0) {
        ++ length_of_path;
        a[end.first][end.second] = '0';
        end = par[end.first][end.second];
    }
    a[end.first][end.second] = '0';
    return;
}

void printAns() {
    cout << no_of_states_explored << "\n" << length_of_path << "\n";
    for(string& s : a)
        cout << s << "\n";
    return;
}

bool bfs(pair<int, int> start = {0, 0}) {
    queue<pair<int, int>> st;
    st.push(start);
    v[start.first][start.second] = 1;
    while(st.size()) {
        pair<int, int> t = st.front();
        st.pop();
        no_of_states_explored ++;
        if(GoalTest(t.first, t.second)) {
            markPath(t);
            printAns();
            return 1;
        }
        for(pair<int, int> node : MoveGen(t.first, t.second))
            st.push(node), v[node.first][node.second]=1, par[node.first][node.second] = t;
    }
    return 0;
}

bool dfs(pair<int, int> start = {0, 0}) {
    stack<pair<int, int>> st;
    st.push(start);
    v[start.first][start.second] = 1;
    while(st.size()) {
        pair<int, int> t = st.top();
        st.pop();
        ++ no_of_states_explored;
        if(GoalTest(t.first, t.second)) {
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



int main() {
    int T; cin >> T;
	getchar();
	string s;
    while(getline(cin, s))
        if((int)s.size() == 0)
            break;
        else
            a.push_back(s);
    n = (int)a.size();
    m = (int)a[0].size();

    //cerr << T << " " << n << " " << m << "\n";
    //for(string& i: a)
        //cerr << i << "\n";

    if(T == 0) {
        bfs();
    } else if(T == 1) {
        dfs();
    } else {
        ;
    }
	return 0;
}



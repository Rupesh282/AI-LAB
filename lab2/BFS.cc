#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;
const int N = 3; // no of stacks
int no_of_states_explored = 0; // total states explored until we find a solution

/*
 * Generates random states for start and goal
 */
vector<vector<int>> GenerateRandom() {
    vector<vector<int>> t(3) ;
    vector<int> Blocks{ 1, 2, 3, 4, 5 ,6};

    std::random_device rd;
    std::mt19937 g(rd());
    shuffle(Blocks.begin(), Blocks.end() , g) ; 

    int column , element ;

    for(int i=0 ; i<(int)Blocks.size() ; i++ )
    {   
        column = rand()%3 ;
        element = Blocks[i] ;
        t[column].push_back(element) ;
    }
 
    return t;
}

/*
* State class which contains 2D
* vector to represent our 
* current state
*/
class State {
    public:
        vector<vector<int>> st;

        State() {
            ;
        }

        State(int n) {
            st.resize(n, vector<int>());
        }

        State(vector<vector<int>> a) {
            st = a;
        }

        State(vector<vector<int>>& a) {
            st = a;
        }

        State(State& a) {
            st = a.st;
        }

        State(const State& a) {
            st = a.st;
        }

        bool operator == (State& a) {
            for(int i=0;i<(int)st.size();++i) {
                if(st[i].size() != a.st[i].size())
                    return 0;
                bool fl = 1;
                for(int j=0;j<(int)st[i].size();++j)
                    fl &= (st[i][j] == a.st[i][j]);
                if(!fl)
                    return 0;
            }
            return 1;
        }

        /*
         *Returns a new state after transfering top element "from" stack
         *to "to" stack
         */
        State getState(int from, int to) {
            State T(*this);
            if(T.st[from].size()) {
                T.st[to].push_back(T.st[from].back());
                T.st[from].pop_back();
            }  
            return T;
        }

        /*
        *prints current state
        */
        void print() {
            for(vector<int>& i: st) {
                cout << "[ ";
                for(int& j: i)
                    cout << j << " ";
                cout << "]\n";
            }
            cout << "\n";
        }
};

/*
 *vector for recording visited states
 */
vector<State> v;

State start(N);
State Goal(N);


/*
 *return if a state is visited or not
 */
bool isVisited(State& a) {
    for(State& i: v)
        if(i == a)
            return 1;
    return 0;
}

/*
 *returns 1 if state is goal state else returns 0
 */
bool GoalTest(State& a) {
    return a == Goal;
}

/*
 *returns valid states possible from current states
 */
vector<State> MoveGen(State& cur) {
    vector<State> res;
    for(int i=0;i<N;++i) {
        for(int j=0;j<N;++j) {
            if(i == j) 
                continue;
            State nextState = cur.getState(i, j);
            if(!isVisited(nextState)) {
                v.push_back(nextState);
                res.push_back(nextState);
            }
        }
    }
    return res;
}


/*
 *Comparator function used to sort the priority queue 
 *elements according to huristic value
 */
bool operator < (const State& a, const State& b) {
    // all huristics are mentioned here
    
    auto baseCorrect = [&](State a) -> int {
        int ca=0;
        for(int i=0;i<N;++i) {
            for(int j=0;j<(int)min(a.st[i].size(), Goal.st[i].size());++j)
                if(a.st[i][j] == Goal.st[i][j])
                    ca ++;
                else
                    break;
        }
        return ca;
    };

    auto totalCorrect = [&](State a) -> int {
        int ca = 0;
        for(int i=0;i<N;++i) 
            for(int j=0;j<(int)min(a.st[i].size(), Goal.st[i].size());++j)
                if(a.st[i][j] == Goal.st[i][j])
                    ca ++;
        return ca;
    };

    return baseCorrect(a) < baseCorrect(b);
}


/*
 * standard bfs
 */
bool bfs(State start) {
    priority_queue<State, vector<State>> P;
    P.push(start);
    v.push_back(start);
    while(P.size()) {
        State cur = P.top();
        P.pop();

        cur.print();
        no_of_states_explored ++;

        if(GoalTest(cur)) {
            cur.print();
            cout << "Goal state reached in " << no_of_states_explored << " moves\n";
            return 1;
        }

        for(State& nextState: MoveGen(cur))
            P.push(nextState);
    }
    cout << "Solution not found !\n";
    return 0;
}

int main() {
    srand ( unsigned ( std::time(0) ) );

    /*
     * start and goal state generation using GenerateRandom() function
     */
    State T1(GenerateRandom());
    start = T1;

    State T2(GenerateRandom());
    Goal = T2;


    //start.st[0] = {9, 5, 7};
    //start.st[1] = {6, 2, 8, 3, 1};
    //start.st[2] = {4};

    //Goal.st[0] = {1, 8};
    //Goal.st[1] = {2, 7, 5, 3, 6};
    //Goal.st[2] = {9, 4};

    cout << "start state is : \n";
    start.print();

    cout << "goal state is : \n";
    Goal.print();


    cout << "BFS:\n";
    cout << bfs(start) << "\n";

    return 0;
}

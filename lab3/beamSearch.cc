#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>


int no_of_clauses, no_of_literals, beam_width = 9;
int no_of_states_explored = 0;

std::vector<std::string> CNF;
std::map<int, bool> visited;


int H(int& a) {
    int val = 0;
    for(std::string& clause: CNF) {
        bool fl = 0;
        for(char literal: clause) {
            if(literal >= 'A' && literal <= 'Z')
                fl |= (1 ^ ((a & (1 << (int)((literal | ' ') - 'a')))>0));
            else
                fl |= ((a & (1 << (literal - 'a'))) > 0);
        }
        val += fl;
    }
    return val;
}



class State {
    public:
        int mask, Hvalue;

        State(int m=0) {
            mask = m;
            Hvalue = H(mask);
        }

        void toggle(int i) {
            mask ^= (1 << i);
            Hvalue = H(mask);
        }

        bool ithBit(int i) {
            return mask & (1 << i);
        }

        void print() {
            std::cout << "Hvalue: " << Hvalue << "\n";
            std::cout << "literals: ";
            for(int i=0; i<no_of_literals; ++i)
                std::cout << ((mask & (1 << i))>0) << " ";
            std::cout << "\n";
        }

};

bool operator < (const State& a, const State& b) {
    return a.Hvalue < b.Hvalue;
}



std::vector<State> MoveGen(State& cur) {
    std::vector<State> res;
    for(int i=0; i<no_of_literals; ++i) {
        State nextState = cur;
        nextState.toggle(i);
        if(!visited[nextState.mask]) {
            visited[nextState.mask] = 1;
            res.push_back(nextState);
        }
    }
    std::sort(res.begin(), res.end());
    reverse(res.begin(), res.end());
    while((int)res.size() > beam_width) {
        res.pop_back();
    }
    return res;
}

bool GoalTest(State& a) {
    return a.Hvalue == no_of_clauses;
}

bool bestFirstSearch(State start) {
    std::priority_queue<State, std::vector<State>> P;
    P.push(start);
    visited[start.mask] = 1;
    while(P.size()) {
        State cur = P.top();
        P.pop();

        cur.print();
        no_of_states_explored ++;

        if(GoalTest(cur)) {
            std::cout << "\nGoal State : \n";
            cur.print();
            return 1;
        }

        for(State& nextState: MoveGen(cur))
            P.push(nextState);
    }
    return 0;
}

int main() {

    std::cin >> no_of_literals >> no_of_clauses;


    for(int i=0; i<no_of_clauses; ++i) {
        std::string s;
        std::cin >> s;
        CNF.push_back(s);
    }

    std::cout << "Starting Beam search ... \n";

    State a(0);
    bool found = bestFirstSearch(a);
    if(found) {
        std::cout << "Answer found !\n";
    } else {
        std::cout << "Answer not found !\n";
    }
    std::cout << "No. of states explored : " << no_of_states_explored << "\n";
    return 0;
}

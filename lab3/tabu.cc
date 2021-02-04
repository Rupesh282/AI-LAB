#include <iostream>
#include <vector>
#include <string>

#define TABU_TENURE 2

int no_of_literals, no_of_clauses, TIME;
std::vector<std::string> CNF;

class State {
    public:
        int mask, Hvalue;
        std::vector<int> tabu_tenure;

        State(int m=0) {
            mask = m;
            tabu_tenure.assign(30, 0);
            Hvalue = 0;
        }

        void toggle(int i) {
            mask ^= (1 << i);
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

int H(State& a) {
    int val = 0;
    for(std::string& clause: CNF) {
        bool fl = 0;
        for(char literal: clause) {
            if(literal >= 'A' && literal <= 'Z')
                fl |= (1^a.ithBit((char)(literal | ' ') - 'a'));
            else
                fl |= a.ithBit(literal - 'a');
        }
        val += fl;
    }
    return val;
}

std::vector<State> moveGen(State& a) {
    std::vector<State> neigbours;

    for(int i=0; i<no_of_literals; ++i) {
        if(a.tabu_tenure[i] > 0)
            -- a.tabu_tenure[i];
    }

    for(int i=0; i<no_of_literals; ++i) {
        if(a.tabu_tenure[i] == 0) {
            State newState = a;
            newState.tabu_tenure[i] = TABU_TENURE;
            newState.toggle(i);
            newState.Hvalue = H(newState);
            neigbours.push_back(newState);
        }    
    }
    return neigbours;
}

bool goalState(State& a) {
    return a.Hvalue == no_of_clauses;
}

bool tabuSearch(State start) {
    State globalBest = start, curBest = start;
    while(!goalState(globalBest) && TIME--) {
        globalBest.print();
        std::vector<State> nbs = moveGen(curBest);
        for(State& state: nbs) {
            if(state.Hvalue >= curBest.Hvalue)
                curBest = state;
        }
        if(globalBest.Hvalue <= curBest.Hvalue)
            globalBest = curBest;
    }
    globalBest.print();
    if(goalState(globalBest))
        std::cout << "Goal state reached\n";
    return 1;
}

int main() {

    std::cin >> no_of_literals >> no_of_clauses;

    TIME = (1 << no_of_literals);

    for(int i=0; i<no_of_clauses; ++i) {
        std::string s;
        std::cin >> s;
        CNF.push_back(s);
    }

    State a(0);
    a.Hvalue = H(a);
    std::cout << tabuSearch(a) << "\n";
}

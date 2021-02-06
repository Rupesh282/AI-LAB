#include <iostream>
#include <vector>
#include <string>

#define TABU_TENURE 2

int no_of_literals, no_of_clauses, TIME, no_of_steps = 0;
std::vector<std::string> CNF;

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
        std::vector<int> tabu_tenure;

        State(int m=0) {
            mask = m;
            tabu_tenure.assign(30, 0);
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
            //for(int i=0; i<no_of_literals; ++i)
                //std::cout << tabu_tenure[i] << " ";
            //std::cout << "\n";
            std::cout << "literals: ";
            for(int i=0; i<no_of_literals; ++i)
                std::cout << ((mask & (1 << i))>0) << " ";
            std::cout << "\n";
        }

};


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
    while(TIME--) {
        globalBest.print();

        no_of_steps ++;

        if(goalState(globalBest))
            break;

        std::vector<State> nbs = moveGen(curBest);
        curBest.Hvalue = 0;

        for(State& state: nbs) {
            if(state.Hvalue >= curBest.Hvalue)
                curBest = state;
        }

        if(globalBest.Hvalue <= curBest.Hvalue)
            globalBest = curBest;
    }
    std::cout << "\nBest solution found from tabu search: \n";
    globalBest.print();
    return goalState(globalBest);
}

int main() {

    std::cin >> no_of_literals >> no_of_clauses;

    TIME = (1 << no_of_literals);

    for(int i=0; i<no_of_clauses; ++i) {
        std::string s;
        std::cin >> s;
        CNF.push_back(s);
    }

    std::cout << "Starting Tabu search ... \n";


    /* random start state generation */
    //srand(time(NULL));
    //int rand = random() % (1 << no_of_literals);

    State a(0);

    bool found = tabuSearch(a);

    if(found) {
        std::cout << "Answer found !\n";
    } else {
        std::cout << "Answer not found !\n";
    }
    std::cout << "No. of steps : " << no_of_steps << "\n";

    return 0;
}

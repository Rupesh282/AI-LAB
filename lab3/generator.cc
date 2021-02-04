#include "bits/stdc++.h"
#define LIT_COUNT_IN_CLAUSE 3

int main(int argc, char** argv) {
    srand(time(NULL));
    assert(argc == 3);
    int no_of_literals = std::stoi(argv[1]); // no. of literals 
    int no_of_clauses = std::stoi(argv[2]);  // no of clauses
    assert(no_of_literals <= 26 && no_of_clauses > 0);


    std::cout << no_of_literals << " " << no_of_clauses << "\n";


    std::vector<char> LIT;
    for(int i=0;i<no_of_literals;++i)
        LIT.push_back((char)(i+'a'));
    for(int i=0;i<no_of_literals;++i)
        LIT.push_back((char)((int)LIT[i]&'_'));

    std::cerr << "'a' : simple literal and 'A' is negation of 'a'\n";
    std::cerr << "taken literals : ";
    for(char& i : LIT)
        std::cerr << i << " ";
    std::cerr << "\n";

    std::map<std::string, bool> taken_clause;
    for(int c=0; c<no_of_clauses; ) {
        std::map<char, bool> taken_literal;
        std::string cur_literal = "";
        for(int l=0; l<LIT_COUNT_IN_CLAUSE; ) {
            int random_lit = rand() % (no_of_literals << 1);
            if(taken_literal.find(LIT[random_lit]) != taken_literal.end())
                continue;
            else {
                taken_literal[LIT[random_lit]] = 1;
                cur_literal += LIT[random_lit];
                if(LIT[random_lit] >= 'a')
                    taken_literal[LIT[random_lit]&'_'] = 1;
                else
                    taken_literal[LIT[random_lit]|' '] = 1;
                l++;
            }
        }
        if(taken_clause.find(cur_literal) != taken_clause.end())
            continue;
        else {
            std::cout << cur_literal << "\n";
            ++ c;
        }

    }
    return 0;
}



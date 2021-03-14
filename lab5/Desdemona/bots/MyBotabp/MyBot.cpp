/*
* @file botTemplate.cpp
* @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
* @date 2010-02-04
* Template for users to create their own bots
*/

#include "Othello.h"
#include "OthelloBoard.h"
#include "OthelloPlayer.h"
#include <cstdlib>
using namespace std;
using namespace Desdemona;

class MyBot: public OthelloPlayer
{
    public:
        /**
         * Initialisation routines here
         * This could do anything from open up a cache of "best moves" to
         * spawning a background processing thread. 
         */
        MyBot( Turn turn );

        /**
         * Play something 
         */
        virtual Move play( const OthelloBoard& board );
    private:
};

MyBot::MyBot( Turn turn )
    : OthelloPlayer( turn )
{
}

const int D = 10;

int H(OthelloBoard board) {
    int corners = 0;
    corners += (board.get(0, 0) == BLACK) * 5;
    corners += (board.get(7, 0) == BLACK) * 5;
    corners += (board.get(0, 7) == BLACK) * 5;
    corners += (board.get(7, 7) == BLACK) * 5;
    return board.getBlackCount() - board.getRedCount() + corners;
}

Turn flip(Turn turn) {
    return (turn == BLACK ? RED : BLACK);
}

class MMnode {
    public:
        OthelloBoard board;
        int Hvalue;
        Turn turn;

        MMnode(OthelloBoard board, Turn turn) {
            this->board = board;
            this->turn = turn;
            Hvalue = H(board);
        }

        void updateH() {
            this->Hvalue = H(board);
        }
};

class returnNode {
    public: 
        int Hvalue;
        bool type;
        Move* move;

        returnNode(int Hvalue, bool type, Move move) {
            this->Hvalue = Hvalue;
            this->type = type;
            this->move = new Move(move.x, move.y);

        }

        returnNode() {
            ;
        }
};

void printMove(Move move) {
    cout << "[ x, y ] : " << move.x << ", " << move.y << endl;
}



//type = 0 : alpa / max
//type = 1 : beta / min

returnNode dfs(MMnode& parNode, bool type, int depth, Move move) {
    bool isInit = (move.x == -1 && move.y == -1);

    MMnode curNode(parNode.board, parNode.turn);
    if(!isInit) {
        curNode.board.makeMove(curNode.turn, move);
        curNode.turn = flip(parNode.turn);
        curNode.updateH();
    }

    if(depth == D) {
        return returnNode(curNode.Hvalue, type, move);    
    }

    list<Move> moves = curNode.board.getValidMoves(curNode.turn);
    int tempHvalue = (type == 0 ? -1e9 : 1e9);
    returnNode curReturn(tempHvalue, type, Move(-1, -1));

    for(auto& mv : moves) {
        returnNode res = dfs(curNode, type^isInit^1, depth+(isInit^1), mv);
        if(type == 0) {
            //appha / max
            if(res.Hvalue > curReturn.Hvalue)
                curReturn.Hvalue = res.Hvalue, curReturn.move = new Move(mv);
            else
                break;
        } else {
            //beta / min
            if(res.Hvalue < curReturn.Hvalue) 
                curReturn.Hvalue = res.Hvalue, curReturn.move = new Move(mv);
            else
                break;
        }
    }
    if(move.x == -1 && move.y == -1) {
        if(moves.size() > 0)
            curReturn.move = &(*moves.begin());
    }
    return curReturn;
}


Move MyBot::play( const OthelloBoard& board )
{
    board.print( turn );
    cout << "WITH alphabita : " << turn << "\n";

    MMnode parNode(board, turn);
    bool type = 0; // start as alpha 
    int depth = 0;
    Move move(-1, -1);

    returnNode res = dfs(parNode, type, depth, move);


    cout << "move played by black : " << res.move->x << ", " << res.move->y << endl;
    return *res.move;
}

// The following lines are _very_ important to create a bot module for Desdemona

extern "C" {
    OthelloPlayer* createBot( Turn turn )
    {
        return new MyBot( turn );
    }

    void destroyBot( OthelloPlayer* bot )
    {
        delete bot;
    }
}



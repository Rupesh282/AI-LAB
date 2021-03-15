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
#include <functional>
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

const int D = 4;
Turn TURN;
#define TREE 0 // make it 1 if want to print tree (use piping for printing tree)
#define CurH 0


Turn flip(Turn turn) {
    return (turn == BLACK ? RED : BLACK);
}

/*
 *All heuristics
 */
int H(OthelloBoard board, int which) {
    function<int(OthelloBoard board)> F[3];

    F[0] = [&](OthelloBoard board)->int {
        int corners = 0;
        corners += (board.get(0, 0) == TURN) * 5;
        corners += (board.get(7, 0) == TURN) * 5;
        corners += (board.get(0, 7) == TURN) * 5;
        corners += (board.get(7, 7) == TURN) * 5;
        return board.getBlackCount() - board.getRedCount() + corners;
    };

    F[1] = [&](OthelloBoard board)->int {
        return (int)board.getValidMoves(TURN).size() - (int)board.getValidMoves(flip(BLACK)).size();
    };

    F[2] = [&](OthelloBoard board)->int {
        return board.getBlackCount() - board.getRedCount(); 
    };

    return F[which](board);
}


/*
 *class for minimax tree node
 */
class MMnode {
    public:
        OthelloBoard board;
        int Hvalue;
        Turn turn;

        MMnode(OthelloBoard board, Turn turn) {
            this->board = board;
            this->turn = turn;
            Hvalue = H(board, CurH);
        }

        void updateH() {
            this->Hvalue = H(board, CurH);
        }
};

/*
 *class for returnvalue of dfs function for minimax
 */
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

/*
 *minimax algo implementation
 */
returnNode dfs(MMnode parNode, bool type, int depth, Move move) {
    bool isInit = (move.x == -2 && move.y == -2);

    MMnode curNode(parNode.board, parNode.turn);
    if(!isInit) {

        if(TREE) {
            cerr << "cur move : " << "\n";
            cerr << "[ x, y ] : " << move.x << ", " << move.y << endl;
            cerr << "cur depth : " << depth << "\n";
            cerr << "childrens : " << "\n";
        }

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

        if(TREE) {
            cerr << "[ x, y ] : " << move.x << ", " << move.y << endl;
        }

        returnNode res = dfs(curNode, type^isInit^1, depth+1, mv);
        if(type == 0) {
            //appha / max
            if(res.Hvalue > curReturn.Hvalue)
                curReturn.Hvalue = res.Hvalue, curReturn.move = new Move(mv);
            //else
                //break;
        } else {
            //beta / min
            if(res.Hvalue < curReturn.Hvalue) 
                curReturn.Hvalue = res.Hvalue, curReturn.move = new Move(mv);
            //else
                //break;
        }
    }
    if(moves.size() == 0) {
        curReturn.Hvalue = H(curNode.board, CurH);
    }
    return curReturn;
}


Move MyBot::play( const OthelloBoard& board )
{
    TURN = turn;
    // board.print( turn );

    MMnode parNode(board, turn);
    bool type = 0; // start as alpha 
    int depth = 0;
    Move move(-2, -2);

    returnNode res = dfs(parNode, type, depth, move);

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



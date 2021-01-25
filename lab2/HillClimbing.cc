#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std ;

// Generate Random Start and Goal State using shuffle() and rand()
vector<vector<int>> GenerateRandom()
{
    vector<vector<int>> t(3) ;
    vector<int> Blocks{ 1, 2, 3, 4, 5 ,6};

    std::random_device rd;
    std::mt19937 g(rd());
    shuffle(Blocks.begin(), Blocks.end() , g) ; 

    int column , element ;

    for(int i=0 ; i<Blocks.size() ; i++ )
    {   
        column = rand()%3 ;
        element = Blocks[i] ;
        t[column].push_back(element) ;
    }
 
    return t ;
}
// To print the current status of the table ( showing all the blocks )
void PrintTable(vector<vector<int>> t)
{   
    cout << "\n" ;

    for(int i=0 ; i<t.size() ; ++i)
    {   
        cout<<"[ " ;

        for(int j=0; j<t[i].size() ; ++j)
        {
            cout<<t[i][j]<<" " ;
        }

        cout<<"]\n" ;
    }
}

// Basic Heuristic which simply matches positions of Blocks in Current and Goal State 
/*
int Heuristic1(vector<vector<int>> current, vector<vector<int>> goal)
{   
    int score=0 ;
    
    for(int i=0 ; i<3 ; ++i)
    {
        for(int j=0 ; j<current[i].size()  && j<goal[i].size() ; ++j )
        {
            if(current[i][j]==goal[i][j])
            {
                score++ ;
            }
        }
    }

    return score ;
} */

// using Manhattan Distance, closer the blocks are relative to position in goal state , higher is its heuristic value 

/*
int Heuristic1(vector<vector<int>> current, vector<vector<int>> goal)
{   
    int score=20 ;
    
    for(int x=0 ; x<goal.size() ; ++x)
    {
    for(int y=0 ; y<goal[x].size() ; ++y)
    {

    for(int i=0 ; i<current.size() ; ++i)
    {
        for(int j=0 ; j<current[i].size(); ++j )
        {
            if(current[i][j]==goal[x][y])
            {
                score = score - abs(i-x) - abs(j-y) ;
            }
        }
    }

    }
    }

    return score ;
}
*/

int Heuristic1(vector<vector<int>> current, vector<vector<int>> goal)
{   // Score based System which takes in account multiple conditions of "similarity"
    int score=0 ;

    for(int i=0; i<current.size() ; ++i)
    {
        for(int j=0; j<goal.size(); ++j)
        {
            if(current[i]==goal[j])
            {
                score+=10;
            }
        }
    }
    
    for(int i=0 ; i<3 ; ++i)
    {   
        int c_sum=0 , g_sum=0 ;

        for(int j=0 ; j<current[i].size()  && j<goal[i].size() ; ++j )
        {   
            c_sum += current[i][j] ;
            g_sum += goal[i][j] ;

            if(current[i][j]==goal[i][j])
            {
                score+=20 ;
            }
        }

        if(c_sum==g_sum)
        {
            score+=10 ;
        }
    }

    return score ;
}
// some hardcoded Random Moves to try and get out of Local Maxima

vector<vector<int>> RandomMove(vector<vector<int>> c)
{    
    vector<vector<int>> c1=c,c3=c,c5=c ;

    if(c[0].empty()==0)
    {  
        c1[1].push_back(c1[0].back());
        c1[0].pop_back();
        return c1 ;
    }
    else if(c[1].empty()==0)
    {
        c3[0].push_back(c3[1].back());
        c3[1].pop_back();
        return c3 ;
    }
    else if(c[2].empty()==0)
    {
        c5[0].push_back(c5[2].back());
        c5[2].pop_back();
        return c5 ;
    }
    
    return c1;
}

// MoveGen to generate next possible states , and return the best one according to Heuristic
vector<vector<int>> MoveGen(vector<vector<int>> c , vector<vector<int>> goal)
{
    vector<vector<int>> c1=c,c2=c,c3=c,c4=c,c5=c,c6=c ;
    int scores[] = {-1,-1,-1,-1,-1,-1} ;

    if(c[0].empty() == 0)
    {
        c1[1].push_back(c1[0].back());
        c1[0].pop_back();

        c2[2].push_back(c2[0].back());
        c2[0].pop_back();

        scores[0] = Heuristic1(c1,goal) ;
        scores[1] = Heuristic1(c2,goal) ;
    }

    if(c[1].empty() == 0)
    {
        c3[0].push_back(c3[1].back());
        c3[1].pop_back();

        c4[2].push_back(c4[1].back());
        c4[1].pop_back();

        scores[2] = Heuristic1(c3,goal) ;
        scores[3] = Heuristic1(c4,goal) ;
    }

    if(c[2].empty() == 0)
    {
        c5[0].push_back(c5[2].back());
        c5[2].pop_back();

        c6[1].push_back(c6[2].back());
        c6[2].pop_back();

        scores[4] = Heuristic1(c5,goal) ;
        scores[5] = Heuristic1(c6,goal) ;
    }

    int max_score = -1 ;
    int max_score_index = 0 ;

    for(int i=0 ; i<6 ; ++i)        // Find the best move 
    {   
        if(scores[i]>=max_score)
        {
            max_score = scores[i] ;
            max_score_index = i ;
        }
    }

    switch(max_score_index)          // Perform the best move 
    {
        case 0: return c1;
        case 1: return c2;
        case 2: return c3;
        case 3: return c4;
        case 4: return c5;
        case 5: return c6;
    }
    
    return c1;
}

// Check if the current state is the goal State
bool GoalTest(vector<vector<int>> current, vector<vector<int>> goal)
{
    if(current==goal) return 1 ;
    else return 0 ;
}



void HillClimb(vector<vector<int>> current, vector<vector<int>> goal)
{  
     vector<vector<int>> last1(3);
     vector<vector<int>> last2(3);

     int moves=0 ; int Random_count=0;

    while(GoalTest(current,goal)==0)
    {
        
        current = MoveGen(current,goal) ;
        moves++ ;
        
        if(last1==current || last2==current )
        {
            cout<<"\nSearch is stuck in Local Maxima , Attempting 5 Random Moves to escape it\n" ; 

            for(int i=0 ; i<5 ; ++i)
            {
                current = RandomMove(current);
            }

            moves+=5 ;
            Random_count++ ;
        }

        if(Random_count>=5)
        {
            cout<<"\nStill stuck in Maxima\n" ;
            exit(0);
           /* cout<<" Attempting Random Restart\n" ;                     // For Random restart , not applicable here
             Random_count = 0;                                           // but you can verify that it can solve the problem 
            current = GenerateRandom() ;
           */ 
        }

        PrintTable(current);
        cout<<"Moves : "<<moves <<endl;

        last2 = last1 ;
        last1 = current ;
    }

    cout<<"\nTotal "<<moves<<" Moves\n";

}

int main()
{
    // Generate 2 Tables , initial and goal state
    // Hill Climb()
    srand ( unsigned ( std::time(0) ) );
    
    vector<vector<int>> Table(3) ;
    vector<vector<int>> GoalTable(3) ;

    Table = GenerateRandom() ;
    GoalTable = GenerateRandom() ;

    cout<<"Randomly selected Initial State is :" ; 
    PrintTable(Table) ;

    cout<<"\nRandomly selected Goal State is :" ; 
    PrintTable(GoalTable) ;

    cout<<"\nHill Climbing\n";
    HillClimb(Table,GoalTable) ;

    cout<<"\nSolved\n";

    return 0;
}



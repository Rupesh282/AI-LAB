#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <string>

using namespace std ;

vector<string> SAT {"ACD", "dac", "Bac" , "aDB" ,"AbD"};
vector<int> V {0,0,0,0} ;

int number_of_moves = 0;
int current_neighbourhood = 0;


// To print the current status of the table ( showing all the blocks )
void PrintV()
{   
    for(int i=0 ; i<V.size() ; ++i)
    {  cout<<V[i]<<"  ";  }
    cout << "\n" ;
}

int Value(char c )
{
    switch(c)
    {
        case 'a' : return V[0]; break ;
        case 'b' : return V[1]; break ;
        case 'c' : return V[2]; break ;
        case 'd' : return V[3]; break ;

        case 'A' : return ((V[0]==0)?1:0) ; break ;
        case 'B' : return ((V[1]==0)?1:0) ; break ;
        case 'C' : return ((V[2]==0)?1:0) ; break ;
        case 'D' : return ((V[3]==0)?1:0) ; break ;
    }

    return -1 ;
}

int Evaluate(vector<int> T)
{
    int Score = 0 ;
    for(int i=0 ; i<SAT.size() ; ++i)
    {
        int temp = 1 ;
        for(int j=0 ; j<3 ; ++j)
        {
            temp = temp*Value(SAT[i][j]) ;
        }

        if(temp==1)
        { ++Score; }
    }

    return Score ;
}
 
void ChangeNeighbourhood()
{
    ++current_neighbourhood;

    if(current_neighbourhood>2)
    {   cout << endl ;
        cout<<"This is the local optimal solution in all neighbourhoods" << endl ;
        cout<<"Current Values : "; PrintV() ;  
        cout<<"No. of Clauses satisfied : " << Evaluate(V) << endl ;
        cout<<"No. of states explored : " << number_of_moves << endl;
        exit(1);
    }
}

vector<int> Change( int i )
{
    vector<int> temp = V ;
    temp[i] = (temp[i]==0)? 1:0 ;
    return temp ;
}

vector<int> Change( vector<int> B , int i )
{
    vector<int> temp = B ;
    temp[i] = (temp[i]==0)? 1:0 ;
    return temp ;
}

void Best_of_0()
{
    int scores[] = {0,0,0,0} ;
    for(int i=0 ; i<4 ; ++i)
    {
        scores[i] = Evaluate(Change(i)) ;
    }
    int max_score = scores[0] ; int max_index = 0 ;
    for(int i=0 ; i<4 ; ++i)
    {
        if(scores[i]>=max_score)
        {
            max_score = scores[i] ;
            max_index = i ;
        }
    }

    if(Evaluate(Change(max_index)) >= Evaluate(V))
    { V = Change(max_index) ;
      cout<<" "<<Evaluate(Change(max_index))<< " "<< Evaluate(V) << endl ;
    }   
}

void Best_of_1()
{
    int scores[] = {0,0,0,0,0,0} ;

    scores[0]=Evaluate(Change(Change(0),1)) ;
    scores[1]=Evaluate(Change(Change(0),2)) ;
    scores[2]=Evaluate(Change(Change(0),3));
    scores[3]=Evaluate(Change(Change(1),2)) ;
    scores[4]=Evaluate(Change(Change(1),3)) ;
    scores[5]=Evaluate(Change(Change(2),3)) ;

    int max_score = scores[0] ; int max_index = 0 ;

    for(int i=0 ; i<6 ; ++i)
    {
        if(scores[i]>=max_score)
        { 
            max_score = scores[i] ;
            max_index = i ;
        }
    }

    if(max_score>= Evaluate(V))
    {
    switch(max_index)
    {
        case 0: V=Change(Change(0),1) ; break ;
        case 1: V=Change(Change(0),2) ; break ;
        case 2: V=Change(Change(0),3) ; break ;
        case 3: V=Change(Change(1),2) ; break ;
        case 4: V=Change(Change(1),3) ; break ;
        case 5: V=Change(Change(2),3) ; break ;
    } 
    } 
}

void Best_of_2()
{
    int scores[] = {0,0,0,0} ;

    scores[0]=Evaluate(Change(Change(Change(0),1),2)) ;
    scores[0]=Evaluate(Change(Change(Change(0),1),3)) ;
    scores[0]=Evaluate(Change(Change(Change(1),2),3)) ;
    scores[0]=Evaluate(Change(Change(Change(2),3),0)) ;

    int max_score = scores[0] ; int max_index = 0 ;

    for(int i=0 ; i<4 ; ++i)
    {
        if(scores[i]>=max_score)
        { 
            max_score = scores[i] ;
            max_index = i ;
        }
    }

    if(max_score>= Evaluate(V))
    {
    switch(max_index)
    {
        case 0: V=Change(Change(Change(0),1),2) ; break ;
        case 1: V=Change(Change(Change(0),1),3) ; break ;
        case 2: V=Change(Change(Change(1),2),3) ; break ;
        case 3: V=Change(Change(Change(2),3),0) ; break ;
    }  
    }

}


// MoveGen to generate next possible states , and return the best one according to Heuristic
void MoveGen(vector<int> A)
{
    switch(current_neighbourhood)
    {
        case 0: Best_of_0() ; break ;
        case 1: Best_of_1() ; break ;
        case 2: Best_of_2() ; break ;
    /*    case 3: Best_of_3() ; break ;
        case 4: Best_of_all() ; break ;  */
    }
}

// Check if the current state is the goal State
bool GoalTest()
{
    if(Evaluate(V)==5) return 1 ;
    else return 0 ;
}

void VND()
{   vector<int> Last ;
    vector<int> SecondLast ;
    while(GoalTest()==0)
    {   
        SecondLast = Last ;
        Last = V ;
        MoveGen(V);
        ++number_of_moves ;

        cout<< endl ;
        cout<<"Current Values : "; PrintV() ;  
        cout<<"No. of Clauses satisfied : " << Evaluate(V) << endl  ;
    
        if(V==Last || V==SecondLast)
        {
           ChangeNeighbourhood();
        }
    }
}




int main()
{
    cout << "Initializing Variables to ";
    PrintV();

    // Insert values into SAT from file 

   VND() ;

    return 0;
}



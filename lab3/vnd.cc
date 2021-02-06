#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <string>

using namespace std ;

vector<string> SAT ;
vector<int> V {0,0,0,0} ;

int number_of_moves = 1;
int current_neighbourhood = 0;

// To print the values of variables a,b,c,d
void PrintV()
{   
    for(int i=0 ; i<V.size() ; ++i)
    {  cout<<V[i]<<"  ";  }
    cout << "\n" ;
}

// To print the values of a particular assignment of variables
void PrintV(vector<int> A)
{   
    for(int i=0 ; i<A.size() ; ++i)
    {  cout<<A[i]<<"  ";  }
    cout << "\n" ;
}

//To return the values of variables according to the clause
int Value(char c , vector<int> T )
{
    switch(c)
    {
        case 'a' : return T[0]; break ;
        case 'b' : return T[1]; break ;
        case 'c' : return T[2]; break ;
        case 'd' : return T[3]; break ;

        case 'A' : return ((T[0]==0)?1:0) ; break ;
        case 'B' : return ((T[1]==0)?1:0) ; break ;
        case 'C' : return ((T[2]==0)?1:0) ; break ;
        case 'D' : return ((T[3]==0)?1:0) ; break ;
    }

    return -1 ;
}

// Heuristic to evaluate how good this assignment of variables is
int Evaluate(vector<int> T)
{
    int Score = 0 ;
    for(int i=0 ; i<5 ; ++i)
    {
        int temp = 0 ;
        for(int j=0 ; j<3 ; ++j)
        {   if( Value(SAT[i][j],T) == 1 )
            {
                temp=1 ;
            }
        }

        if(temp==1)
        { Score = Score+1 ; }

    }

    return Score ;
}

// To go to a different neighbourhood if local minima is encountered
void ChangeNeighbourhood()
{
    ++current_neighbourhood;
    cout <<"\nChanging Neighbourhood\n";

    if(current_neighbourhood>3)
    {   cout << endl ;
        cout<<"This is the local optimal solution in all neighbourhoods" << endl ;
        cout<<"Current Values : "; PrintV() ;  
        cout<<"No. of Clauses satisfied : " << Evaluate(V) << endl ;
        cout<<"No. of states explored : " << number_of_moves << endl;
        exit(1);
    }
}

// Change the ith variable in current assignment
vector<int> Change( int i )
{
    vector<int> temp = V ;
    temp[i] = (temp[i]==0)? 1:0 ;
    return temp ;
}


// Overloaded to change the ith variable in the passed assignent of variables
vector<int> Change( vector<int> B , int i )
{
    vector<int> temp = B ;
    temp[i] = (temp[i]==0)? 1:0 ;
    return temp ;
}

// Returns the best possible move by pertubing ONLY 1 variable
void Best_of_0()
{
    int scores[] = {0,0,0,0} ;

    scores[0]=Evaluate(Change(0));
    scores[1]=Evaluate(Change(1));
    scores[2]=Evaluate(Change(2));
    scores[3]=Evaluate(Change(3));

    int max_score = scores[0] ; int max_index = 0 ;
    for(int i=0 ; i<4 ; ++i)
    {
        if(scores[i]>=max_score)
        {
            max_score = scores[i] ;
            max_index = i ;
        }
    }

    if(max_score >= Evaluate(V))
    { V = Change(max_index) ; }   

}


// Returns the best possible move by pertubing ONLY 2 variables
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

    if(max_score >= Evaluate(V))
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


// Returns the best possible move by pertubing ONLY 3 variables
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

// Returns the move by pertubing all 4 variables
void Best_of_3()
{   int max_score = Evaluate(Change(Change(Change(Change(0),1),2),3)) ;

    if(max_score>= Evaluate(V))
    { V = Change(Change(Change(Change(0),1),2),3) ; }
}


// MoveGen to generate next possible states , and return the best one according to Heuristic
void MoveGen(vector<int> A)
{
    switch(current_neighbourhood)
    {
        case 0: Best_of_0() ; break ;
        case 1: Best_of_1() ; break ;
        case 2: Best_of_2() ; break ;
        case 3: Best_of_3() ; break ;
    }
}

// Check if the current state is the goal State
bool GoalTest()
{
    if(Evaluate(V)==5) 
    {
     cout << "\nSolution Found\n" ;  
     cout<<"Current Values : "; PrintV() ;
     cout<<"No. of states explored : " << number_of_moves << endl;
     exit(0);
    }
    else
    {  return 0 ; }
}

// Implement Variable Neighbourhood Descent 
void VND()
{   vector<int> Last ;
    vector<int> SecondLast ;
    while(GoalTest()==0)
    {   
        SecondLast = Last ;
        Last = V ;
        MoveGen(V);
        switch(current_neighbourhood)
        {
            case 0: number_of_moves+=4 ; break ;
            case 1: number_of_moves+=6 ; break ;
            case 2: number_of_moves+=4 ; break ;
            case 3: number_of_moves+=1 ; break ;

        }

        cout<< endl ;
        cout<<"Current Iteration Values : "; PrintV() ;  
        cout<<"No. of Clauses satisfied : " << Evaluate(V) << endl  ;
    
        if(V==Last || V==SecondLast)    // If Local minima is encountered then switch neighbourhoods
        {
           ChangeNeighbourhood();
        }
    }
}




int main()
{   int no_of_literals , no_of_clauses , TIME ;
    // Insert values into SAT from file 
    std::cin >> no_of_literals >> no_of_clauses;

    TIME = (1 << no_of_literals);

    for(int i=0; i<no_of_clauses; ++i) {
        std::string s;
        std::cin >> s;
        SAT.push_back(s);
    }

    std::cout << "Starting Hill Climbing with Variable Neighbourhood Descent ... \n";
    
    VND();

    return 0;
}



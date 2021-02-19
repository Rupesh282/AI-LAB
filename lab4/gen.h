#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#define _TIME_ 299


//over-all outline for genetic algo : 
// 1 ) create chromosomes
// 2 ) mutate + crossover
// 3 ) select best N chromosomes
// 4 ) repeat from 1 )
// 5 ) keep track of best ans



// total no. of cities
int N_cities; 

std::string E_type;
std::vector<std::pair<double, double>> cities;

// distance matrix
std::vector<std::vector<double>> dist_matrix; 

//random generator
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> distr(0, 499);


/*
 * class for chromosome
 * Each chromosome contains a permutation of order of cities we visit
 */
class chromo {
    public:
        int n;
        double value;
        std::vector<int> order;
        chromo() {
            ;
        }
        
        chromo(int N) {
            n = N;
            order.resize(n);
            //order vector is intialized with 0 .. n-1 
            std::iota(order.begin(), order.end(), 0);
            shuffle(order.begin(), order.end(), rng);
            value = cost();
        }

        // gives a random range {L, R} such that L < R and R < n
        std::pair<int, int> randomRange() {
            int L, R;
            L = distr(rng) % n;
            do {
                R = distr(rng) % n;
            } while(R == L);
            if(R < L)
                std::swap(L, R);
            return {L, R};
        }

        // mutation : reverses random range in order
        chromo mutate() {
            std::pair<int, int> range = randomRange();
            chromo mutated = *this;
            std::reverse(mutated.order.begin()+range.first, mutated.order.begin()+range.second);
            mutated.value = mutated.cost();
            return mutated;
        }

        // crossover : stick random range from one chromo to another and vise-versa
        std::pair<chromo, chromo> cross(chromo& another) {
            chromo F_cross, S_cross;
            std::pair<int, int> range = randomRange();
            int L = range.first, R = range.second;

            //cross in current chromo
            for(int i=0;i<n;++i) {
                bool present = false;
                for(int j=L;j<=R && !present ;++j)
                    if(another.order[j] == order[i])
                        present = true;
                if(!present)
                    F_cross.order.push_back(order[i]);
            }
            for(int i=L;i<=R;++i)
                F_cross.order.push_back(another.order[i]);


            //cross in another chromo
            for(int i=0;i<n;++i) {
                bool present = false;
                for(int j=L;j<=R && !present ;++j)
                    if(order[j] == another.order[i])
                        present = true;
                if(!present)
                    S_cross.order.push_back(another.order[i]);
            }
            for(int i=L;i<=R;++i)
                S_cross.order.push_back(order[i]);

            F_cross.n = (int)F_cross.order.size();
            S_cross.n = (int)S_cross.order.size();
            F_cross.value = F_cross.cost();
            S_cross.value = S_cross.cost();
            return {F_cross, S_cross};
        }

        // below function finds cost of a tour 
        double cost() {
            double _cost = 0.00;
            for(int i=0;i<n;++i) 
                _cost += dist_matrix[order[i]][order[(i+1) % n]];
            return _cost;
        }

        // print current chromosome data
        void print() {
            std::cout << "cost : " << value << "\n";
            for(int i=0;i<n;++i)
                std::cout << order[i] << " ";
            std::cout << "\n";
        }

        // operator overloading for <chromo> objects which
        // we will use while sorting
        bool operator < (chromo& A) {
            double this_cost = value;
            double A_cost = A.value;
            return this_cost < A_cost;
        }
};



class geneticModel0 {
    int ITER = 50; // default value for iteration
    int N_chromosomes = 250; // end selection count
    int M_count = 500; // mutations count
    int C_count = 1000; // cross over count
    chromo BEST; // this will keep track of best chromosome/solution
    public:

        geneticModel0() {
            ;
        }

        geneticModel0(int N, int M, int C, int I) {
            N_chromosomes = N;
            M_count = M;
            C_count = C;
            ITER = I;
        }

        // below function returns random {L, R} random where R < mod and L < R
        std::pair<int, int> randomInd(int mod) {
            int L, R;
            L = distr(rng) % mod;
            do {
                R = distr(rng) % mod;
            } while(R == L);
            if(R < L)
                std::swap(L, R);
            return {L, R};
        }

        // below function actually starts our simulation
        void boot() {
            //start measuring the time from here
            auto start = std::chrono::high_resolution_clock::now();

            // first create some random population
            std::vector<chromo> population;
            for(int i=0;i<N_chromosomes;++i) {
                chromo T = chromo(N_cities);
                population.push_back(T);
            }
            BEST = population[0];
            for(int i=0;i<N_chromosomes;++i) {
                if(BEST.value > population[i].value)
                    BEST = population[i];
            }
            BEST.print();

            sort(population.begin(), population.end());

            //int iteration = 0;
            //iterate for ITER no. of times
            while (true) {
                // do cross over
                for(int i=0;i<C_count;++i) {
                    std::pair<int, int> ind = randomInd((int)population.size() / 3);
                    std::pair<chromo, chromo> product = population[ind.first].cross(population[ind.second]);
                    population.push_back(product.first);
                    population.push_back(product.second);
                }

                // do mutations in population
                for(int i=0;i<M_count;++i) {
                    int randomIndex = distr(rng) % ((int)population.size() / 3);
                    chromo mutated = population[randomIndex].mutate();
                    population.push_back(mutated);
                }

                //putting some random chromosomes 
                int randomInserts = 100;
                for(int i=0;i<randomInserts;++i)
                    population.push_back(chromo(N_cities));

                //sort accoring to cost
                sort(population.begin(), population.end());

                //select best N
                while((int)population.size() > N_chromosomes) 
                    population.pop_back();

                //update BEST
                if(BEST.value > population[0].value) {
                    BEST = population[0];
                    BEST.print();
                }
                
                // std::cout << "[+] Iteration count : " << iteration << "\n";
                // iteration ++;

                //check current time
                auto end = std::chrono::high_resolution_clock::now();

                //find duration from start
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
                
                if(duration > _TIME_)
                    break;
                //std::cout << "duration : " << duration << " seconds." << "\n";
            }
            BEST.print();
            /* done */
        }
};


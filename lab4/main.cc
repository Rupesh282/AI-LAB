#include <iostream>
#include <iomanip>
#include "gen.h"

int main() {

    //take input
    std::cin >> E_type >> N_cities;

    // take co-ordinates of cities
    for(int i=0;i<N_cities;++i) {
        double x, y; std::cin >> x >> y;
        cities.push_back({x, y});
    }

    // take input of distance matrix
    dist_matrix.resize(N_cities);
    for(int i=0;i<N_cities;++i) {
        for(int j=0;j<N_cities;++j) {
            double d; std::cin >> d;
            dist_matrix[i].push_back(d);
        }
    }

    //initialize model
    geneticModel0 model(50, 20, 20, 10000);

    std::cout << std::fixed << std::setprecision(5) ;
    //simulate
    model.boot();

    return 0;
}

#include <iostream>
#include "gen.h"

int main() {
    std::cin >> E_type >> N_cities;

    for(int i=0;i<N_cities;++i) {
        double x, y; std::cin >> x >> y;
        cities.push_back({x, y});
    }

    dist_matrix.resize(N_cities);
    for(int i=0;i<N_cities;++i) {
        for(int j=0;j<N_cities;++j) {
            double d; std::cin >> d;
            dist_matrix[i].push_back(d);
        }
    }

    mark1 jarvis(50, 20, 20, 10000);

    jarvis.boot();

    return 0;
}

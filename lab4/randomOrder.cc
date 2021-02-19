#include <bits/stdc++.h>
#include <random>
#include <chrono>

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

std::string E_type;
int N;
std::vector<std::pair<int, std::pair<double, double>>> co_ord;
std::vector<std::vector<double>> cost_mat;



double _cost() {
    double cost = 0;
    
    for(int i=1;i<N;++i)
        cost += cost_mat[co_ord[i-1].first][co_ord[i].first];
    cost += cost_mat[co_ord[N-1].first][co_ord[0].first];

    return cost;
}

void print_tour() {
    for(int i=0;i<N;++i)
        std::cout << co_ord[i].first << " ";
    std::cout << "\n";
}

int main() {
    std::cin >> E_type >> N;
    for(int i=0;i<N;++i) {
        double x, y; std::cin >> x >> y;
        co_ord.push_back({i, {x, y}});
    }

    cost_mat.assign(N, std::vector<double>());

    for(int i=0;i<N;++i) {
        for(int j=0;j<N;++j) {
            double d; std::cin >> d;
            cost_mat[i].push_back(d);
        }
    }

    double in_cost = _cost();

    std:: cout << "cur best tour : " << in_cost << "\n";
    print_tour();


    for(int i=0;i<250;++i) {
        shuffle(co_ord.begin(), co_ord.end(), rng);
        double cur_cost = _cost();
        //std::cerr << cur_cost << "\n";
        if(cur_cost < in_cost) {
            in_cost = cur_cost;
            std:: cout << "cur best tour : " << in_cost << "\n";
            print_tour();
        }
    }

    return 0;
}


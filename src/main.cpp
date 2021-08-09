#include "network_load_balancer.h"
#include "parser.h"

#include <iostream>

int main() {
    using namespace My_NLB;
    try {
        Parser prs;
        NLB nlb(std::move(prs.getConns()), prs.getNomps());
    } catch (const exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception!" << std::endl;
    }

    return 0;
}
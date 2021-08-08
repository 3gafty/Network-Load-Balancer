#include "pars_address.h"
#include "pars_freq_of_udp.h"
#include "network_load_balancer.h"

#include <iostream>
#include <memory>

auto network_load_balancer = std::make_unique<NetworkLoadBalancer>();

int main(int argc, const char* argv[]) {
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);
    try {
        std::cout << "Start Program" << std::endl;
        if (argc < 2) {
            throw std::runtime_error(std::string("Not select config file"));
        } else if (argc > 2) {
            throw std::runtime_error(std::string("To many arguments."));
        } else {
            std::string file_name(argv[1]);
            network_load_balancer->setConnections(parsAddress(file_name));
            network_load_balancer->setNumberOfMessagesPerSecond(parsFreqOfUDP(file_name));
        }
        network_load_balancer->run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "unknown exception!" << std::endl;
        return 2;
    }
    std::cout << "Stop program" << std::endl;
    return 0;
}
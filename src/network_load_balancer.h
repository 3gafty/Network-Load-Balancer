#ifndef NETWORK_LOAD_BALANCER_H
#define NETWORK_LOAD_BALANCER_H

#include <chrono>
#include <list>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>
#include <vector>

namespace My_NLB
{
    using namespace std;
    using namespace std::chrono;

    class NLB
    {
    public:
        explicit NLB(pair<sockaddr_in, vector<sockaddr_in>>&& conns, uint32_t nomps);
        void run();
        ~NLB();

    private:
        pair<sockaddr_in, vector<sockaddr_in>> conns_;
        list<chrono::_V2::steady_clock::time_point> times_;
        vector<uint8_t> buff_;
        int buff_size_;
        uint32_t sec_;
        uint32_t nomps_;
        int listener_;
        bool run_;
        int num_;
        int senderr_;

    };
}   //!My_NLB

#endif  //!NETWORK_LOAD_BALANCER_H

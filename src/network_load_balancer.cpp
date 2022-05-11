#include "network_load_balancer.h"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <chrono>

namespace My_NLB
{
    using namespace std;
    using namespace std::chrono;

    NLB::NLB(pair<sockaddr_in, vector<sockaddr_in>>&& conns, uint32_t nomps) : conns_(move(conns)), nomps_(nomps)
    {
        buff_size_ = 1024;
        sec_ = 1000000;
        num_ = 0;
        senderr_ = 0;
        buff_.resize(buff_size_);
        run();
    }

    void NLB::run()
    {
        run_ = true;
        listener_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (listener_ < 0)
        {
            throw runtime_error("Socket failed. The errno value is: " + to_string(errno));
        }

        int opt = 1;
        if (setsockopt (listener_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
            throw runtime_error("Setsockopt failed. The errno value is: " + to_string(errno));
        }

        if (bind(listener_, reinterpret_cast<struct sockaddr*>(&conns_.first), sizeof(conns_.first)) < 0)
        {
            throw runtime_error("Bind failed. The errno value is: " + to_string(errno));
        }

        duration<double> delta{};

        while (run_)
        {
            int err = recvfrom(listener_, buff_.data(), buff_size_, MSG_TRUNC, 0, 0);
            auto current_tp = steady_clock::now();
            if (err > buff_size_)
            {
                cerr << "MSG more then SIZEBUFF. Not implemented." << endl;
            }
            else if (err > 0 && err <= buff_size_)
            {
                times_.empty() ? delta = current_tp - times_.front() : delta = current_tp - times_.front();
                while (!times_.empty() && duration_cast<microseconds>(delta).count() > sec_)
                {
                    times_.pop_front();
                    if (!times_.empty())
                    {
                        delta = current_tp - times_.front();
                    }
                }
                if (run_ && (times_.size() < nomps_))
                {
                    do
                    {
                        sockaddr_in client_addr = conns_.second[num_];
                        socklen_t size_addr = sizeof(client_addr);
                        senderr_ = sendto(listener_, buff_.data(), err, 0,
                                          reinterpret_cast<struct sockaddr*>(&client_addr),
                                          size_addr);
                        auto send_tp = steady_clock::now();
                        if (senderr_ < 0)
                        {
                            cerr << "Error sendto. The errno value is : " << errno << endl;
                        }
                        else
                        {
                            times_.push_back(send_tp);
                        }
                        ++num_;
                        num_ %= conns_.second.size();
                    }
                    while (senderr_ < 0);
                }
            }
            else
            {
                cerr << "Error resieved. The errno value is : " << errno << endl;
            }
        }
    }

    NLB::~NLB()
    {
        run_ = false;
        close(listener_);
    }

}   //!My_NLB

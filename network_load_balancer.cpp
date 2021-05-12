#include "network_load_balancer.h"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>

#include <iostream>
#include <chrono>

#define SIZE_BUFF 1024
#define ONE_SECOND 1000

namespace mynamespace {

	NetworkLoadBalancer::NetworkLoadBalancer()
	{}

	void NetworkLoadBalancer::setConnections(std::pair<sockaddr_in, std::vector<sockaddr_in>>&& t_conn)
	{
		connections_ = std::move(t_conn);
	}

	void NetworkLoadBalancer::setNumberOfMessagesPerSecond(unsigned int t_freq)
	{
		number_of_messages_per_second_ = t_freq;
	}

	void NetworkLoadBalancer::run()
	{
		using namespace std::chrono;

		run_ = true;

		listener_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (listener_ < 0)
		{
			throw std::runtime_error("Socket failed. The errno value is: " + std::to_string(errno));
		}

		int opt = 1;
		if (setsockopt (listener_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		{
			throw std::runtime_error("Setsockopt failed. The errno value is: " + std::to_string(errno));
		}

		if (bind(listener_, (struct sockaddr *)&connections_.first, sizeof(connections_.first)) < 0)
		{
			throw std::runtime_error("Bind failed. The errno value is: " + std::to_string(errno));
		}

		int num{0};
		int senderr{1};

		while (run_)
		{
			char buf[SIZE_BUFF];
			int err = recvfrom(listener_, buf, SIZE_BUFF, MSG_TRUNC, 0, 0);
			auto current_time_point = steady_clock::now();
			if (err > SIZE_BUFF)
			{
				std::cerr << "PacketSize more then SIZEBUFF. Not implemented case" << std::endl;
			}
			else if (err > 0 && err <= SIZE_BUFF)
			{
				while (!time_stamps_of_sended_messages_.empty() &&
						duration_cast<microseconds>(current_time_point - time_stamps_of_sended_messages_.front()).count() >
						ONE_SECOND)
				{
					time_stamps_of_sended_messages_.pop_front();
				}
				if (run_ && time_stamps_of_sended_messages_.size() < number_of_messages_per_second_)
				{
					do
					{
						sockaddr_in client_addr = connections_.second[num];
						socklen_t size_ddr = sizeof(client_addr);
						senderr = sendto(listener_, buf, err, 0, (struct sockaddr *)&client_addr, size_ddr);
						auto send_time_stamp = steady_clock::now();
						if (senderr < 0)
						{
							std::cerr << "Error sendto. The errno value is : " << errno << std::endl;
						}
						else
						{
							time_stamps_of_sended_messages_.push_back(send_time_stamp);
						}
						++num;
						num %= connections_.second.size();
					}
					while (senderr < 0);
				}
			}
			else
			{
				std::cerr << "Error resieved. The errno value is : " << errno << std::endl;
			}
		}
	}

	void NetworkLoadBalancer::stop()
	{
		run_ = false;
		closingMessage();
	}

	NetworkLoadBalancer::~NetworkLoadBalancer()
	{
		close(listener_);
		std::cout << "exit NetworkLoadBalancer" << std::endl;
	}

	void NetworkLoadBalancer::closingMessage()
	{
		int finalizer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (finalizer < 0)
		{
			std::cerr << "Socket failed. The errno value is: " + std::to_string(errno) << std::endl;
		}

		std::string byby("exit");
		socklen_t size_addr = sizeof(connections_.first);

		int senderr_final = sendto(finalizer, byby.data(), byby.size(), 0, (struct sockaddr *)&connections_.first, size_addr);
		if (senderr_final < 0)
		{
			std::cerr << "Error sendto. The errno value is : " << errno << std::endl;
		}

		close(finalizer);
	}

}	//	mynamespace
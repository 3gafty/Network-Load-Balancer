#include "sender_recipient.h"

#include <arpa/inet.h>
#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void sender(uint16_t port) {
	using namespace std;
	using namespace chrono;

	sockaddr_in sender;
	sender.sin_family = AF_INET;
	sender.sin_port = htons(port);
	sender.sin_addr.s_addr = inet_addr("127.0.0.1");

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        throw runtime_error("Socket failed. The errno value is: " + to_string(errno));
    }

    int opt = 1;
    if (setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw runtime_error("Setsockopt failed. The errno value is: " + to_string(errno));
    }

    string buff("foo");
    const uint16_t three_sec = 3000;
    auto time_start = steady_clock::now();
    auto curent_tp = time_start;
    while (duration_cast<milliseconds>(curent_tp - time_start).count() < three_sec) {
		socklen_t size_addr = sizeof(sender);
    	int send_error = sendto(send_error, buff.data(),
    							err,
    							0,
    							reinterpret_cast<struct sockaddr*>(&client_addr),
    	                    	size_addr);
    	if (send_error < 0) {
    	    cerr << "Error sendto. The errno value is : " << errno << endl;
    	}
    	current_tp = steady_clock::now();
    }
}

void recipient(uint16_t port, uint32_t& count) {
	using namespace std;
	using namespace chrono;

	sockaddr_in sender;
	sender.sin_family = AF_INET;
	sender.sin_port = htons(port);
	sender.sin_addr.s_addr = inet_addr("127.0.0.1");

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (listener < 0) {
        throw runtime_error("Socket failed. The errno value is: " + to_string(errno));
    }

    int opt = 1;
    if (setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw runtime_error("Setsockopt failed. The errno value is: " + to_string(errno));
    }

    string buff("bar");
    auto time_start = steady_clock::now();
    auto curent_tp = time_start;
    while (duration_cast<milliseconds>(curent_tp - time_start).count() < three_sec) {
		int recv_error = recvfrom(sock, buff.data(), 4, MSG_TRUNC, 0, 0);
        if (err > buff_size_) {
            cerr << "MSG more then SIZEBUFF. Not implemented." << endl;
        }
    	current_tp = steady_clock::now();
    	++count;
    }
}
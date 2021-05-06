#ifndef NETWORKLOADBALANCER_H
#define NETWORKLOADBALANCER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstring>
#include <chrono>
#include <thread>
#include <cmath>
#include <exception>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define SIZEBUFF 1024
#define ONE_SECOND 1000

void networkLoadBalancer(const std::pair<sockaddr_in, std::vector<sockaddr_in>>& conn, const unsigned int freq);

#endif // !NETWORKLOADBALANCER_H

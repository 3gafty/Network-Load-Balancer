#ifndef NETWORKLOADBALANCER_H
#define NETWORKLOADBALANCER_H

#include <vector>
#include <utility>

#include <arpa/inet.h>

#define SIZEBUFF 1024
#define ONE_SECOND 1000

void networkLoadBalancer(const std::pair<sockaddr_in, std::vector<sockaddr_in>>& conn, const unsigned int freq);

#endif // !NETWORKLOADBALANCER_H

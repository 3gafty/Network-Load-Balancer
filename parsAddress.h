#ifndef PARSADDRESS_H
#define PARSADDRESS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstring>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

std::pair<sockaddr_in, std::vector<sockaddr_in>> parsAddress(const char* f);

#endif // !PARSADDRESS_H
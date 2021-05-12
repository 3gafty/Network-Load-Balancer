#ifndef PARS_ADDRESS_H
#define PARS_ADDRESS_H

#include <arpa/inet.h>
#include <fcntl.h>

#include <vector>
#include <utility>
#include <string>

std::pair<sockaddr_in, std::vector<sockaddr_in>> parsAddress(const std::string& t_str);

#endif // PARS_ADDRESS_H

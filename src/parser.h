#ifndef PARSER_H
#define PARSER_H

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>
#include <vector>

namespace My_NLB {

	using namespace std;
	class Parser {
	public:
		explicit Parser(const string path = "../config.json");
		pair<sockaddr_in, vector<sockaddr_in>>& getConns();
		uint32_t getNomps() const;

	private:
		pair<sockaddr_in, vector<sockaddr_in>> conns_;
		uint32_t nomps_;

	};

}	//!My_NLB

#endif	//!PARSER_H
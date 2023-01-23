#include "webserver.hpp"

#include <format>
#include <iostream>

webserver::webserver() {}

webserver::webserver(int t) : n(t) {}

webserver::~webserver() {}

void webserver::print() { std::cout << std::format("{}", n) << std::endl; }

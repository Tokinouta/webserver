#include <unistd.h>
#include <string>
#include "log.hpp"

int main() {
    auto& log{Logger::get()};
    log.debug(std::string("This is a debug message."));
    log.trace(std::string("This is a trace message."));
    log.info(std::string("This is a info message."));
    log.warning(std::string("This is a warning message."));
    log.error(std::string("This is a error message."));

    sleep(3);
    
    auto fs(std::ifstream("./log"));
    for (std::string a; std::getline(fs, a); ) {
        std::cout << a << std::endl;
    }
    return 0;
}
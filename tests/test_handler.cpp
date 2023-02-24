#include <iostream>
#include "handler.hpp"
#include "http_response.hpp"

int main() {
  auto handler{Handler()};
  auto someFunction = [](int a, float b, const std::string& c,
                         const std::string& url) -> HttpResponse {
    std::cout << "someFunction called with a = " << a << ", b = " << b
              << ", c = " << c << std::endl;
    return HttpResponse();
  };
  handler.register_route<HttpResponse>("string path", someFunction);
  return 0;
}
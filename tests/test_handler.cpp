#include <iostream>
#include <string>
#include <cassert>

#include "handler.hpp"
#include "http_response.hpp"

int main() {
  auto handler{Handler()};
  auto someFunction = [](const std::string& url) -> HttpResponse {
    std::cout << url << std::endl;
    return HttpResponse();
  };
  auto url{string("string path")};
  handler.register_route(url, someFunction);
  handler.call(url);
  assert(1 == 0);
  return 0;
}
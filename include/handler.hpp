#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include "http_connection.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
class Handler {
  std::unordered_map<string, std::function<HttpResponse(const string&)>>
      router;

 public:
  //   void handler(const HttpRequest& req, HttpResponse& res);
  // the function we add to the map needs to do the following:
  // extract parameters from path
  // pass parameters to the handler
  // execute the handler
  // it seems we need a templete here since the second paramater is a function
  // with undetermined parameters
  //   template <typename F, typename... Args>
  //   auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
  //   template <typename... Args>

  // template <typename... T>
  // std::tuple<T...> stringToValues(const std::string& str) {
  //   std::tuple<T...> values;
  //   std::istringstream iss(str);
  //   int index = 0;
  //   // ((iss >> std::get<index++>(values)), ...);
  //   return values;
  // }

  // template <typename F, typename... Args>
  void register_route(string&& path, std::function<HttpResponse(const string&)>&& func) {
    // auto f{std::forward<F>(func)};
    // using func_type = std::function<F>;
    // using arg_type = typename func_type::argument_type;
    // auto parser = [](const string& url) -> Args
    // std::cout << typeid(arg_type).name() << std::endl;
    auto handler = [=](const string& url) -> HttpResponse {
      // std::cout << "Making HTTP request to " << url << std::endl;
      // Call the second parameter function with the arguments passed to the
      // lambda function.
      // auto para{stringToValues<arg_type>(url)};
      return func(url);
    };
    router.insert({path, handler});
  }

  void call(HttpConnection& conn) {
    auto path{conn.path()};
    conn.response() = router[path](path);
  }
};

#endif
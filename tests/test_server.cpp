#include <unistd.h>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <string>

#include "webserver.hpp"

namespace beast = boost::beast;  // from <boost/beast.hpp>
namespace http = beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;     // from <boost/asio.hpp>
using tcp = net::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

// Performs an HTTP GET and prints the response
int main(int argc, char** argv) {
  auto const host = "127.0.0.1";
  auto const port = "12345";
  auto pid{fork()};
  if (pid == 0) {
    auto enable_et{true};
    auto timeout{-1};
    auto server{webserver(host, atoi(port), enable_et, timeout)};
    server.connect();
    if (server.is_connected()) {
      server.run();
    }
  } else {
    sleep(1);
    // Check command line arguments.
    auto const target = "/";
    int version = 11;

    // The io_context is required for all I/O
    net::io_context ioc;

    // These objects perform our I/O
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    // Look up the domain name
    auto const results = resolver.resolve(host, port);

    // Make the connection on the IP address we get from a lookup
    stream.connect(results);

    // Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::get, target, version};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Send the HTTP request to the remote host
    http::write(stream, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    // Receive the HTTP response
    http::read(stream, buffer, res);

    // Write the message to standard out
    std::cout << res << std::endl;

    beast::flat_buffer buffer2;
    http::response<http::dynamic_body> res2;
    sleep(1);
    http::write(stream, req);
    http::read(stream, buffer2, res2);
    std::cout << res2 << std::endl;

    // Gracefully close the socket
    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    // not_connected happens sometimes
    // so don't bother reporting it.
    //
    if (ec && ec != beast::errc::not_connected) throw beast::system_error{ec};

    // If we get here then the connection is closed gracefully
    sleep(1);
    kill(pid, SIGINT);
  }

  return 0;
}
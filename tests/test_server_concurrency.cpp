#include <unistd.h>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <csignal>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>

#include "webserver.hpp"

namespace beast = boost::beast;  // from <boost/beast.hpp>
namespace http = beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;     // from <boost/asio.hpp>
using tcp = net::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

// Report a failure
void fail(beast::error_code ec, char const* what) {
  std::cerr << what << ": " << ec.message() << "\n";
}

// Performs an HTTP GET and prints the response
void do_session(std::string const& host, std::string const& port,
                std::string const& target, int version, net::io_context& ioc,
                net::yield_context yield) {
  beast::error_code ec;

  // These objects perform our I/O
  tcp::resolver resolver(ioc);
  beast::tcp_stream stream(ioc);

  // Look up the domain name
  auto const results = resolver.async_resolve(host, port, yield[ec]);
  if (ec) return fail(ec, "resolve");

  // Set the timeout.
  stream.expires_after(std::chrono::seconds(30));

  // Make the connection on the IP address we get from a lookup
  stream.async_connect(results, yield[ec]);
  if (ec) return fail(ec, "connect");

  // Set up an HTTP GET request message
  http::request<http::string_body> req{http::verb::get, target, version};
  req.set(http::field::host, host);
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  // Set the timeout.
  stream.expires_after(std::chrono::seconds(30));

  // Send the HTTP request to the remote host
  http::async_write(stream, req, yield[ec]);
  if (ec) return fail(ec, "write");

  // This buffer is used for reading and must be persisted
  beast::flat_buffer b;

  // Declare a container to hold the response
  http::response<http::dynamic_body> res;

  // Receive the HTTP response
  http::async_read(stream, b, res, yield[ec]);
  if (ec) return fail(ec, "read");

  // Write the message to standard out
  std::cout << res << std::endl;

  // Gracefully close the socket
  stream.socket().shutdown(tcp::socket::shutdown_both, ec);

  // not_connected happens sometimes
  // so don't bother reporting it.
  //
  if (ec && ec != beast::errc::not_connected) return fail(ec, "shutdown");

  // If we get here then the connection is closed gracefully
}

// Performs an HTTP GET and prints the response
int main(int argc, char** argv) {
  auto const host = "127.0.0.1";
  auto const port = "12345";
  auto pid{fork()};
  if (pid == 0) {
    auto enable_et{true};
    auto timeout{-1};
    auto server{webserver(host, atoi(port), enable_et, timeout, 10)};
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

    // Launch the asynchronous operation
    for (auto i{0}; i < 10; i++) {
      boost::asio::spawn(
          ioc, std::bind(&do_session, std::string(host), std::string(port),
                         std::string(target), version, std::ref(ioc),
                         std::placeholders::_1));
    }
    // Run the I/O service. The call will return when
    // the get operation is complete.
    ioc.run();

    // If we get here then the connection is closed gracefully
    sleep(1);
    kill(pid, SIGINT);
  }

  return 0;
}
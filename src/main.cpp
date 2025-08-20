#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <spdlog/spdlog.h>

#include <iostream>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

int main() {
    // Set log level (debug, info, warn, error, critical, off)
    spdlog::set_level(spdlog::level::warn);

    try {
        net::io_context ioc;

        // Listen on 0.0.0.0:8080
        tcp::acceptor acceptor{ioc, {tcp::v4(), 8080}};
        std::cout << "Server running at http://localhost:8080\n";

        while(true) {
            tcp::socket socket{ioc};
            acceptor.accept(socket);

            beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);

            // Clean, simple logging
            spdlog::info("Request: {} {}", req.method_string(), req.target());
            
            // Make response
            http::response<http::string_body> res{
                http::status::ok, req.version()};
            res.set(http::field::server, "Beast");
            res.set(http::field::content_type, "text/html");
            res.body() = "<h1>Hello World</h1>";  
            res.prepare_payload();

            http::write(socket, res);
        }
    } catch (std::exception const& e) {        
        spdlog::error("Error: {}", e.what());
        return 1;
    }
}
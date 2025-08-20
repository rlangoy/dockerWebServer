#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <string>

namespace beast = boost::beast;     // Boost.Beast
namespace http = beast::http;       // HTTP
namespace net = boost::asio;        // Networking
using tcp = net::ip::tcp;

int main() {
    try {
        net::io_context ioc;

        // Listen on 0.0.0.0:8080
        tcp::acceptor acceptor{ioc, {tcp::v4(), 8080}};
        std::cout << "Server running at http://localhost:8080\n";

        for (;;) {
            tcp::socket socket{ioc};
            acceptor.accept(socket);

            beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);

            std::cout << "Request: " << req << "\n";

            // Make response
            http::response<http::string_body> res{
                http::status::ok, req.version()};
            res.set(http::field::server, "Beast");
            res.set(http::field::content_type, "text/html");
            res.body() = "<h1>Hello World </h1>";  
            res.prepare_payload();

            http::write(socket, res);
        }
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}

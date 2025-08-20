#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <spdlog/spdlog.h>

// syslog on windows/linux/osx/freebsd
#ifdef _WIN32
    #include "spdlog/sinks/win_eventlog_sink.h"   
#else
    #include "spdlog/sinks/syslog_sink.h"   
#endif


#include <iostream>
#include <string>


namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

int main() {
    
    //System evelog log messages
    auto eventlog_sink = std::make_shared<spdlog::sinks::win_eventlog_sink_mt>("spdlog-example");
    auto eventlog_logger = std::make_shared<spdlog::logger>("eventlog", eventlog_sink);
    spdlog::register_logger(eventlog_logger);
    eventlog_logger->warn("This is a warning that will end up in Windows/Linx Event Log.");

    // Set log level (trace,debug, info, warn, error, critical, off)
    spdlog::set_level(spdlog::level::trace);

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
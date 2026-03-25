#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "JobService.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    HttpSession(tcp::socket&& socket, std::shared_ptr<JobService> jobService);
    void run();

private:
    void doRead();
    void onRead(beast::error_code ec, std::size_t bytes_transferred);
    void processRequest();
    void sendResponse();
    void onWrite(bool close, beast::error_code ec, std::size_t bytes_transferred);
    
    http::response<http::string_body> handleRequest(http::request<http::string_body>&& req);
    http::response<http::string_body> handleWorkerRoutes(const http::request<http::string_body>& req);
    http::response<http::string_body> handleEmployerRoutes(const http::request<http::string_body>& req);
    http::response<http::string_body> handleJobPostingRoutes(const http::request<http::string_body>& req);
    http::response<http::string_body> handleReplyRoutes(const http::request<http::string_body>& req);
    
    http::response<http::string_body> createResponse(http::status status, const std::string& body);
    http::response<http::string_body> createErrorResponse(http::status status, const std::string& message);

    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    http::response<http::string_body> res_;
    std::shared_ptr<JobService> jobService_;
};

class HttpListener : public std::enable_shared_from_this<HttpListener> {
public:
    HttpListener(net::io_context& ioc, tcp::endpoint endpoint, std::shared_ptr<JobService> jobService);
    void run();

private:
    void doAccept();
    void onAccept(beast::error_code ec, tcp::socket socket);

    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    std::shared_ptr<JobService> jobService_;
};

class HttpServer {
public:
    HttpServer(const std::string& address, unsigned short port, int threads);
    ~HttpServer();
    
    void run();
    void stop();

private:
    std::string address_;
    unsigned short port_;
    int threads_;
    net::io_context ioc_;
    std::shared_ptr<JobService> jobService_;
    std::vector<std::thread> threadPool_;
};

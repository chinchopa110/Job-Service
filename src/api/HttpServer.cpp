#include "api/HttpServer.hpp"
#include "api/JsonSerializer.hpp"
#include "api/JsonParser.hpp"
#include <iostream>
#include <sstream>

HttpSession::HttpSession(tcp::socket&& socket, std::shared_ptr<JobService> jobService)
    : stream_(std::move(socket))
    , jobService_(jobService) {
}

void HttpSession::run() {
    net::dispatch(stream_.get_executor(),
        beast::bind_front_handler(&HttpSession::doRead, shared_from_this()));
}

void HttpSession::doRead() {
    req_ = {};
    stream_.expires_after(std::chrono::seconds(30));
    
    http::async_read(stream_, buffer_, req_,
        beast::bind_front_handler(&HttpSession::onRead, shared_from_this()));
}

void HttpSession::onRead(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    
    if (ec == http::error::end_of_stream) {
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
        return;
    }
    
    if (ec) {
        return;
    }
    
    processRequest();
}

void HttpSession::processRequest() {
    res_ = handleRequest(std::move(req_));
    sendResponse();
}

void HttpSession::sendResponse() {
    auto self = shared_from_this();
    
    res_.set(http::field::server, "JobService API");
    res_.set(http::field::content_type, "application/json");
    res_.prepare_payload();
    
    http::async_write(stream_, res_,
        beast::bind_front_handler(&HttpSession::onWrite, self, res_.need_eof()));
}

void HttpSession::onWrite(bool close, beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    
    if (ec) {
        return;
    }
    
    if (close) {
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
        return;
    }
    
    res_ = {};
    doRead();
}

http::response<http::string_body> HttpSession::createResponse(http::status status, const std::string& body) {
    http::response<http::string_body> res{status, req_.version()};
    res.set(http::field::content_type, "application/json");
    res.body() = body;
    res.keep_alive(req_.keep_alive());
    return res;
}

http::response<http::string_body> HttpSession::createErrorResponse(http::status status, const std::string& message) {
    return createResponse(status, JsonSerializer::serializeError(message));
}

http::response<http::string_body> HttpSession::handleRequest(http::request<http::string_body>&& req) {
    std::string target = std::string(req.target());
    
    if (target.find("/api/workers") == 0) {
        return handleWorkerRoutes(req);
    } else if (target.find("/api/employers") == 0) {
        return handleEmployerRoutes(req);
    } else if (target.find("/api/jobs") == 0) {
        return handleJobPostingRoutes(req);
    } else if (target.find("/api/replies") == 0) {
        return handleReplyRoutes(req);
    }
    
    return createErrorResponse(http::status::not_found, "Endpoint not found");
}

http::response<http::string_body> HttpSession::handleWorkerRoutes(const http::request<http::string_body>& req) {
    try {
        if (req.method() == http::verb::post) {
            auto obj = JsonParser::parseObject(req.body());
            
            auto name = JsonParser::getString(obj, "name");
            auto email = JsonParser::getString(obj, "email");
            auto skills = JsonParser::getStringArray(obj, "skills");
            auto experience = JsonParser::getInt(obj, "experience");
            auto city = JsonParser::getString(obj, "city");
            auto resume = JsonParser::getString(obj, "resume");
            
            if (!name || !email || !experience || !city || !resume) {
                return createErrorResponse(http::status::bad_request, "Missing required fields");
            }
            
            size_t id = jobService_->createWorker(*name, *email, skills, *experience, *city, *resume);
            return createResponse(http::status::created, JsonSerializer::serializeId(id));
        }
        
        return createErrorResponse(http::status::method_not_allowed, "Method not allowed");
    } catch (const std::exception& e) {
        return createErrorResponse(http::status::internal_server_error, e.what());
    }
}

http::response<http::string_body> HttpSession::handleEmployerRoutes(const http::request<http::string_body>& req) {
    try {
        if (req.method() == http::verb::post) {
            auto obj = JsonParser::parseObject(req.body());
            
            auto companyName = JsonParser::getString(obj, "companyName");
            auto email = JsonParser::getString(obj, "email");
            auto description = JsonParser::getString(obj, "description");
            auto city = JsonParser::getString(obj, "city");
            
            if (!companyName || !email || !description || !city) {
                return createErrorResponse(http::status::bad_request, "Missing required fields");
            }
            
            size_t id = jobService_->createEmployer(*companyName, *email, *description, *city);
            return createResponse(http::status::created, JsonSerializer::serializeId(id));
        } else if (req.method() == http::verb::get) {
            auto employers = jobService_->getAllEmployers();
            return createResponse(http::status::ok, JsonSerializer::serializeEmployers(employers));
        }
        
        return createErrorResponse(http::status::method_not_allowed, "Method not allowed");
    } catch (const std::exception& e) {
        return createErrorResponse(http::status::internal_server_error, e.what());
    }
}

http::response<http::string_body> HttpSession::handleJobPostingRoutes(const http::request<http::string_body>& req) {
    try {
        std::string target = std::string(req.target());
        
        if (req.method() == http::verb::post) {
            auto obj = JsonParser::parseObject(req.body());
            
            auto employerId = JsonParser::getSizeT(obj, "employerId");
            auto title = JsonParser::getString(obj, "title");
            auto description = JsonParser::getString(obj, "description");
            auto salary = JsonParser::getInt(obj, "salary");
            auto city = JsonParser::getString(obj, "city");
            auto isRemote = JsonParser::getBool(obj, "isRemote");
            auto profession = JsonParser::getString(obj, "profession");
            
            if (!employerId || !title || !description || !salary || !city || !isRemote || !profession) {
                return createErrorResponse(http::status::bad_request, "Missing required fields");
            }
            
            size_t id = jobService_->createJobPosting(*employerId, *title, *description, *salary, *city, *isRemote, *profession);
            return createResponse(http::status::created, JsonSerializer::serializeId(id));
        } else if (req.method() == http::verb::get) {
            if (target.find("/api/jobs/search") == 0) {
                size_t queryStart = target.find('?');
                JobFilter filter;
                
                if (queryStart != std::string::npos) {
                    std::string query = target.substr(queryStart + 1);
                    std::istringstream iss(query);
                    std::string param;
                    
                    while (std::getline(iss, param, '&')) {
                        size_t eqPos = param.find('=');
                        if (eqPos != std::string::npos) {
                            std::string key = param.substr(0, eqPos);
                            std::string value = param.substr(eqPos + 1);
                            
                            if (key == "minSalary") filter.setMinSalary(std::stoi(value));
                            else if (key == "maxSalary") filter.setMaxSalary(std::stoi(value));
                            else if (key == "city") filter.setCity(value);
                            else if (key == "isRemote") filter.setIsRemote(value == "true");
                            else if (key == "profession") filter.setProfession(value);
                        }
                    }
                }
                
                auto jobs = jobService_->searchJobPostings(filter);
                return createResponse(http::status::ok, JsonSerializer::serializeJobPostings(jobs));
            } else if (target.find("/api/jobs/employer/") == 0) {
                std::string idStr = target.substr(19);
                size_t employerId = std::stoull(idStr);
                auto jobs = jobService_->getJobPostingsByEmployer(employerId);
                return createResponse(http::status::ok, JsonSerializer::serializeJobPostings(jobs));
            }
        } else if (req.method() == http::verb::delete_) {
            if (target.find("/api/jobs/") == 0) {
                std::string idStr = target.substr(10);
                size_t jobId = std::stoull(idStr);
                bool success = jobService_->deleteJobPosting(jobId);
                return createResponse(http::status::ok, JsonSerializer::serializeSuccess(success));
            }
        }
        
        return createErrorResponse(http::status::method_not_allowed, "Method not allowed");
    } catch (const std::exception& e) {
        return createErrorResponse(http::status::internal_server_error, e.what());
    }
}

http::response<http::string_body> HttpSession::handleReplyRoutes(const http::request<http::string_body>& req) {
    try {
        std::string target = std::string(req.target());
        
        if (req.method() == http::verb::post) {
            auto obj = JsonParser::parseObject(req.body());
            
            auto jobPostingId = JsonParser::getSizeT(obj, "jobPostingId");
            auto workerId = JsonParser::getSizeT(obj, "workerId");
            
            if (!jobPostingId || !workerId) {
                return createErrorResponse(http::status::bad_request, "Missing required fields");
            }
            
            size_t id = jobService_->createReply(*jobPostingId, *workerId);
            return createResponse(http::status::created, JsonSerializer::serializeId(id));
        } else if (req.method() == http::verb::get) {
            if (target.find("/api/replies/job/") == 0) {
                std::string idStr = target.substr(17);
                size_t jobPostingId = std::stoull(idStr);
                auto replies = jobService_->getRepliesByJobPosting(jobPostingId);
                return createResponse(http::status::ok, JsonSerializer::serializeReplies(replies));
            }
        }
        
        return createErrorResponse(http::status::method_not_allowed, "Method not allowed");
    } catch (const std::exception& e) {
        return createErrorResponse(http::status::internal_server_error, e.what());
    }
}

HttpListener::HttpListener(net::io_context& ioc, tcp::endpoint endpoint, std::shared_ptr<JobService> jobService)
    : ioc_(ioc)
    , acceptor_(net::make_strand(ioc))
    , jobService_(jobService) {
    beast::error_code ec;
    
    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
        std::cerr << "Error opening acceptor: " << ec.message() << std::endl;
        return;
    }
    
    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
        std::cerr << "Error setting reuse_address: " << ec.message() << std::endl;
        return;
    }
    
    acceptor_.bind(endpoint, ec);
    if (ec) {
        std::cerr << "Error binding: " << ec.message() << std::endl;
        return;
    }
    
    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
        std::cerr << "Error listening: " << ec.message() << std::endl;
        return;
    }
}

void HttpListener::run() {
    doAccept();
}

void HttpListener::doAccept() {
    acceptor_.async_accept(net::make_strand(ioc_),
        beast::bind_front_handler(&HttpListener::onAccept, shared_from_this()));
}

void HttpListener::onAccept(beast::error_code ec, tcp::socket socket) {
    if (ec) {
        std::cerr << "Accept error: " << ec.message() << std::endl;
    } else {
        std::make_shared<HttpSession>(std::move(socket), jobService_)->run();
    }
    
    doAccept();
}

HttpServer::HttpServer(const std::string& address, unsigned short port, int threads)
    : address_(address)
    , port_(port)
    , threads_(threads)
    , ioc_(threads)
    , jobService_(std::make_shared<JobService>()) {
}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::run() {
    auto const address = net::ip::make_address(address_);
    
    std::make_shared<HttpListener>(ioc_, tcp::endpoint{address, port_}, jobService_)->run();
    
    std::cout << "Server running on " << address_ << ":" << port_ << " with " << threads_ << " threads" << std::endl;
    
    threadPool_.reserve(threads_ - 1);
    for (int i = 0; i < threads_ - 1; ++i) {
        threadPool_.emplace_back([this] { ioc_.run(); });
    }
    
    ioc_.run();
}

void HttpServer::stop() {
    ioc_.stop();
    for (auto& t : threadPool_) {
        if (t.joinable()) {
            t.join();
        }
    }
}

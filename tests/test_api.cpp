#define BOOST_TEST_MODULE ApiServerTests
#include <boost/test/included/unit_test.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <thread>
#include <chrono>
#include "api/HttpServer.hpp"
#include "api/JsonParser.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class ApiTestFixture {
public:
    ApiTestFixture() : server("127.0.0.1", 8081, 2) {
        serverThread = std::thread([this]() {
            server.run();
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    ~ApiTestFixture() {
        server.stop();
        if (serverThread.joinable()) {
            serverThread.join();
        }
    }

    std::string sendRequest(http::verb method, const std::string& target, const std::string& body = "") {
        try {
            net::io_context ioc;
            tcp::resolver resolver(ioc);
            beast::tcp_stream stream(ioc);

            auto const results = resolver.resolve("127.0.0.1", "8081");
            stream.connect(results);

            http::request<http::string_body> req{method, target, 11};
            req.set(http::field::host, "127.0.0.1");
            req.set(http::field::user_agent, "ApiTestClient");
            req.set(http::field::content_type, "application/json");
            if (!body.empty()) {
                req.body() = body;
                req.prepare_payload();
            }

            http::write(stream, req);

            beast::flat_buffer buffer;
            http::response<http::string_body> res;
            http::read(stream, buffer, res);

            beast::error_code ec;
            stream.socket().shutdown(tcp::socket::shutdown_both, ec);

            return res.body();
        } catch (const std::exception& e) {
            BOOST_TEST_MESSAGE("Request failed: " << e.what());
            return "";
        }
    }

private:
    HttpServer server;
    std::thread serverThread;
};

BOOST_FIXTURE_TEST_SUITE(ApiServerTestSuite, ApiTestFixture)

BOOST_AUTO_TEST_CASE(test_create_employer) {
    BOOST_TEST_MESSAGE("Testing: Create employer");
    std::string body = R"({"companyName":"TestCorp","email":"test@corp.com","description":"Test company","city":"Moscow"})";
    BOOST_TEST_MESSAGE("Sending POST /api/employers with body: " << body);
    std::string response = sendRequest(http::verb::post, "/api/employers", body);
    BOOST_TEST_MESSAGE("Response: " << response);
    
    BOOST_TEST(!response.empty());
    auto obj = JsonParser::parseObject(response);
    auto id = JsonParser::getSizeT(obj, "id");
    BOOST_TEST(id.has_value());
    BOOST_TEST(id.value() > 0);
    BOOST_TEST_MESSAGE("Created employer with ID: " << id.value());
}

BOOST_AUTO_TEST_CASE(test_create_worker) {
    BOOST_TEST_MESSAGE("Testing: Create worker");
    std::string body = R"({"name":"Ivan Petrov","email":"ivan@test.com","skills":["C++","Python"],"experience":5,"city":"Moscow","resume":"Test resume"})";
    BOOST_TEST_MESSAGE("Sending POST /api/workers");
    std::string response = sendRequest(http::verb::post, "/api/workers", body);
    BOOST_TEST_MESSAGE("Response: " << response);
    
    BOOST_TEST(!response.empty());
    auto obj = JsonParser::parseObject(response);
    auto id = JsonParser::getSizeT(obj, "id");
    BOOST_TEST(id.has_value());
    BOOST_TEST(id.value() > 0);
    BOOST_TEST_MESSAGE("Created worker with ID: " << id.value());
}

BOOST_AUTO_TEST_CASE(test_get_all_employers) {
    BOOST_TEST_MESSAGE("Testing: Get all employers");
    std::string body = R"({"companyName":"Company1","email":"c1@test.com","description":"Desc1","city":"Moscow"})";
    BOOST_TEST_MESSAGE("Creating test employer first");
    sendRequest(http::verb::post, "/api/employers", body);
    
    BOOST_TEST_MESSAGE("Sending GET /api/employers");
    std::string response = sendRequest(http::verb::get, "/api/employers");
    BOOST_TEST_MESSAGE("Response length: " << response.length() << " bytes");
    BOOST_TEST(!response.empty());
    BOOST_TEST(response.find("Company1") != std::string::npos);
    BOOST_TEST_MESSAGE("Successfully retrieved employers list");
}

BOOST_AUTO_TEST_CASE(test_create_job_posting) {
    BOOST_TEST_MESSAGE("Testing: Create job posting");
    std::string employerBody = R"({"companyName":"JobCorp","email":"job@corp.com","description":"Job company","city":"Moscow"})";
    BOOST_TEST_MESSAGE("Creating employer first");
    std::string employerResponse = sendRequest(http::verb::post, "/api/employers", employerBody);
    auto employerObj = JsonParser::parseObject(employerResponse);
    auto employerId = JsonParser::getSizeT(employerObj, "id");
    BOOST_REQUIRE(employerId.has_value());
    BOOST_TEST_MESSAGE("Employer created with ID: " << employerId.value());
    
    std::string jobBody = R"({"employerId":)" + std::to_string(employerId.value()) +
                         R"(,"title":"C++ Developer","description":"Need developer","salary":150000,"city":"Moscow","isRemote":false,"profession":"Software Engineer"})";
    BOOST_TEST_MESSAGE("Sending POST /api/jobs");
    std::string jobResponse = sendRequest(http::verb::post, "/api/jobs", jobBody);
    BOOST_TEST_MESSAGE("Response: " << jobResponse);
    
    BOOST_TEST(!jobResponse.empty());
    auto jobObj = JsonParser::parseObject(jobResponse);
    auto jobId = JsonParser::getSizeT(jobObj, "id");
    BOOST_TEST(jobId.has_value());
    BOOST_TEST(jobId.value() > 0);
    BOOST_TEST_MESSAGE("Created job posting with ID: " << jobId.value());
}

BOOST_AUTO_TEST_CASE(test_search_jobs) {
    BOOST_TEST_MESSAGE("Testing: Search jobs");
    std::string employerBody = R"({"companyName":"SearchCorp","email":"search@corp.com","description":"Search company","city":"Moscow"})";
    BOOST_TEST_MESSAGE("Creating employer and job posting");
    std::string employerResponse = sendRequest(http::verb::post, "/api/employers", employerBody);
    auto employerObj = JsonParser::parseObject(employerResponse);
    auto employerId = JsonParser::getSizeT(employerObj, "id");
    BOOST_REQUIRE(employerId.has_value());
    
    std::string jobBody = R"({"employerId":)" + std::to_string(employerId.value()) +
                         R"(,"title":"Python Developer","description":"Python job","salary":120000,"city":"Moscow","isRemote":true,"profession":"Backend Developer"})";
    sendRequest(http::verb::post, "/api/jobs", jobBody);
    
    BOOST_TEST_MESSAGE("Sending GET /api/jobs/search");
    std::string searchResponse = sendRequest(http::verb::get, "/api/jobs/search");
    BOOST_TEST(!searchResponse.empty());
    BOOST_TEST(searchResponse.find("Python Developer") != std::string::npos);
    BOOST_TEST_MESSAGE("Successfully found job in search results");
}

BOOST_AUTO_TEST_CASE(test_search_jobs_with_filters) {
    BOOST_TEST_MESSAGE("Testing: Search jobs with filters");
    std::string employerBody = R"({"companyName":"FilterCorp","email":"filter@corp.com","description":"Filter company","city":"SPB"})";
    BOOST_TEST_MESSAGE("Creating employer and job posting in SPB");
    std::string employerResponse = sendRequest(http::verb::post, "/api/employers", employerBody);
    auto employerObj = JsonParser::parseObject(employerResponse);
    auto employerId = JsonParser::getSizeT(employerObj, "id");
    BOOST_REQUIRE(employerId.has_value());
    
    std::string jobBody = R"({"employerId":)" + std::to_string(employerId.value()) +
                         R"(,"title":"Java Developer","description":"Java job","salary":180000,"city":"SPB","isRemote":false,"profession":"Java Developer"})";
    sendRequest(http::verb::post, "/api/jobs", jobBody);
    
    BOOST_TEST_MESSAGE("Sending GET /api/jobs/search?city=SPB&minSalary=170000");
    std::string searchResponse = sendRequest(http::verb::get, "/api/jobs/search?city=SPB&minSalary=170000");
    BOOST_TEST(!searchResponse.empty());
    BOOST_TEST(searchResponse.find("Java Developer") != std::string::npos);
    BOOST_TEST_MESSAGE("Successfully filtered jobs by city and salary");
}

BOOST_AUTO_TEST_CASE(test_create_reply) {
    BOOST_TEST_MESSAGE("Testing: Create reply");
    std::string employerBody = R"({"companyName":"ReplyCorp","email":"reply@corp.com","description":"Reply company","city":"Moscow"})";
    BOOST_TEST_MESSAGE("Creating employer, worker, and job posting");
    std::string employerResponse = sendRequest(http::verb::post, "/api/employers", employerBody);
    auto employerObj = JsonParser::parseObject(employerResponse);
    auto employerId = JsonParser::getSizeT(employerObj, "id");
    BOOST_REQUIRE(employerId.has_value());
    
    std::string workerBody = R"({"name":"Test Worker","email":"worker@test.com","skills":["C++"],"experience":3,"city":"Moscow","resume":"Resume"})";
    std::string workerResponse = sendRequest(http::verb::post, "/api/workers", workerBody);
    auto workerObj = JsonParser::parseObject(workerResponse);
    auto workerId = JsonParser::getSizeT(workerObj, "id");
    BOOST_REQUIRE(workerId.has_value());
    
    std::string jobBody = R"({"employerId":)" + std::to_string(employerId.value()) +
                         R"(,"title":"Test Job","description":"Test","salary":100000,"city":"Moscow","isRemote":false,"profession":"Developer"})";
    std::string jobResponse = sendRequest(http::verb::post, "/api/jobs", jobBody);
    auto jobObj = JsonParser::parseObject(jobResponse);
    auto jobId = JsonParser::getSizeT(jobObj, "id");
    BOOST_REQUIRE(jobId.has_value());
    
    BOOST_TEST_MESSAGE("Sending POST /api/replies");
    std::string replyBody = R"({"jobPostingId":)" + std::to_string(jobId.value()) +
                           R"(,"workerId":)" + std::to_string(workerId.value()) + R"(})";
    std::string replyResponse = sendRequest(http::verb::post, "/api/replies", replyBody);
    BOOST_TEST_MESSAGE("Response: " << replyResponse);
    
    BOOST_TEST(!replyResponse.empty());
    auto replyObj = JsonParser::parseObject(replyResponse);
    auto replyId = JsonParser::getSizeT(replyObj, "id");
    BOOST_TEST(replyId.has_value());
    BOOST_TEST(replyId.value() > 0);
    BOOST_TEST_MESSAGE("Created reply with ID: " << replyId.value());
}

BOOST_AUTO_TEST_CASE(test_get_replies_for_job) {
    BOOST_TEST_MESSAGE("Testing: Get replies for job");
    std::string employerBody = R"({"companyName":"GetReplyCorp","email":"getrep@corp.com","description":"Get reply company","city":"Moscow"})";
    BOOST_TEST_MESSAGE("Creating employer, worker, job, and reply");
    std::string employerResponse = sendRequest(http::verb::post, "/api/employers", employerBody);
    auto employerObj = JsonParser::parseObject(employerResponse);
    auto employerId = JsonParser::getSizeT(employerObj, "id");
    BOOST_REQUIRE(employerId.has_value());
    
    std::string workerBody = R"({"name":"Reply Worker","email":"repworker@test.com","skills":["Java"],"experience":2,"city":"Moscow","resume":"Resume"})";
    std::string workerResponse = sendRequest(http::verb::post, "/api/workers", workerBody);
    auto workerObj = JsonParser::parseObject(workerResponse);
    auto workerId = JsonParser::getSizeT(workerObj, "id");
    BOOST_REQUIRE(workerId.has_value());
    
    std::string jobBody = R"({"employerId":)" + std::to_string(employerId.value()) +
                         R"(,"title":"Reply Job","description":"Test","salary":100000,"city":"Moscow","isRemote":false,"profession":"Developer"})";
    std::string jobResponse = sendRequest(http::verb::post, "/api/jobs", jobBody);
    auto jobObj = JsonParser::parseObject(jobResponse);
    auto jobId = JsonParser::getSizeT(jobObj, "id");
    BOOST_REQUIRE(jobId.has_value());
    
    std::string replyBody = R"({"jobPostingId":)" + std::to_string(jobId.value()) +
                           R"(,"workerId":)" + std::to_string(workerId.value()) + R"(})";
    sendRequest(http::verb::post, "/api/replies", replyBody);
    
    BOOST_TEST_MESSAGE("Sending GET /api/replies/job/" << jobId.value());
    std::string getRepliesResponse = sendRequest(http::verb::get, "/api/replies/job/" + std::to_string(jobId.value()));
    BOOST_TEST(!getRepliesResponse.empty());
    BOOST_TEST(getRepliesResponse.find(std::to_string(workerId.value())) != std::string::npos);
    BOOST_TEST_MESSAGE("Successfully retrieved replies for job");
}

BOOST_AUTO_TEST_CASE(test_delete_job_posting) {
    BOOST_TEST_MESSAGE("Testing: Delete job posting");
    std::string employerBody = R"({"companyName":"DeleteCorp","email":"delete@corp.com","description":"Delete company","city":"Moscow"})";
    BOOST_TEST_MESSAGE("Creating employer and job posting");
    std::string employerResponse = sendRequest(http::verb::post, "/api/employers", employerBody);
    auto employerObj = JsonParser::parseObject(employerResponse);
    auto employerId = JsonParser::getSizeT(employerObj, "id");
    BOOST_REQUIRE(employerId.has_value());
    
    std::string jobBody = R"({"employerId":)" + std::to_string(employerId.value()) +
                         R"(,"title":"Delete Job","description":"To delete","salary":100000,"city":"Moscow","isRemote":false,"profession":"Developer"})";
    std::string jobResponse = sendRequest(http::verb::post, "/api/jobs", jobBody);
    auto jobObj = JsonParser::parseObject(jobResponse);
    auto jobId = JsonParser::getSizeT(jobObj, "id");
    BOOST_REQUIRE(jobId.has_value());
    BOOST_TEST_MESSAGE("Created job with ID: " << jobId.value());
    
    BOOST_TEST_MESSAGE("Sending DELETE /api/jobs/" << jobId.value());
    std::string deleteResponse = sendRequest(http::verb::delete_, "/api/jobs/" + std::to_string(jobId.value()));
    BOOST_TEST_MESSAGE("Response: " << deleteResponse);
    BOOST_TEST(!deleteResponse.empty());
    auto deleteObj = JsonParser::parseObject(deleteResponse);
    auto success = JsonParser::getBool(deleteObj, "success");
    BOOST_TEST(success.has_value());
    BOOST_TEST(success.value() == true);
    BOOST_TEST_MESSAGE("Successfully deleted job posting");
}

BOOST_AUTO_TEST_CASE(test_concurrent_requests) {
    BOOST_TEST_MESSAGE("Testing: Concurrent requests (10 threads)");
    std::vector<std::thread> threads;
    std::atomic<int> successCount{0};
    
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([this, i, &successCount]() {
            std::string body = R"({"companyName":"Concurrent)" + std::to_string(i) +
                             R"(","email":"conc)" + std::to_string(i) +
                             R"(@test.com","description":"Concurrent test","city":"Moscow"})";
            std::string response = sendRequest(http::verb::post, "/api/employers", body);
            if (!response.empty()) {
                auto obj = JsonParser::parseObject(response);
                auto id = JsonParser::getSizeT(obj, "id");
                if (id.has_value() && id.value() > 0) {
                    successCount++;
                }
            }
        });
    }
    
    BOOST_TEST_MESSAGE("Waiting for all threads to complete...");
    for (auto& t : threads) {
        t.join();
    }
    
    BOOST_TEST(successCount.load() == 10);
    BOOST_TEST_MESSAGE("All 10 concurrent requests completed successfully");
}

BOOST_AUTO_TEST_CASE(test_invalid_request) {
    BOOST_TEST_MESSAGE("Testing: Invalid request handling");
    std::string body = R"({"invalid":"data"})";
    BOOST_TEST_MESSAGE("Sending POST /api/workers with invalid data");
    std::string response = sendRequest(http::verb::post, "/api/workers", body);
    BOOST_TEST_MESSAGE("Response: " << response);
    
    BOOST_TEST(!response.empty());
    BOOST_TEST(response.find("error") != std::string::npos);
    BOOST_TEST_MESSAGE("Server correctly returned error for invalid request");
}

BOOST_AUTO_TEST_CASE(test_not_found_endpoint) {
    BOOST_TEST_MESSAGE("Testing: Not found endpoint");
    BOOST_TEST_MESSAGE("Sending GET /api/nonexistent");
    std::string response = sendRequest(http::verb::get, "/api/nonexistent");
    BOOST_TEST_MESSAGE("Response: " << response);
    
    BOOST_TEST(!response.empty());
    BOOST_TEST(response.find("error") != std::string::npos);
    BOOST_TEST_MESSAGE("Server correctly returned 404 error");
}

BOOST_AUTO_TEST_SUITE_END()

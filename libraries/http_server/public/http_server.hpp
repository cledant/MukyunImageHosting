#ifndef TEST_HTTP_SERVER_HTTP_SERVER_HPP
#define TEST_HTTP_SERVER_HTTP_SERVER_HPP

#include <cstdint>
#include <string>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <pistache/net.h>

#include "nlohmann/json.hpp"

#include "file_saving.hpp"

namespace http_server {

struct server_config
{
    server_config();
    virtual ~server_config() = default;
    server_config(server_config const &src) = default;
    server_config &operator=(server_config const &rhs) = default;
    server_config(server_config &&src) = default;
    server_config &operator=(server_config &&rhs) = default;

    uint32_t nb_threads;
    uint64_t request_max_size;
    Pistache::Address addr;
    std::string api_key;
    std::string save_folder;
    std::string url_address;

    static constexpr uint32_t DEFAULT_THREAD_NB = 4;
    static constexpr uint64_t DEFAULT_REQUEST_SIZE = 1024 * 1024 * 10;
    static constexpr char const *DEFAULT_SERVER_ADDRESS = "127.0.0.10";
    static constexpr uint16_t DEFAULT_PORT = 4242;
    static constexpr char const *DEFAULT_API_KEY = "Patchouli";
    static constexpr char const *DEFAULT_SAVE_FOLDER = ".";
    static constexpr char const *DEFAULT_URL_ADDRESS = "http://127.0.0.0.10";

    static constexpr char const *JSON_NB_THREAD = "nb_thread";
    static constexpr char const *JSON_REQUEST_SIZE = "request_size";
    static constexpr char const *JSON_SERVER_ADDRESS = "server_address";
    static constexpr char const *JSON_SERVER_PORT = "server_port";
    static constexpr char const *JSON_API_KEY = "api_key";
    static constexpr char const *JSON_SAVE_FOLDER = "save_folder";
    static constexpr char const *JSON_URL_ADDRESS = "url_address";
};

void to_json(nlohmann::json &json, server_config const &cfg);
void from_json(nlohmann::json const &json, server_config &cfg);

class server_endpoint
{
  public:
    server_endpoint() = default;
    explicit server_endpoint(server_config &&config);
    virtual ~server_endpoint() = default;
    server_endpoint(server_endpoint const &src) = delete;
    server_endpoint &operator=(server_endpoint const &rhs) = delete;
    server_endpoint(server_endpoint &&src) = delete;
    server_endpoint &operator=(server_endpoint &&rhs) = delete;

    void start();

  private:
    server_config _config;
    std::shared_ptr<Pistache::Http::Endpoint> _endpoint;
    Pistache::Rest::Router _router;
    file_manipulation::file_saving _fs;

    void _set_routes();
    void _health_check(Pistache::Rest::Request const &request,
                       Pistache::Http::ResponseWriter response);
    void _save_image(Pistache::Rest::Request const &request,
                     Pistache::Http::ResponseWriter response);

    static constexpr char const *API_HEADER_NAME = "x-mukyun-ih";
    static constexpr uint32_t NB_SUPPORTED_TYPE = 4;
};

} // namespace http_server

#endif // TEST_HTTP_SERVER_HTTP_SERVER_HPP

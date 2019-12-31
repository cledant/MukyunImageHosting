#include <iostream>

#include "http_server.hpp"

using namespace Pistache;

namespace http_server {

server_config::server_config()
  : nb_threads(DEFAULT_THREAD_NB)
  , request_max_size(DEFAULT_REQUEST_SIZE)
  , api_key(DEFAULT_API_KEY)
  , save_folder(DEFAULT_SAVE_FOLDER)
  , url_address(DEFAULT_URL_ADDRESS)
{
    addr = Pistache::Address(DEFAULT_SERVER_ADDRESS, DEFAULT_PORT);
}

void
to_json(nlohmann::json &json, server_config const &cfg)
{
    json =
      nlohmann::json{ { server_config::JSON_NB_THREAD, cfg.nb_threads },
                      { server_config::JSON_REQUEST_SIZE,
                        cfg.request_max_size },
                      { server_config::JSON_SERVER_ADDRESS, cfg.addr.host() },
                      { server_config::JSON_SERVER_PORT,
                        std::stoi(cfg.addr.port().toString()) },
                      { server_config::JSON_API_KEY, cfg.api_key },
                      { server_config::JSON_SAVE_FOLDER, cfg.save_folder },
                      { server_config::JSON_URL_ADDRESS, cfg.url_address } };
}

void
from_json(nlohmann::json const &json, server_config &cfg)
{
    uint16_t port = 0;
    std::string srv;

    try {
        json.at(server_config::JSON_NB_THREAD).get_to(cfg.nb_threads);
        json.at(server_config::JSON_REQUEST_SIZE).get_to(cfg.request_max_size);
        json.at(server_config::JSON_SERVER_ADDRESS).get_to(srv);
        json.at(server_config::JSON_SERVER_PORT).get_to(port);
        json.at(server_config::JSON_API_KEY).get_to(cfg.api_key);
        json.at(server_config::JSON_SAVE_FOLDER).get_to(cfg.save_folder);
        json.at(server_config::JSON_URL_ADDRESS).get_to(cfg.url_address);
        cfg.addr = Pistache::Address(srv, port);
    } catch (std::exception const &e) {
        std::cout << "Invalid server config file: " << e.what() << std::endl;
        std::cout << "Using default settings" << std::endl;
        cfg.nb_threads = server_config::DEFAULT_THREAD_NB;
        cfg.request_max_size = server_config::DEFAULT_REQUEST_SIZE;
        cfg.api_key = server_config::DEFAULT_API_KEY;
        cfg.addr = Pistache::Address(server_config::DEFAULT_SERVER_ADDRESS,
                                     server_config::DEFAULT_PORT);
        cfg.save_folder = server_config::DEFAULT_SAVE_FOLDER;
        cfg.url_address = server_config::DEFAULT_URL_ADDRESS;
    }
}

server_endpoint::server_endpoint(server_config &&config)
  : _config(config)
{}

void
server_endpoint::start()
{
    _endpoint = std::make_shared<Http::Endpoint>(_config.addr);
    auto opts = Http::Endpoint::options()
                  .threads(_config.nb_threads)
                  .maxRequestSize(_config.request_max_size);
    _endpoint->init(opts);
    _set_routes();
    _endpoint->setHandler(_router.handler());
    std::cout << "Starting server on : " << _config.addr.host() << ":"
              << _config.addr.port().toString() << std::endl;
    std::cout << "Saving files in folder: " << _config.save_folder << ":"
              << std::endl;
    _endpoint->serve();
}

void
server_endpoint::_set_routes()
{
    Rest::Routes::Get(
      _router,
      "/healthcheck",
      Rest::Routes::bind(&server_endpoint::_health_check, this));
    Rest::Routes::Post(_router,
                       "/saveimage",
                       Rest::Routes::bind(&server_endpoint::_save_image, this));
}

void
server_endpoint::_health_check(Pistache::Rest::Request const &request,
                               Pistache::Http::ResponseWriter response)
{
    // Add things to check
    static_cast<void>(request);
    response.send(Http::Code::Ok, "");
}

void
server_endpoint::_save_image(Pistache::Rest::Request const &request,
                             Pistache::Http::ResponseWriter response)
{
    // Checking API token
    auto api = request.headers().getRaw(API_HEADER_NAME);
    if (api.value() != _config.api_key) {
        response.send(Http::Code::Forbidden, "");
        return;
    }

    // Checking img type
    static char const type_array[NB_SUPPORTED_TYPE][16] = {
        ".jpg", ".png", ".webp", ".gif"
    };
    static char const valid_mime_array[NB_SUPPORTED_TYPE][32] = {
        "image/jpeg",
        "image/png",
        "image/webp",
        "image/gif",
    };
    auto ct = request.headers().get<Http::Header::ContentType>();
    auto mime = ct->mime().toString();
    for (uint32_t i = 0; i < NB_SUPPORTED_TYPE; ++i) {
        if (mime == valid_mime_array[i]) {
            // Checking body content
            if (request.body().empty()) {
                response.send(Http::Code::Ok, "Empty file are not saved\n");
                return;
            }
            // Saving file
            std::string filename =
              _fs.save_file(request.body(), _config.save_folder, type_array[i]);
            if (!filename.empty()) {
                std::string url = _config.url_address + "/" + filename + "\n";
                response.send(Http::Code::Ok, url);
                return;
            }
            response.send(Http::Code::Ok, "Failed to save file\n");
            return;
        }
    }
    response.send(Http::Code::Unsupported_Media_Type, "");
}

}

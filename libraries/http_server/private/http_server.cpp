#include <iostream>

#include "http_server.hpp"

using namespace Pistache;

namespace http_server {

server_config::server_config()
  : nb_threads(DEFAULT_THREAD_NB)
  , request_max_size(DEFAULT_REQUEST_SIZE)
  , api_key(DEFAULT_API_KEY)
{
    addr = Pistache::Address(DEFAULT_ADDRESS, DEFAULT_PORT);
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
    if (api.value().compare(_config.api_key.c_str())) {
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
        if (!mime.compare(valid_mime_array[i])) {
            auto link = _fs.save_file(request.body(), type_array[i]);
            response.send(Http::Code::Ok, link);
            return;
        }
    }
    response.send(Http::Code::Unsupported_Media_Type, "");
}
}

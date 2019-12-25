#include <iostream>

#include "http_server.hpp"

using namespace Pistache;

namespace http_server {

server_config::server_config()
  : nb_threads(DEFAULT_THREAD_NB)
  , request_max_size(DEFAULT_REQUEST_SIZE)
{
    addr = Pistache::Address(DEFAULT_ADDRESS, DEFAULT_PORT);
}

server_endpoint::server_endpoint(server_config const &config)
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
                       "/savefile",
                       Rest::Routes::bind(&server_endpoint::_save_file, this));
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
server_endpoint::_save_file(Pistache::Rest::Request const &request,
                            Pistache::Http::ResponseWriter response)
{
    // TODO
    static_cast<void>(request);
    response.send(Http::Code::Ok, "");
}
}

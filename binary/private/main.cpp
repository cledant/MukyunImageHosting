#include "http_server.hpp"

int
main()
{
    http_server::server_endpoint srv;

    srv.start();
    return (0);
}
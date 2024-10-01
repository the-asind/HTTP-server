#include "HTTPServer.hpp"

int main()
{
    HttpServer::UPtr server = std::make_unique<HttpServer>();

    int port = 7777;
    std::cout << "Server started at port: " << port << std::endl;
    server->Start(port);

    return 0;
}
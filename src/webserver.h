#pragma once
#include <boost/asio.hpp>

namespace Webs { namespace  WebServer {


class Server
{
public:

    Server( const Server& ) = delete;

    Server& operator=( const Server& ) = delete;

    Server( const std::string &address, const std::string &port );

    void run();
private:
    void accept();

    void stopWait();

    boost::asio::io_service ioservice;

    boost::asio::signal_set signalset;

    boost::asio::ip::tcp::acceptor acceptor;

    ConnectionManager connectionMgr;

    boost::asio::ip::tcp::socket socket;

    RequestHandler reqHandler;

};

}

}

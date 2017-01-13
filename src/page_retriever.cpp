#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <ostream>
#include <istream>
#include <iostream>

#include <boost/asio.hpp>

#include "page_retriever.h"

namespace Webs {

PageRetriever::PageRetriever( std::string &url )
    : m_url( url )
{}

PageRetriever::~PageRetriever()
{}


std::string PageRetriever::url() const {
    return m_url;
}

void PageRetriever::setUrl(const std::string &url)
{
    m_url = url;
}


ErrorType PageRetriever::pageContent( std::string &content ) {
    try {
        std::string::size_type pos =  m_url.find_first_of("/");
        std::string mainDomian;
        std::string page;
        if(  pos != std::string::npos ) {
            mainDomian = m_url.substr( 0, pos );
            page = m_url.substr( pos, m_url.length() - pos );
        } else {
            mainDomian = m_url;
            page = "/";
        }
        boost::asio::io_service ioService;
        boost::asio::ip::tcp::resolver urlResolver( ioService );
        boost::asio::ip::tcp::resolver::query urlQuery( mainDomian, "http" );
        boost::asio::ip::tcp::resolver::iterator iter
                = urlResolver.resolve( urlQuery );
        boost::asio::ip::tcp::resolver::iterator end;
        boost::asio::ip::tcp::endpoint endPoint;
        boost::asio::ip::tcp::socket connSocket( ioService );
        boost::system::error_code error
                = boost::asio::error::host_not_found;

        while( error && iter != end ) {
            connSocket.close();
            endPoint = *iter++;
            connSocket.connect( endPoint, error );
            //std::cout << endPoint << std::endl;
        }
        if( error )
            throw boost::system::system_error( error );
        //Till now socket is connected
        boost::asio::streambuf request;
        std::ostream req_stream( &request );


        req_stream << "GET " << page << " HTTP/1.0\r\n";
        req_stream << "Host: " << mainDomian << "\r\n";
        req_stream << "Accept: */*\r\n";
        req_stream << "Connection: close\r\n\r\n";
        boost::asio::write( connSocket, request );
        boost::asio::streambuf response;
        boost::asio::read_until( connSocket, response, "\r\n" );

        std::istream resp_stream( &response );
        std::string http_ver;
        resp_stream >> http_ver;
        unsigned int status_code;
        resp_stream >> status_code;
        std::string status_msg;
        std::getline( resp_stream, status_msg );
        if( !resp_stream || http_ver.substr( 0, 5 ) != "HTTP/" ) {
            std::cerr << "Invalid response\n";
            return 1;
        }
        if( status_code != 200 ) {
            std::cerr << "Response returned with status code"
                      << status_code << "\n";
            return 1;
        }

        boost::asio::read_until( connSocket, response, "\r\n\r\n" );
        std::string header;
        while( getline( resp_stream, header ) && header != "\r" ) {
            //std::cerr << header << "\n";
        }

        std::cerr << "\n";
        std::cerr << "Writing content data\n";
        if( response.size() > 0 ) {
            //std::cout << &response;
            content.clear();
            std::istream final( &response );
            std::string temp;
            while ( std::getline( final, temp ))
                content += temp;
        }

        while( true ) {
            std::size_t n = boost::asio::read( connSocket,
                                            response,
                                            boost::asio::transfer_at_least( 1 ),
                                            error );
            if( !error ) {
                if( n ) {
                    std::istream final( &response );
                    std::string temp;
                    while ( std::getline( final, temp ))
                        content += temp;
                }
            }
            if( error == boost::asio::error::eof )
                break;
            if( error )
                throw boost::system::system_error( error );
        }
    }

    catch( std::exception &excep ) {
        std::cerr << "Exception: " << excep.what() << "\n";
        return 1;
    }
    return 0;
}


}




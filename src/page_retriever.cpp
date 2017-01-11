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

//#if !defined(BOOST_ASIO_NO_IOSTREAM)
//        /// The UNIX domain iostream type.
//        typedef boost::asio::basic_socket_iostream<seqpacket_protocol> iostream;
//#endif // !defined(BOOST_ASIO_NO_IOSTREAM)

PageRetriever::PageRetriever( std::string &url )
    : m_url( url )
{}

PageRetriever::~PageRetriever()
{}


std::string PageRetriever::url() const {
    return m_url;
}


ErrorType PageRetriever::pageContent( ) {
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
        while( getline( resp_stream, header ) && header != "\r" )
            std::cerr << header << "\n";

        std::cerr << "\n";
        std::cerr << "Writing content data\n";

        if( response.size() > 0 ) {
            std::cout << &response;
        }

        while( true ) {
            std::size_t n = boost::asio::read( connSocket,
                                            response,
                                            boost::asio::transfer_at_least( 1 ),
                                            error );
            if( !error ) {
                if( n )
                    std::cout << &response;
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

/*
bool PageParser::readContent( std::string &fileContent , std::string &filename )
{
    bool result = false;
    std::ifstream reader( filename );
    if( reader.good() ) {
        reader.seekg( 0, std::ios::end );
        fileContent.resize( static_cast<std::size_t>( reader.tellg() ) );
        reader.seekg( 0, std::ios::beg );
        reader.read( &fileContent[0], fileContent.size() );
        result = ! reader.fail();
        reader.close();
    }
    return result;
}

bool PageParser::writeContent( std::string &content,
                               std::string &filename )
{
    bool result = false;
    std::ofstream writer( filename );
    if( writer.is_open() ) {
        writer.write( content.c_str(), content.size() );
    }
    writer.close();
    return result;
}


bool IndexManager::populateStopWords( std::string &stopWordsListFilename )
{
    bool result = false;
    std::string stopWord;
    std::ifstream reader( stopWordsListFilename );
    if( reader.good() ) {
        while( getline( reader, stopWord) ) {
                m_stopWords.insert(
                        std::pair< std::string, std::string >(
                                    stopWord, stopWord ) );
        }
    }
    return result;
}

bool IndexManager::generateIndices(std::string &data)
{
    bool result = false;
    unsigned int startIndex = 0x00;
    bool startTagDetected = false;
    bool endTagDetected = false;
    bool wordStartDetected = false;
    bool wordEndDetected = false;
    bool spaceDetected = false;
    std::vector< unsigned int > positions;
    std::map< std::string, std::vector<int> > dict;
    std::stringstream word;
    std::size_t dataLength = data.length();
    unsigned int position = 0x00;

    //Find the start of the <body> tag in the html
    if( std::string::npos == ( startIndex = data.find( "<body>") ) ) {
        return result;
    }

    //Start parsing the html and add the words to the dictionary,
    //if the word is not present in the stop words list
    for( unsigned int i = startIndex; i < dataLength; ++i ) {
        if( data[ i ] == '>' && data[ i+1 ] == '<' ) {
            continue;
        } else if( '>' == data[ i ] && data[ i+1 ] != '<' ) {
            startTagDetected = true;
            endTagDetected   = false;
            continue;
        } else if( '<' == data[ i ] ) {
            startTagDetected = false;
            endTagDetected = true;
            continue;
        }
        if( startTagDetected ) {
            if( ( '>' == data[ i - 1 ] ) || ( ' ' == data[ i - 1 ] ) ) {
                wordStartDetected = true;
                position = i;
            }
            if( wordStartDetected ) {
                if( ' ' != data[ i ] ) {
                    word << data[ i ];
                } else if( ' ' == data[ i ]  ) {
                    spaceDetected = true;
                    wordEndDetected = true;
                }
                if( wordEndDetected ) {

                }
            }


        }

    }
}

IndexManager::IndexManager( std::string &inputFile )
            : m_parser( inputFile )
{

}

*/
}




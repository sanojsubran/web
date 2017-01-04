#include <boost/asio.hpp>

#include "page_retriever.h"

namespace Webs {


PageRetriever::PageRetriever()
{

}


PageRetriever::~PageRetriever()
{

}


std::string PageRetriever::url() const {
    return m_url;
}


bool PageRetriever::pageContent(std::stringstream ss) {
    ss.str();
    //TO DO: Add implementation
    return true;
}


bool PageParser::readContent( std::string &fileContent )
{
    bool result = false;
    std::ifstream reader( m_filename );
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


}

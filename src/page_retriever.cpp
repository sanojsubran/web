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


}


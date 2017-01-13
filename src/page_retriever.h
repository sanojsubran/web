#pragma once

#include <map>
#include <vector>
#include <string>

namespace Webs {

typedef unsigned int ErrorType;

class PageRetriever
{
public:
    PageRetriever() = delete;

    PageRetriever( const PageRetriever& ) = delete;

    PageRetriever& operator=( const PageRetriever& ) = delete;

    PageRetriever( std::string &url );

    ~PageRetriever();

    std::string url() const;

    ErrorType pageContent( std::string &content );

private:
    std::string m_url;

    bool m_queryStatus;

};

}

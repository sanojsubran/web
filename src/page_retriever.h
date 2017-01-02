#pragma once
#include <string>
#include <sstream>
#include <fstream>

namespace Webs {

class PageRetriever
{
public:
    PageRetriever();

    ~PageRetriever();

    std::string url() const;

    bool pageContent( std::stringstream ss );

private:
    std::string m_url;

    bool m_queryStatus;

};


class PageParser
{
public:
    PageParser( std::string &filename ) : m_filename( filename ) {

    }

    ~PageParser() {}

    bool isFileGood();

    std::stringstream readContent();

private:
    std::string m_filename;

    std::ifstream m_reader;
};


}

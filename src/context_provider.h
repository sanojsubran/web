#pragma once
#include <iostream>
#include <vector>

namespace Webs {

typedef std::vector< std::string > StringVec;

class ContextProvider
{
public:
    ContextProvider( std::string &websiteContent );

    ~ContextProvider();

    std::string getWordContext( std::string &word, std::size_t pos );

    StringVec getAllOccurrences( std::string &word );

private:
    std::string m_websiteContent;
};

}

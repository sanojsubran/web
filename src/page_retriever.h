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

    ErrorType pageContent( std::string content );

private:
    std::string m_url;

    bool m_queryStatus;

};
/*

struct PageParser
{
public:

    bool isFileGood();

    bool readContent( std::string &filContent, std::string &filename );

    bool writeContent( std::string &content, std::string &filename );

};


class IndexManager
{
public:
    IndexManager( std::string &inputFile );

    ~IndexManager() {}

    bool populateStopWords( std::string &stopWordsListFilename );

    bool generateIndices( std::string &fileContent );

private:
    PageParser m_parser;

    std::map< std::string, std::string > m_stopWords;

    std::map< std::string, std::vector< int > > m_wordDictionary;
};
*/
}

#pragma once

#include <set>

#include "page_retriever.h"

namespace Webs {
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
    IndexManager( std::string &url );

    ~IndexManager() {}

    bool populateStopWords( std::string &stopWordsListFilename );

    bool generateIndices( std::string &fileContent );

    bool getWordIndices( std::string &word,
                         std::vector< int> &wordPositions );

private:
    PageRetriever m_pageRetriever;

    std::set< std::string > m_stopWords;

    std::map< std::string, std::vector< int > > m_wordDictionary;
};

}

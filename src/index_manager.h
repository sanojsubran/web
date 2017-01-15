#pragma once

#include <set>
#include <memory>

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
private:
    IndexManager( std::string &url );

    IndexManager() = delete;
public:
    ~IndexManager() {}

    bool populateStopWords( std::string &stopWordsListFilename );

    bool generateIndices();

    bool getWordIndices( std::string &word,
                         std::vector< int> &wordPositions );

    void setUrl( const std::string &url );

    std::string htmlData();

    std::set< std::string > stopWordList();

    std::map< std::string, std::vector< int > > wordDictionary();

    static IndexManager* instance();

    static void createInstance( std::string &url );

private:
    static IndexManager *m_instance;

    std::string m_htmlData;

    PageRetriever m_pageRetriever;

    std::set< std::string > m_stopWords;

    std::map< std::string, std::vector< int > > m_wordDictionary;
};

#define INDEX_MGR() Webs::IndexManager::instance()

}

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
    //typedef std::shared_ptr< IndexManager > Ptr;

    ~IndexManager() {}

    bool populateStopWords( std::string &stopWordsListFilename );

    bool generateIndices( /*std::string &fileContent*/ );

    bool getWordIndices( std::string &word,
                         std::vector< int> &wordPositions );

    void setUrl( const std::string &url );

    std::set< std::string > stopWordList();

    std::map< std::string, std::vector< int > > wordDictionary();

    static IndexManager* instance();

    static void createInstance( std::string &url );

private:
    static IndexManager *m_instance;

    PageRetriever m_pageRetriever;

    std::set< std::string > m_stopWords;

    std::map< std::string, std::vector< int > > m_wordDictionary;
};

#define INDEX_MGR() Webs::IndexManager::instance()

}

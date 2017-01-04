#pragma once

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

    bool readContent( std::string &filContent, std::string &filename );

    bool writeContent( std::string &content, std::string &filename );

    std::string& filename();



private:
    std::string m_filename;

};


class IndexManager
{
public:
    IndexManager( std::string &inputFile );

    ~IndexManager() {}

    bool populateStopWords( std::string &stopWordsListFilename );

    void generateIndices( std::string &fileContent );

private:
    PageParser m_parser;

    std::map< std::string, std::string > m_stopWords;

    std::map< std::string, std::vector< int > > m_wordDictionary;
};

}

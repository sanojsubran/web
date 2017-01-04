#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

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


bool PageParser::readContent( std::string &fileContent , std::string &filename )
{
    bool result = false;
    std::ifstream reader( filename );
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

bool PageParser::writeContent( std::string &content,
                               std::string &filename )
{
    bool result = false;
    std::ofstream writer( filename );
    if( writer.is_open() ) {
        writer.write( content.c_str(), content.size() );
    }
    writer.close();
    return result;
}

std::string &PageParser::filename()
{
    return m_filename;
}

bool IndexManager::populateStopWords( std::string &stopWordsListFilename )
{
    bool result = false;
    std::string stopWord;
    std::ifstream reader( stopWordsListFilename );
    if( reader.good() ) {
        while( getline( reader, stopWord) ) {
            result =  m_stopWords.insert(
                        std::pair< std::string, std::string >(
                                    stopWord, stopWord ) );
        }
    }
    return result;
}

IndexManager::IndexManager( std::string &inputFile )
{
//    std::map< std::string, std::string > words{
//        std::pair< std::string, std::string >( "the", "the"),
//        std::pair< std::string, std::string >( "this", "this"),
//        std::pair< std::string, std::string >( "then", "then"),
//        std::pair< std::string, std::string >( "that", "that"),
//        std::pair< std::string, std::string >( "these", "these"),
//        std::pair< std::string, std::string >( "there", "there") };
    m_stopWords.insert( words.begin(), words.end() );
}


}



//parsing logic
int d( std::string data )
{
    bool wordStartDetected = false;
    bool wordEndDetected = false;
    bool spaceDetected = false;
    std::vector< int> positions;
    std::map< std::string, std::vector<int> > dict;
    std::stringstream word;
    std::size_t dataLength = data.length();
    for( int i = 0; i < dataLength; ++i ) {
        if( data[ i ] == '>' && data[ i+1 ] == '<' ) {
            continue;
        }
        if( '>' == data[ i -1 ] ) {
            wordStartDetected = true;
            positions.push_back( i );
        }
        if( wordStartDetected ) {
            if( ' ' != data[ i ] ) {
                word << data[ i ];
            } else if( ' ' == data[ i ]  ) {
                spaceDetected = true;
                wordEndDetected = true;
            }
            if( wordEndDetected ) {

            }
        }
    }
}

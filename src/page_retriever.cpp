#include <vector>
#include <sstream>
#include <string>
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
                m_stopWords.insert(
                        std::pair< std::string, std::string >(
                                    stopWord, stopWord ) );
        }
    }
    return result;
}

bool IndexManager::generateIndices(std::string &data)
{
    bool result = false;
    unsigned int startIndex = 0x00;
    bool startTagDetected = false;
    bool endTagDetected = false;
    bool wordStartDetected = false;
    bool wordEndDetected = false;
    bool spaceDetected = false;
    std::vector< unsigned int > positions;
    std::map< std::string, std::vector<int> > dict;
    std::stringstream word;
    std::size_t dataLength = data.length();
    unsigned int position = 0x00;

    //Find the start of the <body> tag in the html
    if( std::string::npos == ( startIndex = data.find( "<body>") ) ) {
        return result;
    }

    //Start parsing the html and add the words to the dictionary,
    //if the word is not present in the stop words list
    for( unsigned int i = startIndex; i < dataLength; ++i ) {
        if( data[ i ] == '>' && data[ i+1 ] == '<' ) {
            continue;
        } else if( '>' == data[ i ] && data[ i+1 ] != '<' ) {
            startTagDetected = true;
            endTagDetected   = false;
            continue;
        } else if( '<' == data[ i ] ) {
            startTagDetected = false;
            endTagDetected = true;
            continue;
        }
        if( startTagDetected ) {
            if( ( '>' == data[ i - 1 ] ) || ( ' ' == data[ i - 1 ] ) ) {
                wordStartDetected = true;
                position = i;
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
}

IndexManager::IndexManager( std::string &inputFile )
            : m_parser( inputFile )
{

}

}




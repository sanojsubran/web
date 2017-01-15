#include <sstream>
#include <fstream>
#include <string>
#include <memory>
#include <iostream>
#include <algorithm>

#include "index_manager.h"

namespace Webs {

IndexManager* IndexManager::m_instance = nullptr;

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


bool IndexManager::populateStopWords( std::string &stopWordsListFilename )
{
    bool result = false;
    std::string stopWord;
    std::ifstream reader( stopWordsListFilename );
    if( reader.good() ) {
        result = true;
        while( getline( reader, stopWord, ' ' ) ) {
                m_stopWords.insert( stopWord );
        }
    }
    return result;
}

bool IndexManager::generateIndices( )
{
    bool result = false;
    unsigned int startIndex = 0x00;
    std::size_t startTagPos = 0;
    std::size_t endTagPos = 0;
    m_htmlData.clear();
    if ( 1 == m_pageRetriever.pageContent( m_htmlData ) ) {
        return result;
    }
    std::string htmlData = m_htmlData;
    if( 0 == m_htmlData.length() )
        return result;
    //Find the start of the <body> tag in the html
    if( std::string::npos == ( startIndex = htmlData.find( "<body>") ) ) {
        return result;
    }
    //Start parsing the html and add the words to the dictionary,
    //if the word is not present in the stop words list
    std::replace( htmlData.begin(), htmlData.end(), '\n', ' ' );
    std::replace( htmlData.begin(), htmlData.end(), '\r', ' ' );
    std::replace( htmlData.begin(), htmlData.end(), '\t', ' ' );
    std::replace( htmlData.begin(), htmlData.end(), '.', ' ' );
    std::replace( htmlData.begin(), htmlData.end(), ',', ' ' );
    for( std::size_t i = startIndex + 6; i < htmlData.length(); ) {
        startTagPos = htmlData.find( ">", i );
        endTagPos = 0;
        i = startTagPos;
        if( startTagPos != std::string::npos &&
                htmlData[ startTagPos + 1 ] == '<' ) {
            i = i + 2;
            continue;
        } else if( startTagPos != std::string::npos &&
                              htmlData[ startTagPos + 1 ] != '<' ) {
            endTagPos = htmlData.find( "<", i );
            std::string textData = htmlData.substr( i+1,
                                                ( endTagPos - startTagPos -1) );
            std::stringstream textStream( textData );
            std::string word;
            std::size_t wordIndex = startTagPos;
//            std::cout << textData << std::endl;
//            std::cout << "======================================" << std::endl;
           std::size_t textStreamCursor = 0;
            while( std::getline( textStream, word, ' ' ) ) {
                if( m_stopWords.end() == m_stopWords.find( word ) ) {
                    result = true;
                    std::vector< int > wordList;
                    wordIndex = startTagPos + textData.find( word,
                                                         textStreamCursor );
                    if( m_wordDictionary.end()
                            != m_wordDictionary.find( word ) ) {
                        auto iter = m_wordDictionary.find( word );
                        wordList = iter->second;
                        m_wordDictionary.erase( iter );
                        wordList.push_back( wordIndex );
                        m_wordDictionary.insert(
                            std::pair< std::string, std::vector< int> >( word,
                                                               wordList ) );
                    } else {
                        wordList.push_back( wordIndex );
                        m_wordDictionary.insert(
                            std::pair< std::string, std::vector< int> >( word,
                                                               wordList ) );
                    }
                }
                textStreamCursor += word.length();
            }

        }
        i = endTagPos;
    }
    return result;
}

bool IndexManager::getWordIndices( std::string &word,
                                   std::vector<int> &wordPositions)
{
    bool result = false;
    auto it = m_wordDictionary.find( word ) ;
    if( m_wordDictionary.end() != it )     {
        result = true;
        wordPositions = it->second;
    }
    return result;
}

void IndexManager::setUrl(const std::string &url)
{
    m_pageRetriever.setUrl( url );
}

std::string IndexManager::htmlData()
{
    return m_htmlData;
}

std::set<std::string> IndexManager::stopWordList()
{
    return m_stopWords;
}

std::map<std::string, std::vector<int> > IndexManager::wordDictionary()
{
    return m_wordDictionary;
}


IndexManager::IndexManager( std::string &url )
            : m_pageRetriever( url )
{
    m_stopWords.clear();
    m_wordDictionary.clear();
}

IndexManager* IndexManager::instance()
{
    return m_instance;
}


void IndexManager::createInstance( std::string &url )
{
    if( nullptr == m_instance ) {
        m_instance  = new IndexManager( url );
    }
    m_instance->setUrl( url );
}


}

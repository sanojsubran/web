#include <sstream>
#include <fstream>
#include <string>
#include <memory>
#include <iostream>

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
        while( getline( reader, stopWord) ) {
                m_stopWords.insert( stopWord );
        }
    }
    return result;
}

bool IndexManager::generateIndices( /*std::string &data*/ )
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
    unsigned int position = 0x00;
    std::size_t startTagPos = 0;
    std::size_t endTagPos = 0;
    m_htmlData.clear();
    m_pageRetriever.pageContent( m_htmlData );
    std::size_t dataLength = m_htmlData.length();
    //Find the start of the <body> tag in the html
    if( std::string::npos == ( startIndex = m_htmlData.find( "<body>") ) ) {
        return result;
    }

    //Start parsing the html and add the words to the dictionary,
    //if the word is not present in the stop words list
    for( std::size_t i = startIndex + 6; i < m_htmlData.length(); ) {
        startTagPos = m_htmlData.find( ">", i );
        endTagPos = 0;
        i = startTagPos;
        if( startTagPos != std::string::npos &&
                m_htmlData[ startTagPos + 1 ] == '<' ) {
            i = i + 2;
            continue;
        } else if( startTagPos != std::string::npos &&
                              m_htmlData[ startTagPos + 1 ] != '<' ) {
            endTagPos = m_htmlData.find( "<", i );
            std::string textData = m_htmlData.substr( i+1,
                                                ( endTagPos - startTagPos -1) );
            std::stringstream textStream( textData );
            std::string word;
            std::size_t wordIndex = startTagPos;
//            std::cout << textData << std::endl;
//            std::cout << "======================================" << std::endl;

            while( std::getline( textStream, word, ' ' ) ) {
                if( '.' == word[ word.length() -1 ] ||
                    ',' == word[ word.length() -1 ] ) {
                    word = word.substr( 0, word.length() -1  );
                }
                if( m_stopWords.end() == m_stopWords.find( word ) ) {
                    std::vector< int > wordList;
                    wordIndex = startTagPos + textData.find( word, 0 );
                    if( m_wordDictionary.end()
                            != m_wordDictionary.find( word ) ) {
                        auto iter = m_wordDictionary.find( word );
                        wordList = iter->second;
                        wordList.push_back( wordIndex );
                        m_wordDictionary.erase( iter );
                        m_wordDictionary.insert(
                            std::pair< std::string, std::vector< int> >( word,
                                                               wordList ) );
                    } else {
                        wordList.push_back( wordIndex );
                        m_wordDictionary.insert(
                            std::pair< std::string, std::vector< int> >( word,
                                                               wordList ) );
                    }
                    //wordIndex += word.length();
                }
            }

        }
        i = endTagPos;
    }
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

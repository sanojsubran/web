#include <sstream>
#include <fstream>
#include <string>
#include <memory>

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
    std::size_t startTagPos = 0;
    std::size_t endTagPos = 0;

    //Find the start of the <body> tag in the html
    if( std::string::npos == ( startIndex = data.find( "<body>") ) ) {
        return result;
    }

    //Start parsing the html and add the words to the dictionary,
    //if the word is not present in the stop words list
    for( std::size_t i = startIndex + 6; i < data.length(); ) {
        startTagPos = data.find( ">", i );
        endTagPos = 0;
        if( startTagPos != std::string::npos &&
                data[ startTagPos + 1 ] == '<' ) {
            i = i + 2;
            continue;
        } else if( startTagPos != std::string::npos &&
                              data[ startTagPos + 1 ] != '<' ) {
            endTagPos = data.find( "<", startTagPos );
            std::string textData = data.substr( startTagPos,
                                                ( endTagPos - startTagPos) );
            std::stringstream textStream( textData );
            std::string word;
            std::size_t wordIndex = startTagPos;
            while( std::getline( textStream, word ) ) {
                if( m_stopWords.end() == m_stopWords.find( word ) ) {
                    std::vector< int > wordList;
                    wordIndex = textData.find( textData, wordIndex );
                    if( m_wordDictionary.end()
                            != m_wordDictionary.find( word ) ) {
                        auto iter = m_wordDictionary.find( word );
                        wordList = iter->second;
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
                    wordIndex += word.length();
                }
            }
        }
        i = i + endTagPos;
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

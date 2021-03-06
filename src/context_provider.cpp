#include <vector>
#include <sstream>

#include "index_manager.h"
#include "context_provider.h"


namespace Webs {

ContextProvider::ContextProvider(std::string &websiteContent)
                    : m_websiteContent( websiteContent )
{

}


ContextProvider::~ContextProvider()
{

}


std::string ContextProvider::getWordContext( std::string &word,
                                             std::size_t pos)
{
    std::size_t localBoundary = ( pos > 30 )? 30 : 15;
    std::string contextData = "";
    if( m_websiteContent.empty() || word.empty() ) {
        return contextData;
    }
    std::string contextString = m_websiteContent.substr( pos - localBoundary,
                                                  2 * localBoundary );
    std::stringstream contextStream;
    contextStream.str( contextString );
    std::vector< std::string > wordList;
    std::string item;
    while( std::getline( contextStream, item, ' ' ) ) {
        wordList.push_back( item );
    }
    std::size_t listSize = wordList.size();
    for( unsigned int i = 1; i < listSize - 1; ++i ) {
        std::string one = wordList.at( i );
        if( std::string::npos != one.find( '>' ) ) {
            contextData.clear();
        } else {
            contextData += one + " ";
        }
    }
    return contextData;
}


StringVec ContextProvider::getAllOccurrences(std::string &word)
{
    StringVec contextTextList;
    IndexManager* indexMgr = INDEX_MGR()->instance();
    std::vector< int > posList;
    if( nullptr != indexMgr ) {
        indexMgr->getWordIndices( word, posList );
    }
    if( posList.size() ) {
        for( auto it : posList ) {
            std::string contextInfo = getWordContext( word, (it) );
            if( !contextInfo.empty() ) {
                contextTextList.emplace_back( contextInfo );
            }
        }
    }
    return contextTextList;
}


}

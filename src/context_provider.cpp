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
    std::size_t localBoundary = ( pos > 100 )? 100 : 25;
    std::string contextData = "";
    if( m_websiteContent.empty() || word.empty() ) {
        return contextData;
    }
    std::string contextString = m_websiteContent.substr( pos - localBoundary,
                                                  pos + localBoundary );
    std::size_t mid = contextString.length() / 2;
    std::size_t startTagPos = 0;
    while( mid > 0 ) {
        if( '>' == contextString[ mid ]  ) {
            startTagPos = mid;
            break;
        }
        --mid;
    }
    if( !mid ) {
        return contextData;
    }
    std::size_t endTagPos = contextString.find( '<', startTagPos );
    if( endTagPos != std::string::npos ) {
        contextData = contextString.substr( startTagPos,
                                            ( endTagPos - startTagPos ) );
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
    if( contextTextList.size() ) {
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

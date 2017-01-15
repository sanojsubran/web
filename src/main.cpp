#include <iostream>
#include <fstream>
#include <string>

#include "context_provider.h"
#include "page_retriever.h"
#include "index_manager.h"


int main( int argc, char* argv[] )
{
    if( 3 != argc ) {
        std::cout << "Incorrect number of parameters" << std::endl;
        std::cout << "Hint: web <url> <pattern>" << std::endl;
        return 0;
    } else {
        std::string appname = argv[0];
        std::string url = argv[1];
        std::string pattern = argv[2];
        //std::cout << "App name: " << appname << std::endl;
        std::cout << "Url: " << url << std::endl;
        std::cout << "Pattern: " << pattern << std::endl << std::endl;

        if( url.empty() || pattern.empty() ) {
            std::cout << "The url/pattern is emplty. Please check !"
                      << std::endl;
            return 0;
        }

        Webs::IndexManager::createInstance( url );
        auto indexMg = INDEX_MGR()->instance();
        if( nullptr != indexMg ) {
            std::string stopFile = "stopwords.txt";
              if( ! indexMg->populateStopWords( stopFile ) ) {
                  std::cout << "Unable to generate the stopword list. "
                            << "Aborting program..." << std::endl;
                  return 0;
              }
        }
        if ( ! indexMg->generateIndices() ) {
            std::cout << "Unable to generate the indices. Aborting..."
                      << std::endl;
        }
        std::string html = INDEX_MGR()->htmlData();
        if( ! html.empty() ) {
            Webs::ContextProvider contextMgr( html );
            std::vector< std::string > contextRefs
                    = contextMgr.getAllOccurrences( pattern );
            if( contextRefs.size() ) {
                std::cout << "Sting occurances are found at:" << std::endl;
                unsigned int i = 0;
                for( auto iter : contextRefs ) {
                    std::cout << ++i << ". " <<iter << std::endl;
                }
            } else {
                std::cout << "Unable to find the string" << std::endl;
            }
        }
    }
    return 0;
}

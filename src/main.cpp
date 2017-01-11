#include <iostream>
#include <string>

#include "page_retriever.h"


int main( int argc, char* argv[] )
{
    std::cout << "web" << std::endl;
    if( 3 != argc ) {
        std::cout << "Incorrect number of parameters" << std::endl;
        std::cout << "Hint: web <url> <pattern>" << std::endl;
        return 0;
    } else {
        std::string appname = argv[0];
        std::string url = argv[1];
        std::string pattern = argv[2];
        std::cout << "App name: " << appname << std::endl;
        std::cout << "Url: " << url << std::endl;
        std::cout << "Pattern: " << pattern << std::endl;

        Webs::PageRetriever obj( url );
        obj.pageContent();
    }
    return 0;
}

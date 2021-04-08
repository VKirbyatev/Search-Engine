#include "SearchEngine.hpp"

#include <string>
#include "snippetsBuilder.hpp"

void searchByRequest(std::string request) {
    printSnippetsByDistance(request);
    printSnippetsByTFIDF(request);
}

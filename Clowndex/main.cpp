#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "validation/validation.hpp"
#include "indices/indices.hpp"
#include "fileSystemManager.hpp"
#include "SearhEngine/TFIDFBuilder.hpp"


int main(int argc, const char * argv[]) {
    std::map<double, std::string> f;
    f = getTFIDF("как все запорожцы собрались в круги");

    std::cout << "FOR как все запорожцы собрались в круги:" << std::endl;

    for (auto it = f.begin(); it != f.end(); it++)
        std::cout << "TFIDF: " << it->first << "; Doc Num: " << it->second << std::endl;
   
    return 0;
}

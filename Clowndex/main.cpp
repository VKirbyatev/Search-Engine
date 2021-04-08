#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "validation/validation.hpp"
#include "indices/indices.hpp"
#include "fileSystemManager.hpp"
#include "SearhEngine/TFIDFBuilder.hpp"
#include "SearhEngine/seachByDistance.hpp"


int main(int argc, const char * argv[]) {
    std::map<double, std::string> f;
    f = getTFIDF("Остап и Андрий");

    std::cout << "FOR Остап и Андрий by TFIDF:" << std::endl;

    for (auto it = f.begin(); it != f.end(); it++)
        std::cout << "TFIDF: " << it->first << "; Doc Num: " << it->second << std::endl;
    
    std::cout << std::endl << "FOR рубил и сыпал удары by SUMM DISTANCE:" << std::endl;
    
    std::map<int, std::map<int, std::vector<int>>> d;
    d = searchByDistance("Остап");
    
    for (auto it = d.begin(); it != d.end(); it++) {
        std::cout << "DISTANCE SUMM: " << it->first << " DOC NUM: " << it->second.begin()->first << std::endl;
        std::cout << "TEXT POSITIONS: ";
        for (auto vIt = it->second.begin()->second.begin(); vIt != it->second.begin()->second.end(); vIt++)
            std::cout << *vIt << " ";
        std::cout << std::endl;
    }
    
    
    return 0;
}

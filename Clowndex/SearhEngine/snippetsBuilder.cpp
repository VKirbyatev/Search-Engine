#include "snippetsBuilder.hpp"
#include <string>
#include <map>
#include <vector>
#include "seachByDistance.hpp"
#include "TFIDFBuilder.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../fileSystemManager.cpp"

void printSnippets (std::string request) {
    std::cout << "Результаты запроса для: \" " << request << "по сумме расстояний: " << std::endl;
    
    //забиваем в структуры результаты
    //TDIDF для БД в map, где ключи - TF-IDF, значения - документы
    std::map<double, std::string> byTFIDFResult;
    //map, где ключи - суммы, значения map - где ключи документы, значения позиции
    std::map<int, std::map<int, std::vector<int>>> byDistanceResult;
    byDistanceResult = searchByDistance(request);
    byTFIDFResult = getTFIDF(request);
    
    //разобьем слова в массив
    std::stringstream sstream;
    sstream << request;
    std::string word;
    std::vector<std::string> words(0);
    while (sstream >> word) {
        words.push_back(word);
    }
    
    auto i = 0;
    auto it = byDistanceResult.begin();
    
    //выводим сниппеты для суммы расстояний
    while (i < 5 && it != byDistanceResult.end()) {
        std::cout << "Сумма расстояний: " << it->first << " Документ: " << it->second.begin()->first << std::endl;
        std::string word;
        auto firstPos = it->second.begin()->second[0];
        auto lastPos = it->second.begin()->second[words.size() - 1];
        std::ifstream data(getRootPath() + "/data/doc" + std::to_string(it->second.begin()->first) + ".txt");
        //ситуации
//        0 одно слово
//        0.1 оно вначале
//        0.2 оно в конце
//        0.3 оно по середине
//        1 два слова
//        1.1 если они близко
//        1.1.1 первое в начале
//        1.1.2 последнее в конце
//        1.2 если они далеко
//        1.2.1 первое в начале
//        1.2.2 последнее в конце
//        2 больше двух слов
//        2.1 если они близко
//        2.1.1 первое в начале
//        2.1.2 последнее в конце
//        2.2 если они далеко
//        2.2.1 первое в начале
//        2.2.2 последнее в конце
        
        i++; it++;
        }
    }


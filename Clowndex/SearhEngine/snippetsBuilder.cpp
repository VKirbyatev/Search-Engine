#include "snippetsBuilder.hpp"
#include <string>
#include <map>
#include <vector>
#include "seachByDistance.hpp"
#include "TFIDFBuilder.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../fileSystemManager.hpp"

void printSnippetsByDistance (std::string request) {
    std::cout << "Результаты запроса для: \"" << request << "\" по сумме расстояний: " << std::endl;

    //забиваем в структуры результаты
    //map, где ключи - суммы, значения map - где ключи документы, значения позиции
    std::map<int, std::map<int, std::vector<int>>> byDistanceResult;
    byDistanceResult = searchByDistance(request);

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
        std::cout << " Документ: " << it->second.begin()->first << std::endl << "Сниппет: " << std::endl;
        std::string word;
        auto firstPos = it->second.begin()->second[0];
        auto lastPos = it->second.begin()->second[words.size() - 1];
        std::ifstream data(getRootPath() + "/data/doc" + std::to_string(it->second.begin()->first) + ".txt");
        std::ifstream dirIndex(getRootPath() + "/directIndices/index" + std::to_string(it->second.begin()->first) + ".txt");
        //количество слов
        std::string countOfWords;
        dirIndex >> countOfWords;
        //выводим сниппеты
        //1 слово
        if (words.size() == 1) {

            if (firstPos < 10) {
                for (auto i = 0; i < firstPos + 3; i++) {
                    data >> word;
                    std::cout << word << " ";
                }
                std::cout << "..." << std::endl;
            }
            else if (lastPos > std::stoi(countOfWords) - 10) {
                for (auto i = 0; i < lastPos - 10; i++) {
                    data >> word;
                }
                std::cout << "...";
                for (auto i = 0; i < 10; i++) {
                    data >> word;
                    std::cout << word << " ";
                }
                std::cout << std::endl;
            }
            else {
                for (auto i = 0; i < firstPos - 5; i++) {
                    data >> word;
                }
                std::cout << "...";
                for (auto i = firstPos; i < firstPos + 10; i++) {
                    data >> word;
                    std::cout << word << " ";
                }
                std::cout << "..." << std::endl;
            }
            //2 слова и более
        }
        else
            if (firstPos < 10) {
                for (auto i = 0; i < firstPos; i++) {
                    data >> word;
                }
                for (auto i = firstPos; i < lastPos + 3; i++) {
                    data >> word;
                    std::cout << word << " ";
                }
                std::cout << "..." << std::endl;
            }
            else if (lastPos > std::stoi(countOfWords) - 10) {
                for (auto i = 0; i < firstPos - 5; i++) {
                    data >> word;
                }
                std::cout << "...";
                for (auto i = firstPos; i < lastPos + 6; i++) {
                    data >> word;
                    std::cout << word << " ";
                }
                std::cout << std::endl;
            }
            else {
                for (auto i = 0; i < firstPos - 5; i++) {
                    data >> word;
                }
                std::cout << "...";
                for (auto i = firstPos; i < lastPos + 10; i++) {
                    data >> word;
                    std::cout << word << " ";
                }
                std::cout << "..." << std::endl;
            }
        i++; it++;
    }
}


//Для TF-IDF
void printSnippetsByTFIDF (std::string request) {
    std::cout << "Результаты запроса для: \"" << request << "\" по TFIDF: " << std::endl;
    
    //забиваем в структуры результаты
    //TDIDF для БД в map, где ключи - TF-IDF, значения - документы
    std::map<double, std::string> byTFIDFResult;
    byTFIDFResult = getTFIDF(request);
    
    //разобьем слова в массив
    std::stringstream sstream;
    sstream << request;
    std::string word;
    std::vector<std::string> words(0);
    while (sstream >> word) {
        words.push_back(word);
    }
    
    //забьем обратные индексы в структуру
    std::ifstream invInFile(getRootPath() + "/invertedIndices/invertedIndex.txt");
    std::map<std::string, std::map<int, std::vector<int>>> invInStruct;
    std::string wordKey, docKey, value;
    while (invInFile.is_open() && wordKey != ".") {
        invInFile >> wordKey;
        invInFile >> docKey;
        while (docKey != ";" && wordKey != ".") {
            std::vector<int> positions(0);
            
            invInFile >> value;
            invInFile >> value;
            while (value != "]") {
                positions.push_back(std::stoi(value));
                invInFile >> value;
            }
            invInStruct[wordKey][std::stoi(docKey)] = positions;
            invInFile >> docKey;
        }
    }
    
    auto i = 0;
    auto it = byTFIDFResult.begin();
    
//    выводим сниппеты для суммы расстояний
    while (i < 5 && it != byTFIDFResult.end()) {
        std::cout << "TF-IDF: " << it->first*(-1) <<" Документ: " << it->second << std::endl << "Сниппет: " << std::endl;
        //найдем позиции
        std::vector<int> positions(0);
        for (i = 0; i < words.size(); i++) {
            //если это слово есть в документе
            if (invInStruct[words[i]].find(std::stoi(it->second)) != invInStruct[words[i]].end())
                positions.push_back(invInStruct[words[i]][std::stoi(it->second)][0]);
        }
        std::sort(positions.begin(), positions.end());
        std::string word;
        auto firstPos = positions[0];
        auto lastPos = positions[positions.size() - 1];
        std::ifstream data(getRootPath() + "/data/doc" + it->second + ".txt");
        std::ifstream dirIndex(getRootPath() + "/directIndices/index" + it->second + ".txt");
        //количество слов
        std::string countOfWords;
        dirIndex >> countOfWords;
        //выводим сниппеты
        //1 слово
        if (words.size() == 1) {

            if (firstPos < 10) {
                for (auto i = 0; i < firstPos + 3; i++) {
                    data >> word;
                    std::cout << word << " ";
                }
                std::cout << "..." << std::endl;
            }
            else if (lastPos > std::stoi(countOfWords) - 10) {
                for (auto i = 0; i < lastPos - 10; i++) {
                    data >> word;
                }
                std::cout << "...";
                for (auto i = 0; i < 10; i++) {
                    data >> word;
                    std::cout << word << " ";
                }
                std::cout << std::endl;
            }
            else {
                for (auto i = 0; i < firstPos - 5; i++) {
                    data >> word;
                }
                std::cout << "...";
                for (auto i = firstPos; i < firstPos + 10; i++) {
                    data >> word;
                    std::cout << word << " ";
                }
                std::cout << "..." << std::endl;
            }
            //2 слова и более
        }
        else
            if (firstPos < 10) {
                for (auto i = 0; i < firstPos; i++) {
                    data >> word;
                }
                for (auto i = firstPos; i < lastPos + 3; i++) {
                    data >> word;
                    std::cout << word << " ";
                }
                std::cout << "..." << std::endl;
            }
            else if (lastPos > std::stoi(countOfWords) - 10) {
                for (auto i = 0; i < firstPos - 5; i++) {
                    data >> word;
                }
                std::cout << "...";
                for (auto i = firstPos; i < lastPos + 6; i++) {
                    data >> word;
                    std::cout << word << " ";
                }
                std::cout << std::endl;
            }
            else {
                for (auto i = 0; i < firstPos - 5; i++) {
                    data >> word;
                }
                std::cout << "...";
                for (auto i = firstPos; i < lastPos + 10; i++) {
                    data >> word;
                    std::cout << word << " ";
                }
                std::cout << "..." << std::endl;
            }

        i++; it++;
    }
}

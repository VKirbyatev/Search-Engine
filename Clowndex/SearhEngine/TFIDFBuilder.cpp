#include "TFIDFBuilder.hpp"
#include "../fileSystemManager.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <map>



//Строим TDIDF для БД в map, где ключи - TF-IDF, значения - документы
std::map<double, std::string> getTFIDF (std::string phrase) {
    std::map<double, std::string> mapTFIDF;
    //проверим, что фраза не пустая
    if (phrase.size() == 0) {
        std::cout << "Current phrase is empty" << std::endl;
        return mapTFIDF;
    }
    
    //Забиваем фразу в строковой поток, чтобы считывать ее по пробелами и закинем ее в структуру vector
    std::stringstream sstream;
    sstream << phrase;
    std::string word;
    std::vector<std::string> words(0);
    while (sstream >> word) {
        words.push_back(word);
    }
    
    //счетчик файлов для прямых индексов
    int filesCounter = 1;
    //файлы с прямыми и обратными индексами
    std::ifstream dirInFile(getRootPath() + "/directIndices/index" + std::to_string(filesCounter) + ".txt");
    std::ifstream invInFile(getRootPath() + "/invertedIndices/invertedIndex.txt");
    //Проверка на наличие даты
    if (!dirInFile.is_open() || !invInFile.is_open()) {std::cout << "Couldnt find index files in root directory" << std::endl; return mapTFIDF;}
    
    //Загоним файл обратных индексов в структуру map, где key - слово, val - где встречаемтся
    std::map<std::string, std::vector<int>> invInStruct;
    std::vector<int> wasInDoc(0);
    std::string key, value;
    while (invInFile.is_open() && key != ".") {
        invInFile >> key;
        invInFile >> value;
        while (value != ";" && value != "") {
            wasInDoc.push_back(std::stoi(value));
            invInFile >> value;
        }
        invInStruct[key] = wasInDoc;
    }
    
    
    //считаем кол-во документов
    int documentsCounter = 0;
    while (dirInFile.is_open()) {
        documentsCounter++;
        filesCounter++;
        dirInFile.close();
        //читаем следующий файл
        dirInFile = std::ifstream(getRootPath() + "/directIndices/index" + std::to_string(filesCounter) + ".txt");
    }
    
    //сбрасываем на 1
    filesCounter = 1;
    dirInFile.close();
    dirInFile = std::ifstream(getRootPath() + "/directIndices/index" + std::to_string(filesCounter) + ".txt");
    
  
    
    //TF-IDF
    double TFIDF = 0;
    while (dirInFile.is_open() && invInFile.is_open()) {
        //общее кол-во слов в док-те
        int wordsCounter = 0;
        dirInFile >> wordsCounter;
        //забиваем прямой индекс файл в структуру map, где ключи - слово, значения - встречаемость
        std::map<std::string, int> dirIndexStruct;
        std::string key;
        int value;
        while (dirInFile >> key) {
            dirInFile >> value;
            dirIndexStruct[key] = value;
        }
        //рассчитываем TF*IDF для всей фразы
        for (auto i = 0; i < words.size(); i++) {
            double TF = 1.0*dirIndexStruct[words[i]]/wordsCounter;
            double IDF = log(1.0*documentsCounter/invInStruct[words[i]].size());
            TFIDF += TF*IDF;
        }
        //забиваем в структуру
        if (TFIDF != 0)
        mapTFIDF[TFIDF] = std::to_string(filesCounter);
        //сбрасываем
        TFIDF = 0;
        //счетчик цикла
        filesCounter++;
        dirInFile.close();
        //читаем следующий файл
        dirInFile = std::ifstream(getRootPath() + "/directIndices/index" + std::to_string(filesCounter) + ".txt");
    }
        
        
    return mapTFIDF;
}

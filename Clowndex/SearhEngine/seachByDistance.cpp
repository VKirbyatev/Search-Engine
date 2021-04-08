#include "seachByDistance.hpp"
#include "../fileSystemManager.hpp"
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>
//здесь будем реализовывать алгоритм поиска по расстоянию в строке

//функцция для нахождения сумм расстояний из запроса
std::map<int, std::vector<int>> findSumms(std::vector<std::vector<int> >& positions);

std::map<int, std::map<int, std::vector<int>>> searchByDistance (std::string request) {
    //пихаем данные в map, где ключи - суммы, значения map - где ключи документы, значения позиции
    std::map<int, std::map<int, std::vector<int>>> summs;
    //проверка, что реквест не пустой
    if (request.size() == 0) {
        std::cout << "Current phrase is empty" << std::endl;
        return summs;
    }
    
    //Забиваем фразу в строковой поток, чтобы считывать ее по пробелами и закинем ее в структуру vector
    std::stringstream sstream;
    sstream << request;
    std::string word;
    std::vector<std::string> words(0);
    while (sstream >> word) {
        words.push_back(word);
    }
    
    //Если только 1 слово, то дальше бессмысленно работать
    if (words.size() == 1) {
        std::cout << "There's only one word" << std::endl;
        return summs;
    }
    
    //забиваем файл с обр индексами в структуру
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
    
    //пересечение слов, входящих в запрос
    std::vector<int> intersecDocs(0);
    std::vector<int> unBuff(0);
    std::vector<int> unSecBuff(0);
    for (auto it = invInStruct[words[0]].begin(); it != invInStruct[words[0]].end(); it++) {
        intersecDocs.push_back(it->first);
    }
    
    for (auto i = 1; i < words.size(); i++) {
        for (auto it = invInStruct[words[i]].begin(); it != invInStruct[words[i]].end(); it++) {
            unBuff.push_back(it->first);
        }
        std::set_intersection(intersecDocs.begin(), intersecDocs.end(), unBuff.begin(), unBuff.end(), std::back_inserter(unSecBuff));
        intersecDocs = unSecBuff;
        unBuff = std::vector<int>(0);
        unSecBuff = std::vector<int>();
    }
    
    //пихаем данные в map, где ключи - суммы, значения map - где ключи документы, значения позиции
    
    for (auto i = 0; i < intersecDocs.size(); i++) {
        std::vector<std::vector<int>> posTemp(0);
        std::map<int, std::vector<int>> mapTemp;
        for (auto j = 0; j < words.size(); j++) {
            posTemp.push_back(invInStruct[words[j]][intersecDocs[i]]);
        }
        mapTemp = findSumms(posTemp);
        //отсортируем позиции
        std::sort(mapTemp.begin()->second.begin(), mapTemp.begin()->second.end());
        summs[mapTemp.begin()->first][intersecDocs[i]] = mapTemp.begin()->second;
    }
    
    return summs;
}

std::map<int, std::vector<int>> findSumms(std::vector<std::vector<int> >& positions) {
    //map, где ключи - суммы, значения - позиции
    std::map<int, std::vector<int>> summs;
    // количество массивов
    int n = positions.size();
    
    // отслеживаем следующий элемент в n массивах
    int* indices = new int[n];
    
    // инициализируем первыми элементами
    for (int i = 0; i < n; i++)
        indices[i] = 0;
    
    while (1) {
        
//        текущая комбинация
        std::vector<int> v(0);
        auto summ = 0;
        for (auto i = 0; i < n; i++) {
            v.push_back(positions[i][indices[i]]);
            if (v.size() != 1) {
                summ += std::abs(v[v.size()-2] - v[v.size()-1]);
            }
        }
        summs[summ] = v;
        
        //находим самый правый массив, в котором осталось больше элементов, чем в текущем
        int next = n - 1;
        while (next >= 0 &&
               (indices[next] + 1 >= positions[next].size()))
            next--;
        
        //выходим, когда таких массивов не осталось (значит и комбинаций тоже)
        if (next < 0)
            return summs;
        
        // если нашли, то идем к следующему элементу в этом массиве
        indices[next]++;
        
        // для всех массивов, справа от этого сбрасываем индексы на 0
        for (int i = next + 1; i < n; i++)
            indices[i] = 0;
    }
}


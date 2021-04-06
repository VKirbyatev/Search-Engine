#include "indices.hpp"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <map>
#include "../fileSystemManager.hpp"

//создать файс с прямыми индексами для 1 текстового файла
void createDirectIndices () {
    //DATA
    //считаем кол-во созданных/измененных файлов
    auto filesChangedCouner = 0;
    //количество для перехода по index файлам
    auto filesCounter = 1;
    //первый файл (берем из data)
    std::ifstream fin(getRootPath() + "/data/doc" + std::to_string(filesCounter) + ".txt");
    //Проверка на наличие даты
    if (!fin.is_open()) {std::cout << "Couldnt find data files in root directory, use copy fnc" << std::endl; return;}
    
    while (fin.is_open()) {
        //Проверка существования файла с индексами
        if (std::ifstream(getRootPath() + "/directIndices/index" + std::to_string(filesCounter) + ".txt").is_open()) {
            std::cout << "File index" + std::to_string(filesCounter) + ".txt" + " already exist, rewrite?(y/n)" << std::endl;
            char answer;
            std::cin >> answer;
            tolower(answer);
            if (answer != 'y')
                filesCounter++;
                fin = std::ifstream(getRootPath() + "/data/doc" + std::to_string(filesCounter) + ".txt");
                continue;
        }
        //количество слов
        int numOfWords = 0;
        //слова из файла
        std::map<std::string, int> words;
        //считыываем по словам
        std::string word;
        while (fin >> word) {
            //считаем общее кол=во слов
            numOfWords++;
            //to lower case
            std::transform(word.begin(), word.end(), word.begin(),
                        [](unsigned char c){ return std::tolower(c); });
            //убираем точки, запятые и пр
            word.erase (std::remove_if (word.begin (), word.end (), ispunct), word.end ());
            //вставляем в вектор, если такого слова еще нет
            if (words.find(word) == words.end() )
                words[word] = 1;
            else
                words[word]++;
        
        }
        //создаем файл + записываем кол-во слов
        std::ofstream indexFile(getRootPath() + "/directIndices/index" + std::to_string(filesCounter) + ".txt", std::ofstream::out | std::ofstream::trunc);
        indexFile << numOfWords << std::endl;
        //записываем сами слова
        for (auto it = words.begin(); it != words.end(); it++) {
            indexFile << it->first << " " << it->second << std::endl;
        }
        //перемещаемся на следующий файл
        filesChangedCouner++;
        filesCounter++;
        fin = std::ifstream(getRootPath() + "/data/doc" + std::to_string(filesCounter) + ".txt");
    }
    //оповещаем, если успешно
    std::cout << filesCounter - 1 << " files was created/changed" << std::endl;
}

void createInvertIndices () {
    //контейнер с данными
    std::map<std::string, std::vector<int>> words;
    //сюда пишем слова из файлов с прямыми индексами
    std::string word;
    //строим из прямых индексов обратные
    //открываем файл с прямыми индексами
    int count = 1;
    std::ifstream index(getRootPath() + "/directIndices/index" + std::to_string(count) + ".txt");
    //смотрим существует ли хотя бы 1 файл
    if (!index.is_open()) {
        std::cout << "Index files doesnt exist in this directory, use create dirInd fnc" << std::endl;
        return;
    }
    //Проверка существования файла с индексами
    if (std::ifstream(getRootPath() + "/invertedIndices/" + "invertedIndex.txt").is_open()) {
        std::cout << "File invertedIndex.txt already exist, rewrite?(y/n)" << std::endl;
        char answer;
        std::cin >> answer;
        tolower(answer);
        if (answer != 'y')
            return;
    }
    //пропускаем число слов док-та
    index >> word;
    //строим
    while (index.is_open()) {
        //инициализируем
        while(index >> word) {
            if (words.find(word) == words.end() ) {
                //если такого слова нет, то добавляем ему вектор и пишем номер файла
                words[word] = std::vector<int>(0);
                words[word].push_back(count);
            }
            else
                //если есть, то добавляем новый номер док-та
                words[word].push_back(count);
            //пропускаем цифры
            index >> word;
        }
        //увеличиваем
        count++;
        index.close();
        //читаем следующий файл
        index = std::ifstream(getRootPath() + "/directIndices/index" + std::to_string(count) + ".txt");
        //пропускаем число слов док-та
        index >> word;
    }
    //инициализируем документ с обратными индексами
    std::ofstream invIndex(getRootPath() + "/invertedIndices/" + "invertedIndex.txt", std::ofstream::out | std::ofstream::trunc);
    //записываем
    for (auto it = words.begin(); it != words.end(); it++) {
        invIndex << it->first << " ";
        for (auto vecIt = it->second.begin(); vecIt != it->second.end(); vecIt++) {
            invIndex << *vecIt << " ";
        }
        invIndex << ';';
        invIndex << std::endl;
    }
    invIndex << '.';
    std::cout << "Inverted Index file was succesfully created/changed" << std::endl;
}



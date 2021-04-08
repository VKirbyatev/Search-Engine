#include <string>
#include <iostream>
#include "SearhEngine/SearchEngine.hpp"


int main(int argc, const char * argv[]) {
    
    std::cout << "Введите запрос: " << std::endl;
    std::string request;
    std::getline(std::cin, request);
    searchByRequest(request);
    
    return 0;
}

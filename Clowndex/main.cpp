#include <iostream>
#include <fstream>
#include <string>
#include "validation/validation.hpp"
#include "indices/indices.hpp"


int main(int argc, const char * argv[]) {
    for (int i = 1; i < 11; i++)
        createDirectIndices(std::ifstream("/Users/vladislavkirbyatev/Desktop/C++/Clowndex/Clowndex/DB/doc" + std::to_string(i) + ".txt"), "index" + std::to_string(i));
    createInvertIndices("/Users/vladislavkirbyatev/Library/Developer/Xcode/DerivedData/Clowndex-gafljocflskpydcgxpcqasngkjak/Build/Products/Debug/directIndices");
    
    return 0;
}

//
//  fileSystemManager.hpp
//  Clowndex
//
//  Created by Владислав on 03.04.2021.
//  Copyright © 2021 Vladislav Kirbyatev. All rights reserved.
//

#ifndef fileSystemManager_hpp
#define fileSystemManager_hpp

#include <stdio.h>
#include <string>
std::string getRootPath ();
void copyOneFileToRoot (std::string filePath);
void copyDirectoryFilesToRoot (std::string filePath);
#endif /* fileSystemManager_hpp */

/*
* binary-cpp-api - Binary C++ API client
*
* Copyright (c) 2018 Elektro Yar. Email: git.electroyar@gmail.com
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef BANANA_FILESYSTEM_HPP_INCLUDED
#define BANANA_FILESYSTEM_HPP_INCLUDED
//------------------------------------------------------------------------------
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <dirent.h>
#include <dir.h>
//------------------------------------------------------------------------------
namespace bf
{
//------------------------------------------------------------------------------
        /** \brief Проверить наличие файла
         * \param file_name имя файла
         * \return вернет true если файл есть
         */
        bool check_file(std::string file_name) {
                std::ifstream file;
                file.open(file_name);
                if(!file) return false;
                file.close();
                return true;
        }
//------------------------------------------------------------------------------
        /** \brief Получить список файлов
         * \param path путь, с которого начинается поиск
         * \param output_list список файлов
         * \param is_with_directory данный параметр влияет сразу на два фактора.
         * Если параметр не указан, список формируется только из имен файлов, которые есть в path
         * Поиск файлов во вложенных папках при этом не производится
         * Если параметр указан как true, тогда имя файла формируется с учетом path
         * Также функция тогда делает поиск во всех вложенных папках
         * \return количество найденых файлов
         */

        unsigned long get_list_files(std::string path,
                                     std::vector<std::string> &output_list,
                                     bool is_with_directory = false)
        {
                DIR* directory;                                    // Directory object
                dirent* directory_name;
                //char str_input_file[FILENAME_MAX] = {0};
                unsigned long num_files = 0;

                if(path.back() != '/' && path.back() != '\\')
                        path += "/";
                if(directory = opendir(path.c_str())) {
                        // открываем директорию
                        while(directory_name = readdir(directory)) {
                                unsigned long str_len = strlen(directory_name->d_name);
                                char * search_char = (char*) memchr(directory_name->d_name, '.', strlen(directory_name->d_name));
                                std::string str_directory_name = std::string(directory_name->d_name);
                                if(search_char == NULL) {
                                        if(is_with_directory)
                                                num_files += get_list_files(path + str_directory_name, output_list, is_with_directory);
                                        continue;
                                }
                                if(str_directory_name == "." || str_directory_name == "..")
                                        continue;
                                std::string file_name;
                                if(is_with_directory)
                                        output_list.push_back(path + str_directory_name);
                                else
                                        output_list.push_back(str_directory_name);
                                num_files++;
                        }
                }
                closedir(directory);
                return num_files;
        }
}

#endif // BANANA_FILESYSTEM_HPP_INCLUDED

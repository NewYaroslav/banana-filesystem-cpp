#include <iostream>
#include <banana_filesystem.hpp>

using namespace std;

int main()
{
        cout << bf::get_current_work_dir() << endl;
        std::vector<std::string> list_files;
        bf::get_list_files("..//..//", list_files, true, true);

        for(size_t i = 0; i < list_files.size(); ++i) {
                std::cout << list_files[i] << std::endl;
        }

        string path = "C:/Users\\user/Downloads";

        std::vector<std::string> list_dir;
        bf::parse_path(path, list_dir);

        for( auto & p : list_dir ) cout << "-> " << p << endl;

        bf::create_directory("..//..//test//test2//test3");
        bf::remove_directory("..\\..\\test", true);

        int file_size = bf::get_file_size("test.txt");

        cout << "file size " << file_size << std::endl;

        char *buffer = new char[file_size + 1];
        memset(buffer,0,file_size + 1);
        file_size = bf::load_file("test.txt", buffer, file_size);

        cout << "file size " << file_size << std::endl;

        cout << "data:\n" << buffer << std::endl;

        return 0;
}

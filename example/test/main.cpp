#include <iostream>
#include <banana_filesystem.hpp>

using namespace std;

int main()
{
        cout << "Hello world!" << endl;
        std::vector<std::string> list_files;
        bf::get_list_files("..//..//", list_files, true);
        for(size_t i = 0; i < list_files.size(); ++i) {
                std::cout << list_files[i] << std::endl;
        }
        return 0;
}

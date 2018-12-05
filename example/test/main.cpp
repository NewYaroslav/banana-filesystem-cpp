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

        string path = "C:/Users\\user/Downloads/VK audio";

        std::vector<std::string> list_dir;
        bf::parse_path(path, list_dir);

        for( auto & p : list_dir ) cout << "-> " << p << endl;

        bf::create_directory("..//..//test//test2//test3");
        bf::remove_dir("..\\..\\test", true);
        return 0;
}

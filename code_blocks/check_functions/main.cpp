#include <iostream>
#include <banana_filesystem.hpp>

using namespace std;

int main(int argc, char *argv[]) {
    string path0 = "C:/Users\\user/Downloads\\test-name.dat";
    std::cout << "file name (for path): " << bf::get_file_name(path0) << endl;

#   if defined(__MINGW32__) || defined(_WIN32)
    std::vector<std::string> list_disk = bf::get_list_disk();
    for(size_t i = 0; i < list_disk.size(); ++i) {
        std::cout << list_disk[i] << std::endl;
    }

    std::system("pause");
#   endif

    std::cout << "current work dir: " << bf::get_current_work_dir() << endl;
    std::vector<std::string> list_files;
    bf::get_list_files("..//..//", list_files, true, true);

    for(size_t i = 0; i < list_files.size(); ++i) {
        std::cout << list_files[i] << std::endl;
    }

    string path = "C:/Users\\user/Downloads\\test.dat";
    cout << "path " << path << endl;
    std::vector<std::string> list_dir;
    bf::parse_path(path, list_dir);

    for( auto & p : list_dir ) cout << "-> " << p << endl;

    string path2 = "/root/_repoz/hyperboloid-server/test.dat";
    cout << "path2 " << path2 << endl;
    std::vector<std::string> list_dir_2;
    bf::parse_path(path2, list_dir_2);

    for( auto & p : list_dir_2 ) cout << "-> " << p << endl;

    string path3 = "~/root/_repoz/hyperboloid-server/test.dat";
    cout << "path3 " << path3 << endl;
    std::vector<std::string> list_dir_3;
    bf::parse_path(path3, list_dir_3);

    for( auto & p : list_dir_3 ) cout << "-> " << p << endl;

    std::cout << "file extension (for path): " << bf::get_file_extension(path) << endl;
    std::cout << "file extension (for path2): " << bf::get_file_extension(path2) << endl;
    std::cout << "set file extension (for path): " << bf::set_file_extension(path, ".test") << endl;
    std::cout << "set file extension (for path2): " << bf::set_file_extension(path2, ".test") << endl;
    std::cout << "set file extension (\"storage/AUDCAD\"): " << bf::set_file_extension("storage/AUDCAD", ".qhs4") << endl;
    std::cout << "set file extension (\"/storage/AUDCAD\"): " << bf::set_file_extension("/storage/AUDCAD", ".qhs4") << endl;
    std::string path_test = "~/storage/AUDCAD";
    std::cout << "set file extension (\"~/storage/AUDCAD\"): " << bf::set_file_extension(path_test, ".qhs4") << endl;

    bf::create_directory("..//..//test//test2//test3");
    //bf::remove_directory("..\\..\\test", true);

    int file_size = bf::get_file_size("test.txt");

    cout << "file size (\"test.txt\"): " << file_size << std::endl;

    char *buffer = new char[file_size + 1];
    memset(buffer,0,file_size + 1);
    file_size = bf::load_file("test.txt", buffer, file_size);

    cout << "file size " << file_size << std::endl;

    cout << "data:\n" << buffer << std::endl;
    std::system("pause");
    return 0;
}

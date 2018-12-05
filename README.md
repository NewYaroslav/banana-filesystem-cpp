### banana-filesystem-cpp
Простая header-only С++ библиотека для работы с файловой системой

### Как пользоваться?

* Получить директорию исполняемого файла

```C++
#include <banana_filesystem.hpp>

//...

// выведем в консоль директорию исполняемого файла
cout << bf::get_current_work_dir() << endl;

```

* Проверить наличие файла

```C++

if(check_file("text.txt")) {
	// файл существует
} else {
	// файла нет
}

```

* Получить список файлов (или папок)

```C++

std::vector<std::string> list_files; // список файлов и прочего
/* чтобы получить список файлов только в директории "..//..//"
 * имена файлов будут без пути "..//..//"
 * например: "test.txt" "test2.txt"
 */
bf::get_list_files("..//..//", list_files, false); 

/* чтобы получить список файлов не только в директории "..//..//"
 * но и во всех вложенных папках. Все имена файлов будут начинаться с указанного пути
 * например: "..//..//test.txt" "..//..//test2.txt" "..//..//folder//test3.txt" "..//..//folder2//test4.txt"
 * bf::get_list_files("..//..//", list_files);
 * чтобы получить список только папок, и не только в директории "..//..//"
 * но и во всех вложенных папках. Все имена папок будут начинаться с указанного пути
 * например: "..//..//folder//test3.txt" "..//..//folder2//test4.txt"
 * bf::get_list_files("..//..//", list_files, true, true);
 */

// выводим список
for(size_t i = 0; i < list_files.size(); ++i) {
	std::cout << list_files[i] << std::endl;
}

```

* Разобрать путь на составляющие

```C++

string path = "C:/Users\\user/Downloads";

std::vector<std::string> list_dir;
bf::parse_path(path, list_dir);

for( auto & p : list_dir ) cout << "-> " << p << endl;

/* на экране будет выведено		
 * -> C:
 * -> Users
 * -> user
 * -> Downloads
 */

```

* Создать директорию

```C++

bf::create_directory("..//..//test//test2//test3");

```

* Удалить директорию

```C++

/* Данная функция удаляет директорию со всеми в ней файлами и папками
 * если параметр is_delete_root_directory true, функция удаляет также корневой каталог (который первым указан в path)
 * иначе функция оставляет паку
 */
bf::remove_dir("..\\..\\test", true);

```
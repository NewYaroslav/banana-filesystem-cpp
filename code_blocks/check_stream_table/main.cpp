#include <iostream>
#include <sstream>
#include <fstream>
#include "banana_filesystem_stream_table.hpp"

using namespace std;

void test_stream_table();
void test_stream_table_whith_file(std::string output_file_name) ;

int main(int argc, char *argv[]) {
    test_stream_table();
    test_stream_table_whith_file("test.txt");
    std::system("pause");
    return 0;
}

void test_stream_table() {
    bf::StreamTable st(std::cout);
    st.add_col(5);
    st.add_col(15);
    st.add_col(10);
    st.add_col(10);

    /* разкомментировать, если столбцы имеют одинаковую толщину */
    // st.Clear();
    // st.SetCols(4, 10);

    // st.SetVisible(1, false); // столбцы можно скрывать

    st.make_border_ext(true);
    st.set_delim_row(true, '-'); // st.set_delim_row(false); // без символов-разделителей строк
    st.set_delim_col(true, '|'); // st.set_delim_col(false); // без символов-разделителей строк

    /* заголовок и значения выводятся одинаково */
    st << "#" << "Property" << "Value" << "Unit";
    const size_t ROWS = 10;
    for(size_t i = 0; i < ROWS; i++) {
        st << i + 1 << "Prop" << i << "Unit";
    }
}

void test_stream_table_whith_file(std::string output_file_name) {
    std::ofstream file(output_file_name);
    bf::StreamTable st(file);
    st.add_col(5);
    st.add_col(15);
    st.add_col(10);
    st.add_col(10);

    /* разкомментировать, если столбцы имеют одинаковую толщину */
    // st.Clear();
    // st.SetCols(4, 10);

    // st.SetVisible(1, false); // столбцы можно скрывать

    st.make_border_ext(false);
    st.set_delim_row(false); // без символов-разделителей строк
    st.set_delim_col(false); // без символов-разделителей строк

    /* заголовок и значения выводятся одинаково */
    st << "#" << "Property" << "Value" << "Unit";
    const size_t ROWS = 10;
    for(size_t i = 0; i < ROWS; i++) {
        st << i + 1 << "Prop" << i << "Unit";
    }
    file.close();
}

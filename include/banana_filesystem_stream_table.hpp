#ifndef BANANA_FILESYSTEM_STREAM_TABLE_HPP
#define BANANA_FILESYSTEM_STREAM_TABLE_HPP

#undef max
#undef min

#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>

//стратегия формирования единой таблицы
#define CRLF "\n"

//стратегия построчной выгрузки таблицы
//#define CRLF std::endl
namespace bf {

    /** \brief Прямоугольная таблица с разделителями строк и столбцов
     * Синтаксис как у потоков C++
     */
    class StreamTable {
        protected:
        int colIndex_;

    private:
        bool borderExtOn_;
        bool delimRowOn_;
        char delimRow_;

        bool delimColOn_;
        char delimCol_;

        std::vector<int> colWidth_;
        bool firstCell_;
        std::vector<int> visible_;

        template <typename T>
        void push(const T &obj) {
            if (firstCell_) {
                if (borderExtOn_)
                    MakeRowBorder();

                firstCell_ = 0;
            }

            if (visible_[colIndex_]) {
                delim_col();

                os_.width(colWidth_[colIndex_]);
                os_.fill(' ');
                os_ << /*std::setiosflags(std::ios::left) << */obj;
            }

            if (++colIndex_ == (int)colWidth_.size()) {
                delim_col();

                if (delimRowOn_)
                    MakeRowBorder();
                else
                    os_ << CRLF;

                colIndex_ = 0;
            }
        }

        void MakeRowBorder() {
            os_ << CRLF;
            delim_col();

            int ic;
            for (ic = 0; ic < (int)colWidth_.size(); ic++) {
                if (visible_[ic]) {
                    os_.width(colWidth_[ic] + 1);
                    os_.fill(delimRow_);
                    delim_col();
                }
            }
            os_ << CRLF;
        }

        void delim_col() {
            if (delimColOn_ && (borderExtOn_ || colIndex_))
                os_ << delimCol_;
            else
                os_ << ' ';
        }

        //запрет на копирование
        StreamTable &operator = (const StreamTable &);

    public:
        std::ostream &os_;

        StreamTable(std::ostream &os = std::cout, char delimRow = ' ', char delimCol = ' ') :
            colIndex_(0),
            borderExtOn_(true),
            delimRowOn_(true),
            delimRow_(delimRow),
            delimColOn_(true),
            delimCol_(delimCol),
            firstCell_(1),
            os_(os) {}

        virtual ~StreamTable() {}

        virtual std::ostream &os() const {
            return os_;
        }

        /** \brief Отображать внешние границы
         * \param on Если true, то будут отображаться внешние границы
         */
        void make_border_ext(bool on) {
            borderExtOn_ = on;
        }

        /** \brief Установить символ разделителя строк
         * \param delimOn Если true, то будет использован символ разделителя строк
         * \param delimRow Символ разделителя строк
         */
        void set_delim_row(bool delimOn, char delimRow = ' ') {
            delimRowOn_ = delimOn;
            if(delimRowOn_) delimRow_ = delimRow;
            else if (!delimColOn_) make_border_ext(false);
        }

        /** \brief Установить символ разделителя столбцов
         * \param delimOn Если true, то будет использован символ разделителя столбцов
         * \param delimCol Символ разделителя столбцов
         */
        void set_delim_col(bool delimOn, char delimCol = ' ') {
            delimColOn_ = delimOn;
            if(delimColOn_) delimCol_ = delimCol;
            else if (!delimRowOn_) make_border_ext(false);
        }

        /** \brief Добавить столбец
         * \param colWidth Ширина столбца
         * \param visible Видимость
         */
        int add_col(int colWidth, bool visible = true) {
            colWidth_.push_back(colWidth);
            visible_.push_back(visible);
            return colWidth_.back();
        }

        /** \brief Установить видимость
         * \param col Столбец
         * \param flg Флаг видимости
         */
        void set_visible(int col, bool flg) {
            visible_[col - 1] = flg;
        }

        /** \brief Установить столбцы
         * \param colCount Количество столбцов
         * \param colWidth Ширина столбцов
         */
        void set_cols(int colCount, int colWidth = 0) {
            clear();
            for(int ic = 0; ic < colCount; ic++) {
                add_col(colWidth);
            }
        }

        /** \brief Очистить параметры
         */
        virtual void clear() {
            colWidth_.clear();
            visible_.clear();
            colIndex_ = 0;
            firstCell_ = 1;
        }

        /** \brief Добавить пустую строку
         */
        void add_empty_row() {
            for (int ic = 0; ic < (int)colWidth_.size(); ic++) {
                *this << "";
            }
        }

        template <typename T> StreamTable &operator << (const T &obj) {
            push(obj);
            return *this;
        }

        StreamTable &operator << (const std::string &s) {
            colWidth_[colIndex_] = std::max(colWidth_[colIndex_], (int)s.size() + 1);
            push(s);
            return *this;
        }

        StreamTable &operator << (const char *s) {
            colWidth_[colIndex_] = std::max(colWidth_[colIndex_], (int)strlen(s) + 1);
            push(s);
            return *this;
        }
    };
}
#endif // BANANA_FILESYSTEM_STREAM_TABLE_HPP

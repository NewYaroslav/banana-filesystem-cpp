#ifndef BANANA_FILESYSTEM_STORAGE_HPP_INCLUDED
#define BANANA_FILESYSTEM_STORAGE_HPP_INCLUDED

#include <iostream>
#include <map>
#include <vector>
#include <cstring>
#include <memory>
#include <mutex>
#include <atomic>
#include <future>
#include "sqlite3.h"

#define BF_STORAGE_TABLE_NAME "storage"

namespace bf {

    /** \brief Класс хранилища значений ключ-значение
     */
    class Storage {
    private:
        sqlite3 *db = 0;
        std::mutex db_mutex;

        std::map<std::string, std::pair<long, std::string>> data;   /**< Все данные типа "ключ - значение" */
        std::mutex data_mutex;

        std::map<std::string, std::pair<long, std::string>> buffer; /**< Все свежие данные типа "ключ - значение" */
        std::mutex buffer_mutex;

        std::atomic<bool> is_error = ATOMIC_VAR_INIT(false);        /**< Флаг ошибки */
        std::atomic<bool> is_open = ATOMIC_VAR_INIT(false);         /**< Флаг открытой базы данных */

        /** \brief Создаем или открываем таблицу
         * \param database_name Имя таблицы
         */
        void create_or_open_table(const std::string &database_name) {
            /* блокируем доступ к базе данных */
            std::lock_guard<std::mutex> lock(db_mutex);

            char *err = 0;

            /* таблица для хранения виртуальных аккаунтов */
            const char *create_table_sql =
                "CREATE TABLE IF NOT EXISTS " BF_STORAGE_TABLE_NAME " ("
                "id                             INT     PRIMARY KEY NOT NULL,"
                "key                            TEXT    NOT NULL,"
                "value                          TEXT    NOT NULL); ";

            /* открываем и возможно еще создаем таблицу */
            if(sqlite3_open(database_name.c_str(), &db) != SQLITE_OK) {
                std::cerr << "bf::Storage error opening / creating a database: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_close(db);
                db = 0;
                is_error = true;
                return;
            } else

            /* создаем таблицу в базе данных, если она еще не создана */
            if(sqlite3_exec(db, create_table_sql, callback, this, &err) != SQLITE_OK) {
                std::cerr << "bf::Storage SQL error: " << err << std::endl;
                sqlite3_free(err);
                sqlite3_close(db);
                db = 0;
                is_error = true;
                return;
            }

            /* читаем данные */
            {
                std::lock_guard<std::mutex> lock(data_mutex);
                const char *select_sql = "SELECT * from " BF_STORAGE_TABLE_NAME;
                if(sqlite3_exec(db, select_sql, callback, this, &err) != SQLITE_OK) {
                    std::cerr << "bf::Storage SQL error: " << err << std::endl;
                    sqlite3_free(err);
                    sqlite3_close(db);
                    db = 0;
                    is_error = true;
                    return;
                }
            }
            is_open = true;
        }

        /** \brief Вставляем элемент в таблицу
         * \param key Ключ
         * \param value Значение
         * \return Вернет true в случае успеха
         */
        bool insert_item_to_table(const std::string &key, const std::string &value) {
            /* блокируем доступ к базе данных */
            std::lock_guard<std::mutex> lock(db_mutex);

            /* проверяем наличие ошибок */
            if(!db) return false;
            if(is_error || !is_open) return false;

            /* получаем id */
            long id = 0;
            {
                std::lock_guard<std::mutex> lock(data);
                if(!data.empty()) {
                    auto it = data.begin();
                    auto it_end = std::next(it, data.size() - 1);
                    id = it_end->first + 1;
                }
            }

            /* формируем запрос */
            std::string str("INSERT INTO " BF_STORAGE_TABLE_NAME " ("
                "id,key,value) ");
            str += "VALUES (";
            str += std::to_string(id);
            str += ",'";
            str += key;
            str += "','";
            str += value;
            str += "'); ";
            str += "SELECT * from virtual_accounts";

            /* обновляем данные */
            std::lock_guard<std::mutex> lock(data_mutex);
            data.clear();

            char *err = 0;
            if(sqlite3_exec(db, buffer.c_str(), callback, this, &err) != SQLITE_OK) {
                std::cerr << "bf::Storage SQL error: " << err << std::endl;
                sqlite3_free(err);
                sqlite3_close(db);
                db = 0;
                is_error = true;
                return false;
            }
            return true;
        }

        bool update_item_to_table(const std::string &key, const std::string &value, const long id) {
            /* блокируем доступ к базе данных */
            std::lock_guard<std::mutex> lock(db_mutex);

            /* проверяем наличие ошибок */
            if(!db) return false;
            if(is_error || !is_open) return false;

            /* формируем запрос */
            std::string str("INSERT INTO " BF_STORAGE_TABLE_NAME " ("
                "id,key,value) ");
            str += "VALUES (";
            str += std::to_string(id);
            str += ",'";
            str += key;
            str += "','";
            str += value;
            str += "'); ";
            str += "SELECT * from virtual_accounts";

            /* обновляем данные */
            std::lock_guard<std::mutex> lock(data_mutex);
            data.clear();

            char *err = 0;
            if(sqlite3_exec(db, buffer.c_str(), callback, this, &err) != SQLITE_OK) {
                std::cerr << "bf::Storage SQL error: " << err << std::endl;
                sqlite3_free(err);
                sqlite3_close(db);
                db = 0;
                is_error = true;
                return false;
            }
            return true;
        }

        bool delete_virtual_account(const uint64_t va_id) {
            std::lock_guard<std::mutex> lock(va_editot_mutex);

            if(!db) return false;
            if(is_error) return false;
            std::string buffer("DELETE from virtual_accounts WHERE id = ");
            buffer += std::to_string(va_id);
            buffer += "; SELECT * from virtual_accounts";


            std::lock_guard<std::mutex> lock2(callback_virtual_accounts_mutex);
            callback_virtual_accounts.clear();

            char *err = 0;
            if(sqlite3_exec(db, buffer.c_str(), callback, this, &err) != SQLITE_OK) {
                std::cerr << "VirtualAccounts SQL error: " << err << std::endl;
                sqlite3_free(err);
                return false;
            } else {
                //std::cout << "VirtualAccounts DELETE created successfully" << std::endl;
            }

            /* блокируем доступ к virtual_accounts из других потоков */
            std::lock_guard<std::mutex> lock3(virtual_accounts_mutex);
            virtual_accounts = callback_virtual_accounts;
            return true;
        }
    public:

        template<typename T>
        constexpr bool is_number_v =
            std::is_same<T, uint8_t>::value ||
            std::is_same<T, int8_t>::value ||
            std::is_same<T, uint16_t>::value ||
            std::is_same<T, int16_t>::value ||
            std::is_same<T, uint32_t>::value ||
            std::is_same<T, int32_t>::value ||
            std::is_same<T, uint64_t>::value ||
            std::is_same<T, int64_t>::value ||
            std::is_same<T, float>::value ||
            std::is_same<T, double>::value;

        /** \brief Функция  обратного вызова для загрузки значений
         */
        static int callback(void *userdata, int argc, char **argv, char **key_name) {
            Storage* stor = static_cast<Storage*>(userdata);
            if(stor) {
                const char comp_str[3][6] = {"id","key","value"};
                const int comp_size[3] = {2,3,5};
                int counter = 0;
                long id = 0;
                std::string key, value;
                for(int i = 0; i < argc; ++i) {
                    for(int j = 0; j < 3; ++j) {
                        if(strncmp(key_name[i],comp_str[j],comp_size[j]) == 0) {
                            switch(j) {
                            case 0:
                                id = atoll(argv[i]);
                                break;
                            case 1:
                                key = std::string(argv[i]);
                                break;
                            case 2:
                                value = std::string(argv[i]);
                                break;
                            };
                            ++counter;
                        }
                    }
                }
                /* если есть пара ключ - значение, то добавляем данные */
                if(counter == 3) {
                    stor->data[key] = std::make_pair(id, value);
                }
            }
            /* далее вывод данных только для отладки */
#           if(0)
            for(int i = 0; i< argc; i++) {
                printf("%s = %s\n", key_name[i], argv[i] ? argv[i] : "NULL");
            }
            printf("\n");
#           endif
            return 0;
        }

        bool flush(const bool is_asyn = false) {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            for(auto &it : buffer) {
                bool is_insert = false;
                {
                    std::lock_guard<std::mutex> lock(data_mutex);
                    auto it = data.find(it.fisrt);
                    if(it == data.end()) is_insert = true;
                }
                if(is_insert) {
                    /* вставляем новое значение */
                    insert_item_to_table(it.fisrt, it.second.second);
                } else {
                    /* обновляем новое значение */
                    update_item_to_table(it.fisrt, it.second.second, it.second.fisrt);
                }
            }
            return !is_error;
        }

        void close() {
            std::lock_guard<std::mutex> lock(db_mutex);
            if(db) sqlite3_close(db);
        }

        void open(const std::string &database_name) {
            if(is_open) {
                close();
                is_open = false;
            }
        }

        Storage() {};

        Storage(const std::string &database_name) {
            create_or_open_table(database_name);
        };

        ~Storage() {

        };

        /** \brief Установить значение по ключу
         * \param key Ключ
         * \param value Значение
         * \param is_flush Выгрузить значения в хранилище. По умолчанию false
         * \param is_asyn Асинхронная выгрузка значений
         * \return Вернет true в случае успеха
         */
        bool set(const std::string &key, const std::string &value, const bool is_flush = false, const bool is_asyn = false) {
            if(is_error || !is_open) return false;
            {
                std::lock_guard<std::mutex> lock(buffer_mutex);
                buffer[key] = value;
            }
            if(is_flush) return flush(is_asyn);
            return true;
        }

        template<class T, class=std::enable_if_t<!is_number_v<T>>, class=void>
        bool set(const std::string &key, const T value, const bool is_flush = false, const bool is_asyn = false) = delete;

        /** \brief Установить значение по ключу
         * \param key Ключ
         * \param value Значение
         * \param is_flush Выгрузить значения в хранилище. По умолчанию false
         * \param is_asyn Асинхронная выгрузка значений
         * \return Вернет true в случае успеха
         */
        template<class T, class=std::enable_if_t<is_number_v<T>>>
        bool set(const std::string &key, const T value, const bool is_flush = false, const bool is_asyn = false) {
            return set(key, std::to_string(value),is_flush,is_asyn);
        }


        bool get(const std::string &key, std::string &value) {
            if(is_error || !is_open) return false;
            std::lock_guard<std::mutex> lock(data_mutex);
            auto it = data.find(key);
            if(it == data.end()) return false;
            value = it.second;
            return true;
        }

        std::string get(const std::string &key) {
            if(is_error || !is_open) return std::string();
            std::lock_guard<std::mutex> lock(data_mutex);
            auto it = data.find(key);
            if(it == data.end()) return std::string();
            return it.second;
        }

        bool get(const std::string &key, double &value) {
            if(is_error || !is_open) return false;
            std::lock_guard<std::mutex> lock(data_mutex);
            auto it = data.find(key);
            if(it == data.end()) return false;
            value = atof(it.second);
            return true;
        }
    };
}

#endif // BANANA_FILESYSTEM_STORAGE_HPP_INCLUDED

/*16.	Написать программу для учета сведений об ассортименте обуви в магазине фирмы.
Структура записи - артикул, наименование, количество, стоимость одной пары.
 Артикул начинается с буквы Ж для женской обуви, М для мужской, Д для детской.
 В программе необходимо предусмотреть:
1) функцию для удаления записей;
2) функцию поиска данных;
3) функцию сортировки записей по одному из полей.
 */

//#define NDEBUG
#include <iostream>
#include <assert.h>
#include <cstring>
#include <fstream>
#include <string>
#include <cstdio>
#include <sstream>
#include <unistd.h>

const int IS_VALID_SHOES_NAME_STATUS = 0;
const int IS_EMPTY_SHOES_NAME_STATUS = 11;
const int IS_OVERSIZE_SHOES_NAME_STATUS = 12;

const int IS_VALID_SHOES_COUNT_STATUS = 0;
const int IS_NEGATIVE_SHOES_COUNT_STATUS = 21;

const int IS_VALID_SHOES_PRICE_STATUS = 0;
const int IS_NEGATIVE_SHOES_PRICE_STATUS = 31;
const int IS_ZERO_SHOES_PRICE_STATUS = 32;

const int IS_VALID_SHOES_ARTICLE_STATUS = 0;

const int IS_VALID_SIZE_SHOES_ARTICLE_STATUS = 0;
const int IS_EMPTY_SHOES_ARTICLE_STATUS = 41;
const int IS_OVERSIZE_SHOES_ARTICLE_STATUS = 42;

const int IS_VALID_PREFIX_SHOES_ARTICLE_STATUS = 0;
const int IS_SHORTS_SHOES_ARTICLE_STATUS = 43;
const int IS_INCORECT_PREFIX_SHOES_ARTICLE_STATUS = 44;

const int IS_VALID_SHOES_STATUS = 0;

const int SUCCESSFUL_APPEND_SHOES_STATUS = 0;
const int ERROR_OPEN_SHOES_FILE_STATUS = 51;
const int ERROR_WRITE_SHOES_FILE_STATUS = 52;

const int SHOES_NOT_FOUND_STATUS = 0;
const int SHOES_FOUNDED_STATUS = 1;
const int IS_UNICUE_SHOES_STATUS = 0;
const int ISNT_UNICUE_SHOES_STATUS = 1;

const int IS_CORECT_OUTPUT_ASSORTIMENT_SHOES_STATUS = 0;
const int IS_CORECT_SORTED_ASSORTIMENT_SHOES_STATUS = 0;
const int IS_EMPTY_OUTPUT_ASSORTIMENT_SHOES_STATUS = 1;

const int IS_CORECT_SORTED_BY_PRICE_ASSORTIMENT_SHOES_STATUS = 0;
const int IS_CORECT_SORTED_BY_COUNT_ASSORTIMENT_SHOES_STATUS = 0;
const int IS_CORECT_SORTED_BY_ARTICLE_ASSORTIMENT_SHOES_STATUS = 0;
const int IS_CORECT_SORTED_BY_NAME_ASSORTIMENT_SHOES_STATUS = 0;

const int IS_CORECT_FIND_ASSORTIMENT_SHOES_STATUS = 0; 
const int IS_EMPTY_FIND_ASSORTIMENT_SHOES_STATUS = 61;
const int IS_CORECT_FIND_OUTPUT_ASSORTIMENT_SHOES_STATUS = 62;

const int IS_CORECT_REMOVE_SHOES_STATUS = 0;

const int SUCCESSFUL_REMOVE_SHOES_STATUS = 0;
const int CANT_OPEN_REMOVE_FILE_STATUS = 71;
const int ERRORE_REMOVE_SIZE_FILE_STATUS = 72;
const int ERRORE_REMOVE_SHOES_NOT_FOUND_IN_FILE_STATUS = 73;

const int SHOES_NAME_SIZE = 256;
const int SHOES_ARTICLE_SIZE = 32;

const char ASSORTMENT_SHOES_FILE_NAME[] = "assortment_shoes.bin";

struct shoes{
    char article[SHOES_ARTICLE_SIZE];
    char name[SHOES_NAME_SIZE];
    int count;
    double price;
};

shoes append_shoes_test_source;
shoes append_shoes_test_find_arg;
shoes append_shoes_test_give_out_arg;
bool append_shoes_test_find_called = false;
bool append_shoes_test_give_out_called = false;
int append_shoes_test_find_status = SHOES_NOT_FOUND_STATUS;
int append_shoes_test_give_out_status = SUCCESSFUL_APPEND_SHOES_STATUS;

shoes* get_sorted_test_assortment = nullptr;
shoes* get_sorted_test_sort_arg = nullptr;
shoes* get_sorted_test_output_arg = nullptr;
bool get_sorted_test_get_called = false;
bool get_sorted_test_sort_called = false;
bool get_sorted_test_output_called = false;
bool get_sorted_test_compare_called = false;
int get_sorted_test_output_status = IS_CORECT_OUTPUT_ASSORTIMENT_SHOES_STATUS;

shoes* finder_user_shoes_test_source = nullptr;

int get_shoes_array_count(shoes* assortment_shoes){
    if (assortment_shoes == nullptr){
        return 0;
    }

    int shoes_count = 0;
    while (!(assortment_shoes[shoes_count].article[0] == '\0' &&
        assortment_shoes[shoes_count].name[0] == '\0' &&
        assortment_shoes[shoes_count].count == 0 &&
        assortment_shoes[shoes_count].price == 0.0)){
        shoes_count++;
    }

    return shoes_count;
}

bool is_valid_prefix_shoes_article(const char* shoes_artcle){
    const size_t prefix_size = strlen("Ж");

    return ((strncmp(shoes_artcle, "Ж", prefix_size) == 0 ||
        strncmp(shoes_artcle, "М", prefix_size) == 0 ||
        strncmp(shoes_artcle, "Д", prefix_size) == 0));
}

bool is_oversize_shoes_article(const char* shoes_artcle){
    return strlen(shoes_artcle) > SHOES_ARTICLE_SIZE - 1;
}

bool is_shorts_prefix_article(const char* shoes_artcle){
    return strlen(shoes_artcle) < 3; // 3 из-за русского языка
}

int check_prefix_shoes_article(const char* shoes_artcle){
    if (is_shorts_prefix_article(shoes_artcle)){
        return IS_SHORTS_SHOES_ARTICLE_STATUS;
    }
    if (!is_valid_prefix_shoes_article(shoes_artcle)){
        return IS_INCORECT_PREFIX_SHOES_ARTICLE_STATUS;
    }
    return IS_VALID_PREFIX_SHOES_ARTICLE_STATUS;
}

int check_size_shoes_article(const char* shoes_artcle){
    if (strlen(shoes_artcle) == 0){
        return IS_EMPTY_SHOES_ARTICLE_STATUS;
    }
    if (is_oversize_shoes_article(shoes_artcle)){
        return IS_OVERSIZE_SHOES_ARTICLE_STATUS;
    }
    return IS_VALID_SIZE_SHOES_ARTICLE_STATUS;
}

int is_valid_shoes_article(const char* shoes_artcle){
    int prefix_size_status = check_size_shoes_article(shoes_artcle);
    if (prefix_size_status != IS_VALID_SIZE_SHOES_ARTICLE_STATUS){
        return prefix_size_status;
    }
    int prefix_status = check_prefix_shoes_article(shoes_artcle);
    if (prefix_status != IS_VALID_PREFIX_SHOES_ARTICLE_STATUS){
        return prefix_status;
    }
    return IS_VALID_SHOES_ARTICLE_STATUS;
}

int is_valid_shoes_name(const char* shoes_name){
    if (strlen(shoes_name) == 0){
        return IS_EMPTY_SHOES_NAME_STATUS;
    }
    if (strlen(shoes_name) > SHOES_NAME_SIZE - 1){
        return IS_OVERSIZE_SHOES_NAME_STATUS;
    }
    return IS_VALID_SHOES_NAME_STATUS;
}

int is_valid_shoes_count(int shoes_count){
    if (shoes_count < 0){
        return IS_NEGATIVE_SHOES_COUNT_STATUS;
    }
    return IS_VALID_SHOES_COUNT_STATUS;
}

int is_valid_shoes_price(double shoes_price){
    if (shoes_price == 0.0){
        return IS_ZERO_SHOES_PRICE_STATUS;
    }
    if (shoes_price < 0.0){
        return IS_NEGATIVE_SHOES_PRICE_STATUS;
    }
    return IS_VALID_SHOES_PRICE_STATUS;
}

int is_valid_shoes(shoes shoes_changed){
    int error_shoes_article_status = is_valid_shoes_article(shoes_changed.article);
    if (error_shoes_article_status != IS_VALID_SHOES_ARTICLE_STATUS){
        return error_shoes_article_status;
    }
    int error_shoes_name_status = is_valid_shoes_name(shoes_changed.name);
    if (error_shoes_name_status != IS_VALID_SHOES_NAME_STATUS){
        return error_shoes_name_status;
    }
    int error_shoes_count_status = is_valid_shoes_count(shoes_changed.count);
    if (error_shoes_count_status != IS_VALID_SHOES_COUNT_STATUS){
        return error_shoes_count_status;
    }
    int error_shoes_price_status = is_valid_shoes_price(shoes_changed.price);
    if (error_shoes_price_status != IS_VALID_SHOES_PRICE_STATUS){
        return error_shoes_price_status;
    }
    return IS_VALID_SHOES_STATUS;
}

int append_shoes_to_binary_file(shoes target_shoes){
    std::ofstream assortment_shoes_file(ASSORTMENT_SHOES_FILE_NAME, std::ios::binary | std::ios::app);

    if (!assortment_shoes_file.is_open()){
        return ERROR_OPEN_SHOES_FILE_STATUS;
    }

    assortment_shoes_file.write(reinterpret_cast<const char*>(&target_shoes), sizeof(target_shoes));
    if (!assortment_shoes_file.good()){
        assortment_shoes_file.close();
        return ERROR_WRITE_SHOES_FILE_STATUS;
    }

    assortment_shoes_file.close();

    return SUCCESSFUL_APPEND_SHOES_STATUS;
}

int find_shoes(shoes target_shoes){
    shoes current_shoes;
    std::ifstream assortment_shoes_file(ASSORTMENT_SHOES_FILE_NAME, std::ios::binary);

    if (!assortment_shoes_file.is_open()){
        return ERROR_OPEN_SHOES_FILE_STATUS;
    }

    while (assortment_shoes_file.read(reinterpret_cast<char*>(&current_shoes), sizeof(current_shoes))){
        int error_shoes_status = is_valid_shoes(current_shoes);
        if (error_shoes_status != IS_VALID_SHOES_STATUS){
            return error_shoes_status;
        }
        
        if (strcmp(current_shoes.article, target_shoes.article) == 0){
            assortment_shoes_file.close();
            return SHOES_FOUNDED_STATUS;
        }
    }

    assortment_shoes_file.close();
    return SHOES_NOT_FOUND_STATUS;
}



int is_unicue_shoos(shoes target_shoes, int (*find_shoos)(shoes)){
    int is_unicue_shoes_status = find_shoos(target_shoes);
    if (is_unicue_shoes_status == SHOES_NOT_FOUND_STATUS){
        return IS_UNICUE_SHOES_STATUS;
    } else if (is_unicue_shoes_status == SHOES_FOUNDED_STATUS){
        return ISNT_UNICUE_SHOES_STATUS;
    }
    return is_unicue_shoes_status;
}

int append_shoes(shoes (*get_shoes)(),int (*give_out_shoes)(shoes), int (*find_shoos)(shoes)){
    shoes target_shoes = get_shoes();

    int error_shoes_status = is_valid_shoes(target_shoes);
    if (error_shoes_status != IS_VALID_SHOES_STATUS){
        return error_shoes_status;
    }

    int unicue_shoes_status = is_unicue_shoos(target_shoes, find_shoos);
    if (unicue_shoes_status != IS_UNICUE_SHOES_STATUS)
    {
        return unicue_shoes_status;
    }

    int out_shoes_status = give_out_shoes(target_shoes);
    if (out_shoes_status != SUCCESSFUL_APPEND_SHOES_STATUS){
        return out_shoes_status;
    }

    return SUCCESSFUL_APPEND_SHOES_STATUS;
}

shoes* get_assortment_shoes(){
    std::ifstream assortment_shoes_file(ASSORTMENT_SHOES_FILE_NAME, std::ios::binary | std::ios::ate);

    if (!assortment_shoes_file.is_open()){
        return nullptr;
    }

    std::streamsize assortment_shoes_file_size = assortment_shoes_file.tellg();
    if (assortment_shoes_file_size <= 0 || assortment_shoes_file_size % static_cast<std::streamsize>(sizeof(shoes)) != 0){
        assortment_shoes_file.close();
        return nullptr;
    }

    int assortment_shoes_count = static_cast<int>(assortment_shoes_file_size / static_cast<std::streamsize>(sizeof(shoes)));
    shoes* assortment_shoes = new shoes[assortment_shoes_count + 1]{};

    assortment_shoes_file.seekg(0, std::ios::beg);
    assortment_shoes_file.read(reinterpret_cast<char*>(assortment_shoes), assortment_shoes_file_size);
    if (!assortment_shoes_file.good()){
        assortment_shoes_file.close();
        delete[] assortment_shoes;
        return nullptr;
    }

    assortment_shoes_file.close();

    assortment_shoes[assortment_shoes_count] = {};
    return assortment_shoes;
}

bool is_valid_assortment_shoes(shoes* assortment_shoes){
    if (assortment_shoes == nullptr){
        return false;
    }
    int assortment_shoes_count = get_shoes_array_count(assortment_shoes);
    for (int i = 0; i < assortment_shoes_count; i++){
        if (is_valid_shoes(assortment_shoes[i]) != IS_VALID_SHOES_STATUS){
            return false;
        }
    }
    return true;
}

bool find_user_changed_article_shoes(shoes first_shoes_fr_collation,shoes second_shoes_fr_collation){
    size_t first_article_length = strlen(first_shoes_fr_collation.article);
    size_t second_article_length = strlen(second_shoes_fr_collation.article);
    size_t min_article_length = first_article_length;

    if (second_article_length < min_article_length){
        min_article_length = second_article_length;
    }

    for (int i = 0; i < min_article_length; i++){
        if (first_shoes_fr_collation.article[i] < second_shoes_fr_collation.article[i]){
            return false;
        }
        if (first_shoes_fr_collation.article[i] > second_shoes_fr_collation.article[i]){
            return true;
        }
    }

    if (first_article_length > second_article_length){
        return true;
    }

    return false;
}

bool find_user_changed_name_shoes(shoes first_shoes_fr_collation,shoes second_shoes_fr_collation){
    size_t first_name_length = strlen(first_shoes_fr_collation.name);
    size_t second_name_length = strlen(second_shoes_fr_collation.name);
    size_t min_name_length = first_name_length;

    if (second_name_length < min_name_length){
        min_name_length = second_name_length;
    }

    for (int i = 0; i < min_name_length; i++){
        if (first_shoes_fr_collation.name[i] < second_shoes_fr_collation.name[i]){
            return false;
        }
        if (first_shoes_fr_collation.name[i] > second_shoes_fr_collation.name[i]){
            return true;
        }
    }

    if (first_name_length > second_name_length){
        return true;
    }

    return false;
}

bool find_user_changed_count_shoes(shoes first_shoes_fr_collation,shoes second__shoes_fr_collation){
    if (first_shoes_fr_collation.count > second__shoes_fr_collation.count){
        return true;
    }
    return false;
}

bool find_user_changed_price_shoes(shoes first_shoes_fr_collation,shoes second__shoes_fr_collation){
    if (first_shoes_fr_collation.price > second__shoes_fr_collation.price){
        return true;
    }
    return false;
}

int output_sorted_assortiment_shoes(shoes* assortment_shoes){
    int assortment_shoes_count = get_shoes_array_count(assortment_shoes);
    if (assortment_shoes_count == 0){
        std::cout << " Массив в данный момент пуст. ";
        return IS_CORECT_OUTPUT_ASSORTIMENT_SHOES_STATUS;
    }

    std::cout << std::left
              << std::setw(12) << "Артикул"
              << std::setw(28) << "Название"
              << std::setw(12) << "Количество"
              << std::setw(12) << "Цена" << '\n';
    std::cout << std::string(64, '-') << '\n';

    for (int i = 0; i < assortment_shoes_count; i++){
        std::cout << std::left
                  << std::setw(12) << assortment_shoes[i].article
                  << std::setw(28) << assortment_shoes[i].name
                  << std::setw(12) << assortment_shoes[i].count
                  << std::fixed << std::setprecision(2)
                  << std::setw(12) << assortment_shoes[i].price << '\n';
    }

    return IS_CORECT_OUTPUT_ASSORTIMENT_SHOES_STATUS;
}

int get_sorted_assortment_shoes(shoes* (*get_assortment_shoes)(),int (*output_sorted_assortiment_shoes)(shoes*),void (*sort_assortment_shoes)(shoes*, bool (*find_user_changed_shoes)(shoes,shoes)),bool (*find_user_changed_shoes)(shoes,shoes)){
    shoes* assortment_shoes = get_assortment_shoes();
    if (!is_valid_assortment_shoes(assortment_shoes)){
        delete[] assortment_shoes;
        assortment_shoes = nullptr;
        return IS_EMPTY_OUTPUT_ASSORTIMENT_SHOES_STATUS;
    }
    sort_assortment_shoes(assortment_shoes,find_user_changed_shoes);
    int output_assortiment_shoes_status = output_sorted_assortiment_shoes(assortment_shoes);
    
    if (assortment_shoes != nullptr)
    {
        delete[] assortment_shoes;
        assortment_shoes = nullptr;
    }

    if (output_assortiment_shoes_status != IS_CORECT_OUTPUT_ASSORTIMENT_SHOES_STATUS)
    {
        return output_assortiment_shoes_status; 
    }
    return IS_CORECT_SORTED_ASSORTIMENT_SHOES_STATUS;
}

void sort_assortment_shoes(shoes* assortment_shoes,bool (*find_user_changed_shoes)(shoes,shoes)){
    if (assortment_shoes == nullptr || find_user_changed_shoes == nullptr){
        return;
    }
    int assortment_shoes_count = get_shoes_array_count(assortment_shoes);
    for (int i = 0; i < assortment_shoes_count - 1; i++){
        for (int j = 0; j < assortment_shoes_count - i - 1; j++){
            if (find_user_changed_shoes(assortment_shoes[j],assortment_shoes[j + 1])){
                shoes buf_shoes = assortment_shoes[j];
                assortment_shoes[j] = assortment_shoes[j + 1];
                assortment_shoes[j + 1] = buf_shoes;
            }
        }
    }
}

int sort_by_price_assortment_shoes(){
    int sorted_status = get_sorted_assortment_shoes(get_assortment_shoes,output_sorted_assortiment_shoes,sort_assortment_shoes,find_user_changed_price_shoes);
    if (sorted_status != IS_CORECT_SORTED_ASSORTIMENT_SHOES_STATUS){
        return sorted_status;
    }
    return IS_CORECT_SORTED_BY_PRICE_ASSORTIMENT_SHOES_STATUS;
}

int sort_by_count_assortment_shoes(){
    int sorted_status = get_sorted_assortment_shoes(get_assortment_shoes,output_sorted_assortiment_shoes,sort_assortment_shoes,find_user_changed_count_shoes);
    if (sorted_status != IS_CORECT_SORTED_ASSORTIMENT_SHOES_STATUS){
        return sorted_status;
    }
    return IS_CORECT_SORTED_BY_COUNT_ASSORTIMENT_SHOES_STATUS;
}

int sort_by_article_assortment_shoes(){
    int sorted_status = get_sorted_assortment_shoes(get_assortment_shoes,output_sorted_assortiment_shoes,sort_assortment_shoes,find_user_changed_article_shoes);
    if (sorted_status != IS_CORECT_SORTED_ASSORTIMENT_SHOES_STATUS){
        return sorted_status;
    }
    return IS_CORECT_SORTED_BY_ARTICLE_ASSORTIMENT_SHOES_STATUS;
}

int sort_by_name_assortment_shoes(){
    int sorted_status = get_sorted_assortment_shoes(get_assortment_shoes,output_sorted_assortiment_shoes,sort_assortment_shoes,find_user_changed_name_shoes);
    if (sorted_status != IS_CORECT_SORTED_ASSORTIMENT_SHOES_STATUS){
        return sorted_status;
    }
    return IS_CORECT_SORTED_BY_NAME_ASSORTIMENT_SHOES_STATUS;
}

void print_output_assortment_error(int status){
    if (status == IS_EMPTY_OUTPUT_ASSORTIMENT_SHOES_STATUS){
        std::cout << "Ошибка: ассортимент пуст или невалиден.\n";
    } else if (status == ERROR_OPEN_SHOES_FILE_STATUS){
        std::cout << "Ошибка: не удалось открыть файл ассортимента.\n";
    } else if (status == ERROR_WRITE_SHOES_FILE_STATUS){
        std::cout << "Ошибка: не удалось вывести ассортимент.\n";
    } else if (status == IS_EMPTY_SHOES_ARTICLE_STATUS){
        std::cout << "Ошибка: обнаружена запись с пустым артикулом.\n";
    } else if (status == IS_OVERSIZE_SHOES_ARTICLE_STATUS){
        std::cout << "Ошибка: обнаружена запись со слишком длинным артикулом.\n";
    } else if (status == IS_SHORTS_SHOES_ARTICLE_STATUS){
        std::cout << "Ошибка: обнаружена запись со слишком коротким артикулом.\n";
    } else if (status == IS_INCORECT_PREFIX_SHOES_ARTICLE_STATUS){
        std::cout << "Ошибка: обнаружена запись с некорректным префиксом артикула.\n";
    } else if (status == IS_EMPTY_SHOES_NAME_STATUS){
        std::cout << "Ошибка: обнаружена запись с пустым названием.\n";
    } else if (status == IS_OVERSIZE_SHOES_NAME_STATUS){
        std::cout << "Ошибка: обнаружена запись со слишком длинным названием.\n";
    } else if (status == IS_NEGATIVE_SHOES_COUNT_STATUS){
        std::cout << "Ошибка: обнаружена запись с отрицательным количеством.\n";
    } else if (status == IS_ZERO_SHOES_PRICE_STATUS){
        std::cout << "Ошибка: обнаружена запись с нулевой ценой.\n";
    } else if (status == IS_NEGATIVE_SHOES_PRICE_STATUS){
        std::cout << "Ошибка: обнаружена запись с отрицательной ценой.\n";
    } else {
        std::cout << "Ошибка: неизвестный статус " << status << ".\n";
    }
}

void display_assortment_shoes() {
    int choice = -1;

    while (choice != 0) {
        std::cout << std::string(64, '-') << '\n';
        std::cout << std::string(24, ' ') << "Вывод ассортимента обуви(сортировка):" << '\n';
        std::cout << std::string(64, '-') << '\n';

        std::cout << " 1 - по артиклю \n";
        std::cout << " 2 - по названию \n";
        std::cout << " 3 - по количеству \n";
        std::cout << " 4 - по цене \n";
        std::cout << " 0 - вернуться обратно \n";
        std::cout << std::string(64, '-') << '\n';

        std::cout << "Выберите действие: ";
        std::cin >> choice;

        if (!std::cin.good()){
            std::cout << "Ошибка: введите номер действия.\n";
            std::cin.clear();
            std::cin.ignore(32767, '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                int status = sort_by_article_assortment_shoes();
                if (status != IS_CORECT_SORTED_BY_ARTICLE_ASSORTIMENT_SHOES_STATUS){
                    print_output_assortment_error(status);
                }
                return;
            }
            case 2: {
                int status = sort_by_name_assortment_shoes();
                if (status != IS_CORECT_SORTED_BY_NAME_ASSORTIMENT_SHOES_STATUS){
                    print_output_assortment_error(status);
                }
                return;
            }
            case 3: {
                int status = sort_by_count_assortment_shoes();
                if (status != IS_CORECT_SORTED_BY_COUNT_ASSORTIMENT_SHOES_STATUS){
                    print_output_assortment_error(status);
                }
                return;
            }
            case 4: {
                int status = sort_by_price_assortment_shoes();
                if (status != IS_CORECT_SORTED_BY_PRICE_ASSORTIMENT_SHOES_STATUS){
                    print_output_assortment_error(status);
                }
                return;
            }
            case 0:
                break;
            default:
                std::cout << "\nНеверный ввод! Попробуйте еще раз.\n";
                std::cin.clear();
                std::cin.ignore(32767, '\n');
        }
        std::cout << '\n';
    }
}

void get_shoes_article(char* article){
    while (true){
        std::string article_input;
        std::cout << "Введите артикул (Ж/М/Д...): ";
        std::getline(std::cin >> std::ws, article_input);

        int article_status = is_valid_shoes_article(article_input.c_str());
        if (article_status == IS_VALID_SHOES_ARTICLE_STATUS){
            std::strncpy(article, article_input.c_str(), SHOES_ARTICLE_SIZE - 1);
            article[SHOES_ARTICLE_SIZE - 1] = '\0';
            return;
        }

        if (article_status == IS_EMPTY_SHOES_ARTICLE_STATUS){
            std::cout << "Ошибка: артикул не может быть пустым.\n";
        } else if (article_status == IS_OVERSIZE_SHOES_ARTICLE_STATUS){
            std::cout << "Ошибка: слишком длинный артикул.\n";
        } else if (article_status == IS_SHORTS_SHOES_ARTICLE_STATUS){
            std::cout << "Ошибка: артикул слишком короткий.\n";
        } else if (article_status == IS_INCORECT_PREFIX_SHOES_ARTICLE_STATUS){
            std::cout << "Ошибка: артикул должен начинаться с Ж, М или Д.\n";
        }
    }
}

void get_shoes_name(char* name){
    while (true){
        std::string name_input;
        std::cout << "Введите название обуви: ";
        std::getline(std::cin >> std::ws, name_input);

        int name_status = is_valid_shoes_name(name_input.c_str());
        if (name_status == IS_VALID_SHOES_NAME_STATUS){
            std::strncpy(name, name_input.c_str(), SHOES_NAME_SIZE - 1);
            name[SHOES_NAME_SIZE - 1] = '\0';
            return;
        }

        if (name_status == IS_EMPTY_SHOES_NAME_STATUS){
            std::cout << "Ошибка: название не может быть пустым.\n";
        } else if (name_status == IS_OVERSIZE_SHOES_NAME_STATUS){
            std::cout << "Ошибка: слишком длинное название.\n";
        }
    }
}

void get_shoes_count(int& count){
    while (true){
        std::cout << "Введите количество пар: ";
        std::cin >> count;

        if (!std::cin.good()){
            std::cout << "Ошибка: введите целое число.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        int count_status = is_valid_shoes_count(count);
        if (count_status == IS_VALID_SHOES_COUNT_STATUS){
            return;
        }

        if (count_status == IS_NEGATIVE_SHOES_COUNT_STATUS){
            std::cout << "Ошибка: количество не может быть отрицательным.\n";
        }
    }
}

void get_shoes_price(double& price){
    while (true){
        std::cout << "Введите цену за пару: ";
        std::cin >> price;

        if (!std::cin.good()){
            std::cout << "Ошибка: введите число.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        int price_status = is_valid_shoes_price(price);
        if (price_status == IS_VALID_SHOES_PRICE_STATUS){
            return;
        }

        if (price_status == IS_ZERO_SHOES_PRICE_STATUS){
            std::cout << "Ошибка: цена не может быть равна 0.\n";
        } else if (price_status == IS_NEGATIVE_SHOES_PRICE_STATUS){
            std::cout << "Ошибка: цена не может быть отрицательной.\n";
        }
    }
}

shoes get_shoes() {
    shoes target_shoes = {};
    get_shoes_article(target_shoes.article);
    get_shoes_name(target_shoes.name);
    get_shoes_count(target_shoes.count);
    get_shoes_price(target_shoes.price);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return target_shoes;
}

void print_add_new_record_error(int status){
    if (status == ERROR_OPEN_SHOES_FILE_STATUS){
        std::cout << "Ошибка: не удалось открыть файл ассортимента.\n";
    } else if (status == ERROR_WRITE_SHOES_FILE_STATUS){
        std::cout << "Ошибка: не удалось сохранить запись в файл.\n";
    } else if (status == ISNT_UNICUE_SHOES_STATUS){
        std::cout << "Ошибка: запись с таким артикулом уже существует.\n";
    } else if (status == IS_EMPTY_SHOES_ARTICLE_STATUS){
        std::cout << "Ошибка: артикул не может быть пустым.\n";
    } else if (status == IS_OVERSIZE_SHOES_ARTICLE_STATUS){
        std::cout << "Ошибка: слишком длинный артикул.\n";
    } else if (status == IS_SHORTS_SHOES_ARTICLE_STATUS){
        std::cout << "Ошибка: артикул слишком короткий.\n";
    } else if (status == IS_INCORECT_PREFIX_SHOES_ARTICLE_STATUS){
        std::cout << "Ошибка: артикул должен начинаться с Ж, М или Д.\n";
    } else if (status == IS_EMPTY_SHOES_NAME_STATUS){
        std::cout << "Ошибка: название не может быть пустым.\n";
    } else if (status == IS_OVERSIZE_SHOES_NAME_STATUS){
        std::cout << "Ошибка: слишком длинное название.\n";
    } else if (status == IS_NEGATIVE_SHOES_COUNT_STATUS){
        std::cout << "Ошибка: количество не может быть отрицательным.\n";
    } else if (status == IS_ZERO_SHOES_PRICE_STATUS){
        std::cout << "Ошибка: цена не может быть равна 0.\n";
    } else if (status == IS_NEGATIVE_SHOES_PRICE_STATUS){
        std::cout << "Ошибка: цена не может быть отрицательной.\n";
    } else {
        std::cout << "Ошибка: неизвестный статус " << status << ".\n";
    }
}

void add_new_record_shoes() {
    int append_status = append_shoes(get_shoes, append_shoes_to_binary_file, find_shoes);
    if (append_status != SUCCESSFUL_APPEND_SHOES_STATUS){
        print_add_new_record_error(append_status);
        return;
    }

    std::cout << "Запись успешно добавлена.\n";
}

int output_find_assortiment_shoes(shoes* assortment_shoes){
    int assortment_shoes_count = get_shoes_array_count(assortment_shoes);
    if (assortment_shoes_count == 0){
        std::cout << " Массив в данный момент пуст. ";
        return IS_CORECT_OUTPUT_ASSORTIMENT_SHOES_STATUS;
    }

    std::cout << std::left
              << std::setw(12) << "Артикул"
              << std::setw(28) << "Название"
              << std::setw(12) << "Количество"
              << std::setw(12) << "Цена" << '\n';
    std::cout << std::string(64, '-') << '\n';

    for (int i = 0; i < assortment_shoes_count; i++){
        std::cout << std::left
                  << std::setw(12) << assortment_shoes[i].article
                  << std::setw(28) << assortment_shoes[i].name
                  << std::setw(12) << assortment_shoes[i].count
                  << std::fixed << std::setprecision(2)
                  << std::setw(12) << assortment_shoes[i].price << '\n';
    }

    return IS_CORECT_OUTPUT_ASSORTIMENT_SHOES_STATUS;
}

bool find_condition_by_count_shoes(shoes first_shoes,shoes second_shoes){
    return (first_shoes.count == second_shoes.count);
}

bool find_condition_by_price_shoes(shoes first_shoes,shoes second_shoes){
    return (first_shoes.price == second_shoes.price);
}

bool find_condition_by_article_shoes(shoes first_shoes,shoes second_shoes){
    return (strcmp(first_shoes.article, second_shoes.article) == 0);
}

bool find_condition_by_name_shoes(shoes first_shoes,shoes second_shoes){
    return (strcmp(first_shoes.name, second_shoes.name) == 0);
}

shoes* find_user_shoes(shoes find_shoes, bool (*find_user_changed_shoes)(shoes,shoes)){
    shoes current_shoes;
    std::ifstream assortment_shoes_file(ASSORTMENT_SHOES_FILE_NAME, std::ios::binary);

    if (!assortment_shoes_file.is_open() || find_user_changed_shoes == nullptr){
        return nullptr;
    }

    int matched_shoes_count = 0;
    while (assortment_shoes_file.read(reinterpret_cast<char*>(&current_shoes), sizeof(current_shoes))){
        int error_shoes_status = is_valid_shoes(current_shoes);
        if (error_shoes_status != IS_VALID_SHOES_STATUS){
            return nullptr;
        }

        if (find_user_changed_shoes(find_shoes, current_shoes)){
            matched_shoes_count++;
        }
    }

    shoes* assortment_shoes = new shoes[matched_shoes_count + 1]{};

    assortment_shoes_file.clear();
    assortment_shoes_file.seekg(0, std::ios::beg);

    int write_index = 0;
    while (assortment_shoes_file.read(reinterpret_cast<char*>(&current_shoes), sizeof(current_shoes))){
        if (find_user_changed_shoes(find_shoes, current_shoes)){
            assortment_shoes[write_index] = current_shoes;
            write_index++;
        }
    }

    assortment_shoes_file.close();
    return assortment_shoes;
}

int finder_user_shoes(shoes find_shoes, shoes* (*get_assortment_shoes)(shoes, bool (*find_user_changed_shoes)(shoes,shoes)),int (*output_find_assortiment_shoes)(shoes*),bool (*find_user_changed_shoes)(shoes,shoes)){
    shoes* assortment_shoes = get_assortment_shoes(find_shoes, find_user_changed_shoes);
    if (!is_valid_assortment_shoes(assortment_shoes)){
        delete[] assortment_shoes;
        assortment_shoes = nullptr;
        return IS_EMPTY_FIND_ASSORTIMENT_SHOES_STATUS;
    }
    int output_assortiment_shoes_status = output_sorted_assortiment_shoes(assortment_shoes);
    
    delete[] assortment_shoes;
    assortment_shoes = nullptr;

    if (output_assortiment_shoes_status != IS_CORECT_FIND_OUTPUT_ASSORTIMENT_SHOES_STATUS)
    {
        return output_assortiment_shoes_status; 
    }
    return IS_CORECT_FIND_ASSORTIMENT_SHOES_STATUS;
}

void print_find_by_article_error(int find_shoes_status){
    if (find_shoes_status == IS_EMPTY_FIND_ASSORTIMENT_SHOES_STATUS){
        std::cout << "Ошибка: записи по заданному артикулу не найдены или ассортимент пуст.\n";
    } else if (find_shoes_status == IS_CORECT_FIND_OUTPUT_ASSORTIMENT_SHOES_STATUS){
        std::cout << "Поиск выполнен успешно.\n";
    } else if (find_shoes_status == ERROR_OPEN_SHOES_FILE_STATUS){
        std::cout << "Ошибка: не удалось открыть файл ассортимента.\n";
    } else if (find_shoes_status == ERROR_WRITE_SHOES_FILE_STATUS){
        std::cout << "Ошибка: не удалось вывести найденные записи.\n";
    } else if (find_shoes_status == IS_EMPTY_SHOES_ARTICLE_STATUS){
        std::cout << "Ошибка: обнаружена запись с пустым артикулом.\n";
    } else if (find_shoes_status == IS_OVERSIZE_SHOES_ARTICLE_STATUS){
        std::cout << "Ошибка: обнаружена запись со слишком длинным артикулом.\n";
    } else if (find_shoes_status == IS_SHORTS_SHOES_ARTICLE_STATUS){
        std::cout << "Ошибка: обнаружена запись со слишком коротким артикулом.\n";
    } else if (find_shoes_status == IS_INCORECT_PREFIX_SHOES_ARTICLE_STATUS){
        std::cout << "Ошибка: обнаружена запись с некорректным префиксом артикула.\n";
    } else if (find_shoes_status == IS_EMPTY_SHOES_NAME_STATUS){
        std::cout << "Ошибка: обнаружена запись с пустым названием.\n";
    } else if (find_shoes_status == IS_OVERSIZE_SHOES_NAME_STATUS){
        std::cout << "Ошибка: обнаружена запись со слишком длинным названием.\n";
    } else if (find_shoes_status == IS_NEGATIVE_SHOES_COUNT_STATUS){
        std::cout << "Ошибка: обнаружена запись с отрицательным количеством.\n";
    } else if (find_shoes_status == IS_ZERO_SHOES_PRICE_STATUS){
        std::cout << "Ошибка: обнаружена запись с нулевой ценой.\n";
    } else if (find_shoes_status == IS_NEGATIVE_SHOES_PRICE_STATUS){
        std::cout << "Ошибка: обнаружена запись с отрицательной ценой.\n";
    } else {
        std::cout << "Ошибка: неизвестный статус " << find_shoes_status << ".\n";
    }
}

void user_shoes_to_find_by_article(){
    while (true){
        std::string article_input;
        std::cout << "Введите артикул (Ж/М/Д...): ";
        std::getline(std::cin >> std::ws, article_input);

        int article_status = is_valid_shoes_article(article_input.c_str());
        if (article_status == IS_VALID_SHOES_ARTICLE_STATUS){

            shoes cur_shoes;
            std::strncpy(cur_shoes.article, article_input.c_str(), SHOES_ARTICLE_SIZE - 1);
            cur_shoes.article[SHOES_ARTICLE_SIZE - 1] = '\0';
            cur_shoes.name[0] = '\0';
            cur_shoes.count = 0;
            cur_shoes.price = 0.0;

            int find_shoes_status = finder_user_shoes(cur_shoes,find_user_shoes,output_find_assortiment_shoes,find_condition_by_article_shoes);
            if (find_shoes_status != IS_CORECT_FIND_ASSORTIMENT_SHOES_STATUS){
                print_find_by_article_error(find_shoes_status);
            }
            return;
        }

        if (article_status == IS_EMPTY_SHOES_ARTICLE_STATUS){
            std::cout << "Ошибка: артикул не может быть пустым.\n";
        } else if (article_status == IS_OVERSIZE_SHOES_ARTICLE_STATUS){
            std::cout << "Ошибка: слишком длинный артикул.\n";
        } else if (article_status == IS_SHORTS_SHOES_ARTICLE_STATUS){
            std::cout << "Ошибка: артикул слишком короткий.\n";
        } else if (article_status == IS_INCORECT_PREFIX_SHOES_ARTICLE_STATUS){
            std::cout << "Ошибка: артикул должен начинаться с Ж, М или Д.\n";
        }
    }  
}

void user_shoes_to_find_by_name(){
    while (true){
        std::string name_input;
        std::cout << "Введите название обуви: ";
        std::getline(std::cin >> std::ws, name_input);

        int name_status = is_valid_shoes_name(name_input.c_str());
        if (name_status == IS_VALID_SHOES_NAME_STATUS){
            shoes cur_shoes;
            cur_shoes.article[0] = '\0';
            std::strncpy(cur_shoes.name, name_input.c_str(), SHOES_NAME_SIZE - 1);
            cur_shoes.name[SHOES_NAME_SIZE - 1] = '\0';
            cur_shoes.count = 0;
            cur_shoes.price = 0.0;

            int find_shoes_status = finder_user_shoes(cur_shoes,find_user_shoes,output_find_assortiment_shoes,find_condition_by_name_shoes);
            if (find_shoes_status != IS_CORECT_FIND_ASSORTIMENT_SHOES_STATUS){
                print_find_by_article_error(find_shoes_status);
            }
            return;
        }

        if (name_status == IS_EMPTY_SHOES_NAME_STATUS){
            std::cout << "Ошибка: название не может быть пустым.\n";
        } else if (name_status == IS_OVERSIZE_SHOES_NAME_STATUS){
            std::cout << "Ошибка: слишком длинное название.\n";
        }
    }
}

void user_shoes_to_find_by_count(){
    while (true){
        int count_input;
        std::cout << "Введите количество пар: ";
        std::cin >> count_input;

        if (!std::cin.good()){
            std::cout << "Ошибка: введите целое число.\n";
            std::cin.clear();
            std::cin.ignore(32767, '\n');
            continue;
        }

        int count_status = is_valid_shoes_count(count_input);
        if (count_status == IS_VALID_SHOES_COUNT_STATUS){
            shoes cur_shoes;
            cur_shoes.article[0] = '\0';
            cur_shoes.name[0] = '\0';
            cur_shoes.count = count_input;
            cur_shoes.price = 0.0;

            int find_shoes_status = finder_user_shoes(cur_shoes,find_user_shoes,output_find_assortiment_shoes,find_condition_by_count_shoes);
            if (find_shoes_status != IS_CORECT_FIND_ASSORTIMENT_SHOES_STATUS){
                print_find_by_article_error(find_shoes_status);
            }
            return;
        }

        if (count_status == IS_NEGATIVE_SHOES_COUNT_STATUS){
            std::cout << "Ошибка: количество не может быть отрицательным.\n";
        }
    }
}

void user_shoes_to_find_by_price(){
    while (true){
        double price_input;
        std::cout << "Введите цену за пару: ";
        std::cin >> price_input;

        if (!std::cin.good()){
            std::cout << "Ошибка: введите число.\n";
            std::cin.clear();
            std::cin.ignore(32767, '\n');
            continue;
        }

        int price_status = is_valid_shoes_price(price_input);
        if (price_status == IS_VALID_SHOES_PRICE_STATUS){
            shoes cur_shoes;
            cur_shoes.article[0] = '\0';
            cur_shoes.name[0] = '\0';
            cur_shoes.count = 0;
            cur_shoes.price = price_input;

            int find_shoes_status = finder_user_shoes(cur_shoes,find_user_shoes,output_find_assortiment_shoes,find_condition_by_price_shoes);
            if (find_shoes_status != IS_CORECT_FIND_ASSORTIMENT_SHOES_STATUS){
                print_find_by_article_error(find_shoes_status);
            }
            return;
        }

        if (price_status == IS_ZERO_SHOES_PRICE_STATUS){
            std::cout << "Ошибка: цена не может быть равна 0.\n";
        } else if (price_status == IS_NEGATIVE_SHOES_PRICE_STATUS){
            std::cout << "Ошибка: цена не может быть отрицательной.\n";
        }
    }
}

void display_finder_user_shoes(){
    int choice = -1;

    while (choice != 0) {
        std::cout << std::string(64, '-') << '\n';
        std::cout << std::string(24, ' ') << "по какому полю производить поиск:" << '\n';
        std::cout << std::string(64, '-') << '\n';

        std::cout << " 1 - по артиклю \n";
        std::cout << " 2 - по названию \n";
        std::cout << " 3 - по количеству \n";
        std::cout << " 4 - по цене \n";
        std::cout << " 0 - вернуться обратно \n";
        std::cout << std::string(64, '-') << '\n';

        std::cout << "Выберите действие: ";
        std::cin >> choice;

        if (!std::cin.good()){
            std::cout << "Ошибка: введите номер действия.\n";
            std::cin.clear();
            std::cin.ignore(32767, '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            user_shoes_to_find_by_article();
            return;
        case 2:
            user_shoes_to_find_by_name();
            return;
        case 3:
            user_shoes_to_find_by_count();
            return;
        case 4:
            user_shoes_to_find_by_price();
            return;
        case 0:
            break;
        default:
            std::cout << "\nНеверный ввод! Попробуйте еще раз.\n";
            std::cin.clear();
            std::cin.ignore(32767, '\n');
        }
        std::cout << '\n';
    }
}

int remove_shoes(char* article){
    std::fstream file(ASSORTMENT_SHOES_FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);
    if (!file){
        return CANT_OPEN_REMOVE_FILE_STATUS;
    }

    // Определяем размер файла и количество записей
    file.seekg(0, std::ios::end);
    int file_size = file.tellg();
    if (file_size < sizeof(shoes)){
        return ERRORE_REMOVE_SIZE_FILE_STATUS;
    } 
    size_t num_shoes = file_size / sizeof(shoes);

    // Читаем последнюю запись
    shoes last_shoes;
    file.seekg(-static_cast<int>(sizeof(shoes)), std::ios::end);
    file.read(reinterpret_cast<char*>(&last_shoes), sizeof(shoes));

     // Ищем удаляемую запись и заменяем её последней
    file.seekg(0);
    shoes delete_shoes;
    std::streampos delete_position_shoes = -1;
    while (file.read(reinterpret_cast<char*>(&delete_shoes), sizeof(delete_shoes))) {
        if (strcmp(article, delete_shoes.article) == 0) {
            delete_position_shoes = file.tellg() - std::streamoff(sizeof(delete_shoes));
            break;
        }
    }
    if (delete_position_shoes == -1){
      return ERRORE_REMOVE_SHOES_NOT_FOUND_IN_FILE_STATUS;  
    } 

     // Если удаляемая запись и так последняя, просто усекаем
    if (delete_position_shoes == std::streamoff((num_shoes - 1) * sizeof(delete_shoes))) {
        file.close();
        if (truncate(ASSORTMENT_SHOES_FILE_NAME, static_cast<off_t>(file_size - sizeof(delete_shoes))) != 0){
            return ERRORE_REMOVE_SIZE_FILE_STATUS;
        }
        return SUCCESSFUL_REMOVE_SHOES_STATUS;
    }

    // последнюю запись на место удалённой
    file.seekp(delete_position_shoes);
    file.write(reinterpret_cast<const char*>(&last_shoes), sizeof(shoes));
    file.close();

    // уменьшение файла
    if (truncate(ASSORTMENT_SHOES_FILE_NAME, static_cast<off_t>(file_size - sizeof(shoes))) != 0){
        return ERRORE_REMOVE_SIZE_FILE_STATUS;
    }

    return SUCCESSFUL_REMOVE_SHOES_STATUS;
}

int remove_user_shoes(char* article,int (*remove_shoes)(char*)){
    int remove_status = remove_shoes(article);
    if (remove_status != SUCCESSFUL_REMOVE_SHOES_STATUS){
        return remove_status;
    }
    return SUCCESSFUL_REMOVE_SHOES_STATUS;
}

void print_remove_by_article_error(int remove_shoes_status){
    if (remove_shoes_status == CANT_OPEN_REMOVE_FILE_STATUS){
        std::cout << "Ошибка: не удалось открыть файл ассортимента.\n";
    } else if (remove_shoes_status == ERRORE_REMOVE_SIZE_FILE_STATUS){
        std::cout << "Ошибка: файл ассортимента пуст или поврежден.\n";
    } else if (remove_shoes_status == ERRORE_REMOVE_SHOES_NOT_FOUND_IN_FILE_STATUS){
        std::cout << "Ошибка: запись с указанным артикулом не найдена.\n";
    } else if (remove_shoes_status == SUCCESSFUL_REMOVE_SHOES_STATUS){
        std::cout << "Запись успешно удалена.\n";
    } else {
        std::cout << "Ошибка: неизвестный статус " << remove_shoes_status << ".\n";
    }
}

void remover_user_shoes(){
    while (true){
        std::string article_input;
        std::cout << "Введите артикул (Ж/М/Д...): ";
        std::getline(std::cin >> std::ws, article_input);

        int article_status = is_valid_shoes_article(article_input.c_str());
        if (article_status == IS_VALID_SHOES_ARTICLE_STATUS){

            char article[SHOES_ARTICLE_SIZE];
            std::strncpy(article, article_input.c_str(), SHOES_ARTICLE_SIZE - 1);
            article[SHOES_ARTICLE_SIZE - 1] = '\0';

            int remove_shoes_status = remove_user_shoes(article,remove_shoes);
            if (remove_shoes_status != IS_CORECT_REMOVE_SHOES_STATUS){
                print_remove_by_article_error(remove_shoes_status);
                return;
            }
            std::cout << "Удаление прошло успешно. ";
            return;
        }

        if (article_status == IS_EMPTY_SHOES_ARTICLE_STATUS){
            std::cout << "Ошибка: артикул не может быть пустым.\n";
        } else if (article_status == IS_OVERSIZE_SHOES_ARTICLE_STATUS){
            std::cout << "Ошибка: слишком длинный артикул.\n";
        } else if (article_status == IS_SHORTS_SHOES_ARTICLE_STATUS){
            std::cout << "Ошибка: артикул слишком короткий.\n";
        } else if (article_status == IS_INCORECT_PREFIX_SHOES_ARTICLE_STATUS){
            std::cout << "Ошибка: артикул должен начинаться с Ж, М или Д.\n";
        }
    }  
}

void drop_interface_in_user() {
    int choice = -1;

    while (choice != 0) {
        std::cout << std::string(64, '-') << '\n';
        std::cout << std::string(29, ' ') << "Menu:" << '\n';
        std::cout << std::string(64, '-') << '\n';

        std::cout << " 1 - вывод отсортированного ассортимента обуви\n";
        std::cout << " 2 - добавление новой записи обуви\n";
        std::cout << " 3 - поиск записи\n";
        std::cout << " 4 - удаление записи\n";
        std::cout << " 0 - выйти из программы\n";
        std::cout << std::string(64, '-') << '\n';

        std::cout << "Выберите действие: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                display_assortment_shoes();
                break;
            case 2:
                add_new_record_shoes();
                break;
            case 3:
                display_finder_user_shoes();
                break;
            case 4:
                remover_user_shoes();
                break;
            case 0:
                std::cout << "\nДо свидания!\n";
                break;
            default:
                std::cout << "\nНеверный ввод! Попробуйте еще раз.\n";
                std::cin.clear();
                std::cin.ignore(32767, '\n');
        }
        std::cout << '\n';
    }
}

bool find_and_created_file(){
    std::ifstream check_file(ASSORTMENT_SHOES_FILE_NAME, std::ios::binary);
    if (check_file.is_open()){
        check_file.close();
        return true;
    }

    std::ofstream create_file(ASSORTMENT_SHOES_FILE_NAME, std::ios::binary);
    if (!create_file.is_open()){
        std::cout << "Ошибка: не удалось создать файл ассортимента.\n";
        return false;
    }

    create_file.close();
    return true;
}

void fill_test_string(char* buffer, size_t length, char symbol){
    memset(buffer, symbol, length);
    buffer[length] = '\0';
}

void fill_valid_article(char* buffer, size_t length){
    const char* prefix = "Ж";
    const size_t prefix_size = strlen(prefix);

    assert(length >= prefix_size);
    memcpy(buffer, prefix, prefix_size);
    memset(buffer + prefix_size, '1', length - prefix_size);
    buffer[length] = '\0';
}

bool is_same_shoes(shoes left_shoes, shoes right_shoes){
    return strcmp(left_shoes.article, right_shoes.article) == 0 &&
        strcmp(left_shoes.name, right_shoes.name) == 0 &&
        left_shoes.count == right_shoes.count &&
        left_shoes.price == right_shoes.price;
}

void reset_append_shoes_test_state(){
    append_shoes_test_source = {};
    append_shoes_test_find_arg = {};
    append_shoes_test_give_out_arg = {};
    append_shoes_test_find_called = false;
    append_shoes_test_give_out_called = false;
    append_shoes_test_find_status = SHOES_NOT_FOUND_STATUS;
    append_shoes_test_give_out_status = SUCCESSFUL_APPEND_SHOES_STATUS;
}

shoes get_append_shoes_test_source(){
    return append_shoes_test_source;
}

int find_append_shoes_test_double(shoes target_shoes){
    append_shoes_test_find_called = true;
    append_shoes_test_find_arg = target_shoes;
    return append_shoes_test_find_status;
}

int give_out_append_shoes_test_double(shoes target_shoes){
    append_shoes_test_give_out_called = true;
    append_shoes_test_give_out_arg = target_shoes;
    return append_shoes_test_give_out_status;
}

void reset_get_sorted_test_state(){
    get_sorted_test_assortment = nullptr;
    get_sorted_test_sort_arg = nullptr;
    get_sorted_test_output_arg = nullptr;
    get_sorted_test_get_called = false;
    get_sorted_test_sort_called = false;
    get_sorted_test_output_called = false;
    get_sorted_test_compare_called = false;
    get_sorted_test_output_status = IS_CORECT_OUTPUT_ASSORTIMENT_SHOES_STATUS;
}

shoes* get_sorted_assortment_test_double(){
    get_sorted_test_get_called = true;
    return get_sorted_test_assortment;
}

bool find_user_changed_get_sorted_test_double(shoes left_shoes, shoes right_shoes){
    get_sorted_test_compare_called = true;
    return left_shoes.count > right_shoes.count;
}

void sort_assortment_get_sorted_test_double(shoes* assortment_shoes, bool (*find_user_changed_shoes)(shoes,shoes)){
    get_sorted_test_sort_called = true;
    get_sorted_test_sort_arg = assortment_shoes;
    if (find_user_changed_shoes != nullptr && assortment_shoes != nullptr){
        find_user_changed_shoes(assortment_shoes[0], assortment_shoes[0]);
    }
}

int output_sorted_assortment_get_sorted_test_double(shoes* assortment_shoes){
    get_sorted_test_output_called = true;
    get_sorted_test_output_arg = assortment_shoes;
    return get_sorted_test_output_status;
}

void rewrite_assortment_file_for_tests(const shoes* source_shoes, int source_shoes_count){
    std::ofstream assortment_shoes_file(ASSORTMENT_SHOES_FILE_NAME, std::ios::binary | std::ios::trunc);
    assert(assortment_shoes_file.is_open());

    for (int i = 0; i < source_shoes_count; i++){
        assortment_shoes_file.write(reinterpret_cast<const char*>(&source_shoes[i]), sizeof(shoes));
    }

    assortment_shoes_file.close();
}

shoes* get_assortment_for_finder_test_double(shoes /*find_shoes*/, bool (*/*find_user_changed_shoes*/)(shoes,shoes)){
    return finder_user_shoes_test_source;
}

void reset_finder_user_shoes_test_state(){
    finder_user_shoes_test_source = nullptr;
}

void test_valid_shoes_name(){
    assert(is_valid_shoes_name("Nike") == IS_VALID_SHOES_NAME_STATUS);
}

void test_empty_shoes_name(){
    assert(is_valid_shoes_name("") == IS_EMPTY_SHOES_NAME_STATUS);
}

void test_valid_shoes_name_max_size(){
    char shoes_name[SHOES_NAME_SIZE];

    fill_test_string(shoes_name, SHOES_NAME_SIZE - 1, 'N');
    assert(is_valid_shoes_name(shoes_name) == IS_VALID_SHOES_NAME_STATUS);
}

void test_oversize_shoes_name(){
    char shoes_name[SHOES_NAME_SIZE + 1];

    fill_test_string(shoes_name, SHOES_NAME_SIZE, 'N');
    assert(is_valid_shoes_name(shoes_name) == IS_OVERSIZE_SHOES_NAME_STATUS);
}

void test_positive_shoes_count(){
    assert(is_valid_shoes_count(5) == IS_VALID_SHOES_COUNT_STATUS);
}

void test_zero_shoes_count(){
    assert(is_valid_shoes_count(0) == IS_VALID_SHOES_COUNT_STATUS);
}

void test_negative_shoes_count(){
    assert(is_valid_shoes_count(-1) == IS_NEGATIVE_SHOES_COUNT_STATUS);
}

void test_positive_shoes_price(){
    assert(is_valid_shoes_price(12.98) == IS_VALID_SHOES_PRICE_STATUS);
}

void test_zero_shoes_price(){
    assert(is_valid_shoes_price(0.0) == IS_ZERO_SHOES_PRICE_STATUS);
}

void test_negative_shoes_price(){
    assert(is_valid_shoes_price(-1.0) == IS_NEGATIVE_SHOES_PRICE_STATUS);
}

void test_valid_shoes_article_starts_with_female(){
    assert(is_valid_shoes_article("Ж123") == IS_VALID_SHOES_ARTICLE_STATUS);
}

void test_valid_shoes_article_starts_with_male(){
    assert(is_valid_shoes_article("М456") == IS_VALID_SHOES_ARTICLE_STATUS);
}

void test_valid_shoes_article_starts_with_kids(){
    assert(is_valid_shoes_article("Д789") == IS_VALID_SHOES_ARTICLE_STATUS);
}

void test_empty_shoes_article(){
    assert(is_valid_shoes_article("") == IS_EMPTY_SHOES_ARTICLE_STATUS);
}

void test_short_shoes_article(){
    assert(is_valid_shoes_article("М") == IS_SHORTS_SHOES_ARTICLE_STATUS);
}

void test_incorrect_first_symbol_shoes_article(){
    assert(is_valid_shoes_article("А123") == IS_INCORECT_PREFIX_SHOES_ARTICLE_STATUS);
}

void test_valid_prefix_starts_with_femle(){
    assert(is_valid_prefix_shoes_article("Ж123") == true);
}

void test_valid_prefix_starts_with_male(){
    assert(is_valid_prefix_shoes_article("М456") == true);
}

void test_valid_prefix_starts_with_kids(){
    assert(is_valid_prefix_shoes_article("Д789") == true);
}

void test_invalid_prefix_starts_with_a(){
    assert(is_valid_prefix_shoes_article("А123") == false);
}

void test_invalid_prefix_starts_with_latin(){
    assert(is_valid_prefix_shoes_article("M123") == false);
}

void test_invalid_prefix_starts_with_digit(){
    assert(is_valid_prefix_shoes_article("1234") == false);
}

void test_invalid_prefix_empty_string(){
    assert(is_valid_prefix_shoes_article("") == false);
}

void test_is_not_oversize_shoes_article(){
    char shoes_article[SHOES_ARTICLE_SIZE];

    fill_test_string(shoes_article, SHOES_ARTICLE_SIZE - 1, 'A');
    assert(is_oversize_shoes_article(shoes_article) == false);
}

void test_is_oversize_shoes_article(){
    char shoes_article[SHOES_ARTICLE_SIZE + 1];

    fill_test_string(shoes_article, SHOES_ARTICLE_SIZE, 'A');
    assert(is_oversize_shoes_article(shoes_article) == true);
}

void test_is_not_short_article(){
    assert(is_shorts_prefix_article("Ж123") == false);
}

void test_is_not_short_article_two_chars(){
    assert(is_shorts_prefix_article("Ж1") == false);
}

void test_is_short_article_one_char(){
    assert(is_shorts_prefix_article("М") == true);
}

void test_is_short_article_empty(){
    assert(is_shorts_prefix_article("") == true);
}

void test_valid_check_prefix_shoes_article(){
    assert(check_prefix_shoes_article("Ж123") == IS_VALID_PREFIX_SHOES_ARTICLE_STATUS);
}

void test_short_check_prefix_shoes_article(){
    assert(check_prefix_shoes_article("Д") == IS_SHORTS_SHOES_ARTICLE_STATUS);
}

void test_invalid_check_prefix_shoes_article(){
    assert(check_prefix_shoes_article("A123") == IS_INCORECT_PREFIX_SHOES_ARTICLE_STATUS);
}

void test_valid_check_size_shoes_article(){
    char shoes_article[SHOES_ARTICLE_SIZE];

    fill_test_string(shoes_article, SHOES_ARTICLE_SIZE - 1, 'A');
    assert(check_size_shoes_article(shoes_article) == IS_VALID_SIZE_SHOES_ARTICLE_STATUS);
}

void test_empty_check_size_shoes_article(){
    assert(check_size_shoes_article("") == IS_EMPTY_SHOES_ARTICLE_STATUS);
}

void test_oversize_check_size_shoes_article(){
    char shoes_article[SHOES_ARTICLE_SIZE + 1];

    fill_test_string(shoes_article, SHOES_ARTICLE_SIZE, 'A');
    assert(check_size_shoes_article(shoes_article) == IS_OVERSIZE_SHOES_ARTICLE_STATUS);
}

void test_valid_shoes_article_max_size(){
    char shoes_article[SHOES_ARTICLE_SIZE];

    fill_valid_article(shoes_article, SHOES_ARTICLE_SIZE - 1);
    assert(is_valid_shoes_article(shoes_article) == IS_VALID_SHOES_ARTICLE_STATUS);
}

void test_oversize_shoes_article(){
    char shoes_article[SHOES_ARTICLE_SIZE + 1];

    fill_valid_article(shoes_article, SHOES_ARTICLE_SIZE);
    assert(is_valid_shoes_article(shoes_article) == IS_OVERSIZE_SHOES_ARTICLE_STATUS);
}

void test_valid_shoes(){
    shoes valid_shoes = {"Ж123", "Nike", 5, 12.98};
    assert(is_valid_shoes(valid_shoes) == IS_VALID_SHOES_STATUS);
}

void test_shoes_with_empty_article(){
    shoes s = {"", "Nike", 5, 12.98};
    assert(is_valid_shoes(s) == IS_EMPTY_SHOES_ARTICLE_STATUS);
}

void test_shoes_with_short_article(){
    shoes s = {"М", "Nike", 5, 12.98};
    assert(is_valid_shoes(s) == IS_SHORTS_SHOES_ARTICLE_STATUS);
}

void test_shoes_with_incorrect_prefix_article(){
    shoes s = {"А123", "Nike", 5, 12.98};
    assert(is_valid_shoes(s) == IS_INCORECT_PREFIX_SHOES_ARTICLE_STATUS);
}

void test_shoes_with_empty_name(){
    shoes s = {"Ж123", "", 5, 12.98};
    assert(is_valid_shoes(s) == IS_EMPTY_SHOES_NAME_STATUS);
}

void test_shoes_with_negative_count(){
    shoes s = {"Ж123", "Nike", -1, 12.98};
    assert(is_valid_shoes(s) == IS_NEGATIVE_SHOES_COUNT_STATUS);
}

void test_shoes_with_zero_price(){
    shoes s = {"Ж123", "Nike", 5, 0.0};
    assert(is_valid_shoes(s) == IS_ZERO_SHOES_PRICE_STATUS);
}

void test_shoes_with_negative_price(){
    shoes s = {"Ж123", "Nike", 5, -1.0};
    assert(is_valid_shoes(s) == IS_NEGATIVE_SHOES_PRICE_STATUS);
}

void test_append_shoes_with_invalid_shoes(){
    reset_append_shoes_test_state();
    append_shoes_test_source = {"", "Nike", 5, 12.98};

    assert(append_shoes(get_append_shoes_test_source, give_out_append_shoes_test_double, find_append_shoes_test_double) == IS_EMPTY_SHOES_ARTICLE_STATUS);
    assert(append_shoes_test_find_called == false);
    assert(append_shoes_test_give_out_called == false);
}

void test_append_shoes_with_not_unique_shoes(){
    reset_append_shoes_test_state();
    append_shoes_test_source = {"Ж123", "Nike", 5, 12.98};
    append_shoes_test_find_status = SHOES_FOUNDED_STATUS;

    assert(append_shoes(get_append_shoes_test_source, give_out_append_shoes_test_double, find_append_shoes_test_double) == ISNT_UNICUE_SHOES_STATUS);
    assert(append_shoes_test_find_called == true);
    assert(is_same_shoes(append_shoes_test_find_arg, append_shoes_test_source) == true);
    assert(append_shoes_test_give_out_called == false);
}

void test_append_shoes_with_give_out_error(){
    reset_append_shoes_test_state();
    append_shoes_test_source = {"Ж123", "Nike", 5, 12.98};
    append_shoes_test_find_status = SHOES_NOT_FOUND_STATUS;
    append_shoes_test_give_out_status = ERROR_WRITE_SHOES_FILE_STATUS;

    assert(append_shoes(get_append_shoes_test_source, give_out_append_shoes_test_double, find_append_shoes_test_double) == ERROR_WRITE_SHOES_FILE_STATUS);
    assert(append_shoes_test_find_called == true);
    assert(append_shoes_test_give_out_called == true);
    assert(is_same_shoes(append_shoes_test_find_arg, append_shoes_test_source) == true);
    assert(is_same_shoes(append_shoes_test_give_out_arg, append_shoes_test_source) == true);
}

void test_append_shoes_success(){
    reset_append_shoes_test_state();
    append_shoes_test_source = {"Ж123", "Nike", 5, 12.98};
    append_shoes_test_find_status = SHOES_NOT_FOUND_STATUS;
    append_shoes_test_give_out_status = SUCCESSFUL_APPEND_SHOES_STATUS;

    assert(append_shoes(get_append_shoes_test_source, give_out_append_shoes_test_double, find_append_shoes_test_double) == SUCCESSFUL_APPEND_SHOES_STATUS);
    assert(append_shoes_test_find_called == true);
    assert(append_shoes_test_give_out_called == true);
    assert(is_same_shoes(append_shoes_test_find_arg, append_shoes_test_source) == true);
    assert(is_same_shoes(append_shoes_test_give_out_arg, append_shoes_test_source) == true);
}

void test_is_unicue_shoos_not_found(){
    reset_append_shoes_test_state();
    append_shoes_test_source = {"Ж123", "Nike", 5, 12.98};
    append_shoes_test_find_status = SHOES_NOT_FOUND_STATUS;

    assert(is_unicue_shoos(append_shoes_test_source, find_append_shoes_test_double) == IS_UNICUE_SHOES_STATUS);
    assert(append_shoes_test_find_called == true);
}

void test_is_unicue_shoos_founded(){
    reset_append_shoes_test_state();
    append_shoes_test_source = {"Ж123", "Nike", 5, 12.98};
    append_shoes_test_find_status = SHOES_FOUNDED_STATUS;

    assert(is_unicue_shoos(append_shoes_test_source, find_append_shoes_test_double) == ISNT_UNICUE_SHOES_STATUS);
    assert(append_shoes_test_find_called == true);
}

void test_is_unicue_shoos_propagates_error(){
    reset_append_shoes_test_state();
    append_shoes_test_source = {"Ж123", "Nike", 5, 12.98};
    append_shoes_test_find_status = ERROR_OPEN_SHOES_FILE_STATUS;

    assert(is_unicue_shoos(append_shoes_test_source, find_append_shoes_test_double) == ERROR_OPEN_SHOES_FILE_STATUS);
    assert(append_shoes_test_find_called == true);
}

void test_is_valid_assortment_shoes_nullptr(){
    assert(is_valid_assortment_shoes(nullptr) == false);
}

void test_is_valid_assortment_shoes_with_valid_first_shoes(){
    shoes assortment_shoes[2] = {{"Ж123", "Nike", 5, 12.98}, {}};
    assert(is_valid_assortment_shoes(assortment_shoes) == true);
}

void test_is_valid_assortment_shoes_with_invalid_first_shoes(){
    shoes assortment_shoes[2] = {{"", "Nike", 5, 12.98}, {}};
    assert(is_valid_assortment_shoes(assortment_shoes) == false);
}

void test_find_user_changed_name_shoes_not_equal(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"Ж124", "Puma", 4, 10.00};

    assert(find_user_changed_name_shoes(first_shoes, second_shoes) == false);
}

void test_find_user_changed_name_shoes_equal(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"Ж124", "Nike", 4, 10.00};

    assert(find_user_changed_name_shoes(first_shoes, second_shoes) == false);
}

void test_find_user_changed_name_shoes_reverse_order(){
    shoes first_shoes = {"Ж123", "Puma", 5, 12.98};
    shoes second_shoes = {"Ж124", "Nike", 4, 10.00};

    assert(find_user_changed_name_shoes(first_shoes, second_shoes) == true);
}

void test_find_user_changed_article_shoes_not_equal(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"Ж124", "Nike", 5, 12.98};

    assert(find_user_changed_article_shoes(first_shoes, second_shoes) == false);
}

void test_find_user_changed_article_shoes_equal(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"Ж123", "Puma", 4, 10.00};

    assert(find_user_changed_article_shoes(first_shoes, second_shoes) == false);
}

void test_find_user_changed_article_shoes_reverse_order(){
    shoes first_shoes = {"М124", "Nike", 5, 12.98};
    shoes second_shoes = {"Ж123", "Puma", 4, 10.00};

    assert(find_user_changed_article_shoes(first_shoes, second_shoes) == true);
}

void test_find_user_changed_count_shoes_equal(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"Ж124", "Puma", 5, 10.00};

    assert(find_user_changed_count_shoes(first_shoes, second_shoes) == false);
}

void test_find_user_changed_count_shoes_not_equal(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"Ж124", "Puma", 4, 10.00};

    assert(find_user_changed_count_shoes(first_shoes, second_shoes) == true);
}

void test_find_user_changed_count_shoes_reverse_order(){
    shoes first_shoes = {"Ж123", "Nike", 3, 12.98};
    shoes second_shoes = {"Ж124", "Puma", 4, 10.00};

    assert(find_user_changed_count_shoes(first_shoes, second_shoes) == false);
}

void test_find_user_changed_price_shoes_equal(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"Ж124", "Puma", 4, 12.98};

    assert(find_user_changed_price_shoes(first_shoes, second_shoes) == false);
}

void test_find_user_changed_price_shoes_not_equal(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"Ж124", "Puma", 4, 10.00};

    assert(find_user_changed_price_shoes(first_shoes, second_shoes) == true);
}

void test_find_user_changed_price_shoes_reverse_order(){
    shoes first_shoes = {"Ж123", "Nike", 5, 9.99};
    shoes second_shoes = {"Ж124", "Puma", 4, 10.00};

    assert(find_user_changed_price_shoes(first_shoes, second_shoes) == false);
}

void test_sort_assortment_shoes_with_nullptr(){
    sort_assortment_shoes(nullptr, find_user_changed_count_shoes);
    assert(true);
}

void test_get_sorted_assortment_shoes_empty_assortment(){
    reset_get_sorted_test_state();

    int sorted_status = get_sorted_assortment_shoes(get_sorted_assortment_test_double, output_sorted_assortment_get_sorted_test_double, sort_assortment_get_sorted_test_double, find_user_changed_get_sorted_test_double);
    assert(sorted_status == IS_EMPTY_OUTPUT_ASSORTIMENT_SHOES_STATUS);
    assert(get_sorted_test_get_called == true);
    assert(get_sorted_test_sort_called == false);
    assert(get_sorted_test_output_called == false);
}

void test_get_sorted_assortment_shoes_success(){
    reset_get_sorted_test_state();
    get_sorted_test_assortment = new shoes[2]{{"Ж123", "Nike", 5, 12.98}, {}};

    int sorted_status = get_sorted_assortment_shoes(get_sorted_assortment_test_double, output_sorted_assortment_get_sorted_test_double, sort_assortment_get_sorted_test_double, find_user_changed_get_sorted_test_double);
    assert(sorted_status == IS_CORECT_SORTED_ASSORTIMENT_SHOES_STATUS);
    assert(get_sorted_test_get_called == true);
    assert(get_sorted_test_sort_called == true);
    assert(get_sorted_test_output_called == true);
    assert(get_sorted_test_compare_called == true);
    assert(get_sorted_test_sort_arg == get_sorted_test_assortment);
    assert(get_sorted_test_output_arg == get_sorted_test_assortment);
}

void test_get_sorted_assortment_shoes_output_error(){
    reset_get_sorted_test_state();
    get_sorted_test_assortment = new shoes[2]{{"Ж123", "Nike", 5, 12.98}, {}};
    get_sorted_test_output_status = ERROR_WRITE_SHOES_FILE_STATUS;

    int sorted_status = get_sorted_assortment_shoes(get_sorted_assortment_test_double, output_sorted_assortment_get_sorted_test_double, sort_assortment_get_sorted_test_double, find_user_changed_get_sorted_test_double);
    assert(sorted_status == ERROR_WRITE_SHOES_FILE_STATUS);
    assert(get_sorted_test_sort_called == true);
    assert(get_sorted_test_output_called == true);
}

void test_find_condition_by_count_shoes_true(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"М123", "Puma", 5, 9.99};

    assert(find_condition_by_count_shoes(first_shoes, second_shoes) == true);
}

void test_find_condition_by_count_shoes_false(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"М123", "Puma", 4, 9.99};

    assert(find_condition_by_count_shoes(first_shoes, second_shoes) == false);
}

void test_find_condition_by_price_shoes_true(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"М123", "Puma", 4, 12.98};

    assert(find_condition_by_price_shoes(first_shoes, second_shoes) == true);
}

void test_find_condition_by_price_shoes_false(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"М123", "Puma", 4, 9.99};

    assert(find_condition_by_price_shoes(first_shoes, second_shoes) == false);
}

void test_find_condition_by_article_shoes_true(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"Ж123", "Puma", 4, 9.99};

    assert(find_condition_by_article_shoes(first_shoes, second_shoes) == true);
}

void test_find_condition_by_article_shoes_false(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"М124", "Puma", 4, 9.99};

    assert(find_condition_by_article_shoes(first_shoes, second_shoes) == false);
}

void test_find_condition_by_name_shoes_true(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"М124", "Nike", 4, 9.99};

    assert(find_condition_by_name_shoes(first_shoes, second_shoes) == true);
}

void test_find_condition_by_name_shoes_false(){
    shoes first_shoes = {"Ж123", "Nike", 5, 12.98};
    shoes second_shoes = {"М124", "Puma", 4, 9.99};

    assert(find_condition_by_name_shoes(first_shoes, second_shoes) == false);
}


void test_finder_user_shoes_returns_empty_status_for_nullptr_source(){
    reset_finder_user_shoes_test_state();
    shoes target_shoes = {"Ж123", "Nike", 5, 12.98};

    int finder_status = finder_user_shoes(
        target_shoes,
        get_assortment_for_finder_test_double,
        output_find_assortiment_shoes,
        find_condition_by_article_shoes
    );

    assert(finder_status == IS_EMPTY_FIND_ASSORTIMENT_SHOES_STATUS);
}

void test_finder_user_shoes_returns_empty_status_for_invalid_source(){
    finder_user_shoes_test_source = new shoes[2]{{"", "Nike", 5, 12.98}, {}};
    shoes target_shoes = {"Ж123", "Nike", 5, 12.98};

    int finder_status = finder_user_shoes(
        target_shoes,
        get_assortment_for_finder_test_double,
        output_find_assortiment_shoes,
        find_condition_by_article_shoes
    );

    assert(finder_status == IS_EMPTY_FIND_ASSORTIMENT_SHOES_STATUS);
    reset_finder_user_shoes_test_state();
}

void test_finder_user_shoes_returns_success_for_valid_source(){
    finder_user_shoes_test_source = new shoes[2]{{"Ж123", "Nike", 5, 12.98}, {}};
    shoes target_shoes = {"Ж123", "Nike", 5, 12.98};

    int finder_status = finder_user_shoes(
        target_shoes,
        get_assortment_for_finder_test_double,
        output_find_assortiment_shoes,
        find_condition_by_article_shoes
    );

    assert(finder_status == IS_CORECT_FIND_ASSORTIMENT_SHOES_STATUS);
    reset_finder_user_shoes_test_state();
}

void tests(){
    test_empty_shoes_name();
    test_valid_shoes_name();
    test_valid_shoes_name_max_size();
    test_oversize_shoes_name();

    test_negative_shoes_count();
    test_positive_shoes_count();
    test_zero_shoes_count();

    test_negative_shoes_price();
    test_positive_shoes_price();
    test_zero_shoes_price();

    test_valid_shoes_article_starts_with_female();
    test_valid_shoes_article_starts_with_male();
    test_valid_shoes_article_starts_with_kids();
    test_valid_shoes_article_max_size();
    test_empty_shoes_article();
    test_oversize_shoes_article();
    test_short_shoes_article();
    test_incorrect_first_symbol_shoes_article();

    test_valid_prefix_starts_with_femle();
    test_valid_prefix_starts_with_male();
    test_valid_prefix_starts_with_kids();
    test_invalid_prefix_starts_with_a();
    test_invalid_prefix_starts_with_latin();
    test_invalid_prefix_starts_with_digit();
    test_invalid_prefix_empty_string();
    test_is_not_oversize_shoes_article();
    test_is_oversize_shoes_article();
    test_is_not_short_article();
    test_is_not_short_article_two_chars();
    test_is_short_article_one_char();
    test_is_short_article_empty();
    test_valid_check_prefix_shoes_article();
    test_short_check_prefix_shoes_article();
    test_invalid_check_prefix_shoes_article();
    test_valid_check_size_shoes_article();
    test_empty_check_size_shoes_article();
    test_oversize_check_size_shoes_article();

    test_valid_shoes();
    test_shoes_with_empty_article();
    test_shoes_with_short_article();
    test_shoes_with_incorrect_prefix_article();
    test_shoes_with_empty_name();
    test_shoes_with_negative_count();
    test_shoes_with_zero_price();
    test_shoes_with_negative_price();

    test_append_shoes_with_invalid_shoes();
    test_append_shoes_with_not_unique_shoes();
    test_append_shoes_with_give_out_error();
    test_append_shoes_success();

    test_is_unicue_shoos_not_found();
    test_is_unicue_shoos_founded();
    test_is_unicue_shoos_propagates_error();

    test_is_valid_assortment_shoes_nullptr();
    test_is_valid_assortment_shoes_with_valid_first_shoes();
    test_is_valid_assortment_shoes_with_invalid_first_shoes();

    test_find_user_changed_name_shoes_not_equal();
    test_find_user_changed_name_shoes_equal();
    test_find_user_changed_name_shoes_reverse_order();
    test_find_user_changed_article_shoes_not_equal();
    test_find_user_changed_article_shoes_equal();
    test_find_user_changed_article_shoes_reverse_order();
    test_find_user_changed_count_shoes_equal();
    test_find_user_changed_count_shoes_not_equal();
    test_find_user_changed_count_shoes_reverse_order();
    test_find_user_changed_price_shoes_equal();
    test_find_user_changed_price_shoes_not_equal();
    test_find_user_changed_price_shoes_reverse_order();

    test_sort_assortment_shoes_with_nullptr();

    test_get_sorted_assortment_shoes_empty_assortment();
    test_get_sorted_assortment_shoes_success();
    test_get_sorted_assortment_shoes_output_error();

    test_find_condition_by_count_shoes_true();
    test_find_condition_by_count_shoes_false();
    test_find_condition_by_price_shoes_true();
    test_find_condition_by_price_shoes_false();
    test_find_condition_by_article_shoes_true();
    test_find_condition_by_article_shoes_false();
    test_find_condition_by_name_shoes_true();
    test_find_condition_by_name_shoes_false();

    test_finder_user_shoes_returns_empty_status_for_nullptr_source();
    test_finder_user_shoes_returns_empty_status_for_invalid_source();
    test_finder_user_shoes_returns_success_for_valid_source();
}

int main(int argc, char const *argv[]){
    std::ostringstream tests_output_buffer;
    std::streambuf* original_cout_buffer = std::cout.rdbuf(tests_output_buffer.rdbuf());
    tests();
    std::cout.rdbuf(original_cout_buffer);

    if (!(find_and_created_file())){
        return 1;
    }
    drop_interface_in_user();
    return 0;
}
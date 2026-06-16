/* Пусть некоторая туристическая анкета, содержит следующие поля:
• Fam — фамилию туриста;
• Name — имя туриста;
• Otch — отчество туриста;
• Gorod — город, в котором проживает турист.
Написать программу, выполняющую следующие действия (оформить с помощью меню, в котором можно выбрать соответствующий пункт):
• ввод с клавиатуры количества записей о туристах (число элементов массива в динамической памяти).
При этом при повторном выборе этого пункта число элементов массива должно будет увеличится на указанное пользователем значение.
Содержимое полей лучше генерировать;
• вывод на экран информации о всех туристах.
• вывод города, из которого в анкете больше всего туристов. */

#include "iostream"
#include "ctime"
#include "string"

using namespace std;

const string rand_names[10] = {"Александр", "Максим", "Иван", "Артем", "Дмитрий", "Никита", "Михаил", "Егор", "Андрей", "Сергей"};
const string rand_second_names[10] = {"Иванов", "Петров", "Сидоров", "Кузнецов", "Попов", "Васильев", "Михайлов", "Новиков", "Федоров", "Морозов"};
const string rand_surnames[10] = {"Иванович", "Петрович", "Сидорович", "Кузнецович", "Попович", "Васильевич", "Михайлович", "Новикович", "Федорович", "Морозович"};
const string rand_cities[11] = {"Москва", "Санкт-Петербург", "Новосибирск", "Екатеринбург", "Казань", "Йошкар-Ола", "Нижний_Новгород", "Челябинск", "Самара", "Омск", "Ростов-на-Дону"};

struct person {
    int id;
    string second_name;
    string name;
    string surname;
    string city;
};

int count_people_by_User_Input(){
    int input_int;
    cin >> input_int;
    return input_int;
}

int change_task_by_User_Input(){
    int input_int;
    cin >> input_int;
    return input_int;
}

void Get_persons_info(person* people, int count_people, int last_id){
    for (int i = 0; i < count_people; i++){
        people[i].id = last_id + i + 1;
        people[i].second_name = rand_second_names[rand() % (sizeof(rand_second_names) / sizeof(rand_second_names[0]))];
        people[i].name = rand_names[rand() % (sizeof(rand_names) / sizeof(rand_names[0]))];
        people[i].surname = rand_surnames[rand() % (sizeof(rand_surnames) / sizeof(rand_surnames[0]))];
        people[i].city = rand_cities[rand() % (sizeof(rand_cities) / sizeof(rand_cities[0]))];
    }
}

void Output_persons_info(person* people, int count_people){
    cout << endl << endl << "Информация о туристах:" << endl;
    for (int i = 0; i < count_people; i++){
        cout << people[i].id << ": " << people[i].second_name << " " << people[i].name << " " << people[i].surname << " - " << people[i].city << endl;
    }
}

void Output_menu(){
    cout << endl << "Выберите действие:" << endl;
    cout << "1. Ввести количество записей о туристах" << endl;
    cout << "2. Вывести на экран информацию о всех туристах" << endl;
    cout << "3. Вывести город, из которого в анкете больше всего туристов" << endl;
    cout << "4. Выйти из программы";
}

void Output_err_count_people_input(){
    cout << endl << "Некорректный ввод. Введите число >= 1: ";
}

void Output_task_number_input(){
    cout << endl << "Введите число от 1 до 4: ";
} 

void Output_err_task_number_input(){
    cout << endl << "Некорректный ввод. Введите число от 1 до 4: ";
}

void Output_err_null_of_persons_for_Find_Most_Common_City(){
    cout << endl << "Нет данных о людях."<< endl<< endl;
}

void Output_err_null_of_persons_for_output(){
    cout << endl << "Нет данных о людях."<< endl<< endl;
}

void Output_count_people_input(){
    cout << endl << "Введите количество записей о туристах: ";
}

void Output_count_cities(int* count_cities){
    cout << endl << "Количество туристов по городам:" << endl;
    for (int i = 0; i < (sizeof(rand_cities) / sizeof(rand_cities[0])); i++){
        cout << rand_cities[i] << " - " << count_cities[i] << endl;
    }
}

void Output_most_common_city(string most_common_city){
    cout << endl << "Город, из которого в анкете больше всего туристов: " << most_common_city << endl;
}

void Checking_Count_People_Input(int input_int){
    while (input_int < 1)
    {
        Output_err_count_people_input();
        input_int = count_people_by_User_Input();
    }
}

int Checking_task_number_input(int input_int){
    while ((input_int < 1)||(input_int > 4))
    {
        Output_err_task_number_input();
        input_int = change_task_by_User_Input();
    }
    return input_int;
}

void forming_people(person* people, int count_people){
    Get_persons_info(people, count_people, people[count_people - 1].id); 
    Checking_Count_People_Input(count_people);
}

void Resize_people_Dynamic_Array(person*& people, int& count_people, int new_count_people){
    person* new_people = new person[new_count_people];
    for (int i = 0; i < count_people; i++){
        new_people[i] = people[i];
    }
    delete[] people;
    people = new_people;
    count_people = new_count_people;
}

int Logic_task (){
    Output_menu();
    Output_task_number_input();
    return Checking_task_number_input(change_task_by_User_Input());
}

int* counter_people_in_city(person* people, int count_people){
    int* count_cities = new int[(sizeof(rand_cities) / sizeof(rand_cities[0]))] {0};
    for (int i = 0; i < count_people; i++){
        for (int j = 0; j < (sizeof(rand_cities) / sizeof(rand_cities[0])); j++){
            if (people[i].city == rand_cities[j]){
                count_cities[j]++;
            }
        }
    }
    return count_cities;
}

string most_counter_cities(int* count_cities){
    int max_count_city = count_cities[0];
    string most_common_city = rand_cities[0];
    for (int i = 1; i < (sizeof(rand_cities) / sizeof(rand_cities[0])); i++){
        if (count_cities[i] > max_count_city){
            max_count_city = count_cities[i];
            most_common_city = rand_cities[i];
        } else if (count_cities[i] == max_count_city){
            most_common_city = most_common_city + (" " + rand_cities[i]);
        }
    }
    return most_common_city;
}

void cleaner_count_cities(int*& count_cities){
    delete[] count_cities;
    count_cities = nullptr;
}

void Find_Most_Common_City(person* people, int count_people){
    int* count_cities = counter_people_in_city(people, count_people);
    Output_count_cities(count_cities);
    Output_most_common_city(most_counter_cities(count_cities));
    cleaner_count_cities(count_cities);
}

void add_new_count_persons_with_forming(person*& people, int& count_people){ 
    Output_count_people_input();
    int new_count = count_people_by_User_Input();
    Checking_Count_People_Input(new_count);
    int old_count = count_people;
    Resize_people_Dynamic_Array(people, count_people, count_people + new_count);
    int last_id = 0;
    if (old_count > 0) {
        last_id = people[old_count - 1].id;
    }
    Get_persons_info(people + old_count, new_count, last_id);
}

void checking_null_of_persons_for_output(person* people, int count_people){
    if (people != nullptr){
        Output_persons_info(people, count_people);
    } else {
        Output_err_null_of_persons_for_output();
    }
    
}

void checking_null_of_persons_for_Find_Most_Common_City(person* people, int count_people){
    if (people != nullptr){
        Find_Most_Common_City(people, count_people);
    } else {
        Output_err_null_of_persons_for_Find_Most_Common_City();
    }
}

void cleaner_people(person*& people){
    delete[] people;
    people = nullptr;
}

void main_logic(person*& people, int& count_people, bool& work_flag, int a){
    switch (a){
        case 1:
            add_new_count_persons_with_forming(people, count_people);
            break;
        case 2:
            checking_null_of_persons_for_output(people, count_people);
            break;
        case 3:
            checking_null_of_persons_for_Find_Most_Common_City(people, count_people);
            break;
        case 4:
            work_flag = false;
            break;
    }
}

int main(){
    srand(time(NULL));
    person* people = nullptr;
    int count_people = 0;
    bool work_flag = true;
    while (work_flag)
    {
        int a = Logic_task();
        main_logic(people, count_people, work_flag, a);
    }

    cleaner_people(people);
}
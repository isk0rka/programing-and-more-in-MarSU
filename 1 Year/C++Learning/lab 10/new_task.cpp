/* Дан список L, элементы которого являются целыми числами.
 Поменять местами элементы списка x1 и xn, x2 и xn-1 и т.д. */

#include <iostream>
#include "ctime"

using namespace std;

struct list_box {
    int value;
    list_box* next;
};

void add_in_back(list_box*& start_box, int value) {
    list_box* new_box = new list_box;
    new_box -> value = value;
    new_box -> next = nullptr;

    if (start_box == nullptr) {
        start_box = new_box;
        return;
    }

    list_box* current_box = start_box;
    while (current_box->next != nullptr) {
        current_box = current_box->next;
    }

    current_box->next = new_box;
}

void print_list(list_box* start_box) {
    list_box* current_box = start_box;
    while (current_box != nullptr) {
        cout << current_box->value << " ";
        current_box = current_box->next;
    }
    cout << endl;
}

void runs_on_list_boxes(list_box*& start_box) {
    list_box* prev_box = nullptr;
    list_box* current_box = start_box;

    while (current_box != nullptr) {
        list_box* next_box = current_box->next;
        current_box->next = prev_box;
        prev_box = current_box;
        current_box = next_box;
    }

    start_box = prev_box;
}

void delete_list(list_box*& start_box) {
    while (start_box != nullptr) {
        list_box* temp = start_box;
        start_box = start_box->next;
        delete temp;
    }
}

int main() {
    int size;
    list_box* start_box = nullptr;
    srand(time(NULL));

    do {
        cout << endl << "введите количество элементов в списке (не менее 0):";
        cin >> size;
    } while (size < 0);
    
    for (int i = 0; i < size; i++) {
        add_in_back(start_box, (rand() % 100));
    }

    if (start_box != nullptr)
    {
        cout << "До сортировки: ";
        print_list(start_box);

        runs_on_list_boxes(start_box);

        cout << "После сортировки: ";
        print_list(start_box);

        delete_list(start_box);
    } else {
        cout << "список пуст" << endl << endl;
    }
    
    return 0;
}
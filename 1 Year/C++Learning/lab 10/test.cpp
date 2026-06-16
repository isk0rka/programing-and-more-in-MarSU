#include <iostream>
#include <ctime>

using namespace std;

struct list_box {
    int value;
    list_box* next;
};

list_box* first_list_box = nullptr;

void add_new_end_list_box() {
    list_box* new_node = new list_box;
    new_node->value = rand() % 100;
    new_node->next = nullptr;

    if (first_list_box == nullptr) {
        first_list_box = new_node;
        return;
    }

    list_box* current = first_list_box;
    while (current->next != nullptr) {
        current = current->next;
    }

    current->next = new_node;
}

void output_ones_list_box(list_box* current_list_box) {
    if (current_list_box == nullptr) {
        return;
    }

    cout << current_list_box->value << ' ';
}

void walk_on_list_boxes_rec(list_box* current_list_box,
                            void (*processing_on_list_box)(list_box*)) {
    if (current_list_box == nullptr) {
        return;
    }

    processing_on_list_box(current_list_box);
    walk_on_list_boxes_rec(current_list_box->next, processing_on_list_box);
}

void walk_on_list_boxes(void (*processing_on_list_box)(list_box*)) {
    walk_on_list_boxes_rec(first_list_box, processing_on_list_box);
}

void input_count_n_fill_new_list_box() {
    int count_list_box;

    do {
        cout << "введите количество элементов в списке (не менее 0)= ";
        cin >> count_list_box;
    } while (count_list_box < 0);

    for (int i = 0; i < count_list_box; i++) {
        add_new_end_list_box();
    }
}

void output_list_boxes() {
    if (first_list_box == nullptr) {
        cout << "список пуст" << '\n';
        return;
    }

    cout << "список : ";
    walk_on_list_boxes(output_ones_list_box);
    cout << '\n';
}

list_box* reverse_list_boxes_tail_rec(list_box* current, list_box* prev) {
    if (current == nullptr) {
        return prev;
    }

    list_box* next_node = current->next;
    current->next = prev;

    return reverse_list_boxes_tail_rec(next_node, current);
}

void mirror_list_boxes() {
    first_list_box = reverse_list_boxes_tail_rec(first_list_box, nullptr);
}

void unload_mem_list_box() {
    list_box* current = first_list_box;

    while (current != nullptr) {
        list_box* next = current->next;
        delete current;
        current = next;
    }

    first_list_box = nullptr;
}

int valid_user_swith_change(int user_swith_change) {
    do {
        cout << "выберите действие: ";
        cin >> user_swith_change;
        cout << endl;
    } while ((user_swith_change > 3) || (user_swith_change < 0));

    return user_swith_change;
}

void context_menu_interaction_list_box() {
    int user_swith_change = -1;
    bool run_prog = true;

    while (run_prog) {
        cout << "выберите действие:" << endl;
        cout << "1 - добавить элементы" << endl;
        cout << "2 - вывести список" << endl;
        cout << "3 - поменять местами x1 и xn, x2 и xn-1 и вывести" << endl;
        cout << "0 - выход из программы" << endl;

        user_swith_change = valid_user_swith_change(user_swith_change);

        switch (user_swith_change) {
            case 1:
                input_count_n_fill_new_list_box();
                output_list_boxes();
                break;

            case 2:
                output_list_boxes();
                break;

            case 3:
                if (first_list_box == nullptr) {
                    cout << "список пуст" << '\n';
                } else {
                    mirror_list_boxes();
                    cout << "после перестановки : ";
                    walk_on_list_boxes(output_ones_list_box);
                    cout << '\n';
                }
                break;

            case 0:
                unload_mem_list_box();
                run_prog = false;
                break;

            default:
                run_prog = false;
                break;
        }
    }
}

int main() {
    srand(time(NULL));
    first_list_box = nullptr;
    context_menu_interaction_list_box();
    return 0;
}
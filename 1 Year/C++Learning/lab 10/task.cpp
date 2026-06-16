/* Дан список L, элементы которого являются целыми числами.
 Поменять местами элементы списка x1 и xn, x2 и xn-1 и т.д. */

#include "iostream"
#include "ctime"
 
using namespace std;

struct list_box{
    int value;
    list_box* next;
};

list_box* first_list_box;

list_box* get_last_list_box(list_box* current_list_box) {
    if (current_list_box == nullptr) {
        return nullptr;
    }

    if (current_list_box -> next == nullptr) {
        return current_list_box;
    }

    return get_last_list_box(current_list_box->next);
}

void add_new_end_list_box() {
    list_box* node_list_box = new list_box;
    node_list_box -> value = rand() % 100;
    node_list_box -> next = nullptr;

    if (first_list_box == nullptr) {
        first_list_box = node_list_box;
        return;
    }

    list_box* last_list_box = get_last_list_box(first_list_box);
    last_list_box -> next = node_list_box;
}

void output_ones_list_box(list_box* current_list_box) {
    if (current_list_box == nullptr) {
        return;
    }
    cout << current_list_box -> value << ' ';
}

void rec_walk_on_list_boxes(list_box* current_list_box,void (*processing_on_list_box)(list_box*)) {
    if (current_list_box != nullptr) {
        processing_on_list_box(current_list_box);
        rec_walk_on_list_boxes(current_list_box->next, processing_on_list_box);   
    }
}

void walk_on_list_boxes(void (*processing_on_list_box)(list_box*)) {
    rec_walk_on_list_boxes(first_list_box, processing_on_list_box);
}

int get_length(list_box* head) {
    int len = 0;
    for (list_box* cur = head; cur != nullptr; cur = cur->next) {
        ++len;
    }
    return len;
}

list_box* get_node_at(list_box* head, int index) {
    int i = 0;
    list_box* cur = head;
    while (cur != nullptr && i < index) {
        cur = cur -> next;
        ++i;
    }
    return cur;
}

void mirror_list_values() {
    int size = get_length(first_list_box);
    for (int i = 0; i < size / 2; ++i) {
        list_box* left  = get_node_at(first_list_box, i);
        list_box* right = get_node_at(first_list_box, size - i - 1);

        int tmp      = left->value;
        left->value  = right->value;
        right->value = tmp;
    }
}

void input_count_n_fill_new_list_box(){
    int count_list_box;
    do {
        cout << "введите количество элементов в списке (не менее 0)= ";
        cin >> count_list_box;
    } while (count_list_box < 0);

    for (int i = 0; i < count_list_box; i++)
    {
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

void mirror_output_list_boxes(){
    if (first_list_box == nullptr) {
        cout << "список пуст" << '\n';
        return;
    }

    mirror_list_values();          

    cout << "зеркальный список : ";
    walk_on_list_boxes(output_ones_list_box);
    cout << '\n';
}

int valid_user_swith_change(int user_swith_change){
    do {
        cout << "выберите действие: ";
        cin >> user_swith_change;
        cout << endl;
    } while ((user_swith_change > 3) || (user_swith_change < 0) );
    return user_swith_change;
}

void unload_mem_list_box_rec(list_box* current_list_box) {
    if (current_list_box == nullptr) {
        return;
    }

    unload_mem_list_box_rec(current_list_box->next);
    delete current_list_box;
}

void unload_mem_list_box() {
    unload_mem_list_box_rec(first_list_box);
    first_list_box = nullptr;
}

void context_menu_interaction_list_box(){
    int user_swith_change = -1;
    bool run_prog = true;

    while (run_prog)
    {
        cout << "выберите действие:" << endl;
        cout << "1 - добавить элементы" << endl;
        cout << "2 - вывести список" << endl;
        cout << "3 - отразить и вывести список" << endl;
        cout << "0 - выход из программы" << endl;
        user_swith_change = valid_user_swith_change(user_swith_change);

        switch (user_swith_change){
            case 1:
                input_count_n_fill_new_list_box();
                output_list_boxes();
                break;
            case 2:
                output_list_boxes();
                break;
            case 3:
                mirror_output_list_boxes();
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

int main(){
    srand(time(NULL));
    first_list_box = nullptr;
    context_menu_interaction_list_box();
    return 0;
}
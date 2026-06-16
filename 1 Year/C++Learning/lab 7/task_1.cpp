/* Запрограммировать решение задачи: разработать алгоритм  и запрограммировать с использованием подпрограмм,
отладить его, запустить на выполнение, проверить на верность выводимые результаты с использованием подпрограмм.
Даны вещественные квадратные матрицы A и B порядка n. Найти матрицу, равную AB+BA.
Указание: реализовать подпрограммы ввода, вывода матрицы, суммы и произведения матриц.
Дополнительно предусмотреть ввод валидных значений для порядкового значения n.
пример:
Введите размер массива (не более 15)
size = -5
Ваше значение меньше 0, повторите ввод
size = 5
Исходный массив А:
2 -3 4 5 7
4 -6 1 5 3
4  6 4 7 9
1 -2 4 2 1
3  7 3 3 1
Исходный массив В:
-25 -5 6 10 5
 12  4 3 17 -1
 -5  4 2 -4 7
-11 -3 0 1  3
  0 -4 6 2  -1
Результат АВ + ВА:
-182  107  27  -101  -72
-166  -150 155  77   184
-94   23   101  173  52
-115  63  -20   -95  -42
-32   61   76   165  80 */

#include <iostream> 
#include <ctime>
#include <string>

using namespace std;

int get_User_Input(int input_int, string ui_massage){
    cout << ui_massage;
    cin >> input_int;
    return input_int;
}

int get_Valid_Size_Matrix() {
    int n;
    n = get_User_Input(n, "Введите размер массива (не более 15): ");
    while ((n < 0)|| (n > 15)) {
        if (n > 15) {
            n = get_User_Input(n, "Ваше значение не может быть больше 15, повторите ввод: ");
        }
        else if (n < 0) {
            n = get_User_Input(n, "Ваше значение не может быть меньше 0, повторите ввод: ");
        }
    }
    return n;
}

int** arise_Pointer_matrix(int Size_Matrix){
    int** New_Matrix = new int* [Size_Matrix];
    for (int i = 0; i < Size_Matrix; i++) {
        New_Matrix[i] = new int [Size_Matrix];  
    }
    return New_Matrix;    
}

void rand_Input_Matrix(int** table, int n) { 
        for (int i = 0; i < n; i++) { 
            for (int j = 0; j < n; j++) {
                table[i][j] = rand() % 101 - 50;
            }
    }
}

void output_Matrix(int** table, int n) { 
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < n; j++) {
            cout.width(8);
            cout << table[i][j];
        }
        cout << endl;
    }
}

void multiply_Matrix(int** A, int** B, int** result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = 0;
            for (int k = 0; k < n; k++) { 
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}   

void sum_Matrix(int** A, int** B, int** result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }
}

void Cleaner_for_Matrix(int** &Del_Matrix,int Size_Matrix){
    for (int i = 0; i < Size_Matrix; i++){
        delete[] Del_Matrix[i];
    }
    delete[] Del_Matrix;
    Del_Matrix = nullptr;
}

int main() {
    srand(time(NULL));
    int n;
    n = get_Valid_Size_Matrix();
    
    int** A = arise_Pointer_matrix(n);
    int** B = arise_Pointer_matrix(n);
    int** AB = arise_Pointer_matrix(n);
    int** BA = arise_Pointer_matrix(n);
    int** sumResult = arise_Pointer_matrix(n);

    cout << "элементы первой матрицы:" << endl;
    rand_Input_Matrix(A, n);
    output_Matrix(A,n);
    cout << "элементы второй матрицы:" << endl;
    rand_Input_Matrix(B, n);
    output_Matrix(B,n);

    multiply_Matrix(A, B, AB, n);
    multiply_Matrix(B, A, BA, n); 
    sum_Matrix(AB, BA, sumResult, n);

    cout << "Результат AB + BA:" << endl;
    output_Matrix(sumResult, n); 


    Cleaner_for_Matrix(A,n);
    Cleaner_for_Matrix(B,n);
    Cleaner_for_Matrix(AB,n);
    Cleaner_for_Matrix(BA,n);
    Cleaner_for_Matrix(sumResult,n);

    return 0;
}
/* Найти номер строки матрицы, сумма которых наибольшая. */
#include "iostream"
#include "ctime"
#include "string"

using namespace std;

int get_User_Input(string ui_massage){
    int input_int;
    cout << ui_massage;
    cin >> input_int;
    return input_int;
}

int Get_Matrix_Size_With_Validation() {
    int size_matrix = get_User_Input("Введите размер матрицы (значение должно былть больше  0): ");
        while (size_matrix < 1) {
            size_matrix = get_User_Input("Размер матрицы не может быть меньше или равным 0. Введите размер матрицы: ");
        }
    return size_matrix;
}

void Randomization_Matrix(int** Matrix, int matrix_size){
    for (int i = 0; i < matrix_size; i++){
        for (int j = 0; j < matrix_size; j++){
            Matrix[i][j] = rand() % 101 - 50;
        }
    }
}

void output_Matrix(int** table, int n){ 
    for (int i = 0; i < n; i++) { 
        cout << (i+1) << ":";
        for (int j = 0; j < n; j++) {
            cout.width(8);
            cout << table[i][j];
        }
        cout << endl;
    }
}

int** arise_Pointer_matrix(int Size_Matrix){
    int** New_Matrix = new int* [Size_Matrix];
    for (int i = 0; i < Size_Matrix; i++) {
        New_Matrix[i] = new int [Size_Matrix];  
    }
    return New_Matrix;    
}


string Find_Max_Sum_line_Matrix(int** Matrix, int matrix_size){
    int current_max_line_sum, line_sum;
    string nomber_line_with_max_sum = "";
    for (int i = 0; i < matrix_size; i++){
        line_sum = 0;
        for (int j = 0; j < matrix_size; j++){
            line_sum += Matrix[i][j];
        }
         if (i == 0){
            current_max_line_sum = line_sum;
        }
        if (line_sum > current_max_line_sum) {
            current_max_line_sum = line_sum;
            nomber_line_with_max_sum = to_string(i);
        } else if (line_sum == current_max_line_sum) {
            nomber_line_with_max_sum = nomber_line_with_max_sum + (" " + to_string(i));
        }
    }
    return (nomber_line_with_max_sum);
}

void Cleaner_for_Matrix(int** &Del_Matrix,int Size_Matrix){
    for (int i = 0; i < Size_Matrix; i++){
        delete[] Del_Matrix[i];
    }
    delete[] Del_Matrix;
    Del_Matrix = nullptr;
}

int main(){
    srand(time(NULL)); 

    int n = Get_Matrix_Size_With_Validation();
    int** A = arise_Pointer_matrix(n);

    cout << "Матрица: " << endl;
    Randomization_Matrix(A, n);
    output_Matrix(A,n);
    cout << "Номер строки с наибольшей суммой: " << Find_Max_Sum_line_Matrix(A, n) << endl;

    Cleaner_for_Matrix(A,n);
}
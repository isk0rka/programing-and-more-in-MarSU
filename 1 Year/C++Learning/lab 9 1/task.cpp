/*16. Для каждой строки исходного файла выбрать группы цифр,
 конвертировать каждую группу цифр в целое число
 найти сумму всех чисел для каждой строки.
 Результат поместить во второй файл в виде:
“1” - 145; “2” -12; “3” - 0; и т.д.*/

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const string INPUT_FILE_PATH = "input.txt";
const string OUTPUT_FILE_PATH = "output.txt";

bool find_input_file(){
    ifstream check_file(INPUT_FILE_PATH,  ios::in);
    if (check_file.is_open()){
        check_file.close();
        return true;
    }
    return false;
}

void sum_from_file_input(){
    string line_from_file;
    string line_by_file = "";

    int line_numb = 1;
    int sum_by_line_from_file = 0;
    int counter_unmub;

    ifstream input_fr_numberic_file(INPUT_FILE_PATH);
    ofstream output_fr_nuberic_file(OUTPUT_FILE_PATH);
    if (input_fr_numberic_file.is_open()){
        char ch_by_file = input_fr_numberic_file.get();
        while (ch_by_file != EOF) {
            if ((ch_by_file >= '0') && (ch_by_file <= '9')){
                line_by_file = line_by_file + ch_by_file;
            } else {
                if (!(line_by_file.empty())){
                    sum_by_line_from_file = sum_by_line_from_file + stoi(line_by_file);
                    line_by_file = "";
                }
                if (ch_by_file == '\n'){
                    output_fr_nuberic_file << "строка :"<< line_numb;
                    output_fr_nuberic_file << " | сумма:"<< sum_by_line_from_file << "\n";
                    sum_by_line_from_file = 0;
                    line_numb++;
                }
            }
    
                ch_by_file = input_fr_numberic_file.get();
            }
            if (!(line_by_file.empty())){
                sum_by_line_from_file = sum_by_line_from_file + stoi(line_by_file);
                output_fr_nuberic_file << "строка :"<< line_numb;
                output_fr_nuberic_file << " | сумма:"<< sum_by_line_from_file << "\n";
            }
            ch_by_file = input_fr_numberic_file.get();
        }
        
        output_fr_nuberic_file << "строка :"<< line_numb;
        output_fr_nuberic_file << " | сумма:"<< sum_by_line_from_file << "\n";
        sum_by_line_from_file = 0;
                    
        output_fr_nuberic_file.close();
        input_fr_numberic_file.close();
}

int main(){
    if (!(find_input_file())){
        return 1;
    }

    sum_from_file_input();
    return 0;
}
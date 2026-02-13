// Реализовать ввод данных в строку до первого введённого с клавиатуры пользователем символа цифры 5.
// Подсчитать количество введённых слов такой строки
#include <iostream>
#include <string>
using namespace std;

int findsep(char str, string cstr){
    int i = 0;
    while (i <= (cstr.length() - 1)) {
        if (str == cstr[i]) {
            return i;
        }
        i++;
    }
    return -1;
}
void logic(){
    char chsyn,chsave;
    int wordcounter = 0;
    string word_sep = "";
    cout << "введите символы разделители слов: \n";
    getline(cin,word_sep);
    cout << "\n\n введите строку (ввод будет произведен пока не будет встречена цифра 5): \n";
    do {
        chsyn = getchar();
        if ((findsep(chsyn,word_sep) != -1) && (findsep(chsave,word_sep) == -1)) {
            wordcounter++;
        }
        chsave = chsyn;
    } while (chsyn != '5');
    cout << "число слов: " << wordcounter + 1 << "\n"; 
}

int main(){
    logic();
    return 0;
}

#include <iostream>
#include <string>
using namespace std;

int main(){
    string str;
    cout << "\n введите строку: ";
    getline(cin,str);

    for (int i = 0; i < str.length()/ 2; i++) {
        if (str[i] != str[str.length()-1-i]) {
            cout << "строка не является палиндромом";
            cout << "\n\n\n\n";

            return 0;
        }
    }
    cout << "строка является палиндромом";
    cout << "\n\n\n\n";
    
    return 0;
}
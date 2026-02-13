#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

void input(string &c,string s){
    cout << "\n" << s;
    getline(cin,c);
}

bool palindrome(string str){
    for (int i = 0; i < str.length()/ 2; i++) {
        if (str[i] != str[str.length()-1-i]) {
            return false;
        }
    }
    return true;
}
     
int main(){
    string str;
    input(str,"введите строку: ");

    if (palindrome(str)) {
        cout << "строка является палиндромом";
    } else {
        cout << "строка не является палиндромом";
    }

    cout << "\n\n\n\n";
    return 0;
}
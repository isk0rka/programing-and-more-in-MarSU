#include <iostream>
#include <stdio.h> 
#include <string>

using namespace std;

int main() 
{
    // объявление переменных
    string namebox;
    int  price, weight_name_box, client_pay, radi;
    // ввод нужных данных

    cout << "\n" << "Введите название товара = ";
    cin >> namebox; // ввод переменной названия продукта
    
    size_t len1 = namebox.length(); // динамическая длина для вычисления на русскую раскладку
    radi = 34 + (len1 / 2);

    cout << "Введите стоимость товара = ";
    cin >> price; // ввод переменной цены продукта

    cout << "Введите вес/количество товара = ";
    cin >> weight_name_box; // ввод переменной количества|веса
    
    cout << "Введите внесённую сумму = ";
    cin >> client_pay; // ввод переменной денег которые дал клиент
    
    // формирование и вывод чека

    cout << radi;

    cout << "\n" << "\n"; // создание красивого и удобного отступа

    cout.width(22);
    cout.fill('=');
    
    cout << right << "чек" ; // оглавление чека

    cout.width(22);
    cout.fill('=');

    cout << "\n" << "\n"; // создание красивого и удобного отступа
    
    cout.fill(' ');

    cout.width(6);
    cout << left << "Товар:";
    cout.width(radi);
    cout << right << namebox << "\n"; //вывод имени товара

    cout.width(5);
    cout << left << "Цена:";
    cout.width(24);
    cout << right << price << right << "руб/(кг/шт)" << "\n"; //вывод цены товара

    cout.width(6);
    cout << left << "Итого:";
    cout.width(34);
    cout << right << ( price * weight_name_box )<< "\n"; // вывод веса|количества товара

    cout.width(8);
    cout << left << "Внесено:";
    cout.width(32);
    cout << right << client_pay  << "\n"; // вывод веса|количества товара

    cout.width(6);
    cout << left << "Сдача:";
    cout.width(34);
    cout << right << (client_pay - ( price * weight_name_box )) << "\n"; // вывод веса|количества товара

    cout.width(41);
    cout.fill('=');

    cout << "\n" << "\n" << "\n"; // отделение чека от строки завершения
    return 0;
}
/*16. Дан прямоугольный треугольник.
 Найти его периметр и площадь.
 (Описать тип – прямоугольный треугольник.)*/
#include "iostream"
#include "cmath"

using namespace std;

struct triangle
{
    double side_a;
    double side_b;
    double hypotenuse;
    double perimeter;
    double area;
};

double calculate_triangle_hypotenuse(triangle user_triangle){
    return (sqrt(pow(user_triangle.side_a, 2) + pow(user_triangle.side_b, 2)));
}

double calculate_triangle_perimeter(triangle user_triangle){
    return (user_triangle.side_a + user_triangle.side_b + user_triangle.hypotenuse);
}

double calculate_triangle_area(triangle user_triangle){
    return ((user_triangle.side_a * user_triangle.side_b) / 2);
}

void calculate_triangle_metric(triangle& user_triangle){
    user_triangle.hypotenuse = calculate_triangle_hypotenuse(user_triangle);
    user_triangle.perimeter = calculate_triangle_perimeter(user_triangle);
    user_triangle.area = calculate_triangle_area(user_triangle);
}

void output_triangle_metric(triangle user_triangle){
    cout<< endl<< endl<< "Сторона a: " << user_triangle.side_a << endl;
    cout << "Сторона b: " << user_triangle.side_b << endl;
    cout << "Гипотенуза: " << user_triangle.hypotenuse << endl;
    cout << "Периметр: " << user_triangle.perimeter << endl;
    cout << "Площадь: " << user_triangle.area << endl<< endl;
}

void output_triangle_side_a_by_user(){
    cout<< endl<< "Введите сторону a прямоугольного треугольника: "<< endl;
}

void output_triangle_side_b_by_user(){
    cout<< endl<< "Введите сторону b прямоугольного треугольника: "<< endl;
}

void output_unvalid_side_a(){
    cout<< endl<< "сторона a должна быть больше 0 : "<< endl;
}

void output_unvalid_side_b(){
    cout<< endl<< "сторона b должна быть больше 0 : "<< endl;
}

void input_triangle_side_a_by_user(triangle& user_triangle){
    cin >> user_triangle.side_a;;
}

bool is_valid_side_a(triangle user_triangle){
    if (user_triangle.side_a > 0){
        return true;
    }
    return false;
}

void validation_input_size_a(triangle& user_triangle){
    while (!(is_valid_side_a(user_triangle))){
        output_unvalid_side_a();
        input_triangle_side_a_by_user(user_triangle);
    }
}

void input_triangle_side_b_by_user(triangle& user_triangle){
    cin >> user_triangle.side_b;;
}

bool is_valid_side_b(triangle user_triangle){
    if (user_triangle.side_b > 0){
        return true;
    }
    return false;
}

void validation_input_size_b(triangle& user_triangle){
    while (!(is_valid_side_b(user_triangle))){
        output_unvalid_side_b();
        input_triangle_side_b_by_user(user_triangle);
    }
}

void input_triangle_metric_by_user(triangle& user_triangle){
    output_triangle_side_a_by_user();
    input_triangle_side_a_by_user(user_triangle);
    validation_input_size_a(user_triangle);
    output_triangle_side_b_by_user();
    input_triangle_side_b_by_user(user_triangle);
    validation_input_size_b(user_triangle);
}

int main()
{
    triangle user_triangle;
    input_triangle_metric_by_user(user_triangle);
    calculate_triangle_metric(user_triangle);
    output_triangle_metric(user_triangle);
    return 0;
}
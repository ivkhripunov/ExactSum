#include <iostream>
#include <ios>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <fstream>


double two_double_sum(double &error, const double &a,
                      const double &b) {
    /// Функция для вычисления суммы a + b и ошибки на шаге

    double sum = a + b; //вычисляем сумму напрямую
    double b_virtual = sum - a;
    double a_virtual = sum - b_virtual;

    double b_round_error = b - b_virtual;
    double a_round_error = a - a_virtual;

    double step_error = a_round_error + b_round_error;

    error += step_error;

    return sum;
}


int main() {
    //создаем нужные переменные
    boost::multiprecision::cpp_dec_float_100 boost_sum("0"); //эталонная сумма
    double trivial_double_sum = 0;
    double improved_double_sum = 0;

    double final_error = 0;

    //открываем файл на считывание
    std::fstream fin;

    fin.open("res.csv", std::ios::in);

    std::string tmp;

    while (fin >> tmp) {

        //приведение строки файла к числу
        boost::multiprecision::cpp_dec_float_100 boost_value(tmp);
        double double_value = std::stod(tmp);

        //выполняем
        boost_sum += boost_value;
        trivial_double_sum += double_value;

        improved_double_sum = two_double_sum(final_error, improved_double_sum, double_value);

    }

    double precision_sum_double = boost::numeric::converter<double, boost::multiprecision::cpp_dec_float_100>::convert(
            boost_sum);

    boost::multiprecision::cpp_dec_float_100 precision_trivial_double_sum = boost::numeric::converter<boost::multiprecision::cpp_dec_float_100, double>::convert(
            trivial_double_sum);

    boost::multiprecision::cpp_dec_float_100 precision_improved_double_sum = boost::numeric::converter<boost::multiprecision::cpp_dec_float_100, double>::convert(
            improved_double_sum + final_error);


    std::cout.precision(100);
    std::cout << "Эталонная сумма: " << boost_sum << std::endl;
    std::cout.precision(20);
    std::cout << "Эталонная сумма (приведение к double): " << precision_sum_double << std::endl;
    std::cout << "Сумма, посчитанная стандартным способом: " << trivial_double_sum << std::endl;
    std::cout.precision(100);
    std::cout << "Ошибка стандартного способа: " << boost_sum - precision_trivial_double_sum << std::endl;
    std::cout << "Относительная ошибка стандартного способа: " << (boost_sum - precision_trivial_double_sum) / boost_sum
              << std::endl;
    std::cout.precision(20);
    std::cout << "Сумма, посчитанная улучшенным методом: " << improved_double_sum + final_error << std::endl;
    std::cout.precision(100);
    std::cout << "Ошибка улучшенного метода: " << boost_sum - precision_improved_double_sum << std::endl;
    std::cout << "Относительная ошибка улучшенного метода: " <<
              abs((boost_sum - precision_improved_double_sum) / boost_sum) << std::endl;

}

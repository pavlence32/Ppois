#ifndef ALGOS_H
#define ALGOS_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
 
/**
 * @brief Правило подстановки нормального алгоритма Маркова
 *
 * Хранит левую и правую части подстановки и признак того,
 * является ли правило заключительным.
 */
class Rule
{
    private:
    std::string right; ///< Правая часть правила (на что заменяем)
    std::string left;  ///< Левая часть правила (что ищем в слове)
    bool is_final;     ///< Признак заключительного правила
 
    public:
    /**
     * @brief Конструктор правила
     * @param right Правая часть правила
     * @param left Левая часть правила
     * @param is_final true, если правило заключительное
     */
    Rule(std::string right,std::string left,bool is_final);
 
    /**
     * @brief Возвращает правую часть правила
     * @return Строка, на которую происходит замена
     */
    std::string Get_right();
 
    /**
     * @brief Возвращает левую часть правила
     * @return Строка, которую нужно найти в слове
     */
    std::string Get_left();
 
    /**
     * @brief Проверяет, является ли правило заключительным
     * @return true, если после применения правила алгоритм останавливается
     */
    bool Is_final();
};
 
/**
 * @brief Разбор правил алгоритма Маркова
 *
 * Содержит статические методы для разбора одной строки
 * и для чтения всех правил из файла.
 */
class Parser
{
    public:
    /**
     * @brief Преобразует строку в правило
     * @param s Строка с описанием правила
     * @return Построенное правило
     */
    static Rule translate(std::string &s);
 
    /**
     * @brief Считывает все правила из файла
     * @param filename Путь к файлу с правилами
     * @return Список правил в порядке их следования в файле
     */
    static std::vector<Rule> parser(const std::string &filename);
};
 
/**
 * @brief Нормальный алгоритм Маркова
 *
 * Хранит текущее слово и список правил, последовательно
 * применяет правила к слову до остановки алгоритма.
 */
class Markov
{
    private:
    std::string word;        ///< Текущее слово
    std::vector<Rule> rules; ///< Список правил подстановки
 
    public:
    /**
     * @brief Конструктор алгоритма
     * @param a Начальное слово
     * @param filename Путь к файлу с правилами
     */
    Markov(std::string a,const std::string & filename);
 
    /**
     * @brief Возвращает текущее слово
     * @return Слово после применённых к этому моменту правил
     */
    std::string get_result();
 
    /**
     * @brief Выполняет один шаг алгоритма
     * @return true, если правило было применено и алгоритм может продолжаться;
     *         false, если ни одно правило не подошло или сработало заключительное
     */
    bool step();
 
    /**
     * @brief Выполняет алгоритм до его остановки
     */
    void run();
};
#endif

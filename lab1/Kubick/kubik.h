#ifndef KUBIK_H
#define KUBIK_H
#include <string>
#include <iostream>
/**
 * @brief Класс, представляющий кубик Рубика
 *
 * Хранит состояние всех 6 граней (каждая — матрица 3x3)
 * и предоставляет операции поворота, загрузки цветов и проверки решённости.
 */
class Kubick
{
    char faces[6][3][3];
     /**
     * @brief Возвращает символ-заполнитель для грани
     * @param index Индекс грани
     * @return Символ, соответствующий цвету/грани
     */
    char fill_place(int index);
      /**
     * @brief Считывает строку матрицы грани
     * @param f Индекс грани
     * @param s Индекс строки
     * @param out Буфер для записи считанных значений
     */
    void get_row(int f,int s,char * out);
     /**
     * @brief Записывает строку в матрицу грани
     * @param f Индекс грани
     * @param s Индекс строки
     * @param out Буфер с данными для записи
     */
    void set_row(int f,int s,char * out);
    /**
     * @brief Считывает столбец матрицы грани
     * @param f Индекс грани
     * @param s Индекс столбца
     * @param out Буфер для записи считанных значений
     */
    void get_col(int f,int s,char * out);
     /**
     * @brief Записывает столбец в матрицу грани
     * @param f Индекс грани
     * @param s Индекс столбца
     * @param out Буфер с данными для записи
     */
    void set_col(int f,int s,char * out);
    /**
     * @brief Поворачивает матрицу указанной грани (саму грань, без соседей)
     * @param face Индекс грани для поворота
     */
    void rotate_face_matrix(int face);
        /**
     * @brief Сдвигает элементы соседних граней при повороте
     * @param face Индекс грани, вокруг которой происходит поворот
     */
    void shift_neighbours(int face);
     /**
     * @brief Сравнивает два состояния кубика на равенство
     * @param a Кубик для сравнения
     * @return true, если состояния совпадают
     */
    public:
    /**
 * @brief Сравнивает два кубика на равенство
 * @param b Первый кубик для сравнения
 * @param a Второй кубик для сравнения
 * @return true, если расцветки всех граней у обоих кубиков совпадают
 */
    friend bool operator==(const Kubick &b,const Kubick&a);
    /**
 * @brief Сравнивает два кубика на неравенство
 * @param b Первый кубик для сравнения
 * @param a Второй кубик для сравнения
 * @return true, если хотя бы одна грань кубиков отличается
 */
    friend bool operator!=(const Kubick&b,const Kubick &a);
     /**
     * @brief Конструктор по умолчанию
     *
     * Инициализирует кубик в собранном состоянии.
     */
    Kubick();
    /**
     * @brief Оператор вывода в поток
     * @param out Выходной поток
     * @param a Кубик для вывода
     * @return Ссылка на поток вывода
     */
    friend std::ostream& operator<<(std::ostream &out,Kubick &a);
     /**
     * @brief Загружает расцветку кубика из входного потока
     * @param inf Входной поток с данными о цветах
     * @return true, если загрузка прошла успешно
     */
    bool load_color(std::istream &inf);

    /**
     * @brief Выполняет поворот грани кубика
     * @param a Индекс грани для поворота
     * @param trav Направление/количество поворотов
     * @return Ссылка на текущий объект (для цепочки вызовов)
     */
    Kubick &turn(int a,int trav);
    /**
     * @brief Проверяет, собран ли кубик (все грани одного цвета)
     * @return true, если кубик в решённом состоянии
     */
    bool checkcolorplace();
      /**
     * @brief Приводит кубик в случайное (перемешанное) состояние
     */
    void random_place();
};
#endif
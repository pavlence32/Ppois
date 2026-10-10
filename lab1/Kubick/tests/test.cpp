#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>
#include "../kubik.h"
 
TEST_CASE("1. Тест инициализации и сравнения кубиков", "[kubick]") {
    Kubick cube1;
    Kubick cube2;
 
    REQUIRE(cube1.checkcolorplace() == true);
    REQUIRE(cube1 == cube2);
    REQUIRE_FALSE(cube1 != cube2);
}
 
TEST_CASE("2. Тест всех возможных поворотов граней", "[kubick]") {
    Kubick cube;
    Kubick original;
 
    for (int face = 0; face < 6; ++face) {
        SECTION("Тестирование грани " + std::to_string(face)) {
            cube.turn(face, 1);
            CHECK(cube.checkcolorplace() == false);
            CHECK(cube != original);
 
            cube.turn(face, 1).turn(face, 1).turn(face, 1);
            CHECK(cube.checkcolorplace() == true);
            CHECK(cube == original);
 
            cube.turn(face, 0);
            CHECK(cube.checkcolorplace() == false);
            CHECK(cube != original);
 
            cube.turn(face, 1);
            CHECK(cube.checkcolorplace() == true);
            CHECK(cube == original);
        }
    }
}
 
TEST_CASE("3. Тест перемешивания и вывода в поток", "[kubick]") {
    Kubick cube;
 
    bool mixed = false;
    for (int i = 0; i < 5 && !mixed; ++i) {
        cube.random_place();
        mixed = !cube.checkcolorplace();
    }
    CHECK(mixed);
 
    std::stringstream ss;
    REQUIRE_NOTHROW(ss << cube);
    CHECK(!ss.str().empty());
}
 
TEST_CASE("4. Полный тест загрузки цветов (load_color)", "[kubick]") {
    Kubick cube;
 
    SECTION("Невалидный пустой ввод") {
        std::stringstream empty_stream("");
        CHECK(cube.load_color(empty_stream) == false);
    }
 
    SECTION("Попытка загрузить валидный поток данных") {
        std::stringstream valid_stream;
        const char colors[] = {'R', 'O', 'W', 'Y', 'G', 'B'};
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 3; ++j) {
                valid_stream << colors[i] << " " << colors[i] << " " << colors[i] << "\n";
            }
        }
        CHECK(cube.load_color(valid_stream) == true);
    }
}
 
TEST_CASE("5. Блок A B A' B' для соседних граней возвращает кубик за 6 повторений", "[kubick]") {
    int pairs[12][2] = {{0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 2}, {1, 3},
                        {1, 4}, {1, 5}, {2, 3}, {2, 5}, {3, 4}, {4, 5}};
 
    for (int p = 0; p < 12; ++p) {
        int a = pairs[p][0];
        int b = pairs[p][1];
        SECTION("Грани " + std::to_string(a) + " и " + std::to_string(b)) {
            Kubick cube;
            for (int i = 0; i < 6; ++i) {
                cube.turn(a, 1).turn(b, 1).turn(a, 0).turn(b, 0);
            }
            CHECK(cube.checkcolorplace() == true);
        }
    }
}
 
TEST_CASE("6. Противоположные грани не мешают друг другу", "[kubick]") {
    int pairs[3][2] = {{0, 1}, {2, 4}, {3, 5}};
 
    for (int p = 0; p < 3; ++p) {
        int a = pairs[p][0];
        int b = pairs[p][1];
        SECTION("Грани " + std::to_string(a) + " и " + std::to_string(b)) {
            Kubick cube;
            cube.turn(a, 1).turn(b, 1).turn(a, 0).turn(b, 0);
            CHECK(cube.checkcolorplace() == true);
        }
    }
}

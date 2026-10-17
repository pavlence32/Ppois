
#include <catch2/catch_test_macros.hpp>

#include "algos.h"

#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    const std::string test_filename = "test_rules.txt";

    void write_rules(const std::string& content)
    {
        std::ofstream file(test_filename);
        REQUIRE(file.is_open());

        file << content;
        file.close();
    }

    void remove_test_file()
    {
        std::remove(test_filename.c_str());
    }
}

TEST_CASE("Rule stores its values")
{
    Rule rule("B", "A", false);

    REQUIRE(rule.Get_left() == "A");
    REQUIRE(rule.Get_right() == "B");
    REQUIRE_FALSE(rule.Is_final());
}

TEST_CASE("Rule stores final flag")
{
    Rule rule("B", "A", true);

    REQUIRE(rule.Get_left() == "A");
    REQUIRE(rule.Get_right() == "B");
    REQUIRE(rule.Is_final());
}

TEST_CASE("Parser translates ordinary rule")
{
    std::string line = "A->B";

    Rule rule = Parser::translate(line);

    REQUIRE(rule.Get_left() == "A");
    REQUIRE(rule.Get_right() == "B");
    REQUIRE_FALSE(rule.Is_final());
}

TEST_CASE("Parser translates final rule")
{
    std::string line = "A->.B";

    Rule rule = Parser::translate(line);

    REQUIRE(rule.Get_left() == "A");
    REQUIRE(rule.Get_right() == "B");
    REQUIRE(rule.Is_final());
}

TEST_CASE("Parser translates rule with multiple characters")
{
    std::string line = "ABC->XYZ";

    Rule rule = Parser::translate(line);

    REQUIRE(rule.Get_left() == "ABC");
    REQUIRE(rule.Get_right() == "XYZ");
    REQUIRE_FALSE(rule.Is_final());
}

TEST_CASE("Parser reads rules from file")
{
    write_rules("A->B\nBC->D\nD->.E\n");

    const std::vector<Rule> rules = Parser::parser(test_filename);

    REQUIRE(rules.size() == 3);

    REQUIRE(rules[0].Get_left() == "A");
    REQUIRE(rules[0].Get_right() == "B");
    REQUIRE_FALSE(rules[0].Is_final());

    REQUIRE(rules[1].Get_left() == "BC");
    REQUIRE(rules[1].Get_right() == "D");
    REQUIRE_FALSE(rules[1].Is_final());

    REQUIRE(rules[2].Get_left() == "D");
    REQUIRE(rules[2].Get_right() == "E");
    REQUIRE(rules[2].Is_final());

    remove_test_file();
}

TEST_CASE("Parser throws when file does not exist")
{
    REQUIRE_THROWS_AS(
        Parser::parser("file_that_does_not_exist.txt"),
        std::runtime_error
    );
}

TEST_CASE("Markov step replaces first matching substring")
{
    write_rules("ab->X\n");

    Markov markov("zzabyyab", test_filename);

    REQUIRE(markov.get_result() == "zzabyyab");

    REQUIRE(markov.step());
    REQUIRE(markov.get_result() == "zzXyyab");

    remove_test_file();
}

TEST_CASE("Markov step returns false when no rule matches")
{
    write_rules("abc->X\n");

    Markov markov("hello", test_filename);

    REQUIRE_FALSE(markov.step());
    REQUIRE(markov.get_result() == "hello");

    remove_test_file();
}

TEST_CASE("Markov stops after applying a final rule")
{
    write_rules("A->.B\n");

    Markov markov("AAA", test_filename);

    REQUIRE_FALSE(markov.step());
    REQUIRE(markov.get_result() == "BAA");

    remove_test_file();
}

TEST_CASE("Markov uses the first applicable rule")
{
    write_rules("A->B\nB->C\n");

    Markov markov("A", test_filename);

    REQUIRE(markov.step());
    REQUIRE(markov.get_result() == "B");

    remove_test_file();
}

TEST_CASE("Markov run applies rules until final rule")
{
    write_rules("A->B\nB->C\nC->.D\n");

    Markov markov("A", test_filename);
    markov.run();

    REQUIRE(markov.get_result() == "D");

    remove_test_file();
}

TEST_CASE("Markov handles multiple replacements")
{
    write_rules("A->B\nB->C\n");

    Markov markov("AA", test_filename);

    REQUIRE(markov.step());
    REQUIRE(markov.get_result() == "BA");

    REQUIRE(markov.step());
    REQUIRE(markov.get_result() == "BB");

    REQUIRE(markov.step());
    REQUIRE(markov.get_result() == "CB");

    REQUIRE(markov.step());
    REQUIRE(markov.get_result() == "CC");

    REQUIRE_FALSE(markov.step());
    REQUIRE(markov.get_result() == "CC");

    remove_test_file();
}

TEST_CASE("Markov applies the supplied rules to AABB")
{
    write_rules("AA->B\nAB->C\nB->A\nC->.D\n");

    Markov markov("AABB", test_filename);
    markov.run();

    REQUIRE(markov.get_result() == "DA");

    remove_test_file();
}

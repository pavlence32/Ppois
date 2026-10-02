
#include <catch2/catch_test_macros.hpp>

#include "algos.h"

#include <fstream>
#include <string>


TEST_CASE("Rule stores left and right parts correctly")
{
    Rule rule("B", "A", false);

    REQUIRE(rule.Get_right() == "B");
    REQUIRE(rule.Get_left() == "A");
    REQUIRE_FALSE(rule.Is_final());
}

TEST_CASE("Rule can be final")
{
    Rule rule("B", "A", true);

    REQUIRE(rule.Get_right() == "B");
    REQUIRE(rule.Get_left() == "A");
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


TEST_CASE("Parser reads rules from file")
{
    const std::string filename = "test_rules.txt";

    {
        std::ofstream out(filename);
        REQUIRE(out.is_open());

        out << "A->B\n";
        out << "BC->D\n";
        out << "D->.E\n";
    }

    std::vector<Rule> rules = Parser::parser(filename);

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

    std::remove(filename.c_str());
}

TEST_CASE("Parser returns empty vector for nonexistent file")
{
    const std::string filename = "file_that_does_not_exist.txt";

    std::vector<Rule> rules = Parser::parser(filename);

    REQUIRE(rules.empty());
}



TEST_CASE("Markov step replaces first matching substring")
{
    const std::string filename = "test_rules.txt";

    {
        std::ofstream out(filename);
        out << "ab->X\n";
    }

    Markov markov("zzabyy", filename);

    REQUIRE(markov.get_result() == "zzabyy");

    bool can_continue = markov.step();

    REQUIRE(can_continue);
    REQUIRE(markov.get_result() == "zzXyy");

    std::remove(filename.c_str());
}

TEST_CASE("Markov step returns false when no rule matches")
{
    const std::string filename = "test_rules.txt";

    {
        std::ofstream out(filename);
        out << "abc->X\n";
    }

    Markov markov("hello", filename);

    REQUIRE_FALSE(markov.step());
    REQUIRE(markov.get_result() == "hello");

    std::remove(filename.c_str());
}

TEST_CASE("Markov stops after final rule")
{
    const std::string filename = "test_rules.txt";

    {
        std::ofstream out(filename);
        out << "A->.B\n";
    }

    Markov markov("AAA", filename);

    bool can_continue = markov.step();

    REQUIRE_FALSE(can_continue);
    REQUIRE(markov.get_result() == "BAA");

    std::remove(filename.c_str());
}

TEST_CASE("Markov uses first applicable rule")
{
    const std::string filename = "test_rules.txt";

    {
        std::ofstream out(filename);
        out << "A->B\n";
        out << "B->C\n";
    }

    Markov markov("A", filename);

    REQUIRE(markov.step());
    REQUIRE(markov.get_result() == "B");

    std::remove(filename.c_str());
}

TEST_CASE("Markov run applies rules until algorithm stops")
{
    const std::string filename = "test_rules.txt";

    {
        std::ofstream out(filename);
        out << "A->B\n";
        out << "B->C\n";
        out << "C->.D\n";
    }

    Markov markov("A", filename);

    markov.run();

    REQUIRE(markov.get_result() == "D");

    std::remove(filename.c_str());
}

TEST_CASE("Markov handles multiple replacements")
{
    std::ofstream file("test_rules.txt");
    file << "A->B\n";
    file << "B->C\n";
    file.close();

    Markov markov("AA", "test_rules.txt");

    REQUIRE(markov.step() == true);
    REQUIRE(markov.get_result() == "BA");

    REQUIRE(markov.step() == true);
    REQUIRE(markov.get_result() == "BB");

    REQUIRE(markov.step() == true);
    REQUIRE(markov.get_result() == "CB");

    REQUIRE(markov.step() == true);
    REQUIRE(markov.get_result() == "CC");

    REQUIRE(markov.step() == false);
    REQUIRE(markov.get_result() == "CC");

    std::remove("test_rules.txt");
}


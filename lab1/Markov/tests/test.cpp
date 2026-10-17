#include <catch2/catch_test_macros.hpp>

#include "algos.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>


static Markov make(const std::string &word, std::vector<std::string> lines)
{
    std::vector<Rule> rules;
    for (std::string &l : lines)
    {
        rules.push_back(Parser::translate(l));
    }
    return Markov(word, rules);
}


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

    REQUIRE(rule.Is_final());
}


TEST_CASE("translate: ordinary rule")
{
    std::string line = "A->B";
    Rule rule = Parser::translate(line);

    REQUIRE(rule.Get_left() == "A");
    REQUIRE(rule.Get_right() == "B");
    REQUIRE_FALSE(rule.Is_final());
}

TEST_CASE("translate: final rule")
{
    std::string line = "A->.B";
    Rule rule = Parser::translate(line);

    REQUIRE(rule.Get_left() == "A");
    REQUIRE(rule.Get_right() == "B");
    REQUIRE(rule.Is_final());
}

TEST_CASE("translate: empty right part (deletion)")
{
    std::string line = "a->";
    Rule rule = Parser::translate(line);

    REQUIRE(rule.Get_left() == "a");
    REQUIRE(rule.Get_right() == "");
    REQUIRE_FALSE(rule.Is_final());
}

TEST_CASE("translate: empty left part")
{
    std::string line = "->X";
    Rule rule = Parser::translate(line);

    REQUIRE(rule.Get_left() == "");
    REQUIRE(rule.Get_right() == "X");
}

TEST_CASE("translate: multi-character parts")
{
    std::string line = "abc->.xy";
    Rule rule = Parser::translate(line);

    REQUIRE(rule.Get_left() == "abc");
    REQUIRE(rule.Get_right() == "xy");
    REQUIRE(rule.Is_final());
}


TEST_CASE("parser reads rules from cin until empty line")
{
    std::istringstream in("A->B\nBC->D\nD->.E\n\nignored->x\n");
    std::streambuf *old = std::cin.rdbuf(in.rdbuf());

    std::vector<Rule> rules = Parser::parser();

    std::cin.rdbuf(old);

    REQUIRE(rules.size() == 3);

    REQUIRE(rules[0].Get_left() == "A");
    REQUIRE(rules[0].Get_right() == "B");
    REQUIRE_FALSE(rules[0].Is_final());

    REQUIRE(rules[1].Get_left() == "BC");
    REQUIRE(rules[1].Get_right() == "D");

    REQUIRE(rules[2].Get_left() == "D");
    REQUIRE(rules[2].Get_right() == "E");
    REQUIRE(rules[2].Is_final());
}

TEST_CASE("parser returns empty vector on immediate empty line")
{
    std::istringstream in("\n");
    std::streambuf *old = std::cin.rdbuf(in.rdbuf());

    std::vector<Rule> rules = Parser::parser();

    std::cin.rdbuf(old);

    REQUIRE(rules.empty());
}

// ---------- Markov::step ----------

TEST_CASE("step replaces first matching substring")
{
    Markov m = make("zzabyy", {"ab->X"});

    REQUIRE(m.get_result() == "zzabyy");
    REQUIRE(m.step());
    REQUIRE(m.get_result() == "zzXyy");
}

TEST_CASE("step replaces only the first occurrence")
{
    Markov m = make("aa", {"a->b"});

    REQUIRE(m.step());
    REQUIRE(m.get_result() == "ba");
}

TEST_CASE("step returns false when no rule matches")
{
    Markov m = make("hello", {"abc->X"});

    REQUIRE_FALSE(m.step());
    REQUIRE(m.get_result() == "hello");
}

TEST_CASE("step with no rules does nothing")
{
    Markov m = make("abc", {});

    REQUIRE_FALSE(m.step());
    REQUIRE(m.get_result() == "abc");
}

TEST_CASE("step stops after final rule")
{
    Markov m = make("AAA", {"A->.B"});

    REQUIRE_FALSE(m.step());
    REQUIRE(m.get_result() == "BAA");
}

TEST_CASE("step uses the first applicable rule (priority by order)")
{
    Markov m = make("ab", {"ab->X", "a->Y"});

    REQUIRE(m.step());
    REQUIRE(m.get_result() == "X");
}

TEST_CASE("step applies later rule if earlier does not match")
{
    Markov m = make("ab", {"zz->X", "a->Y"});

    REQUIRE(m.step());
    REQUIRE(m.get_result() == "Yb");
}

TEST_CASE("step with empty left part inserts at the beginning")
{
    Markov m = make("abc", {"->.X"});

    REQUIRE_FALSE(m.step());
    REQUIRE(m.get_result() == "Xabc");
}

TEST_CASE("step by step: A->B, B->C on AA")
{
    Markov m = make("AA", {"A->B", "B->C"});

    REQUIRE(m.step());
    REQUIRE(m.get_result() == "BA");

    REQUIRE(m.step());
    REQUIRE(m.get_result() == "BB");

    REQUIRE(m.step());
    REQUIRE(m.get_result() == "CB");

    REQUIRE(m.step());
    REQUIRE(m.get_result() == "CC");

    REQUIRE_FALSE(m.step());
    REQUIRE(m.get_result() == "CC");
}


TEST_CASE("run: chain ending with final rule")
{
    Markov m = make("A", {"A->B", "B->C", "C->.D"});

    m.run();
    REQUIRE(m.get_result() == "D");
}

TEST_CASE("run: sorting b and a (ba->ab)")
{
    Markov m = make("bbaa", {"ba->ab"});

    m.run();
    REQUIRE(m.get_result() == "aabb");
}

TEST_CASE("run: deleting all occurrences of a symbol")
{
    Markov m = make("banana", {"a->"});

    m.run();
    REQUIRE(m.get_result() == "bnn");
}

TEST_CASE("run: nothing to apply leaves word unchanged")
{
    Markov m = make("hello", {"xyz->Q"});

    m.run();
    REQUIRE(m.get_result() == "hello");
}

TEST_CASE("run: final rule stops even if other rules still match")
{
    Markov m = make("AB", {"A->.X", "B->Y"});

    m.run();
    REQUIRE(m.get_result() == "XB");
}
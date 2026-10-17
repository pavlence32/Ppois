#include "algos.h"
#include <stdexcept>
Rule::Rule(std::string right, std::string left, bool is_final)
{
    this->right = right;
    this->left = left;
    this->is_final = is_final;
}

std::string Rule::Get_left()
{
    return left;
}

std::string Rule::Get_right()
{
    return right;
}

bool Rule::Is_final()
{
    return is_final;
}

Rule Parser::translate(std::string &s)
{
    const size_t arrow = s.find("->");

    if (arrow == std::string::npos)
    {
        throw std::invalid_argument("Invalid rule: missing ->");
    }

    std::string left = s.substr(0, arrow);
    std::string right = s.substr(arrow + 2);

    const bool is_final = !right.empty() && right[0] == '.';

    if (is_final)
    {
        right.erase(0, 1);
    }

    return Rule(right, left, is_final);
}

std::vector<Rule> Parser::parser(const std::string &filename)
{
    std::vector<Rule> ruls;
    std::string line;

    std::ifstream in(filename);

    if (!in.is_open())
    {
        throw std::runtime_error("Cannot open rules file: " + filename);
    }

    while (std::getline(in, line))
    {
        if (!line.empty())
        {
            ruls.push_back(Parser::translate(line));
        }
    }

    return ruls;
}

std::string Markov::get_result()
{
    return word;
}

bool Markov::step()
{
    for ( auto &rule : rules)
    {
        const size_t pos = word.find(rule.Get_left());

        if (pos != std::string::npos)
        {
            word.replace(
                pos,
                rule.Get_left().length(),
                rule.Get_right()
            );

            return !rule.Is_final();
        }
    }

    return false;
}

void Markov::run()
{
    while (step())
    {
    }
}

Markov::Markov(std::string a, const std::string &filename)
{
    word = a;
    rules = Parser::parser(filename);
}
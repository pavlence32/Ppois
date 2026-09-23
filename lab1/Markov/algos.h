#ifndef ALGOS_H
#define ALGOS_H
#include <iostream>
#include <string>
#include <vector>
class Rule
{
    private:
    std::string right;
    std::string left;
    bool is_final;
    public:
    Rule(std::string right,std::string left,bool is_final);
    std::string Get_right();
    std::string Get_left();
    bool Is_final();
};
class Parser
{
    public:
    static Rule translate(std::string &s);
    static std::vector<Rule> parser(std::string &filename);

};
class Markov
{
    private:
    std::string word;
    std::vector<Rule> rules;
    public:
    std::string get_result();
    bool step();
    void run();

};
#endif
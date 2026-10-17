#include "algos.h"
Rule::Rule(std::string right, std::string left, bool is_final)
{
    this->right = right;
    this->left = left;
    this->is_final = is_final;
};
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
     size_t pos = s.find("->");

    std::string left = s.substr(0, pos);
    std::string right = s.substr(pos + 2);
    bool is_final = (right[0] == '.');  

    if (is_final)
    {
        right.erase(0, 1);
    }

    return Rule(right, left, is_final);
}
 std::vector<Rule> Parser::parser()
{
    std::string line;
    std::vector<Rule> a;
    int choose;
    while(true){
    std::cout<<"Enter rule:"<<std::endl;
    std::getline(std::cin,line);
        if (line.empty()) { 
        break; 
    }
    a.push_back(Parser::translate(line));
    }
return a;
}
std :: string Markov::get_result()
{
    return word;
}
bool Markov::step()
{
  for (size_t i = 0; i < rules.size(); i++)
    {
        size_t pos = word.find(rules[i].Get_left());
        if (pos != std::string::npos)
        {
            word.replace(pos, rules[i].Get_left().length(), rules[i].Get_right());
            return !rules[i].Is_final();
        }
    }
    return false;
}
void Markov::run()
{
    while(step()){};
}
Markov::Markov(std::string a,std::vector<Rule>r)
{
    word=a;
    rules=r;

}
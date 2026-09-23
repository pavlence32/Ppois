#include "algos.h"
#include <fstream>
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
     std::string right, left;
    bool is_final = false;
    int len = -1;   

    for (int i = 0; i < s.length() - 1; i++)  
    {
        if (s[i] == '-' && s[i + 1] == '>')    
        {
            len = i;
            if (s[i + 2] == '.')                
            {
                is_final = true;
            }
            break;
        }
    }

    left = s.substr(0, len);
    right = s.substr(len + 2 + (is_final ? 1 : 0));   

    return Rule(left, right, is_final);
}
std::vector<Rule> parser(std::string &filename)
{
    std::vector<Rule> ruls;
    std::string line;
    std::ifstream in(filename);
    if(in.is_open())
    {
        while(std::getline(in,line))
        ruls.push_back(Parser::translate(line));
    }
    in.close();
    return ruls;

}
std :: string Markov::get_result()
{
    return word;
}
bool Markov::step()
{
    for (int i = 0; i < rules.size(); i++)
    {
        int pos = word.find(rules[i].Get_left());
        if (pos >=0)
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
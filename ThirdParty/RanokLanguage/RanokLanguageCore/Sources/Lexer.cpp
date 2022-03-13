#include "Lexer.h"
#include <sstream>
#include <iostream>

using namespace std;


Lexer Lexer::CreateFrom(const std::string &code)
{
    Lexer lexer;
    lexer.Process(code);
    return lexer;
}

void Lexer::Process(const std::string &code, bool forceClean)
{
    if (code.empty())
        return;

    if (forceClean)
        while (!_lexemes.empty())
            _lexemes.pop();

    unsigned pivot = 0;
    while(_lexemes.empty() || _lexemes.back().Type() != Token::End)
    {
        _lexemes.push(NextLexeme(code, pivot));
    }
}

Lexeme Lexer::NextLexeme(const std::string &data, unsigned int &pivot)
{
    while (pivot < data.size() && isspace(data[pivot]))
        pivot++;

    if(data[pivot] == '/' && data[pivot+1] == '/')
    {
        while(pivot < data.size() && data[pivot] != '\n')
            pivot++;

        pivot++;
        return NextLexeme(data, pivot);
    }

    if(pivot >= data.size() || data[pivot] == '\0')
        return {Token::End, "End"};

    if(data[pivot] == ';')
    {
        pivot++;
        return {Token::Endline, ';'};
    }

    unsigned begin = pivot;
    if ((data[pivot+1] == '.')
        || isdigit (data[pivot])
        // allow decimal numbers without a leading 0. e.g. ".5"
        || (data[pivot] == '.' && isdigit (data[pivot+1])))
    {
        // skip sign for now
        if ((data[pivot] == '+' || data[pivot] == '-'))
            pivot++;

        while (isdigit (data[pivot]) || data[pivot] == '.')
            pivot++;

        // allow for 1.53158e+15
        if (data[pivot] == 'e' || data[pivot] == 'E')
        {
            pivot++; // skip 'e'
            if ((data[pivot]  == '+' || data[pivot]  == '-'))
                pivot++; // skip sign after e

            while (isdigit (data[pivot]))  // now digits after e
                pivot++;
        }

        double val;
        istringstream is (data.substr(begin, pivot - begin));
        is >> val;

        if (is.fail () && !is.eof ())
        {
            _error = "Unexpected eof";
            return {Token::End, "eof"};
        }

        return {Token::Number, val};
    }

    char symbol = data[pivot++];
    switch (symbol)
    {
        case '=':
            return {Token::Assign, symbol};
        case '(':
            return {Token::ParenOpen, symbol};
        case ')':
            return {Token::ParenClose, symbol};
        case '+':
            return {Token::Plus, symbol};
        case '-':
            return {Token::Minus, symbol};
        case '*':
            return {Token::Multiply, symbol};
        case '/':
        case '\\':
            return {Token::Divide, symbol};
        case '&':
            return {Token::Union, symbol};
        case '|':
            return {Token::Cross, symbol};
        case ',':
            return {Token::Comma, symbol};
        case '^':
            return {Token::Pow, symbol};
    }
    pivot--;

    if (!isalpha (data[pivot]))
    {
        _error = "Unexpected symbol "+string(1, data[pivot]);
        return {Token::End, "eof"};
    }

    // we have a word (starting with A-Z) - pull it out
    while (isalnum (data[pivot]) || data[pivot] == '_')
        pivot++;

    auto word = data.substr(begin, pivot-begin);
    return {Token::Id, word};
}

std::string Lexer::GetError() const
{
    return _error;
}

Lexeme Lexer::Pop(Token token)
{
    if (_lexemes.empty())
        return {Token::None, "None"};

    _lexemes.pop();

    if (_lexemes.empty())
        return {Token::None, "None"};

    if (token != Token::None && _lexemes.front().Type() != token)
        std::cout<<"Error: Token is "<< TokenToString(_lexemes.front().Type()) <<"Expected "<< TokenToString(token)<<"\n";

    return _lexemes.front();
}

bool Lexer::Empty() const
{
    return _lexemes.empty();
}

Lexeme Lexer::Top()
{
    if (!_lexemes.empty())
        return _lexemes.front();
    return {Token::None, "None"};
}

string Lexeme::ToPrintableString() const
{
    stringstream res;

    res << TokenToString(_type) << ": ";
    if (_isNumber)
        res << "(num)";
    res << _name;

    return res.str();
}

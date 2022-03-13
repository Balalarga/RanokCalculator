#ifndef LEXER_H
#define LEXER_H

#include "Utils.h"
#include "Tokens.h"

#include <queue>


struct Lexeme
{
    Lexeme(const Token& token, std::string name):
        _type(token),
        _name(name),
        _value(0),
        _isNumber(false) {}
    Lexeme(const Token& token, char name):
        _type(token),
        _name(std::string(1, name)),
        _value(0),
        _isNumber(false) {}

    Lexeme(const Token& token, double value):
        _type(token),
        _name(std::to_string(value)),
        _value(value),
        _isNumber(true) {}

    std::string ToPrintableString() const;

    inline const Token& Type() { return _type; }
    inline const std::string& Name() { return _name; }
    inline const double& Value() { return _value; }
    inline const bool& IsNumber() { return _isNumber; }


private:
    Token _type;
    std::string _name;
    double _value;
    bool _isNumber;
};


class Lexer
{
public:
    Lexer() = default;

    static Lexer CreateFrom(const std::string& code);

    void Process(const std::string& code, bool forceClean = true);

    std::string GetError() const;

    Lexeme Top();
    Lexeme Pop(Token token = Token::None);
    bool Empty() const;


protected:
    Lexeme NextLexeme(const std::string& data, unsigned& pivot);


private:
    std::queue<Lexeme> _lexemes;
    std::string _error;
};

#endif // LEXER_H

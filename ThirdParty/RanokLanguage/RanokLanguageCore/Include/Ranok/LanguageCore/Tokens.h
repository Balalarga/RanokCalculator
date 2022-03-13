//
// Created by Balalarga on 05.03.2022.
//

#ifndef RANOKLANGUAGE_TOKENS_H
#define RANOKLANGUAGE_TOKENS_H

#include <string>


enum class Token
{
    None,
    Id,
    Number,
    Assign,
    Endline,
    End,
    ParenOpen,
    ParenClose,
    Comma,
    Pow,
    Multiply,
    Divide,
    Plus,
    Minus,
    Cross,
    Union
};

static std::string TokenToString(Token token)
{
    switch (token)
    {
        case Token::None:
            return "None";
        case Token::Id:
            return "Id";
        case Token::Number:
            return "Number";
        case Token::Assign:
            return "Assign";
        case Token::Endline:
            return "Endline";
        case Token::End:
            return "End";
        case Token::ParenOpen:
            return "ParenOpen";
        case Token::ParenClose:
            return "ParenClose";
        case Token::Comma:
            return "Comma";
        case Token::Pow:
            return "Pow";
        case Token::Multiply:
            return "Multiply";
        case Token::Divide:
            return "Divide";
        case Token::Plus:
            return "Plus";
        case Token::Minus:
            return "Minus";
        case Token::Cross:
            return "Cross";
        case Token::Union:
            return "Union";
    }
    return "";
}

#endif //RANOKLANGUAGE_TOKENS_H

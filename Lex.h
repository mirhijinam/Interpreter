#include <ostream>

#ifndef INTERPRETER_LEX_H
#define INTERPRETER_LEX_H


enum type_of_lex {
    LEX_NULL,
    LEX_PROGRAM,     LEX_READ,     LEX_WRITE,
    LEX_WHILE,       LEX_DO,       LEX_IF,       LEX_ELSE,
    LEX_INT,         LEX_REAL,     LEX_STRING,   LEX_BOOL,
    LEX_TRUE,        LEX_FALSE,    LEX_NOT,      LEX_AND,      LEX_OR,

    LEX_FIN,        LEX_SEMICOLON,    LEX_COMMA,
    LEX_ASSIGN,     LEX_EQ,           LEX_NEQ,
    LEX_LSS,        LEX_LEQ,          LEX_GEQ,         LEX_GTR,
    LEX_PLUS,       LEX_MINUS,        LEX_TIMES,       LEX_SLASH,
    LEX_LBRACE,     LEX_RBRACE,       LEX_LFIGBRACE,   LEX_RFIGBRACE,
    LEX_ID,

    RPN_LABEL,      RPN_GO,           RPN_GOFALSE,         RPN_GOTRUE,         RPN_ADDRESS
};

class Lex {
private:
    type_of_lex  typeOfLex;
    int          valueLex;
    double       realLex;
    std::string  stringLex;

public:
    Lex(type_of_lex typ = LEX_NULL, int val = 0) {
        typeOfLex = typ;
        valueLex = val;
    }
    Lex(type_of_lex typ, double dbl) {
        typeOfLex = typ;
        realLex = dbl;
    }
    Lex(type_of_lex typ, std::string str) {
        typeOfLex = typ;
        stringLex = str;
    }

    void negative_value() { valueLex *= -1; }
    void negative_real()  { realLex *= -1; }

    type_of_lex  get_type()               { return typeOfLex; }
    int          get_value()       const  { return valueLex; }
    double       get_real()  const  { return realLex; }
    std::string  get_string()          { return stringLex; }

    friend std::ostream& operator<<(std::ostream& s, Lex l) {
        s << '(' << l.typeOfLex << ',' << l.valueLex << ");";
        return s;
    }
};

#endif //INTERPRETER_LEX_H

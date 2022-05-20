#include <stack>

#include "Lex.h"
#include "Scanner.h"

#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H


class Parser
{
private:
    Lex                       currentLex;
    type_of_lex               lexType;
    int                       lexVal;
    Scanner                   scan;
    std::stack<int>           stackID;
    std::stack<type_of_lex>  stackLex;

    void PROG();
    void VAR();
    void VARi();
    void CONSTRUCTION();
    void COMPARES();
    void PARTSOFOPER();
    void T();
    void F();
    void DECLARES();
    void CONSTRUCTIONS();
    void CONSTVAL();

    void dec(type_of_lex type);
    void check_id();
    void check_read_id();
    void check_op();
    void check_not();


    void eq_type();
    void eq_typeOfVar();
    void eq_bool();

    void gl() {
        currentLex = scan.get_lex();
        lexType = currentLex.get_type();
        lexVal = currentLex.get_value();
    }

public:
    std::vector<Lex> rpn;

    explicit Parser(const char* program) : scan(program) {}
    void analyze();

    std::vector<Ident>& get_tid() {
        return (*scan.get_tid());
    }
};

template <class T, class T_EL>
void from_st(T& t, T_EL& x) {
    x = t.top();
    t.pop();
}

#endif //INTERPRETER_PARSER_H

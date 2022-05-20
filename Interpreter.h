#include "Exec.h"

#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H


class Interpreter {
    Parser pars;
    Exec E;
public:
    Interpreter(const char* program) : pars(program) {}
    void interpretation() {
        pars.analyze();
        E.execute(pars.rpn, pars.get_tid());
    }
};


#endif //INTERPRETER_INTERPRETER_H

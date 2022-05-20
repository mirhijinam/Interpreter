#include <string>
#include <cmath>

#include <fstream>
#include <iostream>

#include "Lex.h"
#include "BuildIdent.h"

#ifndef INTERPRETER_SCANNER_H
#define INTERPRETER_SCANNER_H


class Scanner {
private:
    std::ifstream       infile;
    char                sym;
    std::vector<Ident>  tid;

    int look(const std::string& buf, const std::vector<std::string> list) {
        int i = 0;
        while (i < list.size()) {
            if (buf.compare(list[i]) == 0)
                return i;
            i++;
        }
        return 0;
    }
    void get_sym() {
        if (!infile.eof()) { infile.get(sym); }
        else throw "*CAN'T FIND FINAL SYMBOL*";
    }

public:
    explicit Scanner(const char* program) {
        infile.open(program);
        if (!infile.is_open()) throw "*CAN'T OPEN FILE*";
    }
    static const std::vector<std::string> tw, td;
    int set_tid(std::string nam) {
        auto* id = new Ident(nam);
        for (int i = 0; i < tid.size(); i++) {
            if (tid[i] == *id)
                return i;
        }
        tid.push_back(*id);
        return (int)tid.size() - 1;
    }
    std::vector<Ident>  *get_tid() { return &tid; }
    Lex                  get_lex();
};


#endif //INTERPRETER_SCANNER_H

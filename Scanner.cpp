#include "Scanner.h"

const std::vector<std::string> Scanner::tw = {
        "",
        "program", "read",   "write",
        "while",   "do",     "if",     "else",
        "int",     "real",   "string", "bool",
        "true",    "false",  "not",    "and",   "or",
};

const std::vector<std::string> Scanner::td = {
        "",  ";",  ",",
        "=", "==", "!=",
        "<", "<=", ">=", ">",
        "+", "-",  "*",  "/",
        "(", ")",  "{",  "}",
};


Lex Scanner::get_lex() {
    enum state { H, IDENT, NUMB, REAL, STRING, COM, NEQ, ALE, DELIM };
    state CS = H;
    std::string buf;
    int decimal, j,
        pow_cnt = 1;
    double fraction;
    do {
        get_sym();
        switch (CS) { //CS - current state
            case H:
                if (sym == ' ' || sym == '\n' || sym == '\r' || sym == '\t') {}
                else if (isalpha(sym)) { 
                    buf.push_back(sym); 
                    CS = IDENT; 
                }
                else if (isdigit(sym)) {
                    decimal = sym - '0';
                    CS = NUMB; 
                }
                else if (sym == '"') { 
                    CS = STRING; 
                }
                else if (sym == '=' || sym == '<' || sym == '>') {
                    buf.push_back(sym); 
                    CS = ALE;
                }
                else if (sym == '!') { 
                    buf.push_back(sym); 
                    CS = NEQ; 
                }
                else if (sym == '@') {
                    return Lex(LEX_FIN);
                }
                else {
                    buf.push_back(sym);
                    j = look(buf, td);
                    if (j) {
                        if (sym == '/') {
                            get_sym();
                            if (sym == '*') {
                                CS = COM;
                                buf.clear();
                            } else {
                                infile.unget();
                                return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                            }
                        } else 
                            return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                    } else 
                        throw sym;
                    break;
                }
                break;
                
            case IDENT:
                if (isalpha(sym) || isdigit(sym)) { 
                    buf.push_back(sym); 
                } else {
                    infile.unget();
                    j = look(buf, tw);
                    if (j) {
                        return Lex((type_of_lex) j, j);
                    } else {
                        j = set_tid(buf);
                        return Lex(LEX_ID, j);
                    }
                    break;
                }
                break;
                
            case NUMB:
                if (isdigit(sym))
                    decimal = decimal * 10 + (sym - '0');
                else if (sym == '.') {
                    fraction = 0;
                    pow_cnt = 1;
                    CS = REAL;
                } else {
                    infile.unget();
                    return Lex(LEX_INT, decimal);
                }
                break;

            case REAL:
                if (isdigit(sym)) {
                    fraction = fraction + (double)(sym - '0') / (pow(10, pow_cnt));
                    pow_cnt++;
                } else {
                    infile.unget();
                    return Lex(LEX_REAL, (double)decimal + fraction);
                }

            case STRING:
                if (sym != '"')
                    buf.push_back(sym);
                else
                    return Lex(LEX_STRING, buf);
                break;

            case COM:
                if (sym == '*') {
                    get_sym();
                    if (sym == '/') {
                        CS = H;
                        buf.clear();
                    }
                }
                break;

            case NEQ:
                if (sym == '=') {
                    buf.push_back(sym); j = look(buf, td);
                    return Lex(LEX_NEQ, j);
                }
                else throw '*UNDEFINDED LEXEMA*';
                break;

            case ALE:
                if (sym == '=') {
                    buf.push_back(sym);
                    j = look(buf, td);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                }
                else {
                    infile.unget();
                    j = look(buf, td);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                }
                break;

        }
    } while (true);
}
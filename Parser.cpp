#include "Parser.h"

void Parser::analyze() {
    gl();
    PROG();
}

void Parser::PROG() {
    if (lexType == LEX_PROGRAM) {
        gl();
        if (lexType == LEX_LFIGBRACE) {
            gl();
        } else
            throw "*NEED \"{\"*";
    } else
        throw "*NEED \"program\"*";
    VAR();
    CONSTRUCTIONS();
    if (lexType != LEX_RFIGBRACE)
        throw "*NEED \"}\"*";
}

void Parser::VAR() {
    while (lexType == LEX_INT || lexType == LEX_BOOL ||
           lexType == LEX_REAL || lexType == LEX_STRING) {
        VARi();
        if (lexType == LEX_SEMICOLON)
            gl();
        else
            throw "NEED \";\"";
    }
}

void Parser::VARi() {
    type_of_lex tmp_lexType;
    if (lexType == LEX_INT || lexType == LEX_BOOL ||
        lexType == LEX_REAL || lexType == LEX_STRING) {
        tmp_lexType = lexType;
        stackLex.push(lexType);
        gl();
    } else
        throw "*NEED INT, REAL, BOOL OR STRING TYPES*";
    DECLARES();
    while (lexType == LEX_COMMA) {
        gl();
        DECLARES();
    }
    dec(tmp_lexType);
}

void Parser::DECLARES() {
    if (lexType == LEX_ID) {
        stackID.push(lexVal);
        rpn.push_back(Lex(RPN_ADDRESS, lexVal));
        gl();
        if (lexType == LEX_ASSIGN) {
            gl();
            CONSTVAL();
            eq_typeOfVar();
            rpn.push_back(Lex(LEX_ASSIGN));
        }
    }
    else
        throw "*NEED AN IDENTIFIER*";
}

void Parser::CONSTVAL() {
    if (lexType == LEX_PLUS || lexType == LEX_MINUS) {
        type_of_lex tmp_symType = lexType;
        gl();
        if (lexType == LEX_INT) {
            stackLex.push(LEX_INT);

            if (tmp_symType == LEX_MINUS) {
                currentLex.negative_value();
            }
            rpn.push_back(currentLex);
            gl();
        }
        else if (lexType == LEX_REAL) {
            stackLex.push(LEX_REAL);
            if (tmp_symType == LEX_MINUS) {
                currentLex.negative_real();
            }
            rpn.push_back(currentLex);
            gl();
        }
        else
            throw "*NEED NUMBER*";
    }
    else if (lexType == LEX_REAL) {
        stackLex.push(LEX_REAL);
        rpn.push_back(currentLex);
        gl();
    }
    else if(lexType == LEX_INT) {
        stackLex.push(LEX_INT);
        rpn.push_back(currentLex);
        gl();
    }
    else if (lexType == LEX_STRING) {
        stackLex.push(LEX_STRING);
        rpn.push_back(currentLex);
        gl();
    }
    else if (lexType == LEX_TRUE) {
        stackLex.push(LEX_BOOL);
        rpn.push_back(Lex(LEX_TRUE, 1));
        gl();
    }
    else if (lexType == LEX_FALSE) {
        stackLex.push(LEX_BOOL);
        rpn.push_back(Lex(LEX_FALSE, 0));
        gl();
    }
    else
        throw "*NEED CONSTANT*";
}

void Parser::CONSTRUCTIONS() {
    while (lexType != LEX_RFIGBRACE) {
        CONSTRUCTION();
    }
}

void Parser::CONSTRUCTION() {
    int rpn0, rpn1, rpn2, rpn3;
    if (lexType == LEX_ID) {
        check_id();
        rpn.push_back(Lex(RPN_ADDRESS, lexVal));
        gl();
        if (lexType == LEX_ASSIGN) {
            gl();
            COMPARES();
            eq_type();
            if (lexType != LEX_SEMICOLON)
                throw "*VAR: NEED \";\"*";
            gl();
            rpn.push_back(Lex(LEX_ASSIGN));
        } else
            throw "*VAR: NEED \"=\"*";
    }

    else if (lexType == LEX_WHILE) {
        rpn0 = rpn.size();
        gl();
        if (lexType != LEX_LBRACE)
            throw "*WHILE: NEED \"(\"*";
        gl();
        COMPARES();
        eq_bool();

        rpn1 = (int)rpn.size();
        rpn.push_back(Lex());
        rpn.push_back(Lex(RPN_GOFALSE));
        if (lexType == LEX_RBRACE) {
            gl();
            CONSTRUCTION();
            rpn.push_back(Lex(RPN_LABEL, rpn0));
            rpn.push_back(Lex(RPN_GO));
            rpn[rpn1] = Lex(RPN_LABEL, (int)rpn.size());
        }
        else
            throw "*WHILE: NEED \")\"*";
    }

    else if (lexType == LEX_DO) {
        rpn0 = rpn.size();
        gl();
        CONSTRUCTION();
        if (lexType == LEX_WHILE) {
            gl();
            if (lexType == LEX_LBRACE) {
                gl();
                COMPARES();
                eq_bool();
                rpn.push_back(Lex(RPN_LABEL, rpn0));
                rpn.push_back(Lex(RPN_GOTRUE));
                if (lexType != LEX_RBRACE)
                    throw "*DO-WHILE: NEED \")\"*";
                gl();
                if (lexType != LEX_SEMICOLON)
                    throw "*DO-WHILE: NEED \";\"*";
                gl();
            }
            else
                throw "*DO-WHILE: NEED \"(\"*";
        }
        else
            throw "*DO-WHILE: NEED WHILE() IN CONSTRUCTION*";
    }

    else if (lexType == LEX_IF) {
        gl();
        if (lexType != LEX_LBRACE)
            throw "*IF: NEED \"(\"*";
        gl();
        COMPARES();
        eq_bool();
        rpn2 = (int)rpn.size();
        rpn.push_back(Lex());
        rpn.push_back(Lex(RPN_GOFALSE));

        if (lexType == LEX_RBRACE) {
            gl();
            CONSTRUCTION();
            rpn3 = rpn.size();
            rpn.push_back(Lex());

            rpn.push_back(Lex(RPN_GO));
            rpn[rpn2] = Lex(RPN_LABEL, (int)rpn.size());
            if (lexType == LEX_ELSE)
            {
                gl();
                CONSTRUCTION();
                rpn[rpn3] = Lex(RPN_LABEL, (int)rpn.size());
            }
            else
                throw "*NEED \"else\"*";
        }
        else
            throw "*IF: NEED \")\"*";
    }

    else if (lexType == LEX_READ) {
        gl();
        if (lexType == LEX_LBRACE) {
            gl();
            if (lexType == LEX_ID) {
                check_read_id();
                rpn.push_back(Lex(RPN_ADDRESS, lexVal));
                gl();
                if (lexType == LEX_RBRACE) {
                    gl();
                    if (lexType == LEX_SEMICOLON)
                        gl();
                    else
                        throw "*READ: NEED \";\"*";
                    rpn.push_back(Lex(LEX_READ));
                }
                else
                    throw "*READ: NEED \")\"*";
            }
            else
                throw "*READ: NEED AN IDENTIFIER*";
        }
        else
            throw "*READ: NEED \"(\"*";
    }
    else if (lexType == LEX_WRITE) {
        gl();
        if (lexType == LEX_LBRACE) {
            gl();
            COMPARES();
            stackLex.pop();
            rpn.push_back(Lex(LEX_WRITE));
            while (lexType == LEX_COMMA) {
                gl();
                COMPARES();
                stackLex.pop();
                rpn.push_back(Lex(LEX_WRITE));
            }
            if (lexType == LEX_RBRACE) {
                gl();
                if (lexType != LEX_SEMICOLON)
                    throw "*WRITE: NEED \";\"*";
                gl();
            } else
                throw "*WRITE: NEED \")\"*";
        } else
            throw "*WRITE: NEED \"(\"*";
    }
    else if(lexType == LEX_LFIGBRACE) {
        gl();
        CONSTRUCTIONS();
        if (lexType != LEX_RFIGBRACE)
            throw "*NEED \"}\"*";
        gl();
    } else
        throw "*NEED IDENT, IF, ELSE, WHILE, READ OR WRITE*";
}

void Parser::COMPARES() {
    PARTSOFOPER();
    if (lexType == LEX_EQ || lexType == LEX_NEQ ||
        lexType == LEX_LSS || lexType == LEX_GTR ||
        lexType == LEX_LEQ || lexType == LEX_GEQ) {
        stackLex.push(lexType);
        gl();
        PARTSOFOPER();
        check_op();
    }
}

void Parser::PARTSOFOPER() {
    T();
    while (lexType == LEX_PLUS || lexType == LEX_MINUS || lexType == LEX_OR) {
        stackLex.push(lexType);
        gl();
        T();
        check_op();
    }
}

void Parser::T() {
    F();
    while (lexType == LEX_TIMES || lexType == LEX_SLASH || lexType == LEX_AND) {
        stackLex.push(lexType);
        gl();
        F();
        check_op();
    }
}

void Parser::F() {
    if (lexType == LEX_ID) {
        check_id();
        rpn.push_back(currentLex);
        gl();
    }
    else if (lexType == LEX_INT) {
        stackLex.push(LEX_INT);
        rpn.push_back(currentLex);
        gl();
    }
    else if (lexType == LEX_REAL) {
        stackLex.push(LEX_REAL);
        rpn.push_back(currentLex);
        gl();
    }
    else if (lexType == LEX_STRING) {
        stackLex.push(LEX_STRING);
        rpn.push_back(currentLex);
        gl();
    }
    else if (lexType == LEX_TRUE) {
        stackLex.push(LEX_BOOL);
        rpn.push_back(Lex(LEX_TRUE, 1));
        gl();
    }
    else if (lexType == LEX_FALSE) {
        stackLex.push(LEX_BOOL);
        rpn.push_back(Lex(LEX_FALSE, 0));
        gl();
    }
    else if (lexType == LEX_NOT) {
        gl();
        F();
        check_not();
    }
    else if (lexType == LEX_PLUS) {
        gl();
        COMPARES();
    }
    else if (lexType == LEX_MINUS) {
        stackLex.push(LEX_INT);
        stackLex.push(LEX_MINUS);
        rpn.push_back(Lex(LEX_INT, 0));
        gl();
        F();
        check_op();
    }
    else if (lexType == LEX_LBRACE) {
        gl();
        COMPARES();
        if (lexType == LEX_RBRACE) {
            gl();
        }
        else
            throw "*NEED \")\"*";
    }
    else
        throw "*ERROR IN THE F-STATE*";

}



void Parser::dec(type_of_lex type) {
    int i;
    std::vector<Ident> *tid = scan.get_tid();
    while (!stackID.empty()) {
        from_st(stackID, i);
        if ((*tid)[i].get_declare())
            throw "*DECLARED TWICE*";
        else {
            (*tid)[i].set_declare();
            (*tid)[i].set_type(type);
        }
    }
}

void Parser::check_id() {
    std::vector<Ident> *tid = scan.get_tid();
    if ((*tid)[lexVal].get_declare())
        stackLex.push((*tid)[lexVal].get_type());
    else throw "*NEED TO DECLARE*";
}

void Parser::check_read_id() {
    if(!(*scan.get_tid())[lexVal].get_declare())
        throw "*NEED DECLARED IDENTIFIER*";
}

void Parser::check_op() {
    type_of_lex typ1, typ2, op, res;
    from_st(stackLex, typ2);
    from_st(stackLex, op);
    from_st(stackLex, typ1);
    if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH) {
        if (typ1 == LEX_INT && typ2 == LEX_INT) {
            res = LEX_INT;
        }
        else if (typ1 == LEX_REAL && typ2 == LEX_REAL) {
            res = LEX_REAL;
        }
        else if (typ1 == LEX_REAL && typ2 == LEX_INT || typ1 == LEX_INT && typ2 == LEX_REAL) {
            res = LEX_REAL;
        }
        else if (typ1 == LEX_STRING && typ2 == LEX_STRING && op == LEX_PLUS) {
            res = LEX_STRING;
        }
        else
            throw "*NEED CORRECTION OPERATIONS*";
    }
    else if (op == LEX_GTR || op == LEX_LSS || op == LEX_EQ || op == LEX_NEQ || op == LEX_GEQ || op == LEX_LEQ) {
        if ((typ1 == LEX_INT || typ1 == LEX_REAL) && (typ2 == LEX_INT || typ2 == LEX_REAL))
            res = LEX_BOOL;
        else if (typ1 == LEX_STRING && typ2 == LEX_STRING) {
            res = LEX_BOOL;
        }
        else
            throw "*NEED CORRECT COMPARISON*";
    }
    else if (op == LEX_OR || op == LEX_AND) {
        if (typ1 == LEX_BOOL && typ2 == LEX_BOOL) {
            res = LEX_BOOL;
        }
        else
            throw "*NEED BOOL WITH AND OR OR*";
    }
    else
        throw "*NEED SUITABLE TYPES*";

    stackLex.push(res);
    rpn.push_back(Lex(op));
}

void Parser::check_not() {
    if (stackLex.top() != LEX_BOOL)
        throw "*NEED CORRECT TYPE*";
    else {
        rpn.push_back(Lex(LEX_NOT));
    }
}

void Parser::eq_typeOfVar() {
    type_of_lex typ1, typ2;
    from_st(stackLex, typ1);
    typ2 = stackLex.top();
    if ((typ1 == LEX_INT || typ1 == LEX_REAL) && (typ2 == LEX_INT || typ2 == LEX_REAL)) { }
    else if (typ1 == typ2) {}
    else
        throw "*NEED SUITABLE IDENTIFIER OR VALUE*";
}

void Parser::eq_type() {
    eq_typeOfVar();
    stackLex.pop();
}

void Parser::eq_bool() {
    if (stackLex.top() != LEX_BOOL)
        throw "*NEED BOOL*";
    else
        stackLex.pop();
}
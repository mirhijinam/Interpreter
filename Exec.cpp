#include "Parser.h"
#include "Exec.h"

void Exec::execute(std::vector<Lex>& rpn, std::vector<Ident>& tid) {
    Lex pc_el;
    std::stack<Lex> stackOfLex;
    int up, down, rpn_i = 0, size = rpn.size();
    Lex firstLexema, secondLexema;
    while (rpn_i < size) {
        pc_el = rpn[rpn_i];
        switch (pc_el.get_type()) {
            case LEX_TRUE: case LEX_FALSE: case LEX_INT: case LEX_REAL: case LEX_STRING: case RPN_ADDRESS: case RPN_LABEL:
                stackOfLex.push(pc_el);
                break;

            case LEX_ASSIGN:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                up = firstLexema.get_value();
                down = secondLexema.get_value();
                if (firstLexema.get_type() == LEX_STRING)
                    tid[down].set_string(firstLexema.get_string());
                else if(tid[down].get_type() == LEX_INT && firstLexema.get_type() == LEX_INT)
                    tid[down].set_value(firstLexema.get_value());
                else if (tid[down].get_type() == LEX_REAL && firstLexema.get_type() == LEX_REAL)
                    tid[down].set_real(firstLexema.get_real());
                else if (tid[down].get_type() == LEX_INT && firstLexema.get_type() == LEX_REAL)
                    tid[down].set_value((int) firstLexema.get_real());
                else if (tid[down].get_type() == LEX_REAL && firstLexema.get_type() == LEX_INT)
                    tid[down].set_real((double) firstLexema.get_value());
                else
                    tid[down].set_value(up);
                tid[down].set_assign();
                break;

            case LEX_ID:
                up = pc_el.get_value();
                if (tid[up].get_assign()) {
                    if (tid[up].get_type() == LEX_STRING)
                        stackOfLex.push(Lex(tid[up].get_type(), tid[up].get_string()));
                    else if(tid[up].get_type() == LEX_REAL)
                        stackOfLex.push(Lex(tid[up].get_type(), tid[up].get_real()));
                    else
                        stackOfLex.push(Lex(tid[up].get_type(), tid[up].get_value()));
                }
                else
                    throw "*NEED INITIALIZED VARIABLES*";
                break;

            case LEX_NOT:
                from_st(stackOfLex, firstLexema);
                stackOfLex.push(Lex(firstLexema.get_type(), !firstLexema.get_value()));
                break;

            case LEX_OR:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                stackOfLex.push(Lex(firstLexema.get_type(), firstLexema.get_value()
                                        || secondLexema.get_value()));
                break;

            case LEX_AND:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                stackOfLex.push(Lex(firstLexema.get_type(), firstLexema.get_value()
                                        && secondLexema.get_value()));
                break;

            case RPN_GO:
                from_st(stackOfLex, firstLexema);
                rpn_i = firstLexema.get_value() - 1;
                break;

            case RPN_GOFALSE:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                if (!secondLexema.get_value())
                    rpn_i = firstLexema.get_value() - 1;
                break;

            case RPN_GOTRUE:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                if (secondLexema.get_value())
                    rpn_i = firstLexema.get_value() - 1;
                break;

            case LEX_WRITE:
                from_st(stackOfLex, firstLexema);
                if (firstLexema.get_type() == LEX_STRING)
                    std::cout << firstLexema.get_string() << std::endl;
                else if (firstLexema.get_type() == LEX_REAL)
                    std::cout << firstLexema.get_real() << std::endl;
                else
                    std::cout << firstLexema.get_value() << std::endl;
                break;

            case LEX_READ:
                int k;
                from_st(stackOfLex, firstLexema);
                up = firstLexema.get_value();
                if (tid[up].get_type() == LEX_INT) {
                    std::cout << "(int)" << tid[up].get_name() << ":" << std::endl;
                    std::cin >> k;
                    tid[up].set_value(k);
                }
                else if (tid[up].get_type() == LEX_STRING) {
                    std::string str;
                    std::cout << "(string)" << tid[up].get_name() << ":" << std::endl;
                    std::cin >> str;
                    tid[up].set_string(str);
                }
                else if (tid[up].get_type() == LEX_REAL) {
                    double dval;
                    std::cout << "(real)" << tid[up].get_name() << ":" << std::endl;
                    std::cin >> dval;
                    tid[up].set_real(dval);
                }
                else {
                    std::string j;
                    while (1) {
                        std::cout << "(bool)" << tid[up].get_name() << ":" << std::endl;
                        std::cin >> j;
                        if (j != "true" && j != "false") {
                            std::cout << "*NEED TRUE OR FALSE*" << std::endl;
                            continue;
                        }
                        k = (j == "true") ? 1 : 0;
                        break;
                    }
                    tid[up].set_value(k);
                }
                tid[up].set_assign();
                break;

            case LEX_PLUS:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                if (firstLexema.get_type() == LEX_STRING) {
                    stackOfLex.push(Lex(LEX_STRING, secondLexema.get_string() + firstLexema.get_string()));
                }

                else if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_INT,
                                        secondLexema.get_value() + firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_REAL,
                                        secondLexema.get_real() + firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_REAL,
                                        secondLexema.get_real() + firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_REAL,
                                        secondLexema.get_value() + firstLexema.get_real()));

                else
                    stackOfLex.push(Lex(firstLexema.get_type(),
                                        secondLexema.get_value() + firstLexema.get_value()));
                break;

            case LEX_MINUS:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_INT,
                                        secondLexema.get_value() - firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_REAL,
                                        secondLexema.get_real() - firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_REAL,
                                        secondLexema.get_real() - firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_REAL,
                                        secondLexema.get_value() - firstLexema.get_real()));
                break;

            case LEX_TIMES:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_INT,
                                        secondLexema.get_value() * firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_REAL,
                                        secondLexema.get_real() * firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_REAL,
                                        secondLexema.get_real() * firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_REAL,
                                        secondLexema.get_value() * firstLexema.get_real()));
                break;

            case LEX_SLASH:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                if (firstLexema.get_type() == LEX_INT && firstLexema.get_value() != 0 && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_INT, secondLexema.get_value() / firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_REAL &&
                        firstLexema.get_real() != (double)0)
                    stackOfLex.push(Lex(LEX_REAL, secondLexema.get_real() / firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_REAL && firstLexema.get_value() != 0)
                    stackOfLex.push(Lex(LEX_REAL, secondLexema.get_real() / firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_INT &&
                        firstLexema.get_real() != (double)0)
                    stackOfLex.push(Lex(LEX_REAL, secondLexema.get_value() / firstLexema.get_real()));

                else
                    throw "*DON'T USE 0 AS DEVIDER*";
                break;

            case LEX_EQ:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_BOOL, secondLexema.get_value() == firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_real() == firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_real() == firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_BOOL, secondLexema.get_value() == firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_STRING) {
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_string() == firstLexema.get_string()));
                }
                else
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() == firstLexema.get_value()));
                break;

            case LEX_NEQ:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() != firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_real() != firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_real() != firstLexema.get_value()));
                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() != firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_STRING) {
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_string() != firstLexema.get_string()));
                }

                else
                    stackOfLex.push(Lex(LEX_BOOL, secondLexema.get_value() != firstLexema.get_value()));
                break;

            case LEX_LSS:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() < firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_real() < firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_real() < firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() < firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_STRING) {
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_string() < firstLexema.get_string()));
                }
                else
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() < firstLexema.get_value()));
                break;

            case LEX_GTR:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() > firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_real() > firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_real() > firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() > firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_STRING) {
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_string() > firstLexema.get_string()));
                }
                else
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() > firstLexema.get_value()));
                break;

            case LEX_LEQ:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() <= firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_BOOL, secondLexema.get_real() <= firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_BOOL, secondLexema.get_real() <= firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() <= firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_STRING) {
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_string() <= firstLexema.get_string()));
                }

                else
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() <= firstLexema.get_value()));
                break;

            case LEX_GEQ:
                from_st(stackOfLex, firstLexema);
                from_st(stackOfLex, secondLexema);
                if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() >= firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_real() >= firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_INT && secondLexema.get_type() == LEX_REAL)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_real() >= firstLexema.get_value()));

                else if (firstLexema.get_type() == LEX_REAL && secondLexema.get_type() == LEX_INT)
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() >= firstLexema.get_real()));

                else if (firstLexema.get_type() == LEX_STRING) {
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_string() >= firstLexema.get_string()));
                }
                else
                    stackOfLex.push(Lex(LEX_BOOL,
                                        secondLexema.get_value() >= firstLexema.get_value()));
                break;
            default:
                throw "*UNEXPECTED*";
        }
        ++rpn_i;
    }
    std::cout << "Process finished with exit code 0" << std::endl;
}


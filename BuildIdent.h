#include <vector>

#include "Lex.h"

#ifndef INTERPRETER_IDENT_H
#define INTERPRETER_IDENT_H


class Ident {
private:
    type_of_lex   type;
    std::string   name;
    bool          declare;
    bool          assign;
    int           value;
    double        real;
    std::string   str;

public:
    Ident() {
        name = "";
        declare = false;
        assign = false;
    }
    Ident(std::string n) {
        name = n;
        declare = false;
        assign = false;
    }

    void         set_type(type_of_lex t)    { type = t; }
    void         set_name(std::string n)    { name = n; }
    void         set_declare()              { declare = true; }
    void         set_assign()               { assign = true; }
    void         set_value(int v)           { value = v; }
    void         set_real(double r)         { real = r; }
    void         set_string(std::string s)  { str = s; }

    type_of_lex  get_type()                 { return type; }
    std::string  get_name()                 { return name; }
    bool         get_declare() const        { return declare; }
    bool         get_assign()  const        { return assign; }
    int          get_value()   const        { return value; }
    double       get_real()    const        { return real; }
    std::string  get_string()               { return str; }

    bool operator==(const Ident& rvalue) {
        if (this->name == rvalue.name)
            return true;
        return false;
    }


};


#endif //INTERPRETER_IDENT_H

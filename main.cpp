#include <iostream>
#include "Interpreter.h"

int main()
{
    try {
        Interpreter I("Program.txt");
        I.interpretation();
        return 0;
    }
    catch (char c) {
        std::cout << "*I DON'T KNOW THIS SYMBOL*" << c << std::endl;
        return 1;
    }
    catch (Lex l) {
        std::cout << "*I DON'T KNOW THIS  LEXEMA*" << l << std::endl;
        return 1;
    }
    catch (const char* x) {
        std::cout << x << std::endl;
        return 1;
    }
}

cmake_minimum_required(VERSION 3.21)
project(Interpreter)

set(CMAKE_CXX_STANDARD 14)

add_executable(Interpreter main.cpp Lex.h Exec.cpp Exec.h Parser.cpp Parser.h Scanner.cpp Scanner.h BuildIdent.h Interpreter.h)

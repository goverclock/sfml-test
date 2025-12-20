#pragma once
#include <cstring>
#include <print>

#define TODO_MSG "TODO: {}:{} is not implemented"
#define TODO()                                                      \
    do {                                                            \
        size_t msg_len =                                            \
            strlen(TODO_MSG) + strlen(__FILE__) + strlen(__func__); \
        for (size_t i = 0; i < msg_len - 4; i++) std::print("#");   \
        std::println("\n" TODO_MSG, __FILE__, __func__);            \
        for (size_t i = 0; i < msg_len - 4; i++) std::print("#");   \
        std::println("");                                           \
        exit(1);                                                    \
    } while (0)

#define UNREACHABLE() assert(false && "reached unreachable code")

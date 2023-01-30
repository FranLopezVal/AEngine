#pragma once
#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <vector>

#define VECTOR_REMOVE(T,A) T.erase(std::remove(T.begin(), T.end(), A), T.end())

#define CONSOLE_LINE(T) std::cout << T << std::endl;
#define CONSOLE_W(T) std::cout << T;

#endif // !HELPERS_H


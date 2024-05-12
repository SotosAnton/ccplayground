
#ifndef MAZE_UTILS_H
#define MAZE_UTILS_H


#include <iostream>
#include <cstdlib>
#include <random>


#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_BACKGROUND_BLACK "\x1b[40m"
#define ANSI_BACKGROUND_WHITE "\x1b[47m"

extern std::mt19937 rng;


void shuffleArray(int arr[], int size) {
    
    for (int i = size - 1; i > 0; --i) {
        int j = rng() % (i + 1);        
        std::swap(arr[i], arr[j]);
    }
}

#endif
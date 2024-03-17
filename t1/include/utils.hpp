
#ifndef MAZE_UTILS_H
#define MAZE_UTILS_H


#include <iostream>
#include <cstdlib>

void shuffleArray(int arr[], int size) {
    
    for (int i = size - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(arr[i], arr[j]);
    }
}

#endif
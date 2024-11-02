#include <iostream>

void recursiveFunction(int depth, void* stackBottom) {
    // Allocate a local variable to increase stack usage
    int localVar;

    // Print the current stack pointer and stack bottom
    std::cout << depth << ": stack bottom: " << stackBottom
              << ", current: " << &localVar << std::endl;

    // Call the function recursively
    recursiveFunction(depth + 1, stackBottom);
}

int main() {
    // Allocate a local variable in main
    int mainLocalVar;

    // Call the recursive function with initial depth
    recursiveFunction(1, &mainLocalVar);

    return 0;
}

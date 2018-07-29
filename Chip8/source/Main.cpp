#include <iostream>

int main(int argc, char const *argv[])
{
    for (int i = 0; i < argc; ++i)
        std::cout << argv[i] << std::endl;

    std::cin.get();

    return 0;
}

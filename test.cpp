#include <iostream>

int main() {
    std::string str = "momari:abc";
    size_t res = str.find(":", 0);
    std::cout << res << std::endl;
}
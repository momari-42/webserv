#include <iostream>
#include <cstring>
int main() {
    std::string a = "oxcvbnmasdfghjkqwertyuio";
    std::string b = "o";
    std::cout << a.find(b) << std::endl;
}
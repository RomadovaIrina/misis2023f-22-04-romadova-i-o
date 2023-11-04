#include <iostream>
#include <string>


int main() {
    std::string sequence;
    std::cin >> sequence;
    int point = 0, dash = 0;
    for (int i = 0; i < sequence.length(); i += 1) {
        if (sequence[i] == '-') {
            dash += 1;
        }
        else {
            point += 1;
        }
    }
    if (dash == 0 || point==0) {
        std::cout << "YES";
    }
    else if (dash % point == 0) {
        std::cout << "YES";
    }
    else {
        std::cout << "NO";
    }
}

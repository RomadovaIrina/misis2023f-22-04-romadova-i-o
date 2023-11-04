#include <iostream>
#include <vector>
#include <string>
bool is_vowel(char to_check) {
    bool result = 0;
    if (to_check == 'a' || 
        to_check=='o' ||
        to_check == 'y' || 
        to_check == 'e' || 
        to_check == 'u' || 
        to_check == 'i') {
        result = 1;
    }
    return result;
}


int main() {
    std::string sequence;
    std::cin >> sequence;
    std::string final_string;
    for (int i = 0; i < sequence.length(); i += 1) {
        char symb = (char)tolower(sequence[i]);
        if (!is_vowel(symb)) {
            final_string.push_back('.');
            final_string.push_back(symb);
        }
    }
    std::cout << final_string;
}

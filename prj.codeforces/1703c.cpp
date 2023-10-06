#include <iostream>
#include <string>
#include <vector>
void solution() {
    int n;
    std::cin >> n;
    std::vector<int> m(n);
    for (int j = 0; j < n; j += 1) {
        std::cin >> m[j];
    }
    for (int k = 0; k < n; k += 1) {
        int turn_amount; //количество прокруток колечка
        std::cin >> turn_amount; 
        if (turn_amount == 0) {
            continue;
        }
        std::string current_place;
        std::cin >> current_place;
        for (int i = 0; i < turn_amount; i += 1) {
            if (current_place[i] == 'U') {
                m[k] -= 1;
            }
            else if (current_place[i] == 'D') {
                m[k] += 1;
            }
            if (m[k] < 0) {
                m[k] += 10;
            }
            if (m[k] > 9) {
                m[k] -= 10;
            }
        }
    }
    for (int k = 0; k < n; k += 1) {
        std::cout << m[k] << " ";
    }
    std::cout << std::endl;
}


int main()
{
    int t;
    std::cin >> t;
    for (int temp = 0; temp < t; temp += 1) {
        solution();
    }

}

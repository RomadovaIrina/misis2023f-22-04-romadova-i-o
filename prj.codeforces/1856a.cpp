#include <iostream>
#include <string>
#include <vector>


using vec = std::vector<int>;
void moves(vec& data_t) {
    int ans = 0;
    for (int i = 0; i < data_t.size()-1; i += 1) {
        if (data_t[i] > data_t[i + 1]) {
            ans = std::max(data_t[i], ans);
        }
    }
    std::cout << ans << "\n";
}

int main()
{
    int amount;
    std::cin >> amount;
    for (int i = 0; i < amount; i += 1) {
        int leng;
        std::cin >> leng;
        std::vector<int> data(leng);
        for (int j = 0; j < leng; j +=1) {
            std::cin >> data[j];
        }
        for (int k = 0; k < leng; k += 1) {
            std::cout << data[k];
        }
        moves(data);
    }
}

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>


class MeyersSingleton {
private:
    MeyersSingleton() = default;
    MeyersSingleton(const MeyersSingleton&) = delete;
    MeyersSingleton(MeyersSingleton&& other) = delete;

    ~MeyersSingleton() {};
    int example_value_ = 100;
public:


    MeyersSingleton& operator =(const MeyersSingleton& other) = delete;
    MeyersSingleton& operator=(MeyersSingleton&& other) = delete;
    
    int answer_to_all = 42;
    int GetExapleValue() const {
        return example_value_;
    }
    void SetExampleValue(const int& other) {
        example_value_ = other;
    }
    static MeyersSingleton& getInstance() {
        static MeyersSingleton instance;
        return instance;
    }
};

class LeakySingleton {
private:
    // Приватный конструктор, чтобы предотвратить создание экземпляров извне.
    LeakySingleton() {}
    // Указатель на единственный экземпляр класса.
    inline static LeakySingleton* instance = nullptr;

public:
    // Метод для получения единственного экземпляра класса.
    static LeakySingleton* getInstance() {
        if (!instance) {
            instance = new LeakySingleton();
        }
        return instance;
    }
    int ex_value = 123;

    static void deleteInstance() {
        delete instance;
    }
};
using namespace std;

int main()
{ 
    {
        LeakySingleton& naive = *LeakySingleton::getInstance();        
        std::cout << naive.ex_value << " ";
        naive.deleteInstance();
    }

    std::cout << "Not naive singletones:" << "\n";
    {
        MeyersSingleton& unnaive_1 = MeyersSingleton::getInstance();
        std::cout << unnaive_1.answer_to_all << std::endl;
        unnaive_1.answer_to_all=150;
        
    
    
        MeyersSingleton& unnaive_2 = MeyersSingleton::getInstance();
        std::cout << unnaive_2.answer_to_all << std::endl;
        unnaive_2.SetExampleValue(12345);
        
    
    
        MeyersSingleton& try_1 = MeyersSingleton::getInstance();
        std::cout << try_1.answer_to_all << " " << try_1.GetExapleValue() << std::endl;

        std::cout << unnaive_1.answer_to_all << " " << unnaive_2.answer_to_all << " " << try_1.answer_to_all;

    }
	return 0;
}

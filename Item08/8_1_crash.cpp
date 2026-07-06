#include <iostream>
#include <stdexcept>

class BadWidget {
public:
    ~BadWidget() {
        // 🚨 소멸자 안에서 예외를 던져버립니다!
        std::cout << "BadWidget 소멸자 작동 중... 예외 던진다!" << std::endl;
        throw std::runtime_error("소멸자에서 터진 2차 폭탄!");
    }
};

void doSomething() {
    BadWidget w;
    std::cout << "1. 정상적인 작업 중... 근데 여기서 1차 에러 발생!" << std::endl;
    throw std::runtime_error("작업 도중 터진 1차 폭탄!"); 
    // 💥 이 예외 때문에 탈출하면서 w의 소멸자가 켜집니다!
}

int main() {
    try {
        doSomething();
    }
    catch (const std::exception& e) {
        // 과연 여기가 실행될까요?
        std::cout << "main에서 예외 검거 성공: " << e.what() << std::endl;
    }
    return 0;
}

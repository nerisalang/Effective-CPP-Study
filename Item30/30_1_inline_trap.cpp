#include <iostream>

class Base {
public:
    // 1. 암시적 인라인: 클래스 내부에서 구현을 다 적어버리면 컴파일러가 inline으로 취급함
    int getMagicNumber() const { return 42; } 
    
    // 2. 가상 함수: inline을 명시적으로 붙여도, 다형성 때문에 무시될 확률 99.9%
    inline virtual void printName() const {
        std::cout << "  [Base] 나는 베이스 클래스다!\n";
    }
};

class Derived : public Base {
public:
    // 부모의 가상 함수 오버라이딩
    inline virtual void printName() const override {
        std::cout << "  [Derived] 나는 파생 클래스다!\n";
    }
};

// 3. 명시적 인라인: 클래스 밖에서 구현할 때는 inline 키워드를 붙임
// (주의: 헤더 파일에 넣지 않고 cpp 파일에만 두면 다른 파일에서 링크 에러 터짐)
inline void fastLog(const char* msg) {
    std::cout << "⚡ [초고속 로깅] " << msg << "\n";
}

int main() {
    std::cout << "=== 항목 30: 인라인 함수의 동작과 한계 ===\n\n";

    Base b;
    Derived d;

    // 정상적으로 인라인 치환될 가능성이 높은 호출
    fastLog("시스템 부팅 완료");
    std::cout << "매직 넘버: " << b.getMagicNumber() << "\n\n";

    // 🚨 가상 함수를 포인터나 참조로 호출할 때 (인라인 무시됨)
    std::cout << "[가상 함수 호출 테스트 - 런타임 결정]\n";
    Base* ptr = &d;
    
    // 컴파일러: "ptr이 가리키는 게 Base인지 Derived인지 실행해봐야 아니까, 인라인 치환 안 해!"
    ptr->printName(); 

    return 0;
}

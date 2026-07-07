#include <iostream>
#include <string>
#include <exception>

// 의도적으로 메모리 부족 예외를 던지는 가짜 비트맵 클래스
class BadBitmap {
public:
    std::string name;
    BadBitmap(std::string n) : name(n) {}
    BadBitmap(const BadBitmap& rhs) {
        // 복사 도중 갑자기 서버 메모리가 터진 상황을 시뮬레이션
        throw std::bad_alloc(); 
    }
};

class Widget {
private:
    BadBitmap* pb;

public:
    Widget(std::string name) : pb(new BadBitmap(name)) {}
    ~Widget() { delete pb; }

    void print() const {
        if (pb) std::cout << "비트맵 상태: " << pb->name << std::endl;
        else std::cout << "🚨 [좀비 상태] 비트맵이 공중분해 됨 (포인터가 터졌습니다)\n";
    }

    // ❌ 일치성 테스트만 믿다가 예외 안전성에서 뚝배기 깨지는 대입 연산자
    Widget& operator=(const Widget& rhs) {
        if (this == &rhs) return *this; // 자기 대입은 잘 막아주지만...

        delete pb; // 1. 내 원래 소중한 데이터를 먼저 지워버림!

        // 💥 2. 대재앙 발생: 복사 생성자 안에서 예외(bad_alloc)가 터짐!
        pb = new BadBitmap(*rhs.pb); 

        return *this;
    }
};

int main() {
    Widget w1("Liverpool_Logo");
    Widget w2("Chelsea_Logo");

    std::cout << "--- 대입 전 상태 ---\n";
    std::cout << ("w1 "); w1.print();

    std::cout << "\n--- w1 = w2 대입 시도 (예외 발생 시뮬레이션) ---\n";
    try {
        w1 = w2; // 서로 다른 객체라 일치성 테스트는 통과하지만...
    } catch (const std::bad_alloc& e) {
        std::cout << "⚠️ 예외 감지: 메모리가 부족하여 복사에 실패했습니다!\n";
    }

    std::cout << "\n--- 대입 실패 후 w1의 상태 확인 ---\n";
    // 과연 w1은 원래 데이터를 유지하고 있을까? 아니면 좀비가 되었을까?
    std::cout<< ("w1 "); w1.print(); 

    return 0;
}

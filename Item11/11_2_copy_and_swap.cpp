#include <iostream>
#include <string>
#include <utility> // std::swap용

class BadBitmap {
public:
    std::string name;
    BadBitmap(std::string n) : name(n) {}
    BadBitmap(const BadBitmap& rhs) {
        // 복사 시 무조건 예외 발생 시뮬레이션
        throw std::bad_alloc(); 
    }
};

class SafeWidget {
private:
    BadBitmap* pb;

public:
    SafeWidget(std::string name) : pb(new BadBitmap(name)) {}
    
    // 복사 생성자 (Copy 앤 Swap을 위해 필수)
    SafeWidget(const SafeWidget& rhs) : pb(new BadBitmap(*rhs.pb)) {}

    ~SafeWidget() { delete pb; }

    void print() const {
        if (pb) std::cout << "비트맵 상태: " << pb->name << std::endl;
        else std::cout << "🚨 좀비 상태\n";
    }

    // 🌟 [항목 11 최종 정석] Copy and Swap 구현
    SafeWidget& operator=(const SafeWidget& rhs) {
        std::cout << "[대입 연산자 가동] 사본 생성 시도...\n";
        
        // 1. 우변의 사본을 임시 객체로 먼저 만듭니다. (Copy)
        // 만약 여기서 예외가 터지면 함수가 바로 종료되므로, 내 pb 본체는 완벽히 안전함!
        SafeWidget temp(rhs); 

        std::cout << "[사본 생성 성공] 데이터 스왑 수행\n";
        // 2. 사본이 무사히 만들어졌다면 내 포인터와 주소값을 맞바꿉니다. (Swap)
        std::swap(this->pb, temp.pb);

        return *this;
        // 3. 함수가 끝나면서 임시 객체 temp가 소멸될 때, 원래 내 옛날 데이터가 자동으로 delete 됨!
    }
};

int main() {
    // 예외가 터지지 않는 정상적인 복사를 보여주기 위해 임시로 원래 위젯 상태 유지
    std::cout << "--- SafeWidget 생성 ---\n";
    SafeWidget w1("Ohtani_HomeRun");
    
    std::cout << "\n--- Copy and Swap은 예외가 터져도 안전할까? ---\n";
    // 실습 1과 똑같이 복사하다 터지는 상황이지만, 복사 생성자 단계에서 터지게 됨
    // w1 본체는 손끝 하나 건드리지 못함!
    try {
        SafeWidget w2("Salah_Goal");
        w1 = w2; 
    } catch (const std::bad_alloc& e) {
        std::cout << "⚠️ 예외 감지: 복사 중 메모리가 터졌지만, 내 본체는 과연?\n";
    }

    std::cout << "\n--- 예외 발생 후 w1의 상태 확인 ---\n";
    std::cout <<("w1 "); w1.print(); // 원래 데이터가 훌륭하게 보존되어 있는지 확인!

    return 0;
}

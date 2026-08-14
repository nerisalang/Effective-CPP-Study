#include <iostream>
#include <memory>
#include <mutex>
#include <utility>

// 가상의 이미지 클래스 (생성 시 가끔 메모리 에러가 터진다고 가정)
struct Image {
    int id;
    Image(int id) : id(id) {
        if (id == 999) throw std::bad_alloc(); // 강제로 예외 발생!
    }
};

class Menu {
private:
    std::mutex m; // 멀티스레딩 락
    Image* bgImage;
    int changeCount;

public:
    Menu() : bgImage(new Image(0)), changeCount(0) {}

    // ❌ 대참사: 강력한 보장 실패 (기존 데이터 지웠는데 할당 터짐)
    void changeBackground_Bad(int newId) {
        m.lock();
        delete bgImage;      // 1. 기존 이미지를 돌이킬 수 없이 지워버림!
        ++changeCount;       // 2. 카운트도 미리 올려버림!
        
        try {
            bgImage = new Image(newId); // 🚨 여기서 예외 터지면?
        } catch (...) {
            m.unlock();
            std::cout << "  [Bad] 예외 발생! 기존 이미지는 날아갔고 카운트는 꼬였음!\n";
            return;
        }
        m.unlock();
    }

    // ✨ 정답: Copy-and-Swap (강력한 예외 안전성 보장)
    void changeBackground_Good(int newId) {
        std::lock_guard<std::mutex> lock(m); // 자원 누수 방지 (항목 14)

        try {
            // 1. Copy (복사본을 만들어서 거기서 작업)
            Image* newImage = new Image(newId); // 🚨 여기서 예외가 터져도 원본(bgImage)은 안전함!

            // 2. Swap (작업이 성공하면 예외 불가 swap으로 포인터만 싹 교체)
            delete bgImage; // 기존 자원 해제
            bgImage = newImage;
            ++changeCount;  // 모든 게 성공한 후에 상태 변경
            
            std::cout << "  [Good] 배경 변경 성공! 카운트: " << changeCount << "\n";
        } catch (...) {
            std::cout << "  [Good] 예외 발생! 하지만 원본 데이터와 카운트는 완벽하게 보호됨.\n";
        }
    }

    void printState() const {
        std::cout << "  -> 현재 상태 [카운트: " << changeCount << ", 이미지 포인터: " << bgImage << "]\n";
    }
};

int main() {
    std::cout << "=== 항목 29: 강력한 예외 안전성과 Copy-and-Swap ===\n\n";

    Menu menu1;
    std::cout << "[Bad 방식 테스트]\n";
    menu1.printState();
    menu1.changeBackground_Bad(999); // 폭탄 투하!
    menu1.printState(); // 결과: 포인터는 쓰레기(dangling)가 되고, 카운트는 1 올라가 있는 좆버그 상태

    std::cout << "\n--------------------------------\n\n";

    Menu menu2;
    std::cout << "[Good 방식 테스트 (Copy-and-Swap)]\n";
    menu2.printState();
    menu2.changeBackground_Good(999); // 폭탄 투하!
    menu2.printState(); // 결과: 예외는 방어하고 상태는 건드리기 전 원본 그대로 유지됨!

    return 0;
}

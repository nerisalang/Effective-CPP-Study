#include <iostream>
#include <string>

// 상위 클래스 (기본 창)
class Window {
protected:
    int size = 100; // 창 크기

public:
    virtual void onResize() {
        size += 50; // 크기를 50 늘림
        std::cout << "  [Window] 창 크기를 " << size << "으로 늘렸습니다.\n";
    }
    int getSize() const { return size; }
};

// 하위 클래스 (특수 창)
class SpecialWindow : public Window {
public:
    virtual void onResize() override {
        // ❌ 대참사: C스타일/static_cast를 사용해 상위 클래스 함수 호출 시도
        // 원본(*this)이 아니라, 복사본(임시 Window 객체)의 onResize()가 불리고 증발해버림!
        std::cout << "\n[잘못된 방식 (캐스팅 사용)]\n";
        static_cast<Window>(*this).onResize(); 
        std::cout << "  -> 특수 창의 크기 확인: " << size << " (❌ 원본은 안 변함!)\n";

        // ✨ 정답: 범위 지정 연산자(::)를 사용해 정확히 상위 클래스의 함수를 직접 호출!
        std::cout << "\n[올바른 방식 (범위 지정 연산자 사용)]\n";
        Window::onResize();
        std::cout << "  -> 특수 창의 크기 확인: " << size << " (✅ 원본이 정상적으로 변함!)\n";
    }
};

int main() {
    std::cout << "=== 항목 27: 캐스팅을 피해야 하는 이유 (임시 객체 함정) ===\n";

    SpecialWindow myWindow;
    myWindow.onResize();

    return 0;
}

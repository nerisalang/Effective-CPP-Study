#include <iostream>
#include <utility> // std::swap 용도

namespace WidgetStuff {

    // 1. 아주 무겁고 뚱뚱한 실제 데이터 구조체
    struct WidgetImpl {
        int hugeData[10000]; 
    };

    // 2. 껍데기 클래스 (pimpl 이디엄)
    class Widget {
    private:
        WidgetImpl* pImpl; // 진짜 데이터를 가리키는 포인터

    public:
        // 생성자
        Widget(WidgetImpl* impl) : pImpl(impl) {}

        // ✨ 핵심 Step 1: 포인터만 0.001초 만에 싹 바꾸는 public 멤버 함수
        // 포인터 교환은 실패할 리가 없으므로 noexcept를 박아줍니다.
        void swap(Widget& other) noexcept {
            using std::swap; // 내장 포인터이긴 하지만, 제네릭 템플릿 확장을 대비한 정석
            swap(pImpl, other.pImpl); 
            std::cout << "  ⚡ [초고속 커스텀 스왑] 무거운 데이터 복사 없이 '포인터'만 교환 완료!\n";
        }
        
        // 테스트용 주소 확인 함수
        void printAddress() const {
            std::cout << "  현재 pImpl이 가리키는 힙 주소: " << pImpl << "\n";
        }
    };

    // ✨ 핵심 Step 2: 같은 네임스페이스 안에 비멤버 비프렌드 swap 함수 제공
    // 컴파일러는 인자(Widget)의 소속 네임스페이스(WidgetStuff)를 먼저 뒤져서 이 함수를 찾아냅니다! (ADL)
    void swap(Widget& a, Widget& b) noexcept {
        a.swap(b);
    }
}

int main() {
    std::cout << "=== 항목 25: 예외를 던지지 않는 초고속 커스텀 swap ===\n\n";

    // 힙에 무거운 데이터 생성
    WidgetStuff::WidgetImpl* data1 = new WidgetStuff::WidgetImpl();
    WidgetStuff::WidgetImpl* data2 = new WidgetStuff::WidgetImpl();

    WidgetStuff::Widget w1(data1);
    WidgetStuff::Widget w2(data2);

    std::cout << "[교환 전]\n";
    std::cout << "w1:"; w1.printAddress();
    std::cout << "w2:"; w2.printAddress();

    // ✨ 핵심 Step 3: 실무에서 스왑을 호출하는 절대 공식 (ADL 활용)
    std::cout << "\n[교환 실행]\n";
    using std::swap; // 1. "std::swap도 일단 후보에 올려둘게!"
    swap(w1, w2);    // 2. "하지만 w1, w2가 Widget 타입이므로, WidgetStuff::swap을 1순위로 찾아서 쓴다!"

    std::cout << "\n[교환 후]\n";
    std::cout << "w1:"; w1.printAddress();
    std::cout << "w2:"; w2.printAddress();

    delete data1;
    delete data2;

    return 0;
}

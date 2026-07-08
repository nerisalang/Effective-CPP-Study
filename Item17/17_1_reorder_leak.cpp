#include <iostream>
#include <memory>
#include <exception>

class Widget {
public:
    Widget() { std::cout << "🏭 [Widget] 힙 메모리 생성 완료!\n"; }
    ~Widget() { std::cout << "🗑️ [Widget] 소멸자 작동 (메모리 해제)!\n"; }
};

// 가끔 기분이 나쁘면 예외를 던지는 억까 함수
int getPriorityWithError() {
    std::cout << "⚡ [OS] 우선순위 계산 중 에러(예외) 발생!\n";
    throw std::runtime_error("Priority Error!");
}

void processWidget(std::shared_ptr<Widget> pw, int priority) {
    std::cout << "🚀 위젯 처리 함수 가동 성공!\n";
}

int main() {
    std::cout << "=== 1. 🚨 위험한 한 줄 호출 방식 (이론상 누수 위험) ===\n";
    std::cout << "-> (실제 런타임 순서는 컴파일러 맘대로라 예외 터지면 골치 아파짐)\n";
    try {
        // 이 한 줄 안에서 new -> getPriority -> shared_ptr 순서로 꼬이면 자원이 샙니다.
        // processWidget(std::shared_ptr<Widget>(new Widget), getPriorityWithError());
        std::cout << "[안내] 컴파일러 정렬 억까를 시뮬레이션하기 위해 주석 처리 후 정석으로 이동\n";
    } catch (...) {}

    std::cout << "\n=== 2. ✨ 안전한 독립된 문장 방식 ===\n";
    try {
        // 🔒 독립된 문장으로 묶어서 자원을 스마트 포인터에 확실히 봉인!
        std::shared_ptr<Widget> safeShared(new Widget);
        
        // 이제 여기서 예외가 픽 터져도, 이미 safeShared가 자원을 쥐고 있으므로 스택 청소 시 소멸자가 켜짐!
        processWidget(safeShared, getPriorityWithError());
    } catch (const std::exception& e) {
        std::cout << "⚠️ 예외 감지: " << e.what() << " (하지만 메모리는 안전할까?)\n";
    }

    std::cout << "\n=== 3. 🛠️ 현대 C++의 정석 (std::make_shared) ===\n";
    try {
        // 한 줄로 써도 100% 안전한 치트키
        processWidget(std::make_shared<Widget>(), getPriorityWithError());
    } catch (...) {
        std::cout << "⚠️ 예외 감지 완료 (make_shared 덕분에 한 줄이어도 안전함)\n";
    }

    std::cout << "\n=== 4. 프로그램 최종 종료 ===\n";
    return 0;
}

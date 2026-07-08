#include <iostream>

// 로우레벨 시스템의 포트 핸들 (가짜 정수형 데이터)
typedef int PortHandle;

// 구형 C 스타일 커널 API (스마트 객체를 모름. 오직 날것의 PortHandle만 받음)
void sendSystemSignal(PortHandle port) {
    std::cout << "📡 [OS Kernel] " << port << "번 포트로 제어 신호 송신 완료.\n";
}

// 🔒 1. 명시적 변환 방식을 채택한 안전한 RAII 클래스
class ExplicitPort {
private:
    PortHandle handle;
public:
    ExplicitPort(PortHandle p) : handle(p) {}
    ~ExplicitPort() { std::cout << "💀 [Explicit] " << handle << "번 포트 닫힘.\n"; }

    // 명시적으로 알맹이를 꺼내주는 Getter
    PortHandle get() const { return handle; } 
};

// 🔑 2. 암시적 변환 방식을 채택한 편리하고 위험한 RAII 클래스
class ImplicitPort {
private:
    PortHandle handle;
public:
    ImplicitPort(PortHandle p) : handle(p) {}
    ~ImplicitPort() { std::cout << "💀 [Implicit] " << handle << "번 포트 닫힘.\n"; }

    // 🌟 암시적 변환 연산자 (리턴 타입을 명시하지 않는 게 문법적 특징)
    operator PortHandle() const { return handle; } 
};

int main() {
    std::cout << "=== 1. 명시적 변환 (.get()) 검증 ===\n";
    ExplicitPort p1(80);
    // sendSystemSignal(p1); // ❌ 주석 풀면 컴파일 에러! 타입이 안 맞아서 컴파일러가 막아줌
    sendSystemSignal(p1.get()); // ✨ .get()을 붙여야만 안전하게 통과

    std::cout << "\n=== 2. 암시적 변환 (operator)의 편리함 ===\n";
    ImplicitPort p2(443);
    sendSystemSignal(p2); // ✨ .get() 없이 객체만 던져도 컴파일러가 알아서 int로 치환해서 통과!

    std::cout << "\n=== 3. 암시적 변환이 유발하는 끔찍한 실수 시나리오 ===\n";
    ImplicitPort clientPort(10);
    ImplicitPort serverPort(90);

    // [상황] 두 포트가 같은 포트인지 비교(==)하려다가 실수로 대입 연산자(=)를 씀!
    // 원래 일반 객체끼리는 복사 방어가 되어있어야 하지만, 암시적 변환 때문에...
    if (clientPort = serverPort) { 
        std::cout << "🚨 [좆버그 발생] 컴파일러가 에러를 안 잡고 조용히 빌드를 통과시킴!\n";
        std::cout << "-> 이유: clientPort와 serverPort가 각각 숫자로 변환된 뒤 대입이 일어나 버림.\n";
        std::cout << "-> 변조된 clientPort 값: " << (PortHandle)clientPort << " (원래 10이었는데 90으로 오염됨)\n";
    }

    std::cout << "\n=== 4. 프로그램 종료 단계 ===\n";
    return 0;
}

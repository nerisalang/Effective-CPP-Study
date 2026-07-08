#include <iostream>
#include <memory> // 🔒 스마트 포인터를 쓰기 위해 필수!

class NetworkConnection {
public:
    NetworkConnection() { std::cout << "🌐 [서버] 연결 포트 할당 완료!\n"; }
    ~NetworkConnection() { std::cout << "🛑 [서버] 연결 포트 반환 및 해제 완료!\n"; }
    void sendData() { std::cout << "📡 데이터 전송 중...\n"; }
};

void runSafeServerLogic(int condition) {
    // 🌟 RAII 적용: 자원을 획득하자마자 스마트 포인터 객체(스택 변수)의 생성자에 박아넣음!
    std::unique_ptr<NetworkConnection> conn(new NetworkConnection());
    
    conn->sendData(); // 일반 포인터랑 똑같이 -> 연산자 사용 가능

    if (condition == 1) {
        std::cout << "⚠️ [경고] 로직 도중 에러 발생! 조기 탈출합니다.\n";
        return; // ✨ delete를 안 적었지만, 스택이 터지면서 conn의 소멸자가 부모를 자동 해제함!
    }

    // 명시적인 delete 코드가 아예 필요 없음! 함수가 끝나면 알아서 해제됨.
}

int main() {
    std::cout << "=== 1. RAII 정상 흐름 테스트 ===\n";
    runSafeServerLogic(0);

    std::cout << "\n=== 2. RAII 조기 탈출 흐름 테스트 (안전 보장) ===\n";
    runSafeServerLogic(1); // ✨ 조기 탈출이 터져도 해제 로그가 완벽하게 찍힘!

    return 0;
}

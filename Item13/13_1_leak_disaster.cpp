#include <iostream>
#include <string>

class NetworkConnection {
public:
    NetworkConnection() { std::cout << "🌐 [서버] 연결 포트 할당 완료!\n"; }
    ~NetworkConnection() { std::cout << "🛑 [서버] 연결 포트 반환 및 해제 완료!\n"; }
    void sendData() { std::cout << "📡 데이터 전송 중...\n"; }
};

void runServerLogic(int condition) {
    // 🚨 날것의 포인터로 자원 획득
    NetworkConnection* conn = new NetworkConnection();

    conn->sendData();

    if (condition == 1) {
        std::cout << "⚠️ [경고] 로직 도중 에러 발생! 조기 탈출합니다.\n";
        return; // 💥 delete를 실행하지 못하고 함수를 빠져나감!
    }

    delete conn; // 정상적일 때만 켜지는 코드
}

int main() {
    std::cout << "=== 1. 정상 흐름 테스트 ===\n";
    runServerLogic(0); // 정상 작동해서 소멸자가 켜짐

    std::cout << "\n=== 2. 조기 탈출 흐름 테스트 (메모리 누수 발생) ===\n";
    runServerLogic(1); // 💥 할당은 되었는데 해제 로그가 안 뜸!

    return 0;
}

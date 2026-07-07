#include <iostream>

class ServerNode {
public:
    ServerNode() {
        // 🚨 생성자 안에서 일반 초기화 함수를 불렀는데?
        initServer(); 
    }

    void initServer() {
        std::cout << "[부모] 서버 노드 초기화 중...\n";
        // 💥 initServer() 내부에서 순수 가상 함수를 간접 호출!
        loadConfig(); 
    }

    ~ServerNode() {
        // 🚨 소멸자 안에서도 순수 가상 함수를 간접 호출!
        releaseNode();
    }

    void releaseNode() {
        std::cout << "[부모] 서버 노드 정리 중...\n";
        loadConfig(); 
    }

    // 순수 가상 함수 (부모에 몸통 없음)
    virtual void loadConfig() = 0; 
};

class AuthServerNode : public ServerNode {
public:
    virtual void loadConfig() override {
        std::cout << "[자식] 인증 서버 전용 설정 파일 로드 완료!\n";
    }
};

int main() {
    std::cout << "--- 1. 서버 노드 생성 시도 ---\n";
    AuthServerNode authServer; // 💥 여기서 과연 어떤 일이 일어날까?
    
    std::cout << "--- 2. 정상 실행 중 ---\n";
    return 0;
}

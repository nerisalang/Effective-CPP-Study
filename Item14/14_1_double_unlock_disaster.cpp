#include <iostream>

// OS 레벨의 로우레벨 뮤텍스를 흉내 낸 가짜 가상 뮤텍스 구조체
struct FakeMutex {
    bool isLocked = false;
};

// 뮤텍스 제어 함수 시스템
void mockWait(FakeMutex* pm) {
    std::cout << "🔒 [OS] 뮤텍스 락(Lock) 획득 성공!\n";
    pm->isLocked = true;
}

void mockUnlock(FakeMutex* pm) {
    if (!pm->isLocked) {
        std::cout << "💥 [🚨 대재앙] 데드락/더블 언락 감지! 이미 해제된 락을 또 해제하려 했습니다! 시스템 강제 종료!\n";
        return;
    }
    std::cout << "🔓 [OS] 뮤텍스 락(Unlock) 해제 완료.\n";
    pm->isLocked = false;
}

// ❌ 복사 동작을 전혀 고려하지 않은 멍청한 RAII 클래스
class DumbLock {
private:
    FakeMutex* pMutex;
public:
    explicit DumbLock(FakeMutex* pm) : pMutex(pm) { mockWait(pMutex); }
    ~DumbLock() { mockUnlock(pMutex); }
};

int main() {
    FakeMutex systemMutex;

    std::cout << "--- 1. 스레드 로직 진입 및 Lock 생성 ---\n";
    DumbLock m1(&systemMutex);

    {
        std::cout << "\n--- 2. 임의의 블록 내부에서 임시 객체로 복사 발생 ---\n";
        DumbLock m2 = m1; // 💥 기본 복사 연산자가 발동하여 포인터 주소만 얕은 복사됨
        std::cout << "-> 임시 블록을 탈출합니다. m2가 소멸됩니다.\n";
    } // ✨ m2 소멸자 가동 -> mockUnlock() 1등으로 호출됨

    std::cout << "\n--- 3. 메인 함수가 끝납니다. m1이 소멸됩니다. ---\n";
    // ✨ m1 소멸자 가동 -> 이미 m2가 락을 해제했는데 또 unlock() 하려고 시도함!
    return 0;
}

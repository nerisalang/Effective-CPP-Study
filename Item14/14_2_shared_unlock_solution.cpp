#include <iostream>
#include <memory> // std::shared_ptr을 쓰기 위해 필수

struct FakeMutex {
    bool isLocked = false;
};

void mockWait(FakeMutex* pm) {
    std::cout << "🔒 [OS] 뮤텍스 락(Lock) 획득 성공!\n";
    pm->isLocked = true;
}

void mockUnlock(FakeMutex* pm) {
    if (!pm->isLocked) {
        std::cout << "💥 더블 언락 터짐!\n";
        return;
    }
    std::cout << "🔓 [OS] 최종 참조가 끝나 뮤텍스 락(Unlock)을 안전하게 자동 해제합니다.\n";
    pm->isLocked = false;
}

// 🌟 항목 14 정석: shared_ptr의 레퍼런스 카운팅 메커니즘을 자원 관리에 응용!
class SafeLock {
private:
    // shared_ptr이 포인터를 물고 있게 만듭니다. 
    // 두 번째 인자로 우리가 직접 만든 mockUnlock 함수 포인터를 '삭제자(Deleter)'로 등록합니다!
    std::shared_ptr<FakeMutex> mutexPtr;

public:
    explicit SafeLock(FakeMutex* pm) 
        : mutexPtr(pm, mockUnlock) // 🔒 포인터 주소와 함께 "죽을 때 호출할 락 해제 함수"를 바인딩!
    {
        mockWait(pm); // 생성할 때 락 획득
    }
    
    // 포인터가 죽을 때 스마트 포인터 내부 카운터가 알아서 삭제자(mockUnlock)를 켜주므로
    // 소멸자를 굳이 직접 만들어서 unlock을 명시할 필요가 전혀 없음! (자동 관리)
};

int main() {
    FakeMutex systemMutex;

    std::cout << "--- 1. SafeLock 생성 ---\n";
    SafeLock m1(&systemMutex); // Ref Count = 1

    {
        std::cout << "\n--- 2. 임시 블록 내부로 복사 ---\n";
        SafeLock m2 = m1; // 🔒 복사 대입 성공! (Ref Count = 2로 증가, 락 해제 안 됨)
        std::cout << "-> 임시 블록 탈출. m2 소멸 (Ref Count가 1로 감소하므로 락은 유지됨)\n";
    } 

    std::cout << "\n--- 3. 메인 함수 종료 직전 (m1 소멸 시점) ---\n";
    // m1이 죽으면서 Ref Count가 드디어 0이 됨! 
    // 스마트 포인터 내부에서 우리가 등록해 둔 mockUnlock(&systemMutex)을 딱 한 번 안전하게 실행함!
    return 0;
}

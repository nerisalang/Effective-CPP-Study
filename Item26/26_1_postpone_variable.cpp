#include <iostream>
#include <string>

// 무거운 객체라고 가정 (생성/소멸 시 로그 출력)
class HeavyWeapon {
public:
    HeavyWeapon() { std::cout << "  [HeavyWeapon] 깡통 무기 생성! (비용 낭비)\n"; }
    HeavyWeapon(const std::string& name) { std::cout << "  [HeavyWeapon] '" << name << "' 장착 완료! (효율적)\n"; }
    ~HeavyWeapon() { std::cout << "  [HeavyWeapon] 무기 해제 및 소멸.\n"; }
    
    HeavyWeapon& operator=(const std::string& name) {
        std::cout << "  [HeavyWeapon] 깡통 무기에 '" << name << "' 대입 중... (추가 비용)\n";
        return *this;
    }
};

void badStyle(bool isEnemySpotted) {
    std::cout << "\n❌ [Bad Style] C언어식 조기 선언 실행\n";
    HeavyWeapon weapon; // 일단 쓸 줄 알고 미리 만들어둠 (생성자 호출)

    if (!isEnemySpotted) {
        std::cout << "  -> 적이 없어서 그냥 후퇴합니다.\n";
        return; // 무기는 한 번도 안 썼는데 소멸자가 불리며 성능 낭비!
    }

    weapon = "BFG-9000"; // 대입 연산자 추가 호출
    std::cout << "  -> 전투 시작!\n";
}

void goodStyle(bool isEnemySpotted) {
    std::cout << "\n✨ [Good Style] C++식 지연 선언 실행\n";
    
    if (!isEnemySpotted) {
        std::cout << "  -> 적이 없어서 바로 후퇴합니다. (무기 생성 비용 0!)\n";
        return; 
    }

    // 진짜로 적이 나타나서 싸워야 할 때 선언과 동시에 초기화!
    HeavyWeapon weapon("BFG-9000"); 
    std::cout << "  -> 전투 시작!\n";
}

int main() {
    std::cout << "=== 항목 26: 변수 정의 늦추기 테스트 ===\n";

    std::cout << "\n--- 시나리오 1: 적이 없을 때 (조기 반환) ---";
    badStyle(false);
    goodStyle(false);

    std::cout << "\n--- 시나리오 2: 적이 나타났을 때 ---";
    badStyle(true);
    goodStyle(true);

    return 0;
}

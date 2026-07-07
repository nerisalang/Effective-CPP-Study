#include <iostream>
#include <string>

// 부모 클래스 (유저 기본 정보)
class User {
protected:
    std::string name;
public:
    User(){}
    User(std::string n) : name(n) {}
    User(const User& rhs) : name(rhs.name) {
        std::cout << "[부모] 복사 생성자 호출 완료 (이름 복사)\n";
    }
};

// 자식 클래스 (서버 프리미엄 결제 정보)
class PremiumUser : public User {
private:
    int remainingDays;
public:
    PremiumUser(std::string name, int days) : User(name), remainingDays(days) {}

    // ❌ 부모 복사를 누락한 치명적인 자식 복사 생성자
    PremiumUser(const PremiumUser& rhs) 
        : remainingDays(rhs.remainingDays) // 부모(User) 복사를 안 적음!
    {
        std::cout << "[자식] 복사 생성자 호출 완료 (남은 기간 복사)\n";
    }

    void printInfo() const {
        // 부모의 name이 제대로 복사되었는지 확인
        std::cout << "유저 이름: " << (name.empty() ? "❌ [데이터 유기됨]" : name) 
                  << ", 남은 멤버십: " << remainingDays << "일\n";
    }
};

int main() {
    std::cout << "--- 1. 원본 프리미엄 유저 생성 ---\n";
    PremiumUser user1("Salah", 30);
    user1.printInfo();

    std::cout << "\n--- 2. 원본 유저를 새로운 객체로 복사 생성 ---\n";
    PremiumUser user2 = user1; // 복사 생성자 발동

    std::cout << "\n--- 3. 복사본 유저 상태 확인 ---\n";
    user2.printInfo();

    return 0;
}

#include <iostream>
#include <string>

class User {
protected:
    std::string name;
public:
    User(std::string n) : name(n) {}
    User(const User& rhs) : name(rhs.name) {}
    
    User& operator=(const User& rhs) {
        if (this != &rhs) {
            this->name = rhs.name;
        }
        return *this;
    }
};

class PremiumUser : public User {
private:
    int remainingDays;
public:
    PremiumUser(std::string name, int days) : User(name), remainingDays(days) {}

    // 🌟 1. 복사 생성자 정석: 초기화 리스트에서 부모 복사 생성자 명시적 호출!
    PremiumUser(const PremiumUser& rhs) 
        : User(rhs), remainingDays(rhs.remainingDays) 
    {
        std::cout << "[성공] 복사 생성자가 객체 전체를 안전하게 복사함\n";
    }

    // 🌟 2. 복사 대입 연산자 정석: 부모의 대입 연산자를 명시적으로 호출!
    PremiumUser& operator=(const PremiumUser& rhs) {
        if (this == &rhs) return *this;

        // 항목 11에서 배운 안전성을 확보한 뒤 부모 대입 연산자 가동!
        User::operator=(rhs); 
        this->remainingDays = rhs.remainingDays;

        std::cout << "[성공] 복사 대입 연산자가 객체 전체를 안전하게 복사함\n";
        return *this;
    }

    void printInfo() const {
        std::cout << "유저 이름: " << name << ", 남은 멤버십: " << remainingDays << "일\n";
    }
};

int main() {
    PremiumUser user1("Ohtani", 99);
    
    std::cout << "--- 1. 복사 생성자 테스트 ---\n";
    PremiumUser user2 = user1;
    user2.printInfo();

    std::cout << "\n--- 2. 복사 대입 연산자 테스트 ---\n";
    PremiumUser user3("Salah", 10);
    user3 = user1; // 대입 연산자 발동
    user3.printInfo();

    return 0;
}

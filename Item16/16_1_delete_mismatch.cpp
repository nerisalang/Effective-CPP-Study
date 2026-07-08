#include <iostream>
#include <string>

class Player {
private:
    std::string name;
public:
    Player() {
        static int count = 1;
        name = "Player_" + std::to_string(count++);
        std::cout << "🔥 [생성자] " << name << " 입장!\n";
    }
    ~Player() {
        std::cout << "💀 [소멸자] " << name << " 퇴장 완료.\n";
    }
};

int main() {
    std::cout << "=== 1. 정석적인 1개짜리 할당 및 해제 ===\n";
    Player* pSingle = new Player();
    delete pSingle; // 짝이 맞음 (new - delete)

    std::cout << "\n=== 2. 정석적인 배열 할당 및 해제 (delete[]) ===\n";
    Player* pArrayCorrect = new Player[3];
    delete[] pArrayCorrect; // 짝이 맞음 (new[] - delete[])

    std::cout << "\n=== 3. 🚨 대재앙: 배열로 만들고 그냥 delete로 조지기 ===\n";
    Player* pArrayWrong = new Player[3];
    
    std::cout << "-> 이제 대괄호 없이 그냥 delete를 때려보겠습니다.\n";
    // 💥 짝이 안 맞음! 과연 3마리가 다 무사히 퇴장할 수 있을까?
    delete pArrayWrong; 

    std::cout << "\n=== 4. 프로그램 정상 종료 시도 ===\n";
    return 0;
}

#include <iostream>
#include <stdexcept>
#include <memory>

// 1. 값의 유효성 검증(생성자 가드)이 들어간 독립 타입 Day
struct Day {
    int val;
    explicit Day(int d) : val(d) {
        if (d < 1 || d > 31) {
            throw std::invalid_argument("🚨 [Day 에러] 날짜는 1일부터 31일 사이여야 합니다!");
        }
    }
};

// 2. 외부에서 엉뚱한 숫자 입력을 원천 차단하는 선택지 제한형 클래스 Month
class Month {
private:
    int val;
    explicit Month(int m) : val(m) {} // 🔒 생성자 폐쇄
public:
    int getVal() const { return val; }
    
    // 오직 정해진 12개의 선택지만 허용 (정적 팩토리)
    static Month Jan() { return Month(1); }
    static Month Feb() { return Month(2); }
    static Month Mar() { return Month(3); }
    static Month Dec() { return Month(12); }
};

struct Year {
    int val;
    explicit Year(int y) : val(y) {}
};

// 🌟 항목 18 정석: 그릇된 사용이 불가능한 안전한 날짜 클래스
class SafeDate {
private:
    Month month; Day day; Year year;
public:
    SafeDate(const Month& m, const Day& d, const Year& y) : month(m), day(d), year(y) {}
    void print() const {
        std::cout << "📅 [SafeDate] " << year.val << "년 " << month.getVal() << "월 " << day.val << "일\n";
    }
};

// ==========================================
// 자원 관리 오용 차단 시뮬레이션 (커스텀 삭제자 콤보)
// ==========================================
struct Resource {
    Resource() { std::cout << "🏭 [시스템 자원] 할당 성공!\n"; }
};

// 자원을 해제할 땐 delete가 아니라 반드시 이 전용 함수를 써야 하는 규칙이 있다고 가정
void closeSystemResource(Resource* r) {
    std::cout << "🛑 [시스템 자원] 전용 함수 호출로 안전하게 반환 및 폐쇄 완료.\n";
    delete r;
}

// ✨ 유저에게 delete 책임을 주지 않고, 스마트 포인터에 삭제자를 묶어서 강제 자동화
std::shared_ptr<Resource> createSafeResource() {
    return std::shared_ptr<Resource>(new Resource, closeSystemResource);
}

int main() {
    std::cout << "=== 1. 인터페이스 정석 사용법 ===\n";
    SafeDate d1(Month::Mar(), Day(30), Year(2026));
    d1.print();

    std::cout << "\n=== 2. 인자 순서 변경 시도 (컴파일러의 빌드 컷) ===\n";
    // 주석을 풀면 순서가 틀려서 컴파일 에러가 터집니다. 
    // SafeDate d2(Day(30), Month::Mar(), Year(2026)); 
    std::cout << "[통과] 순서 변경은 컴파일러가 완벽하게 차단하고 있습니다.\n";

    std::cout << "\n=== 3. 범위 초과 값 입력 시도 (생성자 가드의 진압) ===\n";
    try {
        std::cout << "-> 3월 40일을 생성해 보겠습니다...\n";
        SafeDate d3(Month::Mar(), Day(40), Year(2026)); // 💥 여기서 예외 터짐!
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << " -> 안전하게 객체 오염 방어 성공!\n";
    }

    std::cout << "\n=== 4. 자원 오용(해제 누수) 차단 검증 ===\n";
    {
        std::cout << "-> 자원을 획득합니다...\n";
        std::shared_ptr<Resource> myRes = createSafeResource();
        std::cout << "-> 유저가 따로 해제 코드를 적지 않고 블록을 탈출합니다.\n";
    } // ✨ 블록을 나가는 순간 스마트 포인터 소멸자가 작동하여 전용 함수를 자동 호출함!

    std::cout << "\n=== 5. 프로그램 최종 종료 ===\n";
    return 0;
}

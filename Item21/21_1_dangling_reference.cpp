#include <iostream>

class Rational {
public:
    int num, den;
    Rational(int n = 0, int d = 1) : num(n), den(d) {}

    // ❌ 1. 죽은 자의 온기가 남아있는 레퍼런스 리턴 함수
    friend const Rational& multiplyBad(const Rational& lhs, const Rational& rhs) {
        Rational localResult(lhs.num * rhs.num, lhs.den * rhs.den);
        return localResult; // 🚨 위험: 지역 객체의 참조자를 리턴!
    }

    // ✨ 2. 정석적인 값 리턴 함수 (RVO 최적화 버프 적용됨)
    friend const Rational multiplyGood(const Rational& lhs, const Rational& rhs) {
        return Rational(lhs.num * rhs.num, lhs.den * rhs.den); // 안전하게 복사본 리턴
    }
};

int main() {
    Rational a(2, 3);
    Rational b(3, 4);

    std::cout << "=== 1. 정석적인 값 리턴 테스트 ===\n";
    Rational goodResult = multiplyGood(a, b);
    std::cout << "📈 정상 결과: " << goodResult.num << "/" << goodResult.den << "\n";

    std::cout << "\n=== 2. 🚨 대재앙: 레퍼런스 리턴 테스트 ===\n";
    // 과연 스택에서 소멸한 객체의 주소를 가져오면 어떻게 될까?
    const Rational& badResult = multiplyBad(a, b);
    
    std::cout << "-> 과연 소멸된 메모리의 값을 읽어올 수 있을까요?\n";
    // 높은 확률로 쓰레기 값이 찍히거나 프로세스가 터집니다.
    std::cout << "💥 시체 주소 결과: " << badResult.num << "/" << badResult.den << "\n";

    return 0;
}

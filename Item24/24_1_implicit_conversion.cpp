#include <iostream>

class Rational {
private:
    int num, den;

public:
    // explicit 키워드가 없으므로, int가 들어오면 Rational로 자동(암시적) 변환됨
    Rational(int numerator = 0, int denominator = 1) 
        : num(numerator), den(denominator) {}

    int numerator() const { return num; }
    int denominator() const { return den; }

    // ❌ 1. 멤버 함수로 구현했을 때 (주석 해제하면 '2 * a'에서 컴파일 터짐)
    /*
    const Rational operator*(const Rational& rhs) const {
        return Rational(this->num * rhs.num, this->den * rhs.den);
    }
    */
};

// ✨ 2. 비멤버 함수로 구현했을 때 (양쪽 모두 동등하게 타입 변환 버프를 받음)
const Rational operator*(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.numerator() * rhs.numerator(), 
                    lhs.denominator() * rhs.denominator());
}

int main() {
    std::cout << "=== 항목 24: 비멤버 함수와 암시적 타입 변환 ===\n\n";

    Rational a(1, 2); // 1/2

    // Case 1: 객체 * 객체 (항상 성공)
    Rational result1 = a * a; 
    std::cout << "1. a * a = " << result1.numerator() << "/" << result1.denominator() << "\n";

    // Case 2: 객체 * 정수 (멤버 함수든 비멤버든 성공)
    // a.operator*(2) 또는 operator*(a, 2)
    Rational result2 = a * 2; 
    std::cout << "2. a * 2 = " << result2.numerator() << "/" << result2.denominator() << "\n";

    // Case 3: ✨ 정수 * 객체 (비멤버 함수일 때만 성공!)
    // 2.operator*(a)는 불가! 하지만 operator*(2, a)로 변환되어 대성공!
    Rational result3 = 2 * a; 
    std::cout << "3. 2 * a = " << result3.numerator() << "/" << result3.denominator() << "\n";

    return 0;
}

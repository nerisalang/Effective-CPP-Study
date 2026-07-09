#include <iostream>
#include <cstring>

// 🌟 항목 19: 기본제공 타입(int 등)처럼 완벽하게 동작하는 custom 문자열 타입 설계
class CustomString {
private:
    char* buffer; // 로우레벨 힙 메모리 자원

public:
    // 1. 생성자 (explicit으로 암시적 변환 방어 - Item 18)
    explicit CustomString(const char* str = "") {
        if (str == nullptr) {
            buffer = new char[1];
            buffer[0] = '\0';
        } else {
            buffer = new char[std::strlen(str) + 1];
            std::strcpy(buffer, str);
        }
        std::cout << "🏭 [생성자] 메모리 할당 완료: " << (buffer[0] ? buffer : "빈 문자열") << "\n";
    }

    // 2. 소멸자 (자원의 확실한 소멸 - Item 16 짝맞추기)
    ~CustomString() {
        std::cout << "🗑️ [소멸자] 메모리 해제 완료: " << (buffer[0] ? buffer : "빈 문자열") << "\n";
        delete[] buffer; 
    }

    // 3. 복사 생성자 (깊은 복사 구현하여 11장의 Double Free 방어 - Item 14)
    CustomString(const CustomString& other) {
        buffer = new char[std::strlen(other.buffer) + 1];
        std::strcpy(buffer, other.buffer);
        std::cout << "👥 [복사 생성자] 깊은 복사 완료\n";
    }

    // 4. 대입 연산자 (자기대입 방어 및 일관성 - Item 11)
    CustomString& operator=(const CustomString& rhs) {
        std::cout << "🔄 [대입 연산자] 작동\n";
        if (this == &rhs) return *this; // 자기대입 방어

        delete[] buffer; // 기존 자원 해제
        buffer = new char[std::strlen(rhs.buffer) + 1];
        std::strcpy(buffer, rhs.buffer);
        return *this;
    }

    // 5. 연산자 일관성 (값 출력을 위한 명시적 함수 - Item 15)
    const char* c_str() const { return buffer; }
};

int main() {
    std::cout << "=== 1. 일반적인 객체 생성 ===\n";
    CustomString s1("Liverpool");

    std::cout << "\n=== 2. 복사 동작 검증 (깊은 복사로 메모리 터짐 방지) ===\n";
    CustomString s2 = s1; // 복사 생성자 가동

    std::cout << "\n=== 3. 대입 동작 검증 ===\n";
    CustomString s3("Salah");
    s3 = s1; // 대입 연산자 가동

    std::cout << "\n=== 4. 암시적 변환 차단 확인 ===\n";
    // CustomString s4 = "Ohtani"; // ❌ 주석 풀면 explicit 때문에 컴파일 에러! 타입 기강 확립.
    CustomString s4("Ohtani");    // ✨ 명시적으로만 생성 가능

    std::cout << "\n=== 5. 블록 종료 시 안전한 해제 ===\n";
    return 0;
}

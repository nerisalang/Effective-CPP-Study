#include <iostream>
#include <string>

class SmartText {
public:
    SmartText(std::string text) : content(text), accessCount(0) {}

    // [1] 상수 버전: 읽기 전용, 하지만 내부적으로 횟수는 세고 싶음!
    const char& operator[](std::size_t pos) const {
        // 상수 함수지만 mutable 덕분에 값을 바꿀 수 있음 (논리적 상수성)
        accessCount++; 
        std::cout << "(상수 호출 - 횟수: " << accessCount << ") ";
        return content[pos];
    }

    // [2] 비-상수 버전: [1]번을 호출해서 코드 중복을 피함!
    char& operator[](std::size_t pos) {
        std::cout << "(일반 호출) ";
        // const_cast를 이용해 상수 버전을 재사용하는 테크닉
        return const_cast<char&>(
            static_cast<const SmartText&>(*this)[pos]
        );
    }

private:
    std::string content;
    mutable int accessCount; // '상수성'의 예외를 허용하는 마법의 키워드
};

int main() {
    SmartText st("Hello");
    const SmartText cst("World");

    std::cout << st[0] << std::endl;  // 일반 호출 -> 상수 호출 순서로 진행
    std::cout << cst[0] << std::endl; // 바로 상수 호출
    std::cout << cst[1] << std::endl; // 횟수가 올라가는지 확인

    return 0;
}

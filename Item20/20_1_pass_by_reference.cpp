#include <iostream>
#include <string>

class Person {
public:
    Person() {}
    Person(const Person& other) { std::cout << "  👥 [Person 복사 생성자] 터짐!\n"; }
    virtual ~Person() {}
};

class Student : public Person {
private:
    std::string name; // 내부 객체 복사 비용 유발자 1
    std::string school; // 내부 객체 복사 비용 유발자 2
public:
    Student(std::string n, std::string s) : name(n), school(s) {}
    Student(const Student& other) : Person(other), name(other.name), school(other.school) {
        std::cout << "  🎓 [Student 복사 생성자] 터짐!\n";
    }
    ~Student() {}
};

// ❌ 1. 대가리 깨지는 값에 의한 전달 함수
void printStudentByValue(Student s) {
    // 아무것도 안 함
}

// ✨ 2. 기강 잡힌 참조에 의한 상수 전달 함수
void printStudentByReference(const Student& s) {
    // 아무것도 안 함
}

int main() {
    Student minho("MinhoKim", "PKNU");

    std::cout << "=== 1. 값에 의한 전달 (Pass-by-value) 시도 ===\n";
    std::cout << "-> 함수 호출 시작...\n";
    printStudentByValue(minho);
    std::cout << "-> 함수 호출 종료\n";

    std::cout << "\n=== 2. 참조에 의한 상수 전달 (Pass-by-reference) 시도 ===\n";
    std::cout << "-> 함수 호출 시작...\n";
    printStudentByReference(minho);
    std::cout << "-> 함수 호출 종료\n";

    return 0;
}

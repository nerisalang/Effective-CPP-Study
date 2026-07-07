#include <iostream>

template <typename T>
class SmartPointer {
private:
    T* ptr;

public:
    // 일반 생성자
    explicit SmartPointer(T* p = nullptr) : ptr(p) {}

    // 알맹이 포인터를 가져오는 함수
    T* get() const { return ptr; }

    // 🔒 항목 10 + 템플릿 결합: 다른 타입(U)의 스마트 포인터를 대입받는 연산자!
    template <typename U>
    SmartPointer<T>& operator=(const SmartPointer<U>& rhs) {
        std::cout << "[템플릿 대입 연산자 켜짐] 다른 타입으로부터 대입 수행\n";
        
        // 데이터 대입 수행 (여기서는 예시로 포인터만 복사)
        // 만약 자식 클래스 타입을 부모 클래스 포인터에 넣는 등의 암시적 변환이 가능할 때만 빌드됨
        this->ptr = rhs.get(); 

        // 🌟 항목 10의 핵심: 대입 후 무조건 좌변 자기 자신의 참조자(*this)를 리턴!
        return *this; 
    }

    // 🚨 중요 팁: 템플릿 대입 연산자를 만들어도, 
    // '동일한 타입'끼리 대입할 때 쓰이는 기본 복사 대입 연산자(Copy Assignment)는 컴파일러가 따로 만듭니다.
    // 그래서 관례상 동일 타입 대입 연산자도 함께 적어주는 게 정석입니다.
    SmartPointer<T>& operator=(const SmartPointer<T>& rhs) {
        std::cout << "[일반 대입 연산자 켜짐] 동일 타입끼리 대입 수행\n";
        if (this != &rhs) {
            this->ptr = rhs.ptr;
        }
        return *this;
    }
};

// 상속 구조 예시 (자식을 부모에게 대입하는 상황 테스트용)
class Base {};
class Derived : public Base {};

int main() {
    Derived* d = new Derived();
    
    SmartPointer<Derived> derivedPtr(d); // 자식 타입 포인터를 품은 객체
    SmartPointer<Base> basePtr;          // 부모 타입 포인터를 품은 객체

    std::cout << "--- 1. 다른 타입끼리 대입 시도 (Derived -> Base) ---\n";
    // SmartPointer<Base>에 SmartPointer<Derived>를 대입! (템플릿 버전 작동)
    basePtr = derivedPtr; 

    std::cout << "\n--- 2. 동일 타입끼리 연쇄 대입 시도 (항목 10 검증) ---\n";
    SmartPointer<Base> basePtr2;
    SmartPointer<Base> basePtr3;
    
    // 항목 10 덕분에 완벽하게 작동하는 연쇄 대입(Chaining)
    basePtr3 = basePtr2 = basePtr; 

    return 0;
}

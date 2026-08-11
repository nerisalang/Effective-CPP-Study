#include <iostream>

// 2D 좌표 클래스
struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

// 사각형 클래스
class Rectangle {
private:
    Point upperLeft;  // 🔒 private으로 숨겨둔 좌상단 좌표
    Point lowerRight; // 🔒 private으로 숨겨둔 우하단 좌표

public:
    Rectangle(int ulX, int ulY, int lrX, int lrY) 
        : upperLeft(ulX, ulY), lowerRight(lrX, lrY) {}

    // ❌ 대참사 1: private 데이터의 참조자를 냅다 던지는 백도어 함수
    Point& getUpperLeft_Bad() { return upperLeft; }

    // ❌ 대참사 2: const를 붙여서 읽기 전용으로 던져도, 임시 객체와 만나면 터짐
    const Point& getUpperLeft_Const() const { return upperLeft; }

    void print() const {
        std::cout << "  현재 사각형 좌상단: (" << upperLeft.x << ", " << upperLeft.y << ")\n";
    }
};

// 화면에 임시 사각형을 만들어 뱉는 함수 (임시 객체 반환)
const Rectangle createTempBoundingBox() {
    return Rectangle(0, 0, 100, 100);
}

int main() {
    std::cout << "=== 항목 28: 핸들 반환의 무서움 ===\n\n";

    Rectangle myRect(10, 10, 50, 50);
    std::cout << "[초기 상태]\n";
    myRect.print();

    // 🚨 1. 캡슐화 붕괴 시연
    std::cout << "\n[해킹: private 데이터 외부에서 변조]\n";
    myRect.getUpperLeft_Bad().x = 999; // 참조자를 타고 들어가서 private 변수 강제 수정!
    myRect.print(); // 결과: (999, 10) 으로 원본이 털림.

    // 🚨 2. 댕글링 핸들(시체 참조) 시연
    std::cout << "\n[댕글링 포인터 발생 시연]\n";
    
    // createTempBoundingBox()가 만든 임시 객체에서 냅다 좌표 참조자를 뽑아냄!
    const Point* pUpperLeft = &(createTempBoundingBox().getUpperLeft_Const());
    
    // ⚠️ 이 문장이 끝난 순간, 임시 사각형 객체는 스택에서 소멸됨!
    // 이제 pUpperLeft 포인터는 이미 날아간 쓰레기 메모리를 가리키고 있음.
    
    std::cout << "  (위험) 시체 포인터 접근 시도: " << pUpperLeft->x << "\n";
    // 런타임에 쓰레기 값이 나오거나, 재수 없으면 Segmentation Fault로 즉사합니다.

    return 0;
}

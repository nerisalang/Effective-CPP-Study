#include <iostream>
#include <vector>
#include <numeric>

class SpeedDataCollection {
private:
    // 🔒 외부에서 절대 볼 수 없는 내부 톱니바퀴들
    std::vector<double> speedData;
    
    // mutable: const 멤버 함수 안에서도 값을 바꿀 수 있게 해주는 치트키
    mutable double averageSoFar; 
    mutable bool averageIsValid; 

public:
    // 초기화: 데이터가 없으니 평균값도 아직 유효하지 않음
    SpeedDataCollection() : averageIsValid(false), averageSoFar(0.0) {}

    // 1. 쓰기 전용(Write-only) 느낌의 함수
    void addValue(double speed) {
        speedData.push_back(speed);
        averageIsValid = false; // 데이터가 추가됐으니 기존 평균값은 이제 쓰레기!
        std::cout << "📈 [데이터 추가] 속도: " << speed << " (캐시 무효화됨)\n";
    }

    // 2. ✨ 책의 진짜 핵심: 읽기 전용(Read-only) 함수 안에 숨겨진 캡슐화의 마법
    double getAverageSoFar() const {
        // 캐시된 평균값이 유효하지 않을 때만 무거운 계산을 쌩으로 돌림 (지연 평가)
        if (!averageIsValid) {
            std::cout << "  ⚙️ [내부 연산] 데이터가 변경되어 평균을 '새로' 계산합니다...\n";
            if (speedData.empty()) {
                averageSoFar = 0.0;
            } else {
                double sum = std::accumulate(speedData.begin(), speedData.end(), 0.0);
                averageSoFar = sum / speedData.size();
            }
            averageIsValid = true; // 이제 캐시 유효함!
        } else {
            std::cout << "  ⚡ [캐시 적중] 계산 없이 저장된 평균값을 0.001초 만에 뱉습니다.\n";
        }
        return averageSoFar;
    }
};

int main() {
    std::cout << "=== 이펙티브 C++ 항목 22 오리지널 실습 ===\n\n";
    
    SpeedDataCollection radar;

    radar.addValue(100.5);
    radar.addValue(120.0);

    // 첫 번째 호출: 데이터를 새로 넣었으니 쌩으로 무거운 계산이 돌아감
    std::cout << "평균 속도: " << radar.getAverageSoFar() << "\n\n";

    // 두 번째 호출: 데이터 변경이 없었으므로, 계산 과정을 건너뛰고 캐시된 값을 즉시 반환
    std::cout << "평균 속도: " << radar.getAverageSoFar() << "\n\n";

    radar.addValue(150.5); // 새로운 데이터 난입! 캐시 다시 파괴됨
    std::cout << "평균 속도: " << radar.getAverageSoFar() << "\n";

    return 0;
}

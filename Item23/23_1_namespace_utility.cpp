#include <iostream>

// 🌟 1. 핵심 클래스 (최소한의 필수 권한만 가짐)
namespace WebBrowserStuff {
    class WebBrowser {
    private:
        int privateData = 100; // 캡슐화되어 지켜져야 할 핵심 자원

    public:
        void clearCache() { std::cout << "🗑️ [WebBrowser] 캐시 삭제 완료.\n"; }
        void clearHistory() { std::cout << "🗑️ [WebBrowser] 방문 기록 삭제 완료.\n"; }
        void removeCookies() { std::cout << "🗑️ [WebBrowser] 쿠키 삭제 완료.\n"; }
    };

    // 🌟 2. 비멤버 비프렌드 유틸리티 함수 (동일한 네임스페이스에 배치)
    // 이 녀석은 WebBrowser의 privateData에 접근할 권한이 원천 차단됨! (완벽한 캡슐화)
    void clearEverything(WebBrowser& wb) {
        std::cout << "🚀 [Utility] 브라우저 전체 청소 매크로 가동!\n";
        wb.clearCache();
        wb.clearHistory();
        wb.removeCookies();
    }
}

int main() {
    std::cout << "=== 항목 23: 네임스페이스를 활용한 비멤버 함수 설계 ===\n\n";
    
    WebBrowserStuff::WebBrowser myBrowser;

    // 유저 입장에서는 클래스 안에 있든 밖에 있든 똑같이 편리하게 호출 가능!
    WebBrowserStuff::clearEverything(myBrowser);

    std::cout << "\n[시스템] 캡슐화(private 접근 권한자 수)를 완벽하게 방어했습니다.\n";
    
    return 0;
}

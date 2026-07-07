#include <iostream>
#include <string>

class DBConnection {
public:
    // 🔒 1. 부모 생성자가 자식이 쏘아 올린 문자열을 매개변수로 직접 받음
    DBConnection(const std::string& serverType) {
        logConnection(serverType); 
    }

    // 가상 함수가 절대 아닌, 튼튼하고 안전한 일반 함수!
    void logConnection(const std::string& type) const {
        std::cout << "[DB 인프라 로그] " << type << " 연결 성공!" << std::endl;
    }

    virtual ~DBConnection() {} // 상속용 기본 가상 소멸자
};

class MySQLConnection : public DBConnection {
public:
    // 🔒 2. 자식 생성자가 부모 생성자에게 자식만의 고유 데이터를 인자로 토스!
    MySQLConnection() 
        : DBConnection("MySQL 프로덕션 서버") {
        std::cout << "[자식] MySQL 드라이버 로드 완료\n";
    }
};

class OracleConnection : public DBConnection {
public:
    OracleConnection() 
        : DBConnection("Oracle 금융 전용 서버") {
        std::cout << "[자식] Oracle 드라이버 로드 완료\n";
    }
};

int main() {
    std::cout << "=== 1. MySQL 객체 생성 ===\n";
    MySQLConnection mysql;

    std::cout << "\n=== 2. Oracle 객체 생성 ===\n";
    OracleConnection oracle;

    return 0;
}

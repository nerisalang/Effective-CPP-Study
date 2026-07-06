#include <iostream>
#include <stdexcept>

class DBConnection {
public:
    void close() {
        // 1. 사용자에게 에러를 핸들링할 기회를 준다!
        std::cout << "DB 연결 닫는 중..." << std::endl;
        // 필요하다면 여기서 throw 가능
    }
};

class DBConn {
private:
    DBConnection db;
    bool closed;

public:
    DBConn(DBConnection dbInstance) : db(dbInstance), closed(false) {}

    // 사용자가 원하면 직접 호출해서 예외 관리를 할 수 있게 기회를 줌
    void close() {
        db.close();
        closed = true;
    }

    ~DBConn() {
        if (!closed) {
            try {
                db.close(); // 2. 사용자가 안 닫았으면 소멸자가 대신 시도
            }
            catch (...) {
                // 💥 3. 만약 여기서 에러가 터져도 절대 밖으로 못 나가게 삼켜버린다!
                std::cout << "[로그] 소멸자에서 DB 닫기 실패했으나, 안전을 위해 예외를 묻습니다." << std::endl;
                
                // 정 프로그램을 죽여야겠다면 탈출시키지 말고 내가 먼저 선빵을 날린다.
                // std::abort(); 
            }
        }
    }
};

int main() {
    DBConnection myDB;
    
    // 안전 가방에 넣어서 관리
    DBConn dbc(myDB);

    // 하던 작업...
    std::cout << "DB 작업 수행 완료." << std::endl;

    // 만약 에러 수습이 깐깐하게 필요하다면? 사용자가 직접 close() 호출!
    // dbc.close(); 

    return 0;
}

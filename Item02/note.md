# 항목 2: #define을 쓰려거든 const, enum, inline을 떠올리자



* 단순한 상수를 쓸때는, #define보다 const 객체 혹은 enum을 우선 생각합시다.
* 함수처럼 쓰이는 매크로를 만들려면, #define 매크로보다 인라인 함수를 우선 생각합시다.


### 실습 요약
1. Item02\_Compare\_Const\_Define.cpp에 각각 전처리기 매크로 #define 과 상수 const를 준비함 
2. g++ -g Item02\_Compare\_Const\_Define.cpp -o Item02\_Compare\_Const\_Define 명령어를 이용하여 디버깅 정보를 포함해서 빌드함
3. gdb ./Item02/Compare\_Const\_Define 명령어를 이용하여서 gdb를 실행함
4. l: 소스 코드 출력
   b: 중단점 잡음
   r: 프로그램을 실행
   p: 프린트
5. p ASPECT\_RATIO\_MACRO의 결과와 p AspectRatioConst의 결과를 확인함


1. 클래스 멤버의 상수를 모든 객체가 공유하도록 static을 붙여 static const int NumTurns = 5; 를 준비 
2. 또한 const char* const authorName = "Minho Kim"를 준비함
3. gdb에서 p authorName 출력값을 확인하고 p &GamePlayer::NumTurns의 출력값을 확인해봄


1. 클래스 안에 배열처럼 컴파일 도중에 클래스 상수의 값이 필요한 상황의 코드를 준비 여기서는 int scores[NumTurns]; 를 준비함
2. 구식 컴파일러에서는 컴파일 과정에서 이 배열의 크기를 알아야 한다며 에러 터트림
3. 따라서 enum { NumTurns = 5 };를 통해 NumTurns를 5에 대한 기호식 이름으로 만듦 
4. 그리고 gdb를 통해 p &GamePlayer::NumTurns의 주소값을 찍어본다.

### 결과 요약
1. p ASPECT\_RATIO\_MACRO의 결과는 No symbol... 로 전처리기가 이미 숫자로 바꿔버려서 gdb는 이 이름의 존재조차 모름
2. 이와 다르게  p AspectRatioConst의 결과는 1.653로 나옴 컴파일러가 심볼 테이블에 이름을 적어놨기 때문에 가능함
3. 따라서 전처리기(#define)보다 컴파일러(const)를 믿으라. 


1. const 상수 포인터와 상수데이터는 포인터가 가리키는 대상과 포인터 자체를 수정할 수 없음을 확인할 수 있음
2. p authorName은 문자열 "Minho Kim"이 실제로 저장된 메모리 주소가 출력됨
3. 이는 authorName이 문자열 자체가 아니라 문자열이 시작되는 위치를 가리키는 포인터 변수이기 때문임
4. p &GamePlayer::NumTurns오류 발생함 Can't take address of ...... isn't an lvalue 라고 뜸
5. 이는 선언과 정의의 차이에서 오는 것으로
   -선언은 코드에 사용되는 '어떤 대상'의 이름과 타입을 컴파일러에게 알려주는    것입니다. 하지만 메모리를 할당하지는 않습니다.
   -정의는 선언에서 빠진 구체적인 세부사항을 컴파일러에게 제공하는 것입니다.    즉 컴파일러가 실제로 메모리 공간을 확보하는 순간입니다. 


1. p &GamePlayer::NumTurns의 결과 값은 Can't take address of ...... isn't anlvalue 라고 뜸
2. static const가 메모리 할당하는것에 반해 enum은 define 처럼 NumTurns를 5로 완전히 치워버리며 메모리 할당을 하지않음
3. 철저한 캡슐화와 메모리 절약을 할 수 있음
4. 다만 #define과는 다르게 심볼 테이블에 이름이 남고 디버깅중에 이름으로 값 확인 가능하며 Scope 또한 클래스 안으로 제한 가능함


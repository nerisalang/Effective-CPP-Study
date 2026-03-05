# 항목 3: 낌새만 보이면 const를 들이대 보자!


* const를 붙여 선언하면 컴파일러가 사용사의 에러를 잡아내는 데 도움을 줍니다. const는 어떤 유효범위에 있는 객체에도 붙을 수 있으며, 함수 매개변수 및 반환 타입에도 붙을 수 있으며, 멤버 함수에도 붙을 수 있습니다.
* 컴파일러 쪽에서 보면 비트수준 상수성을 지켜야 하지만, 여러분은 개념적인(논리적인) 상수엉을 사용해서 프로그래밍해야 합니다.
* 상수 멤버 및 비상수 멤버 함수가 기능적으로 서로 똑같게 구현되어 있을 경우에는 코드 중복을 피하는 것이 좋은데, 이때 비상수 버전이 상수 버전을 호출하도록 만드세요.


### 실습 요약
1. STL 반복자(iterator)를 이용해서 const std::vector<int>::iterator iter = vec.begin(); 와 std::vector<int>::const\_iterator cIter = vec.begin(); const 의 위치를 확인하고 의미가 어떻게 다른게 봄



1. 상수 객체에 대한 operator[]인 const char& operator[](std::size_t position) const {return text[position];}와 비상수 객체에 대한 operator[]인 char& operator[] (std::size_t position) {return text[position];}를 각각 만들어 준비함
2. cout<< tb[0]; tb[0]= 'x'; 비상수 버전의 TextBlock 객체를 읽고 씁니다.
3. cout<< ctb[0]; ctb[0]= 'x'; 상수 버전의 TextBlock 객체를 읽고 씁니다.



1. const 함수는 멤버 변수를 못 건드리지만 mutable를 이용해 상수성에 예외를 두어보자함
2. 코드 중복 현상을 피하기 위해 const\_cast를 이용해 중복을 피하고자함

### 결과 요약
1. STL 반복자(iterator)는 포인트를 본뜬 것이기 때문에 기본적인 동작 원리가 T* 포인터와 흡사함 또한 const 위치에 따라 의미가 달라짐
2. const std::vector<int>::iterator iter = vec.begin(); 은 T* const 처럼 동작합니다. 따라서 포인터 자체가 상수로 주소값을 못 바꿈 --\*iter = 10; 은 가능하지만 ++iter;은 불가
3. std::vector<int>::const\_iterator cIter = vec.begin(); 은 const T* 처럼 동작합니다. 따라서 가리키는 대상이 상수로 값을 바꾸지 못함 --\*cIter = 20; 은 불가능하지만 ++cIter; 가능함



1. ctb[0] = 'x';에서 컴파일 에러가 발생함 상수 버전의 TextBlock 객체에 대해 쓰기는 안됨
2. 원래의 출력값은 ctb[0] = 'x'; 상수 객체에 대한 쓰기를 제외하고 일반호출 -> 상수호출 -> 일반 호출 순으로 출력됨
3. ctb[0] = 'x'; 에서 발생한 컴파일 에러는 const char& operator[](std::size_t position) const {return text[position];} 반환 타입때문에 발생했음
4. (중요) 앞에 온 const은 반환값에 대한 const 이고 따라서 읽기전용으로 만들며 고칠 수가 없어짐, 뒤에온 const는 멤버 함수 자체에 대한 const 이고 따라서 이 함수 내부에서 객체의 멤버 변수를 절대로 건드리지 않겠다라고 하는 것임
5. 따라서 ctb[0] = 'x'은 operator[] 호출은 정상적으로 되지만 값을 x로 수정하려 하여 발생한 컴파일 에러 인것
6. operator[]가 값을 복사해서 주는것이 아닌 참조로 주기 때문에 반환 타입에 const 를 붙이지 않으면 안되심



1. mutable를 사용하면 컴파일러에게 accessCount의 비트가 변해도 상관없으니까 감시 대상에서 빼줘 라고 말하는 꼴로 상수성의 족쇄를 풀어줌
2. 상수성은 지키고 싶지만 내부적 기록이나 캐싱을 위해 특정멤버를 예외로 두고 싶을때 mutable 사용함
3. 코드 중복을 피하면서 const\_cast를 이용해 상수 버전을 재사용할 수 있음
4. 순서는 안전한 상수 함수를 먼저 짜고 비상수함수가 그걸 빌려씀


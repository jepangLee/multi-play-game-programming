# 멀티 플레이어 게임 프로그래밍

# 3. 버클리 소켓 (Berkeley Socket)

## 3. 소켓 주소
---
### 1. sockaddr

- 네트워크 계층에서 필요한 송수신지 주소, 포트 정보를 담은 struct이다.

        struct sockaddr {
            uint16_t sa_family;
            char     sa_data[14];
        };
    
- sa_family 
    - 주소의 종류를 나타내는 상숫값이다.
    
    - 소켓을 생성할 때 사용했던 af변수와 값이 일치해야 한다.

- sa_data
    - sa_family값에 따른 주소를 담는다.
    - 다양한 포맷의 주소를 담을 수 있어야 하므로 byte 배열로 되어 있다.

- 다양한 주소 패밀리의 메모리 레이아웃을 알아야한다는 단점이 있다.
    
    
### 2. sockaddr_in

- sockadd의 단점 보안하기 위해 널리 쓰이는 주소 패밀리인 IPv4에 딱 맞게 정의된 전용 자료형이다.


      struct sockaddr_in {
          short          sin_family;
          uint16_t       sin_port;
          struct in_addr sin_addr;
          char           sin_zero[8];
      };

  - sin_family : sa_family와 같은 기능을 한다.
  - sin_port : 포트값을 16bit로 나타낸다.
  - sin_addr 
    - 4byte의 IP주소를 나타낸다.
    - in_addr는 소켓 라이브러리마다 조금씩 다르다.
      - 단순 4byte도 있다.
      - union으로 struct를 감싸둔 struct로 값을 지정할 수 있게 해놓았다.
          
              struct in_addr 
              {
                  union 
                  {
                      struct 
                      {
                          uint8_t s_b1, s_b2, s_b3, s_b4;
                      } S_un_b;

                      struct 
                      {
                          uint16_t s_w1, s_w2;
                      } S_un_w;

                      uint32_t S_addr;
                  } S_un;
              };

          - S_un 유니온 중 S_un_b struct의 필드에 값을 넣으면 사람이 읽기 쉬운 형태로 주소를 지정할 수 있다.
  - sin_zero 
    - 사용하지 않지만 struct크기를 sockaddr과 맞추기 위한 패딩값이다.
    - 일관성 유지를 위해 값을 0으로 채워야 한다.

### 3. htons() & htonl()
   
- host byte 순서로 된 된 숫자를 network byte 순서 체계로 변환해 주는 함수다.

        uint16_t htons(uint16_t hostshort);
        uint32_t htonl(uint32_t hostlong);

- 각각 부호없는 16비트 정수, 부호없는 32비트 정수에 사용된다.
- host byte 순서와 network byte 순서가 같은 플렛폼에서는 아무런 동작을 하지 않는다.

### 4. ntohs() & ntohl()

- network byte 순서로 된 숫자를 host byte 순서 체계로 변환해 주는 함수다.

        uint16_t ntohs(uint16_t networkshort);
        uint32_t ntohl(uint32_t networklong);

예제 - sockaddr_in 구조체 초기화하기
- IP 주소 : 65.254.248.180
- port : 80

      sockaddr_in myAddr;
      memset(myAddr.sin_zero, 0, sizeof(myAddr.sin_zero));
      myAddr.sin_family = AF_INET;
      myAddr.sin_prot = htos(80);
      myAddr.sin_addr.S_un.S_un_b.s_b1 = 65;
      myAddr.sin_addr.S_un.S_un_b.s_b2 = 254;
      myAddr.sin_addr.S_un.S_un_b.s_b3 = 248;
      myAddr.sin_addr.S_un.S_un_b.s_b4 = 180;

  코드 3-1) sockaddr_in 구조체 초기화하기


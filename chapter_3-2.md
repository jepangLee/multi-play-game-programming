# 멀티 플레이어 게임 프로그래밍

# 3. 버클리 소켓 (Berkeley Socket)

## 3. 소켓 주소
---
### sockaddr

- 네트워크 계층에서 필요한 송수신지 주소, 포트 정보를 담은 struct이다.

        struct sockaddr {
            uint16_t sa_family;
            char     sa_data[14];
        };
    
- sa_family 
    - 주소의 종류를 나타내는 상숫값이다.
    
    - 소켓을 생성할 때 사용했던 af와 값이 일치해야 한다.

- sa_data
    - sa_family값에 따른 주소를 담는다.
    - 다양한 포맷의 주소를 담을 수 있어야 하므로 byte 배열로 되어 있다.

- 다양한 주소 패밀리의 메모리 레이아웃을 알아야한다는 단점이 있다.
    
    
### sockaddr_in
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

### htons() & htonl()
- host byte 순서로 된 된 숫자를 network byte 순서 체계로 변환해 주는 함수다.

        uint16_t htons(uint16_t hostshort);
        uint32_t htonl(uint32_t hostlong);

- 각각 부호없는 16비트 정수, 부호없는 32비트 정수에 사용된다.
- host byte 순서와 network byte 순서가 같은 플렛폼에서는 아무런 동작을 하지 않는다.

### ntohs() & ntohl()
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

### sockaddr 자료형 안정성 확보

    class SocketAddress
    {
    public:
        SocketAddress(uint32_t inAddress, uint16_t inPort)
        {
            this->GetAsSockAddrIn()->sin_family = AF_INET;
            this->GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
            this->GetAsSockAddrIn()->sin_port = htons(inPort);
        }
        SocketAddress(const sockaddr& inSockAddr)
        {
            memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
        }

        size_t GetSize() const { return sizeof(sockaddr); }

    private:
        sockaddr mSockAddr;
        sockaddr_in* GetAsSockAddrIn()
        {
            return reinterpret_cast<sockaddr_in*>(&mSockAddr);
        }
    };

    using SocketAddressPtr = shared_ptr<SocketAddress>;

    코드 3-2) 자료형 안정성이 확보된 SocketAddress 클래스

### inet_pton() & inetPton()
- 문자열을 sockaddr로 변환하여 넣는 함수이다.

- POSIX 계열은 inet_pton()를, 윈도우는 inetPton()를 사용한다.

        sockaddr_in myAddr;
        myAddr.sin_family = AF_INET;
        myAddr.sin_port = hton(80);
        InetPton(AF_INET, "65.254.248.180", &myAddr.sin_addr);
        //Inet_pton(AF_INET, "65.254.248.180", &myAddr.sin_addr);

    코드 3-3) InetPton(Inet_pton)으로 sockaddr 초기화하기
- Inet_pton()는 IP 주소 형태의 문자열만 처리할 수 있다. 즉, 도메인 네임이나 DNS 조회 등은 수행하지 않는다.

### getaddrinfo()
- POSIX 계열에서 DNS 질의를 수행해 도메인 네임을 IP 주소로 변환할때 사용한다. 

        int getaddrinfo(const char* hostname, const char* servname,
            const addrinfo* hints, addrinfo** res);

    - hostname : null 종료 문자열로 도메인 조회를 할 이름 문자열을 가리켜야 한다.

        ex) "live-shore.herokuapp.com"
    - servname : port 번호 또는 서비스 이름을 널 종료 문자열로 지정해야 한다.

        ex) http -> 80

    - hints 
        - 호출자가 어떤 정보를 받고 싶은지를 기재해 둔 addrinfo 구조체 포인터를 넘긴다.
        - 원하는 주소 패밀리나 다른 요구 사항을 지정해 호출할 수 있다.
        - 그냥 모든 결과를 받으려는 경우엔 nullptr를 넘기면 된다.

    - res 
        - 해당 변수로 지정된 포인터 주소로 결과가 반환된다.
        - 여러 개의 결과가 있을 수 있으므로 연결 리스트로 반환되고, 해당 변수는 그 첫째 원소가 된다.
        - DNS서버의 조회 결과가 담겨온다.

### addrinfo
- getaddinfo()함수의 결과값에 사용된다.

        struct addrinfo {
            int         ai_flags;
            int         ai_family;
            int         ai_socktype;
            int         ai_protocol;
            size_t      ai_addrlen;
            char*       ai_canon_name;
            sockaddr*   ai_addr;
            addinfo*    ai_next;
        }
            
    - ai_flags & ai_socktype & ai_protocol : hint에 요구 사항을 정의할 때 사용한다.
    
    - ai_family 
        - addrinfo에 관련된 주소 패밀리는 나타낸다.
        - IPv4 - AF_INET, IPv6 - AF_IENT6
    - ai_addrlen : ai_addr이 가리키는 sockaddr의 길이 값이다.
    - ai_caonon_name 
        - getaddrinfo()함수 호출할 때 hints의 ai_flags 필드에 AI_CANONNAME 플래그를 설정된 경우에만 사용된다.
        - resolve된 호스트명의 대표이름(canonical, CNAME)를 담는다.
    - ai_addr
        - 해당 주소 패밀리의 sockaddr을 담고 있다.
        - 이 주소는 getaddrinfo()호출 시 지정한 hostname과 servname, 즉 호스트명과 포트 조합이 가리키는 주소를 나타낸다.
    - ai_next 
        - 연결 리스트상 다음 addrinfo를 가리킨다.
        - 연결 리스트상 마지막 항목의 해당 변수는 nullptr가 되어 마지막임을 나타낸다.

### freeaddrinfo()
- getaddinfo()의 결과값으로 할당된 구조체의 메모리를 해제할때 사용된다.

        void freeaddrinfo(addinfo* ai);

    ai : getaddinfo()에서 받은 addrinfo 연결리스트 중 가장 첫번째 항목을 넘긴다.
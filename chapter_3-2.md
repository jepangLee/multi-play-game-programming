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
    
    - 소켓을 생성할 때 사용했던 af와 값이 일치해야 한다.

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

### inet_pton() & inetPton()

- 문자열을 sockaddr로 변환하여 넣는 함수이다.
- POSIX 계열은 inet_pton()를, 윈도우는 inetPton()를 사용한다.

        sockaddr_in myAddr;
        myAddr.sin_family = AF_INET;
        myAddr.sin_port = hton(80);
        InetPton(AF_INET, "65.254.248.180", &myAddr.sin_addr);
        //Inet_pton(AF_INET, "65.254.248.180", &myAddr.sin_addr);
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
        - 그냥 모든 결과글 받으려는 경우엔 nullptr를 넘기면 된다.

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
        - 이 주소는 getaddrinfo()호출 시 지정한 hostname과 servname, 즉 호슽트명과 포트 조합이 가리키는 주소를 나타낸다.
    - ai_next 
        - 연결 리스트상 다음 addrinfo를 가리킨다.
        - 연결 리스트상 마지막 항목의 해당 변수는 nullptr가 되어 마지막임을 나타낸다.

### freeaddrinfo()
- getaddinfo()의 결과값으로 할당된 구조체의 메모리를 해제할때 사용된다.

        void freeaddrinfo(addinfo* ai);

        ai : getaddinfo()에서 받은 addrinfo 연결리스트 중 가장 첫번째 항목을 넘긴다.

### GetaddrInfoEx()
- 호스트네임을 IP 주소를 resolve, 즉 해석하기 위해, getaddrinfo()함수는 운영체제에 설정된 대로 DNS 프로토콜 패킷을 만든 다음 UDP나 TCP로 DNS 서버에 보내게 된다.
- 이후 응답받기를 기다렸다가 파싱하여 addinfo 구조체의 연결리스트를 만들어 이것을 호출자에게 돌려준다.
- 이 과정에서 원격 호스트에 정보를 보내고 받는 단계가 포함되므로 시간이 많이 지체될 수 있다. 대개는 초단위의 지연이 수반될 수 있다.
- getaddrinfo()에는 비동기 동작을 하게끔 하는 옵션이 없으므로 호출 스레드는 응답을 받을 때 까지 마냥 블로킹되어 있어야 한다.
- 이는 사용자 입장에서 바람직하지 않으므로 호스트네임을 Ip 주소로 리졸브할 일이 있다면, getaddrinfo()가 메인 스레드를 붙잡고 있지 않도록 별도의 스레드에서 돌리는 방안을 생각해야 한다.
- 윈도우에서는 해당 함수를 사용하는데 이 함수에는 스레드를 따로 만들지 않아도 비동기식으로 동작하도록 하는 옵션이 있다.

### getaddrinfo()함수 캡슐화 하기

    class SocketAddressPactory {

    public:
        static SocketAddressPtr CreateIPv4FromString (const string& inString)
        {
            auto pos = inString.find_last_of(":");
            string host, service;
            if (string::npos != pos) {
                host = inString.substr(0, pos);
                service = inString.substr(pos + 1);
            }
            else {
                // 포트가 정해지지 않았음 -> default 사용
                host = inString;
                service = "0";
            }
            addrinfo hint;
            memset(&hint, 0, sizeof(hint));
            hint.ai_family = AF_INET;

            addrinfo* result = nullptr;
            int error = getaddrinfo(host.c_str(), service.c_str, &hint, &result);
            addrinfo* initResult = result;

            if (NULL != error && nullptr != result) {
                freeaddrinfo(initResult);
                return nullptr;
            }

            while (NULL == (result->ai_addr && result->ai_next) ) {
                result = result->ai_next;
            }

            if (NULL == result->ai_addr) {
                freeaddrinfo(initResult);
                return nullptr;
            }

            auto toRet = std::make_shared<SocketAddress>(*result->ai_addr);

            freeaddrinfo(initResult);
            return toRet;
        }
    };
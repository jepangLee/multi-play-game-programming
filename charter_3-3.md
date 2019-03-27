# 멀티 플레이어 게임 프로그래밍

# 3. 버클리 소켓

## 소켓 주소

### GetaddrInfoEx()
- 호스트네임을 IP 주소를 resolve, 즉 해석하기 위해, getaddrinfo()함수는 운영체제에 설정된 대로 DNS 프로토콜 패킷을 만든 다음 UDP나 TCP로 DNS 서버에 보내게 된다.

- 이후 응답받기를 기다렸다가 파싱하여 addinfo 구조체의 연결리스트를 만들어 이것을 호출자에게 돌려준다.
- 이 과정에서 원격 호스트에 정보를 보내고 받는 단계가 포함되므로 시간이 많이 지체될 수 있다. 대개는 초단위의 지연이 수반될 수 있다.
- getaddrinfo()에는 비동기 동작을 하게끔 하는 옵션이 없으므로 호출 스레드는 응답을 받을 때 까지 마냥 블로킹되어 있어야 한다.
- 이는 바람직하지 않으므로 호스트네임을 Ip 주소로 리졸브할 일이 있다면, getaddrinfo()가 메인 스레드를 붙잡고 있지 않도록 별도의 스레드에서 돌리는 방안을 생각해야 한다.
- 윈도우에서는 해당 함수를 사용하는데 이 함수에는 스레드를 따로 만들지 않아도 비동기식으로 동작하도록 하는 옵션이 있다. 따라서 윈도우에선 이 함수를 사용하자

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
    
코드 3-4) SocketAddressFactory를 써서 네임 resolution하기

- getaddrinfo()가 에러를 반환했거나 result값이 NULL이거나, result의 ai_addr의 값이 NULL인 경우 등에 대한 예외처리를 하여 확실히 유효한 SocketAddress객체만을 반환하게 만든 코드이다.

- 우선 inString에서 콜론( : )를 찾아 포트 번호를 분리한다.
- hint로 쓸 addrinfo 구조체를 만들어, IPv4 결과만 리턴되도록 한다.
- 이들 인자를 getaddrinfo()에 넘긴 뒤 리스트를 순회하여 NULL이 아닌 주소를 찾는다.
- 찾은 주소를 뽑아 SockAddress의 적당한 생성자로 객체를 생성한뒤 리스트는 헤제한다.

### bind()
- 운영체제에 어떤 소켓이 특정 주소와 전속 계층 포트를 쓰겠다고 알려주는 함수이다.

        int bind(SOCKET sock, const sockaddr* address, int address_len);

    - sock : binding할 소켓으로, 앞서 socket() 함수로 만든 것이다.
    - address 
        - 소켓을 바인딩할 주소이다.
        - 회신 주소를 밝혀 두는 정도의 용도이다.
        - 멀티플레이 게임용도로 보면 대게 네트워크 인터페이스가 어느 것인지는 대개 중요치 않고, 오히려에 장착된 모든 네트워크 인터페이스를 몽땅 바인딩하는 것이 바람직할 때가 많다. 
        - 이렇게 하려면 sockaddr_in의 sin_addr 필드에 INADDR_ANY 매크로 값을 넣어주면 된다.

    - adress_len : 주소로 넘긴 sockaddr의 길이를 넣어주어야 한다.
    - 반환값 
        - 성공 : 0 
        - 실패 : -1
- 일반적으로 주소 & 포트 하나당 하나의 소켓만 바인딩 할 수 있다. 이미 사용중인 주소 & 포트에 bind()함수를 쓰면 에러를 리턴할 것이다.
- 사용중이지 않은 포트를 찾기 위해 여러번 바인딩을 시도하는 것을 효율적으로 하려면 bind()함수 호출 시 포트를 0으로 지정하면 된다. 그러면 라이브러리가 알아서 골라 바인딩 해준다.

## UDP 소켓

- 만든 즉시 데이터를 보낼 수 있다.

- 바인딩하지 않은 상태라면 네트워크 모듈이 동적 포트 범위에 남아 있는 포트를 자동으로 찾아 바인딩해 준다.
### sendto()

- 데이터를 보낼때 사용한다.

        int sendto(SOCKET sock, const char* buf, int len, int flags, const sockaddr* to, int to len);

    - sock 
        - 데이터그램을 보낼 소켓이다.
        - 바인딩되지 않았다면 라이브러리가 자동으로 포트를 골라 바인딩해 준다.
        - 바인딩한 주소와 포트는 외부로 나가는 패킷 헤더의 발신자 주소가 된다.
        
    - buf
        - 보낼 데이터의 시작 주소를 가리키는 포인터이다. 
        - char*형으로 형변환이 가능하다면 어떤 데이터든 가능하다.
    - len
        - 데이터의 길이다.
        - 기술상으로는 8byte 헤더를 포함하는 65,535byte지만 MTU와 여러 계층의 헤더, 패킷 래퍼 등을 생각하면 1300byte 이내로 해야 분열을 피할 수 있다.
    - flags
        - 데이터 전송을 제ㅇ하는 비트 플래그이다.
        - 대부분의 게임 코드에선 0으로 둔다.
    - to 
        - 수신자의 목적지를 가리키는 sockaddr이다.
        - 해당 변수속 주소 패밀리와 소켓을 만들 때 지정한 주소 패밀리가 일치해야 한다.
    - tolen : sockaddr의 길이를 지정한다.
    - 반환값 
        - 성공 : 송신 대기열에 넣은 데이터의 길이
        - 실패 : -1

### recvfrom()

- UDP 소켓으로 보낸 데이터를 받는 함수이다.

        int recvfrom(SOCKET sock, char* buf, int len, int flags, sockaddr* from, int* fromlen);

    - sock 
        - 데이터를 받으려는 소켓이다.
        - 별다를 옵션을 설정하지 않은 경우와 더불어 소켓에 아직 수신된 데이터그램이 없으면 데이터그램을 수신할 때까지 기다린다.
    
    - buf
        - 수신한 데이터그램을 복사해 넣을 버퍼를 가리킨다.
        - 별도 플래그를 설정하지 않는 한, 데이터그램이 recvfrom() 호출을 총해 버퍼에 복사되고나뒤에 대하여 소켓 라이브러리는 그 사본을 따로 보관해 두지 않는다.
    - len
        - buf인자가 담을 수 있는 최대 바이트 길이를 지정한다.
        - 버퍼 오버플로 에러를 방지하기 위해 recvfrom()은 여기에 지정된 숫자 이상의 바이트는 복사하지 않는다.
    - flog
        - 데이터 수신을 제어하는 비트 플래그이다.
        - MSG_PEEK 
            - 이 값으로 지정되면 수신된 데이터그램을 buf에 복사한 다음 데이터그램을 입력 대기열에서 제거하지 않는다. 
        
              -> 다음번 recvfrom()호출에서 더 큰 버퍼를 할당해 같은 데이터그램을 다시 받아볼 수 있다.
        
        - 대부분의 게임코드에서 이 값은 0이면 충분하다.

    - from : 발신지의 주소와 포트를 채워줄 곳을 가리키는 sockaddr 구조체 포인터이다.
    - fromlen 
        - from의 길이를 반환해 줄 정수 포인터이다.
        - recvfrom()함수가 from을 채울 일이 없다면 fromlen 또한 채우지 않는다.
    - 반환값
        - 성공 : buf에 복사한 바이트의 길이
        - 실패 : -1

### UDP 소켓 자료형 안전성 보강

    class UDPSocket
    {
    public:
        ~UDPSocket();
        int Bind(const SocketAddress& inBindAddress);
        int SendTo(const void* inData, int inLen, const SocketAddress& inTo);
        int ReceiveFrom(void* inBuffer, int inLen, SocketAddress& ourFrom);

    private:
        friend class SocketUtil;
        UDPSocket(SOCKET inSocket) : mSocket(inSocket) {}
        SOCKET mSocket;
    };

    using UDPSocketPtr = shared_ptr<UDPSocket>;

    int UDPSocket::Bind(const SocketAddress & inBindAddress)
    {
        int err = bind(this->mSocket, &inBindAddress.mSockAddr ,inBindAddress.GetSize() );

        if (err == 0)
            return NO_ERROR;

        SocketUtil::ReportError(L"UDPSocket::bind");
        return SocketUtil::GetLastError();
    }

    int UDPSocket::SendTo(const void * inData, int inLen, const SocketAddress & inTo)
    {
        int byteSentCount = sendto(mSocket, static_cast<const char*>(inData),
                                    inLen, 0, &inTo.mSockAddr, inTo.GetSize());

        if (0 <= byteSentCount)
            return byteSentCount;

        //에러코드를 음수로 반환함
        SocketUtil::ReportError(L"UDPSocket::SendTO");
        return -SocketUtil::GetLastError();

    }

    int UDPSocket::ReceiveFrom(void * inBuffer, int inMaxLengh, SocketAddress & outFrom)
    {
        int fromLength = outFrom.GetSize();
        int readByteCount = recvfrom(mSocket, static_cast<char*>(inBuffer), 
                                      inMaxLengh, 0, &outFrom.mSockAddr, &fromLength);
        if (readByteCount >= 0)
            return readByteCount;

        SocketUtil::ReportError(L"UDPSocket::ReceiveFrom");
        return -SocketUtil::GetLastError();
    }

    UDPSocket::~UDPSocket()
    {
        closesocket(this->mSocket);
    }

코드 3-5) 자료형 안전성을 보강한 UDPSocket 클래스


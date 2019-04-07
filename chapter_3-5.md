# 멀티 플레이어 게임 프로그래밍

# 3. 버클리 소켓

## 블로킹 I/O와 논블로킹I/O

- 소켓 관련 함수는 대부분 받을 데이터가 없을 때 스레드가 블로킹되어 데이터가 수실될 때 까지 기다려야 하는 블로킹 호출이다.

- 하지만 메인 스레드에서 패킷을 처리하려 한다면 이는 바람직하지 않다.
- 이를 해결할 수 있는 방법은 크게 3가지다.
    - 멀티스레딩
    - 논블로킹 I/O
    - select()

### 멀티스레딩

- 블로킹 가능성이 있는 함수의 호출을 별도의 스레드에서 수행하는 방법이다.

- 예시

    ![멀티스레딩 절차](/image/picture_3-1.png)

    그림 3-1) 멀티스레딩 절차

    - 구동 직후 리스닝 스레드는 소켓으로 만들고 바인딩 한 다음, 리스닝을 걸고 accept()를 호출한다.

    - accept()는 클라이언트가 접속하기 전까지 블로킹 상태에 빠진다.
    - 클라이언트가 접속하면 accept() 함수는 새 소켓을 리턴하고 복귀한다.
    - 서버 프로세스는 이제 새 소켓으로 스레드를 하나 더 만드는데, 이 스레드는 반복해서 recv를 호출한다,
    - recv()함수는 클라이언트가 데이터를 보내기 전까지 블로킹 된다.
    - 클라이언트가 데이터를 보내면 recv가 블로킹 상태를 빠져나와 스레드 수행을 재개하며, 일종의 콜백 메커니즘으로 수신된 데이터를 메인 스레드에 보내준 다음 루프를 재개하면 recv)()를 또 호출한다.
    - 그동안 리스닝 소켓은 또 자신만의 블로킹 상태에서 다른 접속을 기다린다.
    - 이리하여 메인 스레드는 원활하게 게임 시뮬레이션을 수행할 수 있다.
- 장점
    - 블로킹으로 인한 딜레이를 어느정도 극복할 수 있다.

- 단점 
    - 클라이언트당 하나씩 스레드를 할당 해야 한다.
    
    - 데이터가 병렬적으로 한꺼번에 여러 스레드에 들어올 수 있어 메인 스레드로 전달하기 까다롭다.
    - 스레드가 데이터를 받는 도중 메인 스레드가 같은 소켓에 데이터를 보내려 하면 여전히 블로킹될 수 밖에 없다.

### 논블로킹 I/O

- 논블로킹 모드 동작시 블로킹을 거는 대신 즉시 리턴을 한다.

- 결과 값으로 errno에는 EAGAIN, WSAGetLastError()에서는 WSAEWOULDBLOOK이 된다.
- 원래는 블로킹되어 있어야 하지만 그러지 않고 빠져나왔다는 뜻이다.
- 매 프레임 새로운 데이터가 있는지 체크하는 방식이다.
- 장점
    - 간편하면서 스레드를 블로킹하지 않고 데이터가 수신된 것이 있는지 확인할 수 있어 직관적이다.

- 단점
    - 체크해야 할 소켓의 수가 상당히 많다면 효율이 떨어지게 된다.
    
#### ioctlsocket()

- 윈도우에서 논블로킹 모드 소켓을 만드는 함수다.

        int ioctlsocket(SOCKET sock, long cmd, u_long* argp);
    
    - sock : 논블로킹 모드로 두고싶은 소켓이다.
    - cmd : 제어하고자 하는 소켓 파라미터다. 여기선 FIONBIO를 지정한다.
    - argp
        - 파라미터에 설정하려는 값이다.
        - 0 : 블로킹 모드
        - 0이 아닌 값 : 논블로킹모드
    
#### fcntl()

- POSIX 호환 운영체제에서 논블로킹 모드 소켓을 만드는 함수다.

        int fcntl(int sock, int cmd, ...)
    
    - sock : 논블로킹 모드로 두고싶은 소켓이다.
    - cmd
        - 소켓에 내리려 하는 명령이다.
        - 구식 POSIX 시스템이 아니라면 먼저 F_GETFL로 소켓에 원래 저장된 플래그를 가져와서 거기에 O_NONBLOCK을 비트 OR한 뒤, 그 값을 다시 F_SETFL명령으로 소켓의 플래그를 덮어씌워야 한다.

#### 논블로킹 모드를 추가한 UDP 코드

    int UDPSocket::SetNonBlockingMode(bool inShouldBenonBlocking)
    {
    #if _WIN32
        u_long arg = inShouldBenonBlocking ? 1 : 0;
        int result = ioctlsocket(this->mSocket, FIONBIO, &arg);
    #else
        int flags = fcntl(mSocket, F_GETFL, 0);
        flags = inShouldBenonBlocking ?
            (flogs | O_NONBLOCK) : (flags & ~O_NONBLOCK);
        fcntl(this.mSocket, F_SETFL, flags);
    #endif

        if (result != SOCKET_ERROR)
            return NO_ERROR;

        SocketUtil::SU_ReportError(L"UDPSocket::SEtNonBlockingMode");
        return SocketUtil::SU_GetLastError();
    }

코드 3-7) UDPSocket 클래스에 논블로킹 모드 추가하기

#### 논블로킹 소켓을 사용한 게임 루프

    void DoUDPLoop() 
    {
        UDPSocketPtr mySock = SocketUtil::CreateUDPSocket(INET);
        mySock->SetNonBlockingMode(true);

        while (gIsGamerunning) {
            char data[1500];
            SocketAddress socketAddress;

            int bytesReceived = mySock->
                ReceiveFrom(data, sizeof(data), socketAddress);
            
            if (bytesReceived > 0)
                ProcessReceivedData(data, socketAddress);
        
            DoGameFrame();
        }
    }

코드 3-8) 논블로킹 소켓을 사용한 게임루프

### select() 함수

- 여러 소켓을 한꺼번에 확인하고 그중 하나라도 준비된면 즉시 대응하는 방법이다.

        int select(int nfds, fd_set* resadfds, fd_set* writefds, 
                fd_set* exceptfds, const timeval* timeout);

    - nfds
        - POSIX 호환 플렛폼
            - 소켓 식별자를 넣는데, 확인하려는 소켓중 최고 높은 숫자여야 한다.
            - POSIX에서 소켓 식별자는 그냥 정수에 불과하므로 함수에 넘기는 소켓의 값 중 최고값을 넘기면 된다.
        - 윈도우 : 소켓이 정수형이 아니라 포인터형이므로 해당 인자는 무시된다.

    - readfds 
        - 소켓 컬렉션을 가리키는 포인터이며 fd_set형이다.

        - 읽을 준비가 되었는지 확인하고 싶은 소켓을 넣는다.
        - nullptr을 넘기면 체크하지 않고 넘어간다.
        - readfds 집합에 포함된 소켓으로 패킷이 도착하면 해당 함수는 최대한 빨리 블로킹 상태에서 빠져나와 호출 스레드를 리턴한다.
        - 리턴 시점이 되어서도 패킷을 수신하지 못한 소켓은 전부 걸러진다.
        - readfds집합에 남은 소켓들은 데이터가 있는 상태이며, 여기서 데이터를 읽으면 블로킹되지 않고 읽을 수 있다.
        
    
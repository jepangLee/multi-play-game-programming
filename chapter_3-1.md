# 멀티 플레이어 게임 프로그래밍

## 3. 버클리 소켓 (Berkeley Socket)

- BSD 4.2에서 일부 배포되었지만, 프로세스와 TCP/IP 스택의 여러 계층 사이에 표준 인터페이스로 쓰기 위해 제공되었다.

- 배포 이후 여러 주요 운영체제 및 프로그래밍 언어로 포팅되어 사실상 네트워크 프록래밍의 표준으로 여겨지고 있다.

### 1. 소켓 만들기

- 프로세스는 실행 도중 하나 이상의 소켓을 만들고 초기화하여 소켓 API로 제어한다.
- 만든 소켓을 통해 데이터를 읽고 쓰게 된다.

1. 소켓 생성 함수
    
        SOCKET socket(int af, int type, int protocol)
        
   - af 
      - 주소 패밀리(address family)를 뜻함

      - 소켓에 사용할 네트워크 계층 프로토콜을 지정하는데 사용한다.
      
      - 사용값
  
        | 메크로 | 의미 |
        | - | - |
        | AF_UNSPEC | 지정하지 않음 |
        | AF_INET | IPv4 |
        | AF_INET6 | IPv6 |
        | AF_IPX | IPX<br />(Internetwork Packet Exchange :  MS-DOS와 노벨 때 사용하던 프로토콜) |
        | AF_APPLETALK | 애플토크(Appletalk : 구형 메킨토시에 사용하던 네트워크 스택) |

        표 3-1)socket함수 af에 넣을 수 있는 값

    - Type
      - 소켓을 주고받을 패킷의 종류를 지정하는데 사용한다.
      
      - 값에 따라 전송 계층 프로토콜이 패킷을 처리하는 방식 또한 달라진다.

      - 사용값

        | 메크로 | 의미 |
        | - | - |
        | SOCK_STREAM | 세그먼트(TCP)|
        | SOCK_DGRAM | 데이터그램(UDP) |
        | SOCK_RAW | 패킷 헤더를 응용 계층에서 직접 만들 수 있음 |
        | SOCK_SEQPACKET | SOCK_STEAM과 유사하나 패킷 수신 시 항상 전체를 읽어 들여야 함 |

    - protocol
      - 사용할 프로토콜의 종류를 명시하는데 사용한다.
      
      - 이 값은 IP헤더의 프로토콜 필드에 직접 기록된다.
      - 사용값

        | 메크로 | 프로 소켓 종류 | 의미 | 
        | - | - | - |
        | IPPROTO_UDP | STREAM_DGRAM  | UDP 데이터그램 패킷 |
        | IPPROTO_TCP | STREAM_STREAM | TCP 세그먼트 패킷   |
        | IPPROTO_IP 또는 0 | 상관없음 | 주어진 소켓 종류의 기본 프로토콜을 사용 |

      - IPPROTO_IP 또는 0로 지정하면 운영체제가 알아서 소켓 형식에 맞는 프로토콜을 골라준다.
      - ex) 

            SOCKET udpSocket = socket(AF_INET, SOCK_DREAM, 0); 
              -> UDP 소켓 생성
            SOCKET tcpSocket = socket(AF_INET, SOCK_STREAM, 0); 
              -> TCP 소켓 생성

2. 소켓 제거 함수

        SOCKET closesocket(SOCKET sock);

3. 소켓 송수신 중단함수

        int shutdown(SOCKET sock, int how);
    - TCP 소켓을 해제하는 경우 송수신중인 데이터를 모두 완료하고 확인응답까지 마친 상태에서 끝내야 한다.

    - 해당 함수는 소켓을 닫기 전 송수신을 중단할 때 사용된다.
    - how
      - 사용값

        | 메크로 | 의미 |
        | - | - |
        | SD_SEND | 송신 중단 |
        | SD_RECEIVE | 수신 중단 |
        | SD_BOTH | 송수신 중단 |

      - SD_SEND를 지정하면 현재 전송 중인 데이터를 모두 전송한 뒤 FIN패킷을 보내여 연결을 안전하게 닫고자 하는 것을 알려준다.
      - 수신측에서 FIN 패킷에 응답하면 소켓을 닫아도 안전한 것이다.

### 2. 운영체제별 버클리 소켓 API차이

1. SOCKET형의 차이
   - 윈도우, XBox : UINT_PTR에 대한 typedef

   - POSIX기반 리눅스, macOS, PlayStation : int
       - 취약점 : 단순 정수를 넣어도 컴파일러는 알지 못한다.
       
2. 라이브러리를 사용하기 위해 사용되는 헤더파일
   - 윈도우
       - WinSock2.h : 소켓 자체에 관련된 함수와 자료형만 선언된 헤더이다.

       - WinSock.h : WinSock2.h의 구형버전, 기능부족과 최적화 문제가 있다.
       - windows.h : 소켓 이외에도 윈도우와 관련된 많은 것들이 담긴 헤더이다.
         - 해당 헤더에 WinSock.h와 WinSock2.h가 같이 선언되어 있어 충돌되기 쉽다.
         - 이를 위해WinSock2.h를 해당 헤더 위에 include하고, 이에 앞서 WIN32_AND_MEAN 매크로를 #define 한다.
       - Ws2tcpip : 주소 변환 기능이 포함된 헤더이다.
     - POSIX 기반 플렛폼
       - sys/socket.h : 단 하나있는 소켓 라이브러리다.
       
       - netinet/in.h : IPv4 전용 기능을 위한 헤더이다.
       - netdb.h : 네임 리솔루션(name resolution)을 위한 헤더이다.

3. 소켓 초기화와 마무리 방법
    - 윈도우
      - Winsock2는 명시적으로 초기화와 마무리를 해줘야 한다.
      
      - 이때 어느버전의 라이브러리를 사용할지도 지정해야 한다.
      - 소켓 라이브러리 활성화 함수

            int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);

          - wVersionRequested
            - 2Byte 워드이다.
            - 하위Byte : 주 버전 번호
            - 사위Byte : 부 버전 번호
            - MAKEWORD( , );로 인자 값을 넘겨주면 된다.
          
          - lpWSAData
            - 윈도우 전용 struct로 해당 함수가 활성화된 라이브러리에 대한 정보로 값을 채워준다.
      - 소켓 라이브러리 비활성화 함수

            int WSACleanup();

        - 파라미터는 없고 return값은 에러 코드이다.
        - 해당 함수가 호출되면 진행 중이던 모든 소켓이 강제 종료되고 소켓 리소스도 모두 소멸된다.
        - WSAStartup()는 레퍼런스 카운트되므로 WSAStartup()를 호출한 만큼 해당 함수를 호출해야 실제 마무리 작업이 일어난다.
    - POSIX 기반 플렛폼
      - 항상 황설화상태이기에 별다른 작업이 없다.

4. 에러 통보 방식
    - 대부분의 플렛폼 : -1
    
    - 윈도우 
      - 매크로 SOCKET_ERROR
      - 에러 원인을 알려주는 함수

            int WSAGetLastError();

        - 현재 실행 중인 스레드에서 마지막으로 발생한 에러 코드만 저장해두므로, 소켓 라이브러리 함수의 결과로 -1을 받으면 즉시 확인해야 한다. 
   - POSIX기반 플렛폼
         - -1
         - errno 
           - 에러 정보 조회용 전역변수이다.
           - 사용하려면 errno.h를 include해야 한다.
           - 해당 변수 또한 WSAGetLastError()처럼 덮어 쓰여질 수 있으므로 에러 감지 시점에 즉시 확인해야 한다.

### 3. 소켓 주소

1. sockaddr

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
    
    
2. sockaddr_in

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
            
                struct in_addr {
                    union {
                        struct {
                            uint8_t s_b1, s_b2, s_b3, s_b4;
                        } S_un_b;
                        struct {
                            uint16_t s_w1, s_w2;
                        } S_un_w;
                        uint32_t S_addr;
                    } S_un;
                };

            - S_un 유니온 중 S_un_b struct의 필드에 값을 넣으면 사람이 읽기 쉬운 형태로 주소를 지정할 수 있다.
    - sin_zero 
      - 사용하지 않지만 struct크기를 sockaddr과 맞추기 위한 패딩값이다.
      - 일관성 유지를 위해 값을 0으로 채워야 한다.

3. htons() & htonl()
   
- 호스트 순서로 된 된 숫자를 네트워크 순서 체계로 변환해 주는 함수다.

        uint16_t htons(uint16_t hostshort);
        uint32_t htonl(uint32_t hostlong);

    - 각각 부호없는 16비트 정수, 부호없는 32비트 정수에 사용된다.
    - 호스트 순서와 네트워크 순서가 같은 플렛폼에서는 아무런 동작을 하지 않는다.
    - 플렛폼에 따라서 바이트 순서가 뒤집힌 경우도 있기에 혼동하지 말자.  
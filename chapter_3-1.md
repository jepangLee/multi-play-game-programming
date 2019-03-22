# 멀티 플레이어 게임 프로그래밍

# 3. 버클리 소켓 (Berkeley Socket)

- BSD 4.2에서 일부 배포되었지만, 프로세스와 TCP/IP 스택의 여러 계층 사이에 표준 인터페이스로 쓰기 위해 제공되었다.

- 배포 이후 여러 주요 운영체제 및 프로그래밍 언어로 포팅되어 사실상 네트워크 프록래밍의 표준으로 여겨지고 있다.

## 1. 소켓 만들기
---
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

## 2. 운영체제별 버클리 소켓 API차이
---
### 1. SOCKET형의 차이
  - 윈도우, XBox : UINT_PTR에 대한 typedef

   - POSIX기반 리눅스, macOS, PlayStation : int
       - 취약점 : 단순 정수를 넣어도 컴파일러는 알지 못한다.
       
### 2. 라이브러리를 사용하기 위해 사용되는 헤더파일
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

### 3. 소켓 초기화와 마무리 방법
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

### 4. 에러 통보 방식
  - 대부분의 플렛폼 : -1
  
  - 윈도우 
    - SOCKET_ERROR
    - 에러 원인을 알려주는 함수

          int WSAGetLastError();

      - 현재 실행 중인 스레드에서 마지막으로 발생한 에러 코드만 저장해두므로, 소켓 라이브러리 함수의 결과로 -1을 받으면 즉시 확인해야 한다. 
 - POSIX기반 플렛폼
      - -1
       - errno 
         - 에러 정보 조회용 전역변수이다.
         - 사용하려면 errno.h를 include해야 한다.
         - 해당 변수 또한 WSAGetLastError()처럼 덮어 쓰여질 수 있으므로 에러 감지 시점에 즉시 확인해야 한다.
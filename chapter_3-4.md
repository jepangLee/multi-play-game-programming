# 멀티 플레이어 게임 프로그래밍

# 3. 버클리 소켓

## TCP 소켓

- UDP와는 달리 신뢰성을 보장하며, 연결을 맺은 상태를 유지해야 한다. 즉, 호스트가 각 TCP 연결마다 별개의 독자적 소켓을 하나씩 유지해야 한다.

### listen()

    int listen(SOCKET sock, int backlog);

- 
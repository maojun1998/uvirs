#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib,"WS2_32.LIB")
int main()
{
    
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    SOCKET sockClient;
    SOCKADDR_IN addrSrv;
    char receBuf[100];
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        return FALSE;
    }
    if (LOBYTE( wsaData.wVersion ) != 1 || HIBYTE( wsaData.wVersion ) != 1) {
        WSACleanup();
        return FALSE;
    }

    sockClient = socket(AF_INET,SOCK_STREAM,0);
    addrSrv.sin_addr.S_un.S_addr = inet_addr("59.110.225.100");
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(6000);

    connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

    send(sockClient, "q", strlen("q") + 1 ,0);
    
    recv(sockClient, receBuf, 100, 0);
    
    printf("%s\n", receBuf);
    
    
    closesocket(sockClient);
    WSACleanup();
    return 1;
}

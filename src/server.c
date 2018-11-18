#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib,"WS2_32.LIB")
void main()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    char receBuf[100];
    unsigned int InfectionNo;
    SOCKADDR_IN addrClient;
    SOCKET sockSrv,sockConn;
    SOCKADDR_IN addrSrv;
    int len;
    InfectionNo = 0;
    wVersionRequested = MAKEWORD( 1, 1 );
    err = WSAStartup( wVersionRequested, &wsaData );

    if (err != 0) {
        return;
    }
    if (LOBYTE(wsaData.wVersion ) != 1 || HIBYTE( wsaData.wVersion) != 1) {
        WSACleanup( );
        return;
    }

    sockSrv = socket(AF_INET,SOCK_STREAM,0); 
    
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); 
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(6000); 

    bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

    listen(sockSrv,20);
    
    len = sizeof(SOCKADDR);
    
    while (1) {
        sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&len);

        recv(sockConn,receBuf,100,0);
        if (receBuf[0] == 'a') {
            printf("Infection add one\n");
            InfectionNo++;
        } else if (receBuf[0] == 'q') {
            sprintf(receBuf ,"Infection Driver Device No : %u", InfectionNo);
            printf("Some one Query\n");
            send(sockConn, receBuf, strlen(receBuf) + 1, 0);
        }
        
        receBuf[0] = 0;
        closesocket(sockConn);
    }
}
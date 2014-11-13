#include "../PrecompiledHeader.h"

NetworkInterface* newNetworkInterfaceForServer() {

    printf("NetworkInterface::newNetworkInterfaceForServer\n");

    NetworkInterface *networkInterfaceForServer = (NetworkInterface *)malloc(sizeof(NetworkInterface));
    networkInterfaceForServer->clientSocketFd = 0;
    networkInterfaceForServer->serverSocketFd = 0;
    networkInterfaceForServer->connectedClientSocketFd = 0;

    //printf("before serverSocketFd = %d\n", networkInterfaceForServer->serverSocketFd);

    // 왜 AccountSystem의 NetworkInputInteface와 BusControl의 NetworkInputInterface의 serverSocketFd에 똑같은 값이 할당되는가?
    networkInterfaceForServer->serverSocketFd = socket(PF_INET, SOCK_STREAM, 0);

    //printf("after serverSocketFd = %d\n", networkInterfaceForServer->serverSocketFd);

    memset(&(networkInterfaceForServer->serverAddr), 0, sizeof(networkInterfaceForServer->serverAddr));
    memset(&(networkInterfaceForServer->connectedClientAddr), 0, sizeof(networkInterfaceForServer->connectedClientAddr));
    networkInterfaceForServer->serverAddr.sin_family = AF_INET;
    networkInterfaceForServer->serverAddr.sin_addr.s_addr = INADDR_ANY;
    networkInterfaceForServer->serverAddr.sin_port = htons(PORTNUMBER);

    if (bind(networkInterfaceForServer->serverSocketFd, (struct sockaddr *) &(networkInterfaceForServer->serverAddr), sizeof(networkInterfaceForServer->serverAddr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }


    // 함수포인터 세팅
    networkInterfaceForServer->listenTerminal = &waitData;
    networkInterfaceForServer->writeDailyAccountInformation = &writeDailyAccountInformation;
    networkInterfaceForServer->sendData = &sendData;
    networkInterfaceForServer->isServer = &isServer;

    return networkInterfaceForServer;
}


NetworkInterface* newNetworkInterfaceForClient() {

    printf("NetworkInterface::newNetworkInterfaceForClient\n");

    NetworkInterface *networkInterfaceForClient = (NetworkInterface *)malloc(sizeof(NetworkInterface));
    networkInterfaceForClient->clientSocketFd = 0;
    networkInterfaceForClient->serverSocketFd = 0;
    networkInterfaceForClient->connectedClientSocketFd = 0;

    //printf("before clientSocketFd = %d\n", networkInterfaceForClient->clientSocketFd);

    networkInterfaceForClient->clientSocketFd = socket(PF_INET, SOCK_STREAM, 0);

    //printf("after clientSocketFd = %d\n", networkInterfaceForClient->clientSocketFd);

    memset(&(networkInterfaceForClient->clientToServerAddr), 0, sizeof(networkInterfaceForClient->clientToServerAddr));
    networkInterfaceForClient->clientToServerAddr.sin_family = AF_INET;
    networkInterfaceForClient->clientToServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    networkInterfaceForClient->clientToServerAddr.sin_port = htons(PORTNUMBER);


    // 함수포인터 세팅
    networkInterfaceForClient->listenTerminal = &waitData;
    networkInterfaceForClient->writeDailyAccountInformation = &writeDailyAccountInformation;
    networkInterfaceForClient->sendData = &sendData;
    networkInterfaceForClient->isServer = &isServer;

    return networkInterfaceForClient;
}

void waitData(NetworkInterface *self) {
    printf("NetworkInterface::waitData\n");

    if(self->isServer(self)) {

        listen(self->serverSocketFd, 5);
        self->terminalAddrLength = sizeof(self->connectedClientAddr);

        self->connectedClientSocketFd = accept(self->serverSocketFd, (struct sockaddr *)&(self->connectedClientAddr), &(self->terminalAddrLength));

        //이 아래에서 write

        if(self->connectedClientSocketFd < 0) {
            perror("accept error");
        }

        recv(self->connectedClientSocketFd, &(self->terminalType), sizeof(int), 0);
        printf("received data(server) : %d\n", self->terminalType);
    } else {
        recv(self->clientSocketFd, &(self->terminalType), sizeof(int), 0);
        printf("received data(client) : %d\n", self->terminalType);
    }

}

DailyAccountInformation writeDailyAccountInformation(NetworkInterface* self) {
    printf("NetworkInterface::writeDailyAccountInformation\n");
}

void sendData(NetworkInterface* self, int data) {

    printf("NetworkInterface::sendData\n");
    printf("sended data : [%d]\n", data);

    if(self->isServer(self)) {
        send(self->connectedClientSocketFd, &data, sizeof(int), 0);
    } else {
        if(connect(self->clientSocketFd, (struct sockaddr*)&(self->clientToServerAddr), sizeof(self->clientToServerAddr))) {
            perror("connect error\n");
        }

        send(self->clientSocketFd, &data, sizeof(int), 0);
    }

}

bool isServer(NetworkInterface* self) {

    printf("NetworkInterface::isServer\n");

    if(self->clientSocketFd == 0) {
        return true;
    } else {
        return false;
    }

}
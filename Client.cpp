#define WIN32_LEAN_AND_MEAN //какой-то макрос

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
using namespace std;
int main()
{
    WSADATA wsaData;//переменная, которая хранит версию, структуры о реализации Windows Sockets
    SOCKET ConnectSocket = INVALID_SOCKET;
    ADDRINFO hints;
    ADDRINFO* addrResult;
    const char* sendBuffer = "Hello from Client";
    const char* VtoroiBuffer = "Eto tresh";
    char recvBuffer[512];


    //процесс определения версии и структуры
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cout << "WSAStartup failed result" << endl;
        return 1;
    }


    ZeroMemory(&hints, sizeof(hints));//зануление памяти
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;


    //функция хранит в себе адрес, порт
    result = getaddrinfo("localhost", "666", &hints, &addrResult);
    if (result != 0) {
        cout << "getaddrinfo failed result" << endl;
        WSACleanup();
        return 1;
    }
    ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) {
        cout << "socket error" << endl;
        WSACleanup();
        freeaddrinfo(addrResult);
        return 1;
    }
    result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);

    if (result == SOCKET_ERROR) {
        cout << "connection error to server" << endl;
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);

    if (result == SOCKET_ERROR) {
        cout << "send failed error" << endl;
        WSACleanup();
        return 1;
    }
    cout << "Sent" << result << "bytes" << endl;

    result = send(ConnectSocket, VtoroiBuffer, (int)strlen(sendBuffer), 0);
    if (result == SOCKET_ERROR) {
        cout << "send failed error" << endl;
        WSACleanup();
        return 1;
    }
    cout << "Sent" << result << "bytes" << endl;
    //sd-send - прием можно, передача нет
    result = shutdown(ConnectSocket, SD_SEND);


    if (result == SOCKET_ERROR) {
        cout << "shtdown error" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    do {
        ZeroMemory(recvBuffer, 512);
        result = recv(ConnectSocket, recvBuffer, 512, 0);
        if (result > 0) {
            cout << "Received " << result << "bytes" << endl;
            cout << "Received data " << recvBuffer << endl;

        }
        else if (result == 0)
            cout << "Connection closed" << endl;

        else
            cout << "recv failed with error" << endl;


    } while (result > 0);
    freeaddrinfo(addrResult);//хакрываем сокет 
    WSACleanup();//доиницилизируем библиотеку
    return 0;

}


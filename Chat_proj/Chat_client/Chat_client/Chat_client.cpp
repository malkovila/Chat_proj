#include <iostream>
#include <thread>
#include <string>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>   
#pragma warning(disable:4996)


using namespace std;


SOCKET Connection;
char n[20];
char other_n[20];
char whom_to_send[20];


void clientHandler() {
    while (true) {
        string s;
        std::getline(cin, s);
        std::getline(cin, s);
        cout << "Whom to send: ";
        cin >> whom_to_send;
        int size_m = s.size();

        send(Connection, whom_to_send, 20, NULL);
        send(Connection, n, 20, NULL);
        send(Connection, (char*)&size_m, sizeof(int), NULL);
        send(Connection, s.c_str(), size_m, NULL);
        Sleep(10);
    }
}

int main()
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        cout << "Не удалось установить библиотеку";
        exit(1);
    } //устанавливаем библиотеку

    SOCKADDR_IN addr; //инфа о адресе сокета
    int size_n = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //ip
    addr.sin_family = AF_INET; //семейство для интернет протокола
    addr.sin_port = htons(1111); //порт
    Connection = socket(AF_INET, SOCK_STREAM, NULL);
    cout << "Enter name: ";
    cin >> n;
    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) { //устанавливаем соединение
        cout << "Error! Failed connection to server";
        return 1;
    }
    cout << n <<" connected\n";
    send(Connection, n, 20, NULL);
    int msg_size;
    
    bool f = false;
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientHandler, (LPVOID)(Connection), NULL, NULL);
    while (true) {
        recv(Connection, other_n, 20, NULL);
        recv(Connection, (char*)&msg_size, sizeof(int), NULL);
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(Connection, msg, msg_size, NULL);
        
        cout << other_n << ": " << msg << "\n";
        
        f = true;
        delete[] msg;
       
    }

    system("pause");
    return 0;
}
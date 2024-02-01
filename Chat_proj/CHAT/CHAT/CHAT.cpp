#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#include <vector>
#include <map>

#include <winsock2.h>   
#include <thread>

using namespace std;

void set_config_addr(SOCKADDR_IN& addr) {
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //ip
    addr.sin_family = AF_INET; //семейство для интернет протокола
    addr.sin_port = htons(1111); //порт
}

vector<SOCKET> Connections;
map<string, SOCKET> Con;
int c = 0;

void clientHandler(int n) {
    
    while (true) {
        int msg_size;
        char name[20];
        char whom_to_send[20];

        recv(Connections[n], whom_to_send, 20, NULL);
        recv(Connections[n], name, 20, NULL);
        recv(Connections[n], (char*)&msg_size, sizeof(int), NULL);
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(Connections[n], msg, msg_size, NULL);
        if (Con.find(whom_to_send) != Con.end()) {
            send(Con[whom_to_send], name, 20, NULL);
            send(Con[whom_to_send], (char*)&msg_size, sizeof(int), NULL);
            send(Con[whom_to_send], msg, msg_size, NULL);
        }
        delete[] msg;
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
    set_config_addr(addr);

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //сокет для прослушивания
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //привязывает адрес сокета для прослушки
    listen(sListen, SOMAXCONN);//запускает прослушивание

    SOCKET newConnection; //сокет для удержания соединения с клиентом
    for (int i = 0; i < 100; i++)
    {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &size_n);  //после выполнения addr будет содержать сведения о клиенте, который
        //выполнил подключение

        if (newConnection == 0) {
            cout << "Error #2";
        }
        else {
            char nn[256];

            recv(newConnection, nn, 20, NULL);
            Con[nn] = newConnection;
            cout << "Client connected\n";
            char msg[256];
            Connections.push_back(newConnection);

            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientHandler, (LPVOID)(i), NULL, NULL);
            //cin >> msg;
            //send(newConnection, msg, sizeof(msg), NULL); //отправляет инфу на сервер
           
        }
    }
    system("pause");

}

#pragma once
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;
 
#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7777 // Будем использовать этот номер порта

struct TCP_client
{
    int socket_file_descriptor;
    int connection;
    struct sockaddr_in serveraddress;
    struct sockaddr_in client;
    char message[MESSAGE_LENGTH];

    void client_start(string &sender, string &to, string &text) {
        // Создадим сокет
        socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        if(socket_file_descriptor == -1){
            cout << "Creation of Socket failed!" << endl;                       //Не удалось создать сокет
        return;
        }
        
        // Установим адрес сервера
        serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
        // Зададим номер порта
        serveraddress.sin_port = htons(PORT);
        // Используем IPv4
        serveraddress.sin_family = AF_INET;
        // Установим соединение с сервером
        connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
        if(connection == -1){
            cout << "Connection with the server failed!" << endl;               //Не удалось установить соединение с сервером
            return;
        }
        
        // Взаимодействие с сервером
        while(1){
            bzero(message, sizeof(message));
            //сборка в единый пакет: отправител + получатель + сообщение
            {
                int i = 0;
                int size_message = (    sender.length() + to.length() + text.length() + 2);
                for (int j = 0; j < sender.length(); i++) {
                    message[i] = sender[j];
                    ++j;
                }
                message[i] = ' ';
                ++i;
                for (int j = 0; j < to.length(); i++) {
                    message[i] = to[j];
                    ++j;
                }
                message[i] = ' ';
                ++i;
                for (int j = 0; j < text.length(); i++) {
                    message[i] = text[j];
                    ++j;
                }
                //cout << "Enter the message you want to send to the server: " << endl;
                //cin >> message;
                if ((strncmp(message, "end", 3)) == 0) {
                    write(socket_file_descriptor, message, sizeof(message));
                    cout << "Client Exit." << endl;
                    break;
                }
            }
for(int i=0;i<1024;++i) {cout<<message[i];} cout<<"\n";
            ssize_t bytes = write(socket_file_descriptor, message, sizeof(message));
            // Если передали >= 0  байт, значит пересылка прошла успешно
            if(bytes >= 0){
                cout << "message send!" << endl;
            }
            break;
            //bzero(message, sizeof(message));
            // Ждем ответа от сервера
            //read(socket_file_descriptor, message, sizeof(message));
            //cout << "Data received from server: " << message << endl;
        }
        
        // закрываем сокет, завершаем соединение
        close(socket_file_descriptor);
    }
};

#pragma once
#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include "Message.h"

using namespace std;

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7777 // Будем использовать этот номер порта

struct TCP_server
{
    struct sockaddr_in serveraddress;
    struct sockaddr_in client;
    socklen_t length;
    int sockert_file_descriptor;
    int connection;
    int bind_status;
    int connection_status;
    char message[MESSAGE_LENGTH];

    
    void buffering_messages() {
        
        fstream messages_file("buffer_messahes.dat", ios::app);//ios::in | ios::out);
	    if (!messages_file) {
		    // Для создания файла используем параметр ios::trunc
		    messages_file = fstream("buffer_messahes.dat", ios::in | ios::out | ios::trunc);
    	}

	    if (messages_file) {
	        
		    for(int i = 0; i < MESSAGE_LENGTH; ++i) {
                if (message[i] == '\0') {
                    break;
                }
                    messages_file << message[i];
		    }
		    
		    messages_file << "\n";
		    messages_file.close();
	    }
    }
    
    void server_start() {
        sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);          //AF_INET — константа, отвечающая за то, что устройство использует глобальную сеть по протоколу IPv4
        if(sockert_file_descriptor == -1){
            cout << "Socket creation failed!" << endl;
            return;
        }
        //
        serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
        // Зададим номер порта для связи
        serveraddress.sin_port = htons(PORT);
        // Используем IPv4
        serveraddress.sin_family = AF_INET;
        // Привяжем сокет
        bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
        if(bind_status == -1)  {
            cout << "Socket binding failed!" << endl;                           //Не удалось выполнить привязку сокета
            return;
        }
        // Поставим сервер на прием данных
        connection_status = listen(sockert_file_descriptor, 5);
        if(connection_status == -1){
            cout << "Socket is unable to listen for new connections!" << endl;  //Сокет не может прослушивать новые подключения!
            return;
        } 
        else {
            cout << "Server run ... ok" << endl;         //Сервер прослушивает новое соединение:
        }

        length = sizeof(client);
        connection = accept(sockert_file_descriptor,(struct sockaddr*)&client, &length);
        if(connection == -1)  {
            cout << "Server is unable to accept the data from client!" << endl; //Сервер не может принять данные от клиента!
            return;
        }
        
        //cout << "Server RUN\n";
        // Communication Establishment                                          // Установление связи
        while(1){
            bzero(message, MESSAGE_LENGTH);
            read(connection, message, sizeof(message));
            if (strncmp("end", message, 3) == 0) {
                cout << "Client Exited." << endl;
                cout << "Server is Exiting!" << endl;
                break;
            }
            //cout << "Message from client: " <<  message << endl;
            ///*if (strncmp("", message, 1) != 0) {
            if (message[0] != '\0') {
                buffering_messages();
            }
            //ssize_t bytes = write(connection, message, sizeof(message));
            //Если передали >= 0  байт, значит пересылка прошла успешно
            //if(bytes >= 0)  {
            //    cout << "Data successfully sent to the client!" << endl;
            //}
            sleep(1);
        }
        
        // закрываем сокет, завершаем соединение
        close(sockert_file_descriptor);
    }
};

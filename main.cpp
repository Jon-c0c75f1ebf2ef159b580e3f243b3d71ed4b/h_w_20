#include "TCP_server.h"
#include "Chat.h"

int main()
{
	int pid = fork(); // в этом месте произойдет раздвоение процесса
	if (pid == 0) {                 //Child 
        TCP_server run_server;
        run_server.server_start();
    }
    
    else if (pid > 0) {             // Parent 
       sleep(1);    //для синхронизации вывода системной информации о запуске сервера
       
       Chat chat;
       chat.startChat();
       
       while (chat.work()) 
       {
		    chat.showLoginMenu();
		    while (chat.getcurrentUser())
		    {
		        chat.showUserMenu();
		    }
       }
    }
    
	return 0;
}

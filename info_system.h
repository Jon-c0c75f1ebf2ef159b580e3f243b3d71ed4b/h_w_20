#pragma once
#include <iostream>

#if defined(__linux__)
#include <sys/utsname.h> // Linux for uname()
#elif defined(_WIN32) || defined(_WIN64)
//#include //sistem book in Windows                           //////////////////  !
#endif

#define cout std::cout
#define endl std::endl

class Info_system
{
public:
    void something() {
#if defined(__linux__)
        //for Linux
        //system("uname -a");
        struct utsname uts_name;
        // uname() - the utility provides the name of the kernel and information about it
        uname(&uts_name);
        // let's print information about the operating system on the screen
        cout << "OS name: " << uts_name.sysname << endl;
        cout << "Host name: " << uts_name.nodename << endl;
        cout << "OS release: " << uts_name.release << endl;
        cout << "OS version: " << uts_name.version << endl;
        cout << "Machine name: " << uts_name.machine << endl;

#elif defined(_WIN32) || defined(_WIN64)
        //for Windows
        cout << "OS Windows" << endl;

#else
        cout << "System information is not available\n" << endl;
#endif
        cout << endl;
    }
};

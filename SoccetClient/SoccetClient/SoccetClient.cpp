#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>


HANDLE conin, conout;
std::mutex mtx;
SOCKET Connection;
std::mutex cat;

int output(std::string msg)
{

	mtx.lock();
	std::cout << "srv: " << msg << std::endl;
	mtx.unlock();
	return 0;
}

void CO()
{
	std::thread newth;
	while (true)
	{
		char msg[256];
		
		recv(Connection, msg, sizeof(msg), NULL);
		std::string str = msg;
		newth = std::thread(output, str);
		newth.detach();
		
		//cout << "try" << i << "times" << endl;
		/*TH = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)output, (void*)i, 0, NULL);*/
		//newth = std::thread(output, msg);
		//newth.detach();
		//cout << "notfailed" << endl;

		//Sleep(1000);

	}
}

int getakey()
{
	INPUT_RECORD input;
	DWORD res;
	while (true) {
		ReadConsoleInput(conin, &input, 1, &res);

		// игнорировать другие события
		if (input.EventType != KEY_EVENT)
			continue;

		// игнорировать события отпускания клавиш 
		// нас интересуют только нажатия
		if (!input.Event.KeyEvent.bKeyDown)
			continue;

		return input.Event.KeyEvent.wVirtualKeyCode;
	}
}

void CI()
{
	char msg[256];
	//std::string msg;
	while (true)
	{	
		getakey();
		mtx.lock();
		std::cout << "Enter massage: ";
		std::string str;
		getline(std::cin, str);
		std::cout << "me:" << str << std::endl;
		send(Connection, str.c_str(), sizeof(msg), NULL);
		mtx.unlock();
	}
}

#pragma warning(disable: 4996)




int main(int argc, char* argv[]) {
	char* name = new char[0];
	std::cout << "What's your Name?";
	std::string str;
	std::getline(std::cin, str);

	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("26.95.165.151");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return 1;
	}
	std::cout << "Connected!\n";
	char msg[256];
	send(Connection, str.c_str(), sizeof(msg), NULL);
	
	conin = GetStdHandle(STD_INPUT_HANDLE);
	std::thread co, ci;
	co = std::thread(CO);
	ci = std::thread(CI);

	co.join();
	system("pause");
	return 0;
}
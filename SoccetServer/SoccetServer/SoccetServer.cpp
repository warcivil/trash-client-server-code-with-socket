#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <thread>

#pragma warning(disable: 4996)

//
std::vector<SOCKET> connections;
std::vector<std::string> names;

void retranslate(int i)
{
	char name[256];
	char msg[256] = " enter to the Club";
	char mmsg[256] = ": ";
	recv(connections[i], name, sizeof(name), NULL);
	std::cout << name << " Connected!\n";
	
	//names.push_back({ name });
	std::cout << connections.size();
	for (int j = 0;j < connections.size() ;j++)
	{
		send(connections[j], name, sizeof(name), NULL);
		send(connections[j], msg, sizeof(msg), NULL);
	}
	while (true)
	{
		recv(connections[i], msg, sizeof(msg), NULL);
		for (int j = 0;j < connections.size();j++)
		{
			std::cout << "j:" << j << " i:" << i <<  (j!=i) << std::endl;
			if (j != i)
			{
				//send(connections[j], names[i].c_str(), sizeof(msg), NULL);
				//send(connections[j], mmsg, sizeof(msg), NULL);
				send(connections[j], msg, sizeof(msg), NULL);
			}
		}
	}
}

int main(int argc, char* argv[]) {

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


	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);
	SOCKET newConnection;
	
	while(true){

		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0) {
			std::cout << "Error #2\n";
		}
		else {
			std::cout << "connected";
			connections.push_back(newConnection);
			int i = connections.size();
			std::thread th = std::thread(retranslate, i - 1);
			th.detach();
		}
	}

	

	//system("pause");
	return 0;
}
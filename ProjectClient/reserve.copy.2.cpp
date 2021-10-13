#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <set>
#include <map>
#include <sstream>
#include <fstream>
#include <iomanip>

#pragma warning(disable: 4996)
using namespace std;

SOCKET Connection;

// функція обробки log-файлу
void writeToFile(ofstream& outputFile, string s) {
	outputFile << s << endl;
}
ofstream outputFileReceived;
ofstream outputFileSent;

void ClientHandler() {
	char msg[256];

	while (true) {
		recv(Connection, msg, sizeof(msg), NULL);
		if (msg[0] == 'Y') {
			outputFileReceived.close();
			exit(1);
		}
		std::cout << msg << std::endl;
		// ведеться запис отриманих повідомлень
		outputFileReceived.open("logfile_received.txt", ofstream::out | ofstream::app);
		outputFileReceived << "Received: " << msg << "\n";
		outputFileReceived.close();
		//outputFile.close();
	}
}

int main(int argc, char* argv[]) {

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1133);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return 1;
	}
	std::cout << "Connected!\n";

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	char msg1[256];
	while (true) {
		std::cin.getline(msg1, sizeof(msg1));
		send(Connection, msg1, sizeof(msg1), NULL);
		// ведеться запис відправлених повідомлень
		outputFileSent.open("logfile_sent.txt", std::ofstream::out | std::ofstream::app);
		outputFileSent << "Sent: " << msg1 << "\n";
		outputFileSent.close();
		Sleep(10);
	}

	system("pause");
	return 0;
}

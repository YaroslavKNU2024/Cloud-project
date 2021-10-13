#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;
#pragma warning(disable: 4996)

// Оголошуємо сокет
SOCKET Connection;
vector<vector<string>> v;

// Клас елементів, типів та їх значень
class MyClass {
public:
	MyClass(string name, string type, string value) {
		this->name = name;
		this->type = type;
		this->value = value;
	}
	string name, type, value;
};

map<string, MyClass*> m;

// Функція, яка розбиває рядок на команди (лексеми)
pair<int, vector<string>> parser(string test) {
	stringstream test0(test);
	string segment;
	vector<string> seglist;

	while (getline(test0, segment, ';')) {
		string seg_no_space;
		for (auto x : segment)
			if (x != ' ') seg_no_space += x;
		seglist.push_back(seg_no_space);
	}
	return make_pair(seglist.size(), seglist);
}

void ClientHandler(int index) {
	char msg[256] = { '\0' };
	// переводимо char[] у string для зручної обробки

	// Починаємо цикл обробки команд
	while (true) {
		recv(Connection, msg, sizeof(msg), NULL);
		string s_copy(msg);
		pair<int, vector<string>> p = parser(s_copy);
		if (p.first == 0)
			send(Connection, "You haven't entered anything. Try again", 256, NULL);
		else if (p.first == 1) {
			if (p.second[0] == "who")
				send(Connection, "Hello there! My name is Yaroslav and this is my first server program", 256, NULL);
			else if (!(m.count(p.second[0])))
				send(Connection, "ERROR!!! VARIABLE DOES NOT YET EXIST! CREATE ONE!", 256, NULL);
			else {
				string s = m[p.second[0]]->value;
				char c[256] = { '\0' };
				for (int i = 0, len = s.length(); i < len; ++i) c[i] = s[i];
				send(Connection, c, 256, NULL);
			}
		}
		else if (p.first == 3) {
			MyClass* C = new MyClass(p.second[0], p.second[1], p.second[2]);
			m[p.second[0]] = C;
		}

	}
}



//void ClientHandler(int index) {
//	//ofstream outputFile;
//	//outputFile.open("logfile.txt");
//	char msg[256];
//	string s_copy;
//	while (true) {
//		auto time = std::time(nullptr);
//		recv(Connection, msg, sizeof(msg), NULL);
//		string s3 = "Received: " + string(msg);
//		//writeToFile(outputFile, s3);
//		string s_copy(msg);
//
//		pair<int, vector<string>> p = parser(s_copy);
//		if (p.first != 1 && p.first != 3) {
//			time = std::time(nullptr);
//			send(Connection, "ERROR!!!", sizeof(msg), NULL);
//			//writeToFile(outputFile, "Sent: an error");
//			//outputFile << std::put_time(std::gmtime(&time), "%F %T") << "\n";
//			continue;
//		}
//		if (p.first == 1) {
//			string result = m[p.second[0]]->value;
//			cout << result << "\n";
//			char str[256] = { '\0' };
//			for (int i = 0, len = result.length(); i < len; ++i)
//				str[i] = result[i];
//			//const char* str = result.c_str();
//			send(Connection, str, sizeof(msg), NULL);
//			//writeToFile(outputFile, "Sent: " + result);
//			//outputFile << std::put_time(std::gmtime(&time), "%F %T") << "\n";
//		}
//
//		/*
//		if (p.first == 1 && p.second[0] != "Who") {
//			if (!(m.count(p.second[0])))
//			{
//				char msg_return[] = "ERROR! ELEMENT DOES NOT YET EXIST! CREATE ONE!\n";
//				string copy_ret(msg_return);
//				send(Connection, msg_return, sizeof(msg_return), NULL);
//				time = std::time(nullptr);
//				writeToFile(outputFile, "Sent: " + copy_ret);
//				outputFile << std::put_time(std::gmtime(&time), "%F %T") << "\n";
//				continue;
//			}
//		}*/
//
//		if (p.first == 3) {
//			MyClass* C = new MyClass(p.second[0], p.second[1], p.second[2]);
//			m[p.second[0]] = C;
//			time = std::time(nullptr);
//			send(Connection, "Hello world!", sizeof(msg), NULL);
//			//time = std::time(nullptr);
//			//writeToFile(outputFile, "Sent: Hello world");
//			//outputFile << std::put_time(std::gmtime(&time), "%F %T") << "\n";
//		}
//
//		map<string, MyClass*>::iterator it = m.find(p.second[0]);
//		MyClass* copyOfClass;
//		if (it != m.end()) {
//			copyOfClass = it->second;
//			cout << "FOUND!!!\n";
//		}
//		else if (p.first >= 3) {
//			MyClass newClass = MyClass(p.second[0], p.second[1], p.second[2]);
//			m[p.second[0]] = &newClass;
//			cout << "ADDED!!!\n";
//
//		}
//	}
//}

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
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	if (newConnection == 0) {
		std::cout << "Error #2\n";
	}
	else {
		std::cout << "Client Connected!\n";
		char msg[256] = "Hello. It`s my first network program!";
		send(newConnection, msg, sizeof(msg), NULL);

		Connection = newConnection;
		//Counter++;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	}


	system("pause");
	return 0;
}
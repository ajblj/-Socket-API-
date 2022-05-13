#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <WINSOCK2.H>
#include <STDIO.H>
#include"FileHelper.h"
#pragma  comment(lib,"ws2_32.lib")


int main(int argc, char* argv[])
{
	WORD sockVersion = MAKEWORD(2, 2);//版本号
	WSADATA data;    //用来保存WSAStartup调用后返回的windows Sockets数据
	FileHelper fh;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket !");
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (connect(sclient, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(sclient);
		return 0;
	}
	while (true)
	{
		FILE* f = fh.selectfile();
		char sendData[BUFSIZ];
		char recData[BUFSIZ];
		char over[BUFSIZ] = "Finnal";
		char* name = fh.getFileName();
		strcpy(sendData, name);
		printf("%s\n", sendData);
		int nCount;
		long long sum = 0;
		send(sclient, sendData, strlen(sendData) + 1, 0);
		int ret = 0;
		ret = recv(sclient, recData, BUFSIZ, 0);
		while ((nCount = fread(sendData, 1, BUFSIZ, f)) > 0)
		{
			//Sleep(3000);
			printf("%d\n", nCount);
			send(sclient, sendData, nCount, 0);
			int ret = recv(sclient, recData, BUFSIZ, 0);
			if (ret <= 0)
			{
				printf("服务器%s失去连接", inet_ntoa(serAddr.sin_addr));
				break;
			}
		}
		send(sclient, over, BUFSIZ, 0);
		ret = recv(sclient, recData, BUFSIZ, 0);
		if (ret > 0 && strcmp(recData, over) == 0)
		{
			printf("传输成功!\n");
		}
		else {
			printf("传输失败!\n");
		}
		fclose(f);
	}

	closesocket(sclient);
	WSACleanup();
	return 0;
}
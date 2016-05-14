#pragma comment (lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h> // for struct ip_mreq(multicast request)

//typedef struct ip_mreq {
//  IN_ADDR imr_multiaddr;  // IP multicast address of group.
//  IN_ADDR imr_interface;  // Local IP address of interface. ������ ȣ��Ʈ�� IP�ּ�
//} IP_MREQ, *PIP_MREQ;

void err_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}
void main() {
  WSADATA wsaData;
  SOCKET recvSock;
  SOCKADDR_IN senderAddr;
  struct ip_mreq joinAddr;
  char buf[BUFSIZ];

  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    err_handling("wsastartup err");
  }
  recvSock = socket(PF_INET, SOCK_DGRAM, 0);
  memset(&senderAddr, 0, sizeof(senderAddr));
  senderAddr.sin_family = AF_INET;
  senderAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  senderAddr.sin_port = htons(9000);

  if (bind(recvSock, (SOCKADDR*)&senderAddr, sizeof(senderAddr)) == SOCKET_ERROR) {
    err_handling("bind err");
  }
  joinAddr.imr_multiaddr.S_un.S_addr = inet_addr("224.1.1.1"); //��Ƽĳ��Ʈ�� �׷� IP�ּ�
  joinAddr.imr_interface.S_un.S_addr = htonl(INADDR_ANY); // ������ȣ��Ʈ�� IP�ּ� (INADDR_ANY = LOCALHOST)

  setsockopt(recvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&joinAddr, sizeof(joinAddr)); // �ɼ��� ���� ��Ƽĳ��Ʈ �׷� ����
  while (1) {
    int len = recvfrom(recvSock, buf, BUFSIZ, 0, NULL, 0); //UDP ������ ���� (������ ȣ��Ʈ�� �ּ� ���ʿ� ���⶧���� NULL, 0 ����)
    if (len < 0) {
      break;
    }
    printf("%s\n", buf);
  }
  closesocket(recvSock);
  WSACleanup();
  
}
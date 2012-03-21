#include"sender.h"

#include<sys/time.h>

int sock;
struct sockaddr_in servAddr;
char* servIP;
char* servPort;

PacketHolder* ph;
char* filename;

FILE* file;

int main(int argC, char** argV) {
  if (argC != 3){
    printf("Usage: <filename> <address>\n");
    return -1;
  }

  // Arg1 is file to be transferred
  filename = argV[1];
  
  // Arg2 is address
  char* address = argV[2];

  file = fopen ( filename , "r" );
  if (file==NULL) {fputs ("File error",stderr); exit (1);}

  ph = (PacketHolder*) malloc(sizeof(PacketHolder));
  initPacketHolder(ph);
  initSocket(address);

  pthread_t thread1, thread2;
  int iret1, iret2;

  iret1 = pthread_create( &thread1, NULL, makePackets, (void*) NULL);
  iret2 = pthread_create( &thread2, NULL, sendPackets, (void*) NULL);

  pthread_join( thread1, NULL);
  pthread_join( thread2, NULL);

  close(sock);
  exit(0);
}


void* makePackets(void * data) {
  int fragment = 1;

  char* s = malloc(PACKET_SIZE);
  Packet* p = malloc(sizeof(Packet));

  for(;;){

    if (!isFull(ph)) {

      s = fgets(s, PACKET_SIZE, file);

      if (s == NULL) {
	      while(ph->lock == TRUE) { /* wait for opening */ }
	      lock(ph);
	      addPacket(ph, initPacket(NULL, DUMMY_FRAG_NUM, p));
	      unlock(ph);
	      return;
      }

      while(ph->lock == TRUE) { /* wait for opening */ }
      lock(ph);
      
      if (s == NULL) {
         addPacket(ph, initPacket(NULL, DUMMY_FRAG_NUM, p));
	      unlock(ph);
	      return;
	   } else {
         addPacket(ph, initPacket(s, fragment++, p));
         unlock(ph);    	      
	   }
    } else {
      printf("ERROR: ITS FULL! WAITING TO EMPTY\n");
    }
    
  }
}

void* sendPackets(void* data) {
  for (;;) {
    if (!isEmpty(ph)) {
      char *s;
      Packet* p = malloc(sizeof(Packet));

      while (ph->lock == TRUE){ /* Wait for it to be unlocked */}
      lock(ph);
      p = removePacket(ph, p);
      unlock(ph);

      s = serialize(p, s);
	
      if (p->fragment == DUMMY_FRAG_NUM) {
	sendto(sock, s, (PACKET_SIZE * 4) + 4, 0, (struct sockaddr *)
	       &servAddr, sizeof(servAddr));
	return;
      } else {
	sendto(sock, s, (PACKET_SIZE * 4) + 4, 0, (struct sockaddr *)
	       &servAddr, sizeof(servAddr));
      }
    }
  }
}
  
void initSocket(char* ip){
  servIP = ip;  
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    printf("socket() failed");

  /* Construct the server address structure */
  memset(&servAddr, 0, sizeof(servAddr));    /* Zero out structure */
  servAddr.sin_family = AF_INET;                 /* Internet addr family */
  servAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */
  servAddr.sin_port   = htons(SERVER_PORT);     /* Server port */
}

#include "reciever.h"

int sock;                        /* Socket */
struct sockaddr_in servAddr; /* Local address */
struct sockaddr_in clntAddr; /* Client address */
unsigned int cliAddrLen;         /* Length of incoming message */
unsigned short servPort;     /* Server port */
int recvMsgSize;                 /* Size of received message */
PacketHolder* ph;


int main(int argC, char** argV){

  servPort = SERVER_PORT;
  
  ph = (PacketHolder*) malloc(sizeof(PacketHolder));
  initPacketHolder(ph);

  initSocket();
  printf("Server started, listening on %d\n", servPort);

  pthread_t thread1, thread2;
  int iret1, iret2;

  iret1 = pthread_create( &thread1, NULL, getPackets, NULL);
  iret2 = pthread_create( &thread2, NULL, makeFile, NULL);

  pthread_join( thread1, NULL);
  pthread_join( thread2, NULL);

  close(sock);
  exit(0);
}

void* getPackets(void* data){
  
  char *s = malloc((PACKET_SIZE * 4) + 4);
  Packet* p;

  for(;;) {
    recvfrom(sock, s, (PACKET_SIZE * 4) + 4, 0, (struct sockaddr*) &clntAddr, &cliAddrLen);
    p = deserialize(s, p);
    
    while(ph->lock == TRUE) { /* Wait for unlock */ }
    lock(ph);
    addPacket(ph, p);
    unlock(ph);
  }

}

void* makeFile(void* data) {

  FILE* file = NULL;

  for(;;) {
    
    if (!isEmpty(ph)) {
      Packet* p;

      while(ph->lock == TRUE) { /* Wait for unlock */ }
      lock(ph);
      p = removePacket(ph, p);
      unlock(ph);

      if (p->fragment == DUMMY_FRAG_NUM) {
	// Close file and make new one for next transmission
	printf("-->Transmeet Complete!\n");
	fclose(file);
	file = NULL;
	} 
      else {
	  // Write contents to current file

	  if (file == NULL) {
	    // open new file
	    file = fopen(OUTFILE, "w");
	  }

	  printf("-->Recieving Packet\n");
	  fputs(p->contents, file);
	}
    }
  }
}

void initSocket(){
  /* Create socket for sending/receiving datagrams */
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    printf("socket() failed");

  /* Construct local address structure */
  memset(&servAddr, 0, sizeof(servAddr));   /* Zero out structure */
  servAddr.sin_family = AF_INET;                /* Internet address family */
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
  servAddr.sin_port = htons(servPort);      /* Local port */

  /* Bind to the local address */
  if (bind(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
    printf("bind() failed");

  cliAddrLen = sizeof(clntAddr);
}

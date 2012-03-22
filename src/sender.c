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

  for(;;){
     while(!isLocked(ph)){
         lock(ph);
	     
         if (!isFull(ph)) {
           s = fgets(s, PACKET_SIZE, file);
      
           if (s == NULL) {
              addPacket(ph, initPacket(NULL, DUMMY_FRAG_NUM));
	           unlock(ph);
              pthread_exit(0);
	         } else {
               addPacket(ph, initPacket(s, fragment++));
	         }	         
         }
         
         unlock(ph); 
      }
   }  
}

void* sendPackets(void* data) {
   // Send Header
   Header* h = initHeader(filename, "dave");
   char* h_s = serializeHeader(h);
   sendto(sock, h_s, strlen(h_s), 0, (struct sockaddr *) &servAddr, sizeof(servAddr));

   // Send the packets
  for (;;) {
     while(!isLocked(ph)) {
        lock(ph);

        if (!isEmpty(ph)) {
           char *s;
           Packet* p = malloc(sizeof(Packet));

           p = removePacket(ph, p); // Get packet
           s = serializePacket(p); // Convert into sendable char array

           sendto(sock, s, strlen(s), 0, (struct sockaddr *) &servAddr, sizeof(servAddr));
	
           if (p->fragment == DUMMY_FRAG_NUM) pthread_exit(0);
        }
        
        unlock(ph);
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

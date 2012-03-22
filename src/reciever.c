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
   char *s = malloc(TRANSMIT_SIZE);
   int count = 0;
   
   for(;;) {
      if (!isLocked(ph)){
         lock(ph);
         
         // Recieve String
         recvfrom(sock, s, TRANSMIT_SIZE, 0, (struct sockaddr*) &clntAddr, &cliAddrLen);

         if (count == 0) {
            // Must be header
            Header* h = deserializeHeader(s);
            printf("^^^^^^ Getting %s From %s\n ^^^^^^^", h->filename, h->username);
            memset(s, 0, TRANSMIT_SIZE);
            count++;
         } else {
            Packet* p = deserializePacket(s);
            addPacket(ph, p);

            if (p->fragment == DUMMY_FRAG_NUM) {
               // Done here, in reality you would keep this open,
               // since it is a server...
               unlock(ph);
               pthread_exit();
            }
            count++;
         }
         
         unlock(ph);
      }
   }
}

void* makeFile(void* data) {
   FILE* file = NULL;
   struct timeval* current = malloc(sizeof(struct timeval));
   int start;
   int now;
   gettimeofday(current, NULL);
   now = current->tv_sec;
   start = now;

   for(;;) {
      if (!isLocked(ph)) {
         lock(ph);
         
         if (!isEmpty(ph)) {
            Packet* p;

            p = removePacket(ph, p);

            if (p->fragment == DUMMY_FRAG_NUM) {
               // Close file and make new one for next transmission
               printf("-->Transfer Complete!\n");
               fclose(file);
               // Since this is for testing, it will now exit
               // in a very unserver-like manner.
               unlock(ph);
               pthread_exit();
            } else {
               // Write contents to current file
               if (file == NULL) {
                  file = fopen(OUTFILE, "w");
               }  

               printf("-->Recieving Packet\n");
               fputs(p->contents, file);
            }
         }
         
         unlock(ph);      
      }
      
      // Time check
      gettimeofday(current, NULL);
      now = current->tv_sec;
      if (now - start > 15) {
         printf("Dying due to old age\n");
         fclose(file);
         unlock(ph);
         exit(-1);
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

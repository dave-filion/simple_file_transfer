#include"constants.h"

// Send before packets to give info
typedef struct header {
   char filename[32];
   char username[32];
} Header;

// Packet containing data
typedef struct packet {
  char contents[PACKET_SIZE];
  int fragment;
} Packet;

// Custom packet holder (WITH LOCK!)
typedef struct packetHolder {
  Packet* packets[NUM_PACKET_SLOTS];
  int lock;
} PacketHolder;

/* PACKET FUNCTIONS */
Packet* initPacket(char*, int);
Header* initHeader(char* filename, char* username);
char* serializeHeader(Header*);
char* serializePacket(Packet*);

Header* deserializeHeader(char*);
Packet* deserializePacket(char*);

/* PACKETHOLDER FUNCTIONS */
void initPacketHolder(PacketHolder*);

void lock(PacketHolder*);
void unlock(PacketHolder*);
int isLocked(PacketHolder*);

int isEmpty(PacketHolder*);
int isFull(PacketHolder*);

Packet* addPacket(PacketHolder*, Packet*); // Adds supplied packet to packetHolder
Packet* removePacket(PacketHolder*, Packet*); // Removes and returns packet with smallest fragment

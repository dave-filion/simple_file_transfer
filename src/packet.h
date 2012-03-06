#include"constants.h"

typedef struct packet {
  char contents[PACKET_SIZE];
  int fragment;
} Packet;

typedef struct packetHolder {
  Packet* packets[NUM_PACKET_SLOTS];
  int lock;
} PacketHolder;

/* PACKET FUNCTIONS */
Packet* initPacket(char*, int, Packet*);
char* serialize(Packet*, char*);
Packet* deserialize(char*, Packet*);

/* PACKETHOLDER FUNCTIONS */
void initPacketHolder(PacketHolder*);

void lock(PacketHolder*);
void unlock(PacketHolder*);

int isEmpty(PacketHolder*);
int isFull(PacketHolder*);

Packet* addPacket(PacketHolder*, Packet*); // Adds supplied packet to packetHolder
Packet* removePacket(PacketHolder*, Packet*); // Removes and returns packet with smallest fragment

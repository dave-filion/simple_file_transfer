#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/time.h>

#include"packet.h"


int main() {
   
   Packet* p = initPacket("THESE ARE THE PACKAGE CONTENTS, LOOK ON AND DESPAIR", 42);
   Header* h = initHeader("dumb.txt", "dave");
   
   printf("USER: %s\n", h->username);
   printf("FILE: %s\n", h->filename);
   
   char* s = serializeHeader(h);
   
   printf("%s\n", s);
   
   char* p_s = serializePacket(p);
   
   printf("%s\n", p_s);
   
   Packet* newP = (Packet*) deserializePacket(p_s);
   
   printf("%p\n", newP);
   
   printf("FRAG: %d\n", newP->fragment);
   printf("CONTENTS: %s\n", newP->contents);
   
}
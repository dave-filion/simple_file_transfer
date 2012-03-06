all: src/sender.c src/makeBigFile.c src/reciever.c
	gcc -g -lpthread -o bin/sender src/sender.c src/packet.c
	gcc -g -lpthread -o bin/reciever src/reciever.c src/packet.c
	gcc -g -lpthread -o bin/makeBigFile src/makeBigFile.c


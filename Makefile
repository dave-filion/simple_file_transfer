all: src/sender.c src/reciever.c src/makeBigFile.c src/compare.c
	gcc -g -lpthread -o bin/sender src/sender.c src/packet.c
	gcc -g -lpthread -o bin/reciever src/reciever.c src/packet.c
	gcc -g -lpthread -o bin/makeBigFile src/makeBigFile.c
	gcc -g -lpthread -o bin/compare src/compare.c


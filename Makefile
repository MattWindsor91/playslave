CC := clang
CFLAGS := -Wall -Wextra -Werror --std=c99 `pkg-config --cflags libavformat1 libavcodec1 ao`
LIBS := `pkg-config --libs libavformat1 libavcodec1 ao`

OBJS := main.o player.o io.o cmd.o

main: $(OBJS) 
	$(CC) -o main $(OBJS) $(LIBS)

main.o: main.c
	$(CC) -c -o main.o main.c $(CFLAGS)

player.o: player.c
	$(CC) -c -o player.o player.c $(CFLAGS)

io.o: io.c
	$(CC) -c -o io.o io.c $(CFLAGS)

cmd.o: cmd.c
	$(CC) -c -o cmd.o cmd.c $(CFLAGS)
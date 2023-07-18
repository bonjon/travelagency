CC = gcc
INCLUDES = -I/usr/include/mysql
LIBS = -L/usr/local/lib/mysql -lmysqlclient
all: client
main.o: main.c 
	$(CC) -c $(INCLUDES) main.c
agenzia.o:
	$(CC) -c $(INCLUDES) agenzia.c
cliente.o:
	$(CC) -c $(INCLUDES) cliente.c
guida.o:
	$(CC) -c $(INCLUDES) guida.c
inout.o:
	$(CC) -c $(INCLUDES) inout.c
parse.o:
	$(CC) -c $(INCLUDES) parse.c
utils.o: utils.c 
	$(CC) -c $(INCLUDES) utils.c
client: main.o agenzia.o cliente.o guida.o inout.o parse.o utils.o
	$(CC) -o client main.o agenzia.o cliente.o guida.o inout.o parse.o utils.o $(LIBS)
clean:
	rm -f client main.o agenzia.o cliente.o guida.o inout.o parse.o utils.o
	echo clean done
 

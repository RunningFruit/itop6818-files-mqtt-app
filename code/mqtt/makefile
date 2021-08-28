all:Client
	@echo ""
	@echo "Start compiling......"
	@echo ""

Client:Server
	gcc -o Client client.c -lmosquitto -lpthread

Server:
	gcc -o Server server.c -lmosquitto -lpthread

clean:
	-rm Server Client

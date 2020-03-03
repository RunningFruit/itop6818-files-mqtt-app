all:Client
	@echo ""
	@echo "Start compiling......"
	@echo ""

Client:Server
	gcc -o Client mqtt_client.c -lmosquitto -lpthread

Server:
	gcc -o Server mqtt_server.c -lmosquitto -lpthread

clean:
	-rm Server Client

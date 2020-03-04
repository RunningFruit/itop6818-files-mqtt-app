all:uart_write_read
	@echo ""
	@echo "Start compiling......"
	@echo ""

OBJECTS= uart_write_read.o mqttclient.o cJSON.o
# OBJECTS= uart_write_read.o mqttclient.o

cJSON.o: cJSON.c
	gcc -Wall -g -c $< -lm -o $@

# cJSON.o: cJSON.c
# 	gcc -c cJSON.c -lm
# 	ar -rc cJSON.a cJSON.o

mqttclient.o: mqttclient.c
	gcc -Wall -g -c $< -lm -o $@ -lmosquitto -lpthread -lcjson

uart_write_read:$(OBJECTS)  
	gcc -Wall -g $^ -o $@ -lmosquitto -lpthread

.PHONY:clean

clean:
	rm -rf *.o
	rm -f uart_write_read

all:buzzer
	@echo ""
	@echo "Start compiling......"
	@echo ""

OBJECTS= buzzer.o mqttclient.o cJSON.o

buzzer:$(OBJECTS)  
	gcc -Wall -g $^ -o $@

mqttclient.o: mqttclient.c
	gcc -Wall -g -c $< -o $@ -lmosquitto -lpthread
cJSON.o: cJSON.c
	gcc -Wall -g -c $< -o $@ -lm

.PHONY:clean

clean:
	rm -rf *.o
	rm -f buzzer

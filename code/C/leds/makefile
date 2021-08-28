all:leds
	@echo ""
	@echo "Start compiling......"
	@echo ""

OBJECTS= leds.o mqttclient.o cJSON.o
# OBJECTS= leds.o mqttclient.o

cJSON.o: cJSON.c
	gcc -Wall -g -c $< -lm -o $@

# cJSON.o: cJSON.c
# 	gcc -c cJSON.c -lm
# 	ar -rc cJSON.a cJSON.o

mqttclient.o: mqttclient.c
	gcc -Wall -g -c $< -lm -o $@ -lmosquitto -lpthread -lcjson

leds:$(OBJECTS)  
	gcc -Wall -g $^ -o $@ -lmosquitto -lpthread

.PHONY:clean

clean:
	rm -rf *.o
	rm -f leds

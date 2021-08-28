all:ADC_7Channel
	@echo ""
	@echo "Start compiling......"
	@echo ""

OBJECTS= ADC_7Channel.o mqttclient.o cJSON.o
# OBJECTS= ADC_7Channel.o mqttclient.o

cJSON.o: cJSON.c
	gcc -Wall -g -c $< -lm -o $@

# cJSON.o: cJSON.c
# 	gcc -c cJSON.c -lm
# 	ar -rc cJSON.a cJSON.o

mqttclient.o: mqttclient.c
	gcc -Wall -g -c $< -lm -o $@ -lmosquitto -lpthread -lcjson

ADC_7Channel:$(OBJECTS)  
	gcc -Wall -g $^ -o $@ -lmosquitto -lpthread

.PHONY:clean

clean:
	rm -rf *.o
	rm -f ADC_7Channel

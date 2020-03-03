#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <math.h>  
#include <string.h>

char iiotype[8][32] = { 
	"in_voltage0_raw",
	"in_voltage1_raw",
	"in_voltage2_raw",
	"in_voltage3_raw",
	"in_voltage4_raw",
	"in_voltage5_raw",
	"in_voltage6_raw",
	"in_voltage7_raw",
};

//adc_channel 0 			:slide Res
//adc_channel 2 			:temperature
//adc_channel 3,4 			:connector J38 ADC3 ADC4
//others adc_channel 1,5,6,7:reserved
int read_adc_channel(int *value,int adc_channel) 
{ 
	int ret = 0; 
	char filepath[100];
	FILE *fp;
	char buf[20]; 
	
	sprintf(filepath, "/sys/bus/iio/devices/iio\:device0/%s", iiotype[adc_channel]);
	
	printf("filepath is  %s!\n", filepath);
	printf("adc_channel is  %d!\n", adc_channel);
	
	fp = fopen(filepath, "rt"); 
	if(fp==NULL){ 
		printf("open %s fail!\n", filepath); 
		*value = 0; 
		ret = -1; 
		return ret; 
	}
	printf("open %s success!\n", filepath);
	
	ret = fread( buf, 1, sizeof(buf), fp); 
	if(ret <0){
		printf("fread %s fail!\n", filepath); 
		return -1;
	}	
	printf("fread %s finish!\n", filepath);
	
	fclose(fp); 
	//printf("return value is %s !\n", buf);
	*value	= atoi(buf);
	
	return ret; 
}

     
int main(int argc, char** argv) 
{ 
	int ret = 0; 
	int value; 

	if(argc !=2){
		printf("Usage: [%s] [0-7] \n",argv[0]);
		printf("type 0 :channel0:slide Res \n");
		printf("type 2 :cpu temperature \n");
		printf("type 3 or 4 :connector J38 ADC3 ADC4 \n");
		printf("type 1,5,6,7 :reserved \n");
		return 0;
	}

	ret = read_adc_channel(&value,atoi(argv[1])); 
	if(ret < 0) 
	{ 
		printf("read channel%d failed!\n",atoi(argv[0]));
		return ret; 
	}
	
	printf("return value is %d!\n",value);
	
	return ret; 
}

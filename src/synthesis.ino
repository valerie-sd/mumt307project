/*
	MUMT 307 Final Project
	Winter 2015
	Valerie

	Digital audio synthesis on Sparkfun Redboard (Arduino Uno)
	Uses the AD5330 8-bit DAC (see datasheet for pin details used in #defines.)
*/

#define CS 18
#define WR 17
#define LDAC 16
#define CLR 15
#define PD 10
#define BUF 9
#define GAIN 14

#define PERIOD 2273
#define FREQ 440
#define MAXVOL 255 // maximum voltage to DAC (amplitude of sine wave)

int summed[8] = {31, 54+31, 63, 54-31, 31, 9+31, 0, 9-31};

void setup(){
	//All of the digital pins on the Arduino will be outputs to the AD5330
	for(int pin=0; pin<19; pin++){
		pinMode(pin, OUTPUT);
	} 

	digitalWrite(PD, HIGH);  //Enable the AD5330
	digitalWrite(GAIN, LOW);  //Set Gain to 1
	digitalWrite(BUF, HIGH);  //Don't buffer the input
	digitalWrite(CS, HIGH);  //Set the CS high by default
	digitalWrite(WR, HIGH);  //Set the WR pin high by default
	digitalWrite(CLR, HIGH);  //Make sure Clear pin is disabled
	digitalWrite(LDAC, LOW);

	//Clock in Gain and Buffer Values
	digitalWrite(CS, LOW);
	delayMicroseconds(10);
	digitalWrite(WR, LOW);
	delayMicroseconds(10);

	digitalWrite(CS, LOW);
	delayMicroseconds(10);
	digitalWrite(WR, LOW);
	delayMicroseconds(10); 
	//All setup() until this point is from the tutorial but since it's also right out of the 
	//datasheet, I would consider it general knowledge.

	//summed = {31, 54+31, 63, 54-31, 31, 9+31, 0, 9-31};
	int x=0;
	for(x=0; x<8; x++){
		summed[x] += 64;
	}
}

void loop(){
	unsigned int test = 3;
	unsigned int VOLT = 127;
	unsigned int index = 0;
	while(test==1){
		//playing VOLT with frequency for A4 (wave1)
		digitalWrite(CS, LOW);//"chip select" pin	

		digitalWrite(WR, LOW);//"input write" pin	
		PORTD = VOLT;//This is a more concise version of doing digitalWrite() to all Dx digital pins.
		digitalWrite(WR, HIGH);		
		delayMicroseconds(PERIOD/2);
	
		digitalWrite(WR, LOW);
		PORTD = 0;
		digitalWrite(WR, HIGH);
		delayMicroseconds(PERIOD/2); 

		digitalWrite(CS, HIGH); 
	}
	while(test==2){
		//playing second wave
		digitalWrite(CS, LOW);		

		digitalWrite(WR, LOW);		
		PORTD = VOLT;			
		digitalWrite(WR, HIGH);		
		delayMicroseconds(PERIOD/4);

		digitalWrite(WR, LOW);
		PORTD = 63;
		digitalWrite(WR, HIGH);
		delayMicroseconds(PERIOD/4); 

		digitalWrite(CS, HIGH); 
	}
	while(test==3){
		//playing wave1+wave2
		//split frequency into 8 regions
		digitalWrite(CS, LOW);		
		for(index=0; index<8; index++){
			digitalWrite(WR, LOW);		
			PORTD = summed[index];			
			digitalWrite(WR, HIGH);		
			delayMicroseconds(PERIOD/8);
		}
		digitalWrite(CS, HIGH); 
	}
}

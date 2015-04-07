/*
	MUMT 307 Final Project
	Winter 2015
	Valerie

	Digital audio synthesis on Sparkfun Redboard (Arduino Uno)
	Uses the AD5330 8-bit DAC (see datasheet for pin details used in #defines.)

	This program is to test volume (voltage) in a gradient because I could only get 
	either one high volume or no volume at all.
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
}

void loop(){
	unsigned int VOLT = 255;
	unsigned int index = 0;
	unsigned int samples = 0;
	for(VOLT=MAXVOL; VOLT>0; VOLT--){
		//Serial.print(VOLT);
		//for(samples=0; samples<5; samples++){
		for(samples=0; samples<10; samples++){
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
	}
	while(true);
}

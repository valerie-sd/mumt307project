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
int squarewv[64];

void setup(){
	//All of the digital pins on the Arduino will be outputs to the AD5330
	/*Arduino takes no input for this system; only output for now. I might change this later
	when/if I add potentiometers for option choices. 
	*/
	for(int pin=0; pin<19; pin++){
		pinMode(pin, OUTPUT);
	} 
	/*PD HIGH: power-down pin. LOW would put the DAC in power-down mode. 
	*/
	digitalWrite(PD, HIGH);  //Enable the AD5330
	/*GAIN LOW: This is the output amplifier gain. Can be set to 1 or 2. (Range for 1: 0.001V to Vref)
	*/
	digitalWrite(GAIN, LOW);  //Set Gain to 1
	/*BUF HIGH: turns off buffering since I want the input to be handled immediately. This setting has fewer
	restrictions for voltage.
	*/
	digitalWrite(BUF, HIGH);  //Don't buffer the input
	/*CS HIGH: LOW would set the device? Not really sure what this means but we disable it for
	now and when it is used, like WR, it will be set to LOW. 
	*/
	digitalWrite(CS, HIGH);  //Set the CS high by default
	/*WR HIGH: LOW would control the input; I will do this later, so this is just a default. 
	*/
	digitalWrite(WR, HIGH);  //Set the WR pin high by default
	/*CLR HIGH: LOW would reset the input and DAC register, but for this I want those to be persistent.
	*/
	digitalWrite(CLR, HIGH);  //Make sure Clear pin is disabled
	/*LDAC LOW: means that the contents of the input buffer are transferred to the DAC register. 
	If it were high, that would mean the DAC re
	*/
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

	int first[64];
	int third[64];
	int fifth[64];
	//summed = {31, 54+31, 63, 54-31, 31, 9+31, 0, 9-31};
	int x=0;
	for(x=0; x<8; x++){//for additive synthesis
		summed[x] += 64;
	}
	for(x=0; x<64; x+=8){
		first[x] = 0;
		first[x+1] = 23;
		first[x+2] = 32;
		first[x+3] = 23;
		first[x+4] = 0;
		first[x+5] = -23;
		first[x+6] = -32;
		first[x+7] = -23;
	}
	for(x=0; x<64; x+=8){
		third[x] = 0;
		third[x+1] = 12;
		third[x+2] = 16;
		third[x+3] = 12;
		third[x+4] = -0;
		third[x+5] = -12;
		third[x+6] = -16;
		third[x+7] = -12;
	}
	for(x=0; x<64; x+=2){//Simple high and low for highest freqency (amplitude half that of fundamental)
		fifth[x] = 16;
		fifth[x+1] = -16;
	}
	//SUMMING for square wave
	for(x=0; x<64; x++){
		squarewv[x] = first[x]+third[x]+fifth[x];
	}
	//adjust for 0-255 range
	for(x=0; x<64; x++){
		squarewv[x] += 95;
	}

}

void loop(){
	unsigned int test = 5;
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
	//option 4: squarish wave (odd harmonics) might do around 4 harmonics? 3 so far.
	while(test==4){
		digitalWrite(CS, LOW);		
		for(index=0; index<64; index++){
			digitalWrite(WR, LOW);		
			PORTD = squarewv[index];			
			digitalWrite(WR, HIGH);		
			delayMicroseconds(PERIOD/64);
		}
		digitalWrite(CS, HIGH); 
	}
	//option 5: amplitude envelope
	while(test==5){
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
	}
	//option 6: delay line for echo
	//option 7: Karplus-Strong plucked string synthesis
}

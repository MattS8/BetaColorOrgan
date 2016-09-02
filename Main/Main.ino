// Example 48.1 - tronixstuff.com/tutorials > chapter 48 - 30 Jan 2013 
// MSGEQ7 spectrum analyser shield - basic demonstration - Modified by Larry Steinhardt
int strobe = 12;    // strobe pins on digital 4
int res = 13;       // reset pins on digital 5
int LEDRed=10;       // PWM
int LEDBlue=11;     // PWM
int LEDGreen=9;    // PWM
int audio[7];       // store band values in these arrays
int filter=90;      // filter value used in Map and Constrain equations
int band;           // loop variable
int BandMax=0;      // Max value of bands
int Not_Null_Cnt=0; // Counter for music present
int Null_Cnt=0;     // Counter for no music
int All_On=0;       // Force Lights On (No Music Present)
int Red=250;        // Red intensity for RGB color Cycle
int Green=250;      // Green intensity for RGB color Cycle
int Blue=250;       // Blue intensity for RGB color Cycle
int incR=1;         // Incremental value for Red cycle
int incG=1;         // Incremental value for Green cycle
int incB=1;         // Incremental value for Blue cycle
int looptime=3000;  // Cycle counter for how long to wait for music off to trigger RGB cycle
int rnd_cnt=0;      // Cycle counter to re-randomize the RGB increment values
int CRed;           // Max values for color organ Red
int CGreen;         // Max values for color organ Green
int CBlue;          // Max values for color organ Blue

void setup()
{
 Serial.begin(9600);
 pinMode(res, OUTPUT);      // reset
 pinMode(strobe, OUTPUT);   // strobe
 digitalWrite(res,LOW);     // set reset low
 digitalWrite(strobe,HIGH); // set strobe high                        
 //analogReference (INTERNAL);// Internal reference 1.1 V instead of the default of 5 V if input levels are low. 
}

void readMSGEQ7()
// Function to read 7 band equalizers
{
 digitalWrite(res, HIGH);
 digitalWrite(res, LOW);
 BandMax=0;
 for(band=0; band <7; band++)
 {
 digitalWrite(strobe,LOW);  // strobe pin increments to the next band on the MSGEQ7
 delayMicroseconds(30);     // 
 audio[band] = map(constrain(analogRead(A0), filter, 1023), filter, 1023, 0, 255); // Band values
 if (audio[band] > BandMax)
 {
  BandMax = audio[band];
  }
 digitalWrite(strobe,HIGH); 
 delayMicroseconds  (1);    // needed to meet timing diagram requirements
 }
  if (BandMax ==0)
 {
  if (All_On == 1)
  {
    All_On = 1;               // Will call LED cycle program instead of color organ
    Null_Cnt = looptime;      // Reset counter for no music cycles to initial value
    Not_Null_Cnt = 0;         // Reset counter for music present cycles to 0
  }
  else
  {
    Null_Cnt++;               // Increment the Null_Cnt (music off time)
    if (Null_Cnt >= looptime) // Music has been off long enough
    {
     All_On = 1;              // Will call LED cycle program instead of color organ
     rnd_cnt++;
     Null_Cnt = looptime;     // Reset counter for no music time to initial value
     Not_Null_Cnt = 0;        // Reset counter for music present cycles to 0
     Red= random(11, 254);    // Rabdomize start value for Red
     Green= random(11, 254);  // Rabdomize start value for Green
     Blue= random(11, 254);   // Rabdomize start value for Blue
     if (rnd_cnt >= 2000)     // Time to change up the cycle rate for RGB
     {
      incR = random(6);       // Randomize Red increment between 1 and 4
      incG = random(6);       // Randomize Green increment between 1 and 4
      incB = random(6);       // Randomize Blue increment between 1 and 4
      rnd_cnt = 0;            // Reset cycle count for increment randomizer
     }
    }
  }
 }
 else
 {
  Not_Null_Cnt++;             // Increment the Not_Null_Cnt (music present cycles)
  if (Not_Null_Cnt >= 2)      // Music has been present for 2 cycles
  {
    All_On = 0;               // Will call color organ instead of LED cycle program
    Not_Null_Cnt = 2;         // Keep counter for music present cycles = to 2
    Null_Cnt = 0;             // Reset counter for no music cycles to 0
  }
 }
}

void writeLEDs()
                            // Function to write band values to LED FET drivers
{
  CRed = max(audio[0],audio[1]);
  //CRed = max(CRed,audio[2]);
  CGreen = max(audio[3],audio[4]);
 // CGreen = max(CGreen,audio[4]);
  CBlue = max(audio[5],audio[6]);
  CBlue = max(CBlue,audio[4]);
 
  analogWrite(LEDRed,CRed);
  analogWrite(LEDGreen,CGreen);
  analogWrite(LEDBlue,CBlue);
   
  //analogWrite(LEDRed,audio[0]);
  //analogWrite(LEDRed,audio[1]);
  //analogWrite(LEDRed,audio[2]);
  //analogWrite(LEDBlue,audio[3]);
  //analogWrite(LEDBlue,audio[4]);
  //analogWrite(LEDGreen,audio[5]);
  //analogWrite(LEDGreen,audio[6]);
}

void writeLEDsOn()
                            // Function to turn on all LED FET drivers
{
  analogWrite(LEDRed,Red);
  analogWrite(LEDBlue,Blue);
  analogWrite(LEDGreen,Green);
}

void RGB_Increment()
                            // Increment the cycle through the RGB loop
{
  Red=Red+incR;Blue=Blue+incB;Green=Green+incG;
  if(Red>=250)
  {
    incR=incR*-1;
  }
  if (Red<=10)
  {
    incR=incR*-1;
  }
  if(Blue>=250)
  {
    incB=incB*-1;
  }
  if (Blue<=10)
  {
    incB=incB*-1;
  }
  if(Green>=250)
  {
    incG=incG*-1;
  }
  if (Green<=10)
  {
    incG=incG*-1;
  }
   delay(10);           // Set the speed for the cycle time
}
void  printaudioValues()
                            // Print out values for audio channel
{
               
  Serial.print("Null Cnt: ");
  Serial.print(Null_Cnt);
  Serial.print("\t");
  Serial.print("Not Null Cnt: ");
  Serial.print(Not_Null_Cnt);
  Serial.print("\t"); 
  Serial.print("BandMax: ");
  Serial.print(BandMax);
  Serial.print("\t");
  Serial.print("All On: ");
  Serial.print(All_On);
  Serial.print("\t");
  Serial.print("Band 1-7: ");
 for (band = 0; band < 7; band++)
 {
 Serial.print(audio[band]); // display values of audio channel on serial monitor
 Serial.print(" ");
 }
  Serial.print("\t");
  Serial.print("Red: ");
  Serial.print(Red);
  Serial.print(" ");
  Serial.print("Green: ");
  Serial.print(Green);
  Serial.print(" ");
  Serial.print("Blue: ");
  Serial.print(Red);
 Serial.println(); 
}

void loop()
{
 readMSGEQ7();              // Get band values
 if (All_On == 1)            // No music present
 {
  RGB_Increment();          // Increment the RGB cycle
  writeLEDsOn();            // Write the RGB values to the LEDs
 }
else
 {
 writeLEDs();               // Write  band values to the LED Driver FETs
 }
// printaudioValues(); // Print out band values to the serial monitor
}

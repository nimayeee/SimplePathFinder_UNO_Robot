/*
Relay UltrasSonic PIR vcc to 5v
Relay UltrasSonic PIR GND to Ground

*/
#define ON   0 
#define OFF  1
const int trigPin = 8;// UltraSonic Trig
const int echoPin = 9; //UltraSonic Echo
const int pirPin = 3; //PIR Sensor Pin
//the relays connect to 
const int    IN1 = 2;// 2 chanel relay pin 
const int    IN2 = 5;// 2 chanel relay pin 

// defines variables
long duration;
int distance;
int lastdist = 0;  

 
void setup() {
 
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input  
  pinMode(pirPin, INPUT); // Sets the PIR Sensor pin as an Input  
  relay_init();//initialize the relay     
  Serial.begin(9600);

}
 
void loop(){

  //-------------------------- Clears the trigPin -------------------
  digitalWrite(trigPin, LOW);
  delayMicroseconds(20);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance - Duration: ");
  Serial.print(distance);  
  Serial.print(" - ");    
  Serial.println(duration);

   
///---------------------------------Manage MOTORS---------------------   
  if(distance<30){
      
      if (lastdist+1<distance){
        ///--- Set Both relay LO to strait. Distance is increasing
        relay_SetStatus(OFF, OFF);//turn Off RELAY_1 
        distance=lastdist;    ////-- to keep last dest constant--
        Serial.print("incresing Distance Move Forward-");
      }else{
        ///--- Set Both relay HIGH to Rotate
        relay_SetStatus(ON, ON);//turn on RELAY_1 

        Serial.print("Rotate Spot-");
      }
      
   }else{
      if(distance>1000){
          ///---- Shortest distance - Contacted
          relay_SetStatus(ON, ON);//turn on RELAY_1,2 
          Serial.print("Reverse Geer Needed-");
       }else{
          if (digitalRead(pirPin)== HIGH ){
              ///--- Set ONE relay HI - STOP THE CAR
              relay_SetStatus(ON, OFF);
              Serial.print("STOP PIR COMMAND-");        
            }else{
              ///--- Set Both relay LOW - Move Strate
              relay_SetStatus(OFF, OFF);//turn Off RELAY_1 
              Serial.print("Move in Line-");        
          }
      }
   }
      lastdist=distance;

  //---------------------------------------------
}

void relay_init(void)//initialize the relay 
{  
  //set all the relays OUTPUT   
  pinMode(IN1, OUTPUT);  
  pinMode(IN2, OUTPUT);  relay_SetStatus(OFF, OFF);//turn off all the relay 
 }//set the status of relays 
 
 void relay_SetStatus( unsigned char status_1,  unsigned char status_2){
  digitalWrite(IN1, status_1);  
  digitalWrite(IN2, status_2);
}

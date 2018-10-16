#define WATER_LVL A7
#define MOIST_SENS 12
#define MOIST_ON 13
#define PUMP_CMD 9

#define PUMPON true
#define PUMPOFF false

int waterLvl = 0;
int moistValue = 0;
bool pumpState = PUMPOFF;

void setup()
{
  // Begin Serial Com with EPS8266
  Serial.begin(9600);

  pinMode(MOIST_ON,OUTPUT);
  pinMode(MOIST_SENS,INPUT);
  pinMode(WATER_LVL,INPUT);
  pinMode(PUMP_CMD,OUTPUT);

  Serial.println("Init Done");
  
}
int value; 
int freq=10000; 
void loop()
{
     waterLvl = analogRead(WATER_LVL); //Read data from analog pin and store it to value variable
     waterLvl= map(waterLvl, 0, 650, 0,100);

     digitalWrite(MOIST_ON,HIGH);
     delay(500);
     moistValue = digitalRead(MOIST_SENS);
     delay(500);
     digitalWrite(MOIST_ON,LOW);

   if(waterLvl > 10 && !moistValue)
   { 
    digitalWrite(PUMP_CMD,HIGH);
    Serial.println("PUMP ON");
    freq=500;
   }
   else
   {
    digitalWrite(PUMP_CMD,LOW);
    freq=5000;
   }
  
  delay(freq); // Check for new value every 5 sec
}


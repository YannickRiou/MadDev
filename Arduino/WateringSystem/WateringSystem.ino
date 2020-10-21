

const int VAL_PROBE = 0; //Analog pin 0
const int MOISTURE_LEVEL = 450; // the value after the LED goes on

int value = 0;
int average[10]={"."};
int i =0;

void setup()
{
  
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(7, OUTPUT);
    pinMode(9, OUTPUT);
  
}

void LedState(int state)
{
  digitalWrite(13,state);
}

void loop()
{
  digitalWrite(7,HIGH);
  delay(100);
  average[i] = analogRead(VAL_PROBE);
  i++;
  digitalWrite(7,LOW);

  if(i>8)
  {
     for(i=0;i<10;i++)
     {
      value = value+average[i];
     }
     value = value/10;
     Serial.print(value);
     Serial.println(".");
    i =0;
  }
  
  //Serial.print("Moisture = ");
 
  
  if(value > MOISTURE_LEVEL)
  {
    //Serial.println("PUMP ON");
    //LedState(HIGH);
    //digitalWrite(9,HIGH);
  }
  else 
  {
   //Serial.println("PUMP OFF");
   //digitalWrite(9,LOW);
  }
  
  delay(10000);
}

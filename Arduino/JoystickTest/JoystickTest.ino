// Arduino pin numbers
const int X_pin = 1; // analog pin connected to X output
const int Y_pin = 0; // analog pin connected to X output

int val = 0;

void setup() {
  Serial.begin(115200);
}
 
void loop() {
  /*
  Serial.print(analogRead(X_pin));
  Serial.print(" ");
  Serial.println(analogRead(Y_pin));
  delay(10);
  */

   if(analogRead(X_pin) > 600)
  {
    if(analogRead(Y_pin) > 600)
    {
      Serial.println("HAUT DROITE");
    }
    else if(analogRead(Y_pin) <100)
    {
    Serial.println("BAS DROITE");
    }
    else
    {
       val = map(analogRead(X_pin), 0, 666, 0, 10);
      Serial.print("Droite : ");
      Serial.println(val);
    }
  }

   if(analogRead(X_pin) < 100)
  {
    if(analogRead(Y_pin) > 600)
    {
      Serial.println("HAUT GAUCHE");
    }
    else if(analogRead(Y_pin) <100)
    {
    Serial.println("BAS GAUCHE");
    }
    else
    {
       val = map(analogRead(X_pin), 0, 666, -10, 0);
      Serial.print("Gauche : ");
      Serial.println(val);
    }
  }

  if(analogRead(Y_pin) > 600)
  {
    Serial.println("HAUT");
  }
  else if( analogRead(Y_pin) < 100)
  {
    Serial.println("BAS");
  }

  delay(500);
  
}

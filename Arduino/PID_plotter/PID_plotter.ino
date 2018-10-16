
double pid = 0;
double valueToGo = 0;
double erreurCour = 0;
double erreur = 0;
double deltaErr = 0;
double somErr = 0;

  float EMA_a = 0.6;      //initialization of EMA alpha
 int EMA_S = 0;          //initialization of EMA S
 

double P =10;
double I = 0;
double D = 0;
double consigne = 0;
int i =0;

void setup() {
  // put your setup code here, to run once:
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(A1,INPUT_PULLUP);
  Serial.begin(115200);

   EMA_S = analogRead(A1);  //set EMA S for t=1
}


void loop() {

  valueToGo = 50;
  EMA_S = (EMA_a*analogRead(A1)) + ((1-EMA_a)*EMA_S);
  consigne = map(EMA_S, 500, 1000, 0, 100);
  erreur = erreurCour;
  Serial.print("Consigne :");
  Serial.println(consigne);
  
 
 
  
  erreurCour = abs(consigne - valueToGo);
  //Serial.print("EreurCour :");
  //Serial.println(erreurCour);
  //Serial.println("------");
  deltaErr = abs(erreurCour - erreur);
  somErr = somErr + erreurCour;
  
  pid = (P*erreur) + (I*somErr) + (D*deltaErr);
  //Serial.println(pid);
  if((consigne - valueToGo) >= 0)
  {
      analogWrite(11,pid);
      digitalWrite(12,LOW);
  }
  else if ((consigne - valueToGo) < 0)
  {
     analogWrite(12,pid);
     digitalWrite(11,LOW);
     //Serial.println(pid);
  }
  delay(10);

  
  //Serial.println(analogRead(A0));
}

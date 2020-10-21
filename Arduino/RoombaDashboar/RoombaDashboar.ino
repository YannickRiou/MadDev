#define BtnRoomba1 11
#define BtnRoomba2 10

#define LEDV 8
#define LEDR1 7
#define LEDR2 9
void setup() {
  // put your setup code here, to run once:
  pinMode(BtnRoomba1, INPUT);
  pinMode(BtnRoomba2, INPUT);
  pinMode(LEDV, OUTPUT);
  pinMode(LEDR1, OUTPUT);
  pinMode(LEDR2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(BtnRoomba2))
  {
   digitalWrite(LEDV,HIGH);
   digitalWrite(LEDR1,HIGH);
    digitalWrite(LEDR2,HIGH);
    delay(1000);
    digitalWrite(LEDV,LOW);
   digitalWrite(LEDR1,LOW);
    digitalWrite(LEDR2,LOW);
        delay(1000);
  }
    
}

#include <AccelStepper.h>
#include <MultiStepper.h>

/*
    réception d’une valeur integer via module nRF24L01
*/

#include <SPI.h> // gestion du bus SPI
#include <Mirf.h> // gestion de la communication
#include <nRF24L01.h> // définition des registres du nRF24L01
#include <MirfHardwareSpiDriver.h> // communication SPI nRF24L01

int valeur = 0;  // contient la valeur à recevoir
byte valeur_octet[2]; // contient la valeur découpée en octet pour la réception

AccelStepper stepper(1, 8, 9);

// Variable to track the position
int pos = 0;

void setup()
{
  Mirf.cePin = 6; // CE sur broche 6
  Mirf.csnPin = 7; // CSN sur broche 7
  Mirf.spi = &MirfHardwareSpi; // utilisation du port SPI hardware
  Mirf.init(); // initialise le module SPI
  Mirf.channel = 0; // utilisation canal 0 pour communiquer (128 canaux disponible, de 0 à 127)
  Mirf.payload = sizeof(unsigned int); // = 2, déclaration taille du message à transmettre, max 32 octets
  // RF_SETUP=0000abcd : a=1–>2Mb/s, a=0–>1Mb/s; puissance émission bc=00–>-18 dBm, bc=01–>-12dBm, bc=10–>-6dBm, bc=11–>0dBm;
  // d=0 pas de gain sur le bruit en réception
  Mirf.configRegister(RF_SETUP, 0x06); // 1 Mb/s et 0 dBm (puissance maximum)
  Mirf.config(); // configure le canal et la taille du message
  Mirf.setTADDR((byte *)"ardu1"); // définition adresse sur 5 octets de la 2ème carte Arduino
  Mirf.setRADDR((byte *)"ardu2"); // définition adresse sur 5 octets de la 1ere carte Arduino
  //Serial.begin(9600);
  stepper.setSpeed(10000);
  stepper.setAcceleration(6000);

  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
}

void loop()
{
  while (!Mirf.dataReady())
  {
    // pas prêt à recevoir
    delay(1);
  }

  Mirf.getData(valeur_octet); // récupére 2 octets
  valeur = valeur_octet[0] + (valeur_octet[1] << 8); // transforme en int
  //Serial.println(pos + valeur);
  if ((valeur > 0) && ((pos + valeur) < 570))
  {
    digitalWrite(A0, LOW);
    stepper.moveTo(pos + valeur);
  }
  else if ((valeur < 0) && ((pos + valeur) > 0))
  {
    digitalWrite(A0, LOW);
    stepper.moveTo(pos + valeur);
  }
  while (stepper.distanceToGo() != 0)
  {
    stepper.run();
  }
  digitalWrite(A0, HIGH);

  if (((valeur < 0) && (pos + valeur) > 0) || ((valeur > 0) && (pos + valeur) < 570))
  {
    pos = pos + valeur;
  }



  //Serial.print(" ");
  //Serial.print(valeur_octet[0]);
  //Serial.print(" ");
  //Serial.println(valeur_octet[1]);
  //delay(1000);
}

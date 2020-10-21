//591 step to make all of the dolly from one side to the other



// Microstepping settings
// MS1 ------ MS2 -------- MS3
//  0 ------   0 --------  0      FULL STEP
//  1 ------   0 --------  0      HALF STEP
//  0 ------   1 --------  0      QUARTER STEP
//  1 ------   1 --------  0      EIGHTH STEP
//  1 ------   1 --------  1      SIXTEENTH STEP
//----------------------------------------------
#define MS1 2
#define MS2 3
#define MS3 4

#define END_SWITCH 7
#define START_SWITCH 10

#define STEP 12
#define DIR 13

#define SHUTTER 5
#define FOCUS 9

#define FORWARD 1
#define BACKWARD -1

#define MAX_NUMBER_OF_FRAMES 2360 // Size of the dolly in steps

String rcvData = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

int positionInStep = 0;
int globalNumberOfPicture = 0;

int numberOfWantedImage = 0;
int delayBetweenFrames = 0;
int exposureTime = 0;

int delayTravMode = 0;

bool ledState = false;
bool runFlag = false;
bool isPaused = false;


enum Boolean {Init, None, Running, Stopped, Error};

int machineState = None;

void MakeStep(int numberOfStep, int dir, int withDelay);
void DollyConfiguration();
void MoveToPosition(int x);
void TakeAPicture();


void setup()
{
  Serial.begin(9600); // Bluetooth communication

  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);

  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);


  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);

  pinMode(END_SWITCH, INPUT_PULLUP);
  pinMode(START_SWITCH, INPUT_PULLUP);

  pinMode(SHUTTER, OUTPUT);
  pinMode(FOCUS, OUTPUT);

  machineState = Init;

  rcvData.reserve(200);
}

void loop()
{
  
  switch (machineState)
  {
    case Init :
      MoveToPosition(0);
      positionInStep =  0;
      globalNumberOfPicture = 0;
      numberOfWantedImage = 0;
      delayBetweenFrames = 0;
      machineState = None;
      break;

    case None :
      break;

    case Running :
   
        if(digitalRead(END_SWITCH) && (globalNumberOfPicture < numberOfWantedImage) && runFlag)
        {
          StartTimelapse(delayBetweenFrames);
        }
        else
        {
          // STOP THE MOTOR ENABLE !!! TODO 
          machineState = Stopped;
        }
     
      break;

    case Error :

      break;

  }
  
}

/*
  SerialEvent occurs whenever a new data comes in the
  hardware serial RX.  This routine is run between each
  time loop() runs, so using delay inside loop can delay
  response.  Multiple bytes of data may be available.
*/
void serialEvent()
{
  int numberOfWantedImageIndex = 0;
  int delayBetweenFramesIndex = 0;
  int exposureTimeIndex =  0;
  while (Serial.available())
  {
    // Read the char received
    char inChar = (char)Serial.read();

    // Add to the string
    rcvData += inChar;

    // End of the command
    if (inChar == '\n')
    {
      Serial.print("Received :");
      Serial.println(rcvData);
      stringComplete = true;
      if (rcvData.startsWith("go"))
      {
        numberOfWantedImageIndex = rcvData.indexOf("o") + 1;
        delayBetweenFramesIndex = rcvData.indexOf(",") + 1;
        exposureTimeIndex = rcvData.indexOf(',', delayBetweenFramesIndex)+1;
        
        numberOfWantedImage = (rcvData.substring(rcvData.indexOf("o") + 1)).toInt();
        delayBetweenFrames = rcvData.substring(rcvData.indexOf(",") + 1).toInt();
        exposureTime = rcvData.substring(exposureTimeIndex).toInt();
        
        if (numberOfWantedImage < MAX_NUMBER_OF_FRAMES &&  numberOfWantedImage < (MAX_NUMBER_OF_FRAMES - positionInStep))
        {
          Serial.print("ok\r\n");
          runFlag = true;
          machineState = Running;
        }
         else
        {
          Serial.println("err\r\n");
           machineState = Stopped;
        }
        
      }
      else if (rcvData.startsWith("move"))
      {
        MoveToPosition((rcvData.substring(rcvData.indexOf("e") + 1)).toInt());
        machineState = Stopped;
      }
      else if (rcvData.startsWith("stop"))
      {
        runFlag = false;
      }
      else if (rcvData.startsWith("pause"))
      {
        isPaused = !isPaused;
        Serial.print("Paused : ");
        Serial.println(isPaused); 
      }
      
      rcvData = "";
      stringComplete = false;
    }

  }
}

void StartTimelapse(int moveDelay)
{
    if(!isPaused)
    {
      MakeStep(10, FORWARD);
      delay(1000);
      TakeAPicture();
       delay(((moveDelay-1) * 1000));
    }
}

void MoveToPosition(int x)
{
  if (x == 0)
  {
    MakeStep(1200, BACKWARD); // Go all the way to the beginning to hit the Start Switch and reset step counter
  }
  else if (x > positionInStep && x < MAX_NUMBER_OF_FRAMES)
  {
    MakeStep(x - positionInStep, FORWARD);
  }
  else if (x < positionInStep && x > 0)
  {
    MakeStep(positionInStep - x, BACKWARD);
  }
}

void MakeStep(int numberOfStep, int dir)
{
  int i = 0;

  if (dir == FORWARD)
  {
    digitalWrite(DIR,HIGH);
    for (i = 0;( (i < numberOfStep) && digitalRead(END_SWITCH) ); i++)
    { 
      digitalWrite(STEP,HIGH); 
      delay(5); 
      digitalWrite(STEP,LOW); 
      delay(5); 
      positionInStep++;
    }
  }
  else
  {
    digitalWrite(DIR,LOW);
    for (i = 0;( (i < numberOfStep) && digitalRead(START_SWITCH) ); i++)
    {  
      digitalWrite(STEP,HIGH); 
      delay(5); 
      digitalWrite(STEP,LOW); 
      delay(5); 
      positionInStep--;
    }
  }
  if (!digitalRead(END_SWITCH))
  {
    Serial.println(positionInStep);
    positionInStep = MAX_NUMBER_OF_FRAMES;
  }

   if (!digitalRead(START_SWITCH))
  {
    positionInStep = 0;
  }

}

void TakeAPicture()
{
  digitalWrite(FOCUS, HIGH);
  delay(5);
  digitalWrite(SHUTTER, HIGH);
  delay(exposureTime*1000);
  digitalWrite(SHUTTER, LOW);
  digitalWrite(FOCUS, LOW);
  globalNumberOfPicture++;
}


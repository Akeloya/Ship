const byte buttonLRPin = A0;
const byte buttonUDPin = A1;
const byte buttonSwPin = 9;

const byte ledPin = 13;
const byte ledPin1 = 12;
const byte ledPin2 = 11;

const byte ButtonCnt = 5;
const byte buttonSw = 0;
const byte buttonR = 1;
const byte buttonL = 2;
const byte buttonU = 3;
const byte buttonD = 4;

bool gButtonKeyPressed[5];
bool gButtonKeyChangedState[5];
bool anyChanges;

void setup() {
  pinMode(buttonUDPin, INPUT);
  pinMode(buttonLRPin, INPUT);
  pinMode(buttonSwPin, INPUT);

  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  UpdateManipulationStates();

  if(keyDown(buttonSw))
    digitalWrite(ledPin, HIGH);

  if(keyUp(buttonSw))
    digitalWrite(ledPin, LOW);

  if(keyDown(buttonR) || keyDown(buttonD))
  {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
  }

  if(keyDown(buttonU) || keyDown(buttonL))
  {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
  }
  //if(anyChanges)
  {
    for(byte i = 0; i < ButtonCnt; i++)
      Serial.print(gButtonKeyPressed[i] + " ");
    Serial.println();
    for(byte i = 0; i < ButtonCnt; i++)
      Serial.print(gButtonKeyChangedState[i] + " ");
    Serial.println();
  }
}

void UpdateManipulationStates()
{
  anyChanges = false;
  //сохраним прошлые значения перед новым считыванием
  bool gButtonLastValue[5];
  for(byte i = 0; i < ButtonCnt; i++)
    gButtonLastValue[i] = gButtonKeyPressed[i];
  
  short lrState = analogRead(buttonLRPin);
  short udState = analogRead(buttonUDPin);
  
  gButtonKeyPressed[buttonL] = lrState == 1023;
  gButtonKeyPressed[buttonR] = lrState == 0;
  gButtonKeyPressed[buttonU] = udState == 1023;
  gButtonKeyPressed[buttonD] = udState == 0;
    
  gButtonKeyPressed[buttonSw] = digitalRead(buttonSwPin);
  /*Serial.print("[");
  Serial.print(lrState);
  Serial.print("] [");
  Serial.print(udState);
  Serial.print("] [");
  Serial.print(gButtonKeyPressed[buttonSw]);
  Serial.print("]");
  Serial.println();*/
  for(byte i = 0; i < ButtonCnt; i++)
  {
    gButtonKeyChangedState[i] = gButtonKeyPressed[i] != gButtonLastValue[i];
    if(gButtonKeyChangedState[i])
      anyChanges = true;
  }
}

bool keyUp(byte key)
{
  return gButtonKeyPressed[key] && gButtonKeyChangedState[key];
}

bool keyDown(byte key)
{
  return !gButtonKeyPressed[key] && gButtonKeyChangedState[key];
}

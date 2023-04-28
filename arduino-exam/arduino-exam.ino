const int POTEN_PIN = A1;
const int PHOTO_PIN = A3;

const int btn_A_PIN = 2;
const int btn_B_PIN = 3;

int btn_A_value = 0;
int btn_B_value = 0;

int previous_btn_A_value = 0;
int previous_btn_B_value = 0;

int myLEDs[2] = {5, 6};

int actuatorValue = 0;
int previousValue = 0;

void setup() {
  // put your setup code here, to run once:

/*___________________________________________

1) Set the mode for each Pin.
_____________________________________________ */ 
  pinMode(btn_A_PIN, INPUT_PULLUP);
  pinMode(btn_B_PIN, INPUT_PULLUP);
  
  Serial.begin(9600); 

}

void loop() {
  // put your main code here, to run repeatedly:
   if(Serial.available() > 0) {
    receivingData();
  } else {
    sendingData();
  }
  
  delay(100);
  
}

void sendingData() {
  
  /*___________________________________________

2) Read the value of each pin below
_____________________________________________ */ 

  actuatorValue = analogRead(POTEN_PIN);
  btn_A_value = digitalRead(btn_A_PIN);
  btn_B_value = digitalRead(btn_B_PIN);

  
    
  if (previousValue != actuatorValue) {
    sendSerialMessage(actuatorValue, btn_A_value, btn_B_value);
    previousValue = actuatorValue;
  }

  if (previous_btn_A_value != btn_A_value) {
    sendSerialMessage(actuatorValue, btn_A_value, btn_B_value);
    previous_btn_A_value = btn_A_value;
  }

    if (previous_btn_B_value != btn_B_value) {
    sendSerialMessage(actuatorValue, btn_A_value, btn_B_value);
    previous_btn_B_value = btn_B_value;
  }

  delay(100);
}

/*___________________________________________

3) Print the value of the actuator and the value of each button.

Example: 
Serial.print(value);
Serial.print(' ');
Serial.println();
_____________________________________________ */ 

void sendSerialMessage(int actuatorValue, int btn_A_value, int btn_B_value) {

  Serial.print(actuatorValue);
  Serial.print(' ');
  Serial.print(btn_A_value);
  Serial.print(' ');
  Serial.println(btn_B_value);
};

/*___________________________________________

4) Receive the messages sended by the Game.
S is for scoring: If you score a point, both leds should turn on and off
L is for loosing: If you loose, both leds should turn on
_____________________________________________ */ 

void receivingData() {
  char inByte = Serial.read();

  switch(inByte){
    case 'S':

      digitalWrite(myLEDs[0], HIGH);
      digitalWrite(myLEDs[1], HIGH);
      delay(1000);
      digitalWrite(myLEDs[0], LOW);
      digitalWrite(myLEDs[1], LOW);
    
      break;
      
    case 'L':
      digitalWrite(myLEDs[0], HIGH);
      digitalWrite(myLEDs[1], HIGH);

      break;
  }
  Serial.flush();
}

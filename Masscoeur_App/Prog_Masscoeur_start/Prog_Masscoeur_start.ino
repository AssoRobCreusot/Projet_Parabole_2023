/// For the project "Mass'coeur"

#define DRIVERPUL 7 //PUL - pin
#define DRIVERDIR 6 //DIR - pin
#define DRIVEROTH1 9
#define DRIVEROTH2 10

#define LEDPIN 5

void initMotor(int dir1, int dir2, int pull1, int pull2, boolean direction = LOW){
  /**
  dir1 : dir +
  dir2 : dir -
  pull1 : pull +
  pull2 : pull -
  */
  pinMode (dir1, OUTPUT);
  pinMode (dir2, OUTPUT);
  pinMode (pull1, OUTPUT);
  pinMode (pull2, OUTPUT);

  digitalWrite(dir1, HIGH);
  digitalWrite(pull1, HIGH);

  digitalWrite(dir2, direction);
}

void accelerate(int driverPul, int pulseDelay, int totalTime, int acceleration, int baseSpeed, int accelerationModulo = 2){
  /**
  driverDir : DIR -
  driverPul : PULL -
  pulseDelay : time between each pulsation (min is faster)
  totalTime : total number of milisecond, acceleration included
  acceleration : Number of diminution in pulse delay
  baseSpeed : pulseDelay at the beggining before acceleration
  */
  // Initialisation
  digitalWrite(LEDPIN, HIGH);
  int time = millis();
  int tmpTime = 0;
  int pd = baseSpeed;
  int iteration = 0;
  
  // Serial.println("Acceleration of the motor");


  while(tmpTime<= totalTime){
    // Making a step
    digitalWrite(driverPul,HIGH);
    delayMicroseconds(pd);
    digitalWrite(driverPul,LOW);
    delayMicroseconds(pd);
    
    // Acceleration of the pulse Delay
    if ((iteration % accelerationModulo) == 0){
      pd = pd - acceleration;
    }
    if (pd < pulseDelay){
      pd = pulseDelay;
    }
    //Checking if the time is still okay
    tmpTime = int(millis() - time);
    iteration += 1;
  }
  digitalWrite(LEDPIN, LOW);
}

  


void setup() {

  
  Serial.begin(9600);
  // Serial.println("-- Démmarrage du script"); 
  delay(2000) ;
  // Serial.println("Configuration du moteur");  
  initMotor(DRIVEROTH1, DRIVERDIR, DRIVEROTH2, DRIVERPUL );
  //accelerate(DRIVERPUL, 135, 10000, 1, 500);
  // accelerate(DRIVERPUL, 500, 10000, 1, 1000);
  //pinMode (activeLed, OUTPUT);
  
}
 
void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil(';'); // Read the input until the first ';'
    int startingSpeed = input.toInt(); // Convert the string to an integer for final speed
    input = Serial.readStringUntil(';'); // Read the next part of the input
    int finalSpeed = input.toInt(); // Convert the string to an integer for starting speed
    input = Serial.readStringUntil(';'); // Read the last part of the input
    input.remove(input.length() - 1); // Remove the 's' character at the end
    int timeInSeconds = input.toInt(); // Convert the string to an integer for time in seconds
    // Convert time to milliseconds
    int timeInMillis = timeInSeconds * 1000;
    if (timeInMillis > 1000 and finalSpeed < 1000 and startingSpeed > 450){
      Serial.println("start");
      accelerate(DRIVERPUL, finalSpeed, timeInMillis, 1, startingSpeed);
      Serial.println("finish");
    }else{
      Serial.println("Error command");
    }
    
    
  }
  
  
}
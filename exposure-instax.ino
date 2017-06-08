#define interrupt_shoot 2 

#define motor_enable_IN1  8
#define motor_enable_IN2  9

#define pot A0 //defining pot pin

float Tmax = 200;

static unsigned long timer;
static bool bShoot = false;
static bool bStart = false;
static bool bMotor1 = false;
static bool bMotor2 = false;
static bool bMotor3 = false;
static bool bMotor4 = false;

void setup() 
{    
  timer = 0;
  
  //Serial that will be use to debug
  Serial.begin(9600);
 
  pinMode(interrupt_shoot, INPUT);
  pinMode(pot, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(interrupt_shoot), interrupt_shoot_func, RISING);
    
  pinMode(motor_enable_IN1, OUTPUT);
  pinMode(motor_enable_IN2, OUTPUT);

  digitalWrite(motor_enable_IN1, 0);
  digitalWrite(motor_enable_IN2, 0);
}

//Function that will be associated with the encoder interrupts (Relogio)
void interrupt_shoot_func()
{
  if(!bShoot)
  {
    Serial.println("Interrupt");

    bShoot = true;
  }
  else
  {
    Serial.println("Interrupt busy"); 
  }
}


  
void loop() 
{ 
  float V = analogRead(pot) * Tmax / 1024;

  unsigned long t = (unsigned long)V; 
  
  if( (bShoot) && (!bStart) && (!bMotor1) && (!bMotor2) && (!bMotor3) && (!bMotor4) )
  { 
    Serial.println("Start Phase 1");

    bStart = true;
    
    timer = millis();
    digitalWrite(motor_enable_IN2, 1);
    bMotor1 = true;
  }
  else if( (bShoot) && (bStart) && (millis() > (timer + 1)) && (bMotor1) && (!bMotor2) && (!bMotor3) && (!bMotor4) )
  {
    Serial.println("Start Phase 2");

    timer = millis();
    digitalWrite(motor_enable_IN2, 0);
    bMotor2 = true;
  }
  else if( (bShoot) && (bStart) && (millis() > (timer + 11)) && (bMotor1) && (bMotor2) && (!bMotor3) && (!bMotor4) )
  {
    Serial.println("Start Phase 3");
    
    timer = millis();
    digitalWrite(motor_enable_IN1, 1);
    bMotor3 = true;
  }

  else if( (bShoot) && (bStart) && (millis() > (timer + t)) && (bMotor1) && (bMotor2) && (bMotor3) && (!bMotor4) )
  {
    Serial.print("Start Phase 4:");
    Serial.println(t);
    
    timer = millis();
    digitalWrite(motor_enable_IN1, 0);
    digitalWrite(motor_enable_IN2, 1);
    bMotor4 = true;
  }
  else if( (bShoot) && (bStart) && (millis() > (timer + 30)) && (bMotor1) && (bMotor2) && (bMotor3) && (bMotor4) )
  {
    Serial.println("Reset Interrupt");
    
    timer = millis();
    digitalWrite(motor_enable_IN2, 0);    
    bMotor1 = false;
    bMotor2 = false;
    bMotor3 = false;
    bMotor4 = false;
    //bShoot = false; 
  }
  else if( (bShoot) && (bStart) && (millis() > (timer + 3000)) && (!bMotor1) && (!bMotor2) && (!bMotor3) && (!bMotor4) )
  { 
    Serial.println("wait");
   
    timer = millis();

    bStart = false;
    bShoot = false;
  }
}



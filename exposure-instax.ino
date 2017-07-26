#define interrupt_shoot 2 

#define GND_FAKE  7
#define VCC_FAKE  6

#define motor_enable_IN1  8
#define motor_enable_IN2  9

#define pot A0 //defining pot pin

float exposure_time_max_ms = 3000;

unsigned long exposure_time_ms;

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

  pinMode(GND_FAKE, OUTPUT);
  pinMode(VCC_FAKE, OUTPUT);
    
  digitalWrite(motor_enable_IN1, 1);

  digitalWrite(GND_FAKE, 0);

  digitalWrite(VCC_FAKE, 1);

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
  if(!bStart)
  {
    float V = analogRead(pot) * exposure_time_max_ms / 1024;
  
    exposure_time_ms = (unsigned long)V;
  
    Serial.print("Exposure (ms): ");
    Serial.println(exposure_time_ms);

    if(exposure_time_ms < 20)
    {
      exposure_time_ms = 20;

      Serial.print("Exposure Fixed (ms): ");
      Serial.println(exposure_time_ms);      
    }
  }
  
  if( (bShoot) && (!bStart) && (!bMotor1) )
  { 
    //Serial.println("Start Phase 1 - open shooter");

    bStart = true;
    
    timer = millis();
    digitalWrite(motor_enable_IN1, 0);
    
    bMotor1 = true;
  }
  else if( (bShoot) && (bStart) && (millis() > (timer + exposure_time_ms)) && (bMotor1) )
  {
    //Serial.print("Start Phase 4 - close shooter ");
    //Serial.println(exposure_time_ms);
    
    timer = millis();

    digitalWrite(motor_enable_IN1, 1);
    
    bMotor1 = false;
  }
  else if( (bShoot) && (bStart) && (millis() > (timer + 3000)) && (!bMotor1) && (!bMotor2) && (!bMotor3) && (!bMotor4) )
  {     
    //Serial.println("Ready");
   
    timer = millis();

    bStart = false;
    bShoot = false;
  }
}



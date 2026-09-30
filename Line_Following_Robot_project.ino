#include "LSA08_Simplified.h"
#include "HardwareSerial.h"
 #include "CytronMotorDriver.h"

HardwareSerial LSA08Serial(2); 

#define LSA08_RX 16   
#define LSA08_TX 17   
#define UEN_PIN 22   

LSA08 sensor = LSA08(&LSA08Serial, 9600, LSA08_RX, LSA08_TX);

CytronMD m1(PWM_DIR, 13, 4); 
CytronMD m2(PWM_DIR, 14, 27);  

int x = 40;
int m_err = 20;
int o_max = 150;
int l,r;
 float Kp = 1.8;

float Kd = 3 ;
float Ki = 0;
int o,error;
int lastError = 0;
int integral = 0;
int c_pos,pos,derivative;

void setup() {
  Serial.begin(9600);

  pinMode(UEN_PIN, OUTPUT);
  digitalWrite(UEN_PIN, LOW);
  LSA08Serial.begin(9600, SERIAL_8N1, LSA08_RX, LSA08_TX);
  sensor.init();
  sensor.set_uart_mode(UART_MODE_ANALOG);
  sensor.set_line_mode(DARK_LINE);
  sensor.calibrate();
  delay(5000);

  Serial.println("LSA08 Initialized and Calibrated");

}

void loop() {
   delay(10);

  c_pos = sensor.read_line();
  if(c_pos != 255){
    pos = c_pos ;
  }
  error = pos - 35; 
  integral += error;
  derivative = error - lastError;
  o = Kp * error + Ki * integral + Kd * derivative;
  lastError = error;

  Serial.print("Position: ");
  Serial.print(pos);
  Serial.print("\tCorrection: ");
  Serial.println(o);
  o = constrain(o,-o_max,o_max);
if (error >= -m_err && error <= m_err){
  forward();
}
else{
  solve();
}

}
void forward(){
  m1.setSpeed(x - o);
  m2.setSpeed(x + o);
}

void solve(){
 if(error > m_err){
  m2.setSpeed(o*2);
  m1.setSpeed(0);
}
 else if(error < -m_err){
  m2.setSpeed(0);
  m1.setSpeed(-o*2);
 }
}
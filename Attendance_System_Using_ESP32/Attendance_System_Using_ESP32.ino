#include <WiFi.h>  
#include <SD.h>  
#include <sd_defines.h>  
#include <sd_diskio.h>  
#include "time.h"  
#include "sntp.h"  
#include "FS.h"  
#include <Keypad.h>  
#include "SPI.h"  
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>  
int PIN_SPI_CS = 5 ;  
const char* wifi              
const char* password          
const char* ntpServer1        
= "Luffy";             
= "asdfghjkl";       
= "pool.ntp.org";   
const char* ntpServer2        
= "time.nist.gov";  
const long  gmtOffset_sec      = 17998 - 1800;  
const int   daylightOffset_sec = 3600;  
const char* time_zone          
int intime_condition = 1 ;  
int outtime_condition = 0 ;  
int numpad ;  
int shri = 1 ;  
int time_delay = 1000 ;  
int pass_num ;  
= "CET-1CEST,M3.5.0,M10.5.0/3";  
char name[4][10] = { {"SHRIKANT"} , {"KHUSHI"} , {"ASAWARI"} , {"SHREYA"} } ;  
int passkey_in[4]    = {   204081   ,   204051    ,    204121    ,    204061   } ;  
int passkey_out[4]   = {   204080   ,   204050    ,    204120    ,    204060    } ;  
int val[6];  
int count ;  
struct tm timeinfo;  
File Attendance;  
int i;  
const byte ROWS = 4;  
const byte COLS = 3;  
int pass_size , check_append_1 , check_append_2 , position_reader ;  
int result ;  
char pass_char;  
char key ;  
int flag ;  
#define r1 12  
#define r2 14  
#define r3 27 
#define r4 26  
#define c1 25  
#define c2 33  
#define c3 32  
char keys[ROWS][COLS] = {  
{'1','2','3'},  
{'4','5','6'},  
{'7','8','9'},  
{'*','0','#'}  
};  
byte rowPins[ROWS] = { r1 , r2 , r3 , r4 };  
byte colPins[COLS] = { c1 , c2 , c3 };  
char* mystring = {};  
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );  
LiquidCrystal_I2C lcd(0x27, 20, 4);   
void timeavailable(struct timeval *t){  
struct tm timeinfo;  
}  
void setup(){  
lcd.init();   
lcd.backlight();  
lcd.setCursor(0, 1);  
Serial.begin(115200);  
Serial.setTimeout(10);  
sntp_set_time_sync_notification_cb( timeavailable );  
sntp_servermode_dhcp(1);  
configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);  
WiFi.begin(wifi,password);
while ( WiFi.status() != WL_CONNECTED ) {  
    delay(500);  
    lcd.print(".");  
    Serial.print(".");  
  }  
  Serial.println(" CONNECTED");  
  lcd.print("WiFi IS CONNECTED");  
  delay(time_delay*2);  
  lcd.clear();  
    
  lcd.print("SD CARD DID NOT CONNECT.");  
  delay(time_delay*2);  
  while (!SD.begin(PIN_SPI_CS)) {  
     Serial.println(" SD CARD FAILED, OR NOT PRESENT!");  
     delay(time_delay*2);  
  }  
  lcd.clear();  
  Serial.println("SD CARD INITIALIZED.");  
  lcd.print("SD CARD INITIALIZED.");  
  delay(time_delay*2);  
  lcd.clear();  
}  
  
void loop(){  
  if(SD.begin(PIN_SPI_CS)){  
    label:  
    struct tm timeinfo;  
    while(!getLocalTime(&timeinfo)){  
 lcd.setCursor(0, 0);  
      lcd.print("WAIT FOR TIME");  
     delay(500);  
     Serial.println("No time available (yet)");  
     lcd.clear();   
    }  
    if(getLocalTime(&timeinfo) && count == 0){  
      Serial.println("ready to record time");  
      delay(time_delay);  
      count = 1 ;  
    }  
    // ready to show time  
    key = keypad.getKey();  
    if(key == '#' && pass_size <= 4 ){  
      lcd.clear();  
      lcd.setCursor(0, 2);  
      lcd.print("INTIME(1)");  
      lcd.setCursor(0, 3);  
      lcd.print("OUTTIME(0)");  
      lcd.setCursor(0, 0);  
      lcd.print("PASSWORD:- ");  
      shri = 1 ;  
    }  
    else if ( shri == 1 ){  
      lcd.clear();  
      lcd.print("ENTER THE PASSWORD.");  
      Serial.print(&timeinfo,"%A, %B %d %Y %H:%M:%S") ;  
      shri = 0 ; 
        }  
    while (key == '#' && pass_size <=5 ){  
      pass_char = keypad.getKey()  ;  
      if(pass_char>0 && pass_char != 35 && pass_char != 42){  
        val[pass_size] = pass_char - 48  ;  
        lcd.printf( "%d" , int(val[pass_size]) ) ;  
        ++pass_size ;  
      }  
      pass_num = val[0]*100000 + val[1]*10000 + val[2]*1000 + val[3]*100 + val[4]*10 + 
val[5] ;  
      Serial.print("inside the loop ") ;  
      Serial.print(&timeinfo,"%A, %B %d %Y %H:%M:%S") ;  
      while(position_reader!=6){  
        Serial.print("  val[");  
        Serial.print(position_reader);  
        Serial.print("]:- ");  
        Serial.print(val[position_reader]);  
        ++position_reader;  
      }  
      position_reader=0;  
      Serial.print("    pass_size:- ") ;  
      Serial.print(pass_size) ;  
      Serial.print("    value:- ");  
      Serial.print(result);  
      Serial.print("    pass:- ") ;  
      Serial.print(int (pass_char)) ;  
      Serial.println();  
    }  
pass_size = 0 ;  
    while(position_reader!=6){  
      val[position_reader]=0;  
      position_reader++;  
    }  
    position_reader=0;  
    Serial.print(&timeinfo,"%A, %B %d %Y %H:%M:%S") ;  
    lcd.setCursor( 0, 2 ) ;  
    lcd.print(&timeinfo , "%A %B %d" );  
    lcd.setCursor( 0, 3 ) ;  
    lcd.print(&timeinfo , "%H:%M:%S" );  
    lcd.setCursor( 0 , 0 ) ;  
    Serial.print("    outside the loop") ;  
    Serial.print("    value:- ");  
    Serial.print(result);  
    Serial.print("    pass_size:- ") ;  
    Serial.println(pass_size) ;  
    i=0;  
  
    if(pass_num % 2 == intime_condition && pass_num != 0){  
      if(i<18 && pass_num != 0){  
        while(pass_num != passkey_in[i] && i < 18 ){  
          ++i;  
          if(i>=18){  
            pass_num = 0 ;  
            lcd.setCursor(0, 1);  
            lcd.print("Wrong password");  
            delay(time_delay); 
             lcd.setCursor(0,0);  
            lcd.clear();  
            flag == 0 ;  
            goto label;  
          }  
        }  
      }  
      if(pass_num = passkey_in[i] ){  
        pass_num = 0 ;  
        Serial.print(name[i]);  
        Serial.println( &timeinfo, "'s intime for %A%B%d%Y%H:%M:%S");  
        Serial.printf("Appending to file: /ATTENDANCE.txt\n");  
        File file = SD.open("/ATTENDANCE.txt", FILE_APPEND);  
        if(!file){  
          Serial.println("Failed to open file for appending");  
          lcd.print("failed to save data");  
          delay(time_delay);  
          }  
        if(file.print(name[i])){  
          Serial.println("name appended   ");  
          check_append_1 = 1 ;  
        }   
        else {  
          Serial.println("Append failed");  
          check_append_1 = 0 ;  
        }  
        if(file.print( &timeinfo, "'s intime for  %A%B%d%Y%H:%M:%S\n")){  
          Serial.println("time appended"); 
           check_append_2 = 1 ;  
        } else {  
          Serial.println("Append failed");  
          check_append_2 = 0 ;  
        }  
        file.close();  
        if(check_append_1 && check_append_2 ){  
          lcd.setCursor(0, 1);  
          lcd.print("Intime Data Saved");  
          delay(time_delay);  
          lcd.setCursor(0, 0);  
          lcd.clear();   
        }  
        else {  
          lcd.print("failed to save data");  
          delay(time_delay);  
        }  
      }  
    }  
    else if (pass_num % 2 == outtime_condition && pass_num !=0){  
      if(i<18 && pass_num != 0){  
        while(pass_num != passkey_out[i] && i < 18 ){  
          ++i;  
          if(i>=18){  
            pass_num = 0 ;  
            lcd.setCursor(0, 1);  
            lcd.print("Wrong password");  
            delay(time_delay); 
             lcd.setCursor(0,0);  
            lcd.clear();  
            flag == 0 ;  
            goto label;  
          }  
        }  
      }  
      if(pass_num = passkey_out[i]){  
        pass_num = 0 ;  
        Serial.print(name[i]);  
        Serial.println( &timeinfo, "'s outtime for  %A%B%d%Y%H:%M:%S");  
        Serial.printf("Appending to file: /ATTENDANCE.txt\n");  
        File file = SD.open("/ATTENDANCE.txt", FILE_APPEND);  
        if(!file){  
          Serial.println("Failed to open file for appending");  
          lcd.print("failed to save data");  
          delay(time_delay);  
          }  
        if(file.print(name[i])){  
          Serial.println("name appended   ");  
          check_append_1 = 1 ;  
        }   
        else {  
          Serial.println("Append failed");  
          check_append_1 = 0 ;  
        }  
        if(file.print( &timeinfo, "'s outtime for  %A%B%d%Y%H:%M:%S\n")){  
          Serial.println("time appended"); 
           check_append_2 = 1 ;  
        } else {  
          Serial.println("Append failed");  
          check_append_2 = 0 ;  
        }  
        file.close();  
        if(check_append_1 && check_append_2 ){  
          lcd.setCursor(0, 1);  
          lcd.print("Outtime Data Saved");  
          delay(time_delay);  
          lcd.setCursor(0, 0) ;  
          lcd.clear();  
        }  
        else {  
          lcd.print("failed to save data");  
          delay(time_delay);  
        }  
      }  
    }  
  } 
} 


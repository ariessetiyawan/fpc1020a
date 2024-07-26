#include <Arduino.h>

#include "fpc1020a.h"

FPC1020A finger;

bool add_user_process(uint8_t id, uint8_t permission) {
    for (uint8_t i = 0; i < 6; i++) {
        while (!finger.addFinger(id, permission, i)) {
            Serial.printf("Finger ID: %d Finger Record:%d error\r\n", id, i);
            Serial.println("Retry...");
            delay(1000);
        };
        Serial.printf("Finger ID: %d Finger Record:%d ok\r\n", id, i);
    }
    return true;
}

bool finger_detection (){
    //finger.getImage()
}

uint8_t new_user_id = 1;
uint8_t userNum = 0;
void setup() {
    // wait for unit boot
    delay(100);
    Serial.begin(115200);
    while (!finger.begin(&Serial2, 16, 17, 19200)) {
        Serial.println("FPC1020A not found");
        delay(1000);
    }
    
    if (finger.SetcompareLevel(5) != 5) {
      Serial.println("***ERROR***");
      Serial.println("Please ensure that the module power supply is 3.3V or 5V,");
      Serial.println("the serial line connection is correct.");
      Serial.println();
    }
    
    /*
    Serial.println("Delete All User");
    finger.delAllFinger();
    delay(1000);
    Serial.println("Start Add User Process");
    Serial.println("Now Keep Your Finger on the Sensor...");
    add_user_process(new_user_id, 1);
    delay(1000);
    // if you wanna check the debug message, you can enable it
    
    //finger.enableDebug(&Serial);
    */
    Serial.println("FPC1020A found");
    userNum = finger.getUserCount();
    Serial.print("userNum:");
    Serial.println(userNum);

}
void waitForFingerTouch()
{
    uint8_t fingered=1;
    do
    {
      fingered = finger.detectFinger(1000);
    } while (fingered != 1);

    Serial.println('ambil gambar jari!');
    //send_cmd(CMD_GET_IMAGE);
}
unsigned int User_ID = 0;

void loop() {
   
    unsigned char incomingNub;
    unsigned int  matchUserID = 0;
    unsigned char rtf = 0;
    
    while(1){
        Serial.println("============== Menu ================");
        Serial.println("Add a New User ----------------- 1");
        Serial.println("Fingerprint Matching --------------- 2");
        Serial.println("Get User Number and Print All User ID ------ 3 ");
        Serial.println("Delete Assigned User --------- 4");
        Serial.println("Delete All User ---------- 5");
        Serial.println("============== End =================");
        
        unsigned char  MODE = 0;
        
        while(Serial.available()<=0);
        
        MODE = Serial.read()-0x30;
        Serial.print("MODE: ");
        Serial.println(MODE);
        switch(MODE){
          case 0:  // Null
              break;
          
          case 1:   // Fingerprint Input and Add a New User
              MODE = 0;
              User_ID= finger.getUserCount();
              Serial.print("userNum:");
              Serial.println(User_ID);
              if (User_ID==0){
                  User_ID=new_user_id;
              } else {
                User_ID++;
              }
              Serial.println("Add Fingerprint, please put your finger on the Fingerprint Sensor.");
              while(Serial.available()<=0);
              delay(100);
              incomingNub = Serial.read()-0x30;
              Serial.print("MODE: ");
              Serial.println(MODE);
              add_user_process(User_ID, 1);
              Serial.printf("Sucess Add Finger ID: %d:\r\n",User_ID);
              delay(100);
              if (finger.available() == ACK_SUCCESS) {
                      Serial.println("Success");
                      Serial.print("User ID: ");
                      Serial.println(finger.getFingerID());
                      Serial.print("User Permission: ");
                      Serial.println(finger.getFingerPermission());
                      
              } 
              delay(100);
              userNum = finger.getUserCount();
              Serial.print("userNum:");
              Serial.println(userNum);
              delay(2000);
              
              break;
          case 2:  // Fingerprint Matching
             MODE = 0 ; 
             Serial.println("Match Fingerprint, please put your finger on the Sensor.");  
             delay(50);         
             while (1){                        
                  if (finger.available() == ACK_SUCCESS) {
                      Serial.println("Success");
                      Serial.print("User ID: ");
                      Serial.println(finger.getFingerID());
                      Serial.print("User Permission: ");
                      Serial.println(finger.getFingerPermission());
                      break;
                  } 
                  else if (finger.available() == ACK_NOUSER) {
                      Serial.println("Fingerprint not found!\r\n");
                      break;
                  } else if (finger.available() == ACK_TIMEOUT) {
                      Serial.println("Timeout...!\r\n");
                      break;
                  }
                  delay(1000);
             }
             delay(1000);
             break;         
          case 3:   // Print all user ID
             MODE = 0;
             Serial.println("Print All User");
             //finger.delAllFinger();
             while(1){
                while(Serial.available()<=0);
                unsigned char  DEL_ID = 0;
                DEL_ID = Serial.read()-0x30;
                delay(20);
                Serial.print("DEL_ID : ");
                Serial.println(DEL_ID);

                DEL_ID = Serial.read();
                delay(20);
                Serial.print("DEL_ID : ");
                Serial.println(DEL_ID);
                break;
             }
             delay(1000);
             break;
             
          case 4:   // Delete Assigned User ID
             MODE = 0;
             User_ID = 0;
             Serial.println("Please input the user ID(0 ~ 99) you want to delecte.");
             while(1){
                while(Serial.available()<=0);
                unsigned char  DEL_ID = 0;
                DEL_ID = Serial.read()-0x30;
                delay(20);
                Serial.print("DEL_ID : ");
                Serial.println(DEL_ID);

                DEL_ID = Serial.read();
                delay(20);
                Serial.print("DEL_ID : ");
                Serial.println(DEL_ID);

                if (finger.delFinger(DEL_ID)){
                    Serial.printf("Fingerprint User ID: %d success deleted.\r\n",DEL_ID);
                    break;
                } else {
                    Serial.println("Deleted Fingerprint failed!\r\n");
                    break;
                }
                
             }
             delay(1000);
             break;
          case 5:  // Delete All User ID
             MODE = 0;
             unsigned char DeleteFlag = 0;
             Serial.println("Delete All Users, Y/N ?");
                while(1){
                  while(Serial.available()<=0);
                  DeleteFlag = Serial.read();
                  delay(20);
                  if (DeleteFlag > 10){
                      Serial.printf("DeleteFlag : available %d \r\n",DeleteFlag);
                      if(DeleteFlag == 89|| DeleteFlag == 121){
                          if(finger.delAllFinger()){
                            Serial.println("Delete All Fingerprint User Success!");
                          }
                          else {
                            Serial.println("Delete All Fingerprint User Fail!");
                          }
                          delay(100);
                          userNum = finger.getUserCount();
                          Serial.print("userNum:");
                          Serial.println(userNum);
                          delay(1000);
                          break;
                      }
                      else if(DeleteFlag == 78|| DeleteFlag == 110){
                        Serial.println("Exit menu Delete!");
                        delay(20);
                        break;
                      }         
                  }
                  delay(1000);
                }
              delay(500);
              break;
     }
    }

}
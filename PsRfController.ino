#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <PS2X_lib.h>

#define CE_PIN 10
#define CSN_PIN 9

#define PS2_DAT        6  //14    
#define PS2_CMD        4  //15
#define PS2_SEL        5  //16
#define PS2_CLK        3  //17

PS2X ps2x; // create PS2 Controller Class
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

//---------------------------constants---------------------------------
uint8_t controle[22] = {0};
const byte address[6] = "00001";
int error = 0;
byte type = 0;
const uint8_t buffer_size = sizeof(controle);

void setup(){
  // Serial.begin(115200);
  // Serial.println("Start");

  //ps2x - setup pins and settings: 
  //config_gamepad(clock, command, attention, data, Pressures?, Rumble?)
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, false, false);
  //rf------------------------------
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  // radio.setDataRate(RF24_2MBPS);
  radio.stopListening();
  // radio.disableAckPayload();
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
  // type = ps2x.readType(); 
  // switch(type) {
  //   case 0:
  //     Serial.print("Unknown Controller type found ");
  //     break;
  //   case 1:
  //     Serial.print("DualShock Controller found ");
  //     break;
  //   case 2:
  //     Serial.print("GuitarHero Controller found ");
  //     break;
	//   case 3:
  //     Serial.print("Wireless Sony DualShock Controller found ");
  //     break;
  //  }
}

void loop(){
    if(error) //skip loop if no controller found
    {
      delay(300);
      return; 
    } 
    
    
    /* You must Read Gamepad to get new values
      You should call this at least once a second
    */ 
    ps2x.read_gamepad();
    if (ps2x.NewButtonState())
    {   
        digitalWrite(0, HIGH);    
        controle[0]=ps2x.Button(PSB_L1);
        controle[1]=ps2x.Button(PSB_R1);
        controle[2]=ps2x.Button(PSB_L2);
        controle[3]=ps2x.Button(PSB_R2);
        controle[4]=ps2x.Button(PSB_L3);
        controle[5]=ps2x.Button(PSB_R3);         
         
        controle[6]=ps2x.Button(PSB_PAD_UP);
        controle[7]=ps2x.Button(PSB_PAD_DOWN); 
        controle[8]=ps2x.Button(PSB_PAD_LEFT);
        controle[9]=ps2x.Button(PSB_PAD_RIGHT);

        controle[10]=ps2x.Button(PSB_RED);            //Circle
        controle[11]=ps2x.Button(PSB_PINK);           //Square
        controle[12]=ps2x.Button(PSB_BLUE);           //X  
        controle[13]=ps2x.Button(PSB_GREEN);          //Triangle
 
        controle[14]=ps2x.Button(PSB_START);                   
        controle[15]=ps2x.Button(PSB_SELECT);
    }
    
    controle[16]=ps2x.Analog(PSS_LY);
    controle[17]=ps2x.Analog(PSS_LX);
    controle[18]=ps2x.Analog(PSS_RY);
    controle[19]=ps2x.Analog(PSS_RX);
    controle[20]=ps2x.Analog(PSAB_BLUE);
    controle[21]=ps2x.Analog(PSAB_PINK);

    // controle[16]=1;
    // controle[17]=2;
    // controle[18]=3;
    // controle[19]=4;
    // controle[20]=5;
    // controle[21]=6;
    
    radio.write(&controle, sizeof(controle)); 
    delay(100);  
    digitalWrite(0,LOW);
}

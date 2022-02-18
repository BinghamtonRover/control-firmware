#include <FlexCAN_T4.h>
#include <Chrono.h>

// (NODE ID << 5) | COMMAND
const uint16_t DATA_PACKET1_ID = (0x07<<5) | 0x1C;
const uint16_t DATA_PACKET2_ID = (0x07<<5) | 0x1D;
const int READ_TIME = 100; // milliseconds
const int SEND_TIME = 500; // milliseconds

uint16_t PS12volt_raw, PS5volt_raw, PS12curr_raw, PS5curr_raw, PSBATT_raw, odrv1_curr_raw, odrv2_curr_raw, odrv3_curr_raw, temp12_raw, temp5_raw;
uint8_t PS12volt, PS5volt, PS12curr, PS5curr, PSBATT, odrv1_curr, odrv2_curr, odrv3_curr, temp12, temp5;
Chrono read_millis(Chrono::Resolution::MILLIS), send_millis(Chrono::Resolution::MILLIS);
uint8_t buf1[8];
uint8_t buf2[8];

typedef FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> CAN_Type; // TX = 22, RX = 23
//typedef FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> CAN_Type; // TX = 1, RX = 0
//typedef FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> CAN_Type; // TX = 31, RX = 30
CAN_Type can;

void print_message(const CAN_message_t &msg) {
  Serial.print("TS: "); Serial.print(msg.timestamp);
  Serial.print(" MB: "); Serial.print(msg.mb);
  Serial.print(" OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print(" LEN: "); Serial.print(msg.len);
  Serial.print(" RTR: "); Serial.print(msg.flags.remote);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" Buffer: ");
  if (msg.flags.remote) Serial.println("REMOTE FRAME");
  else{
    for ( uint8_t i = 0; i < msg.len; i++ ) {
      Serial.print(msg.buf[i], HEX); Serial.print(" ");
    } Serial.println();
  }
}

// function for writing frames to the bus
void writeBus(CAN_Type& can, uint8_t* data, uint16_t ID, int dataSize) {
  CAN_message_t msg; // instantiate message object
  
  int i = 0;
  for (uint8_t* ptr = data; ptr < data + dataSize; ptr++){
    msg.buf[i] = *ptr;
    i++;
  }
  
  msg.id = ID; // set CAN identifier

  can.write(msg);
}

void setup(){
  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.begin(9600);
  can.begin();
  can.setBaudRate(500000);
  can.enableMBInterrupts();
  
  can.setMBFilter(REJECT_ALL);
  
  can.mailboxStatus();
  read_millis.start();
  send_millis.start();
}

void loop(){
  can.events();

  if (read_millis.hasPassed(READ_TIME)){
    PS12volt_raw = analogRead(39);
    PS12volt = (uint8_t)(PS12volt_raw*(12/1023)*10); // raw * scaling factor * 10
    PS5volt_raw = analogRead(38);
    PS5volt = (uint8_t)(PS5volt_raw*(5/1023)*10); // raw * scaling factor * 10
    PS12curr_raw = analogRead(17);
    PS12curr = (uint8_t)(PS12curr_raw*(12/1023)*10); // raw * scaling factor * 10
    PS5curr_raw = analogRead(14);
    PS5curr = (uint8_t)(PS5curr_raw*(5/1023)*10); // raw * scaling factor * 10
    PSBATT_raw = analogRead(40);
    PSBATT = (uint16_t)(PSBATT_raw*(27/1023)*10); // raw * scaling factor * 10
  
    odrv1_curr_raw = analogRead(24);
    odrv2_curr_raw = analogRead(25);
    odrv3_curr_raw = analogRead(26);

    temp12_raw = analogRead();
    temp12 = (uint8_t)(temp12_raw*(125;
    temp5_raw = analogRead();
    temp5 = ;
    
    Serial.println("Sensor Values Taken");
    read_millis.restart();
  }

  if (send_millis.hasPassed(SEND_TIME)){
    memcpy(buf1, &PS12volt, 2);
    memcpy(buf1+2, &PS5volt, 2);
    memcpy(buf1+4, &PS12curr, 2);
    memcpy(buf1+6, &PS5curr, 2);
    writeBus(can, buf1, DATA_PACKET1_ID, 8);
    Serial.println("Packet 1 Sent");
    memcpy(buf2, &PS12volt, 2);
    memcpy(buf2+2, &PS5volt, 2);
    memcpy(buf2+4, &PS12curr, 2);
    memcpy(buf2+6, &PS5curr, 2);
    writeBus(can, buf2, DATA_PACKET2_ID, 8);
    Serial.println("Packet 2 Sent");
    send_millis.restart();
  }
    
  
}

#include <FlexCAN_T4.h>
#include <Chrono.h>

// (NODE ID << 5) | COMMAND
const uint16_t DATA_PACKET1_ID = (0x00<<5) | 0x1C;
const uint16_t DATA_PACKET2_ID = (0x00<<5) | 0x1D;
const int READ_TIME = 100; // milliseconds
const int SEND_TIME = 500; // milliseconds

uint16_t PS12volt_raw, PS5volt_raw, PS12curr_raw, PS5curr_raw, PSBATT_raw, PSBATT, odrv1curr_raw, odrv2curr_raw, odrv3curr_raw, main_curr_raw, temp12_raw, temp5_raw;
uint8_t PS12volt, PS5volt, PS12curr, PS5curr, odrv1curr, odrv2curr, odrv3curr, main_curr, temp12, temp5;
Chrono read_millis(Chrono::Resolution::MILLIS), send_millis(Chrono::Resolution::MILLIS);
uint8_t buf1[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t buf2[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

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
    PSBATT_raw = analogRead(40);
    PSBATT = (uint16_t)(PSBATT_raw*(3.3/1023)*(25.9/3.3)*10); // raw * analog scaling * voltage scaling * 10
    PS12volt_raw = analogRead(39);
    PS12volt = (uint8_t)(PS12volt_raw*(3.3/1023)*(12/2.5)*10); // raw * analog scaling * voltage scaling * 10
    PS5volt_raw = analogRead(38);
    PS5volt = (uint8_t)(PS5volt_raw*(3.3/1023)*(5/2.5)*10); // raw * analog scaling * voltage scaling * 10
    PS12curr_raw = analogRead(17);
    PS12curr = (uint8_t)(PS12curr_raw*(3.3/1023)*(12/1.32)*10); // raw * analog scaling * current scaling * 10
    PS5curr_raw = analogRead(14);
    PS5curr = (uint8_t)(PS5curr_raw*(3.3/1023)*(5/.55)*10); // raw * analog scaling * current scaling * 10
    
    temp12_raw = analogRead(19);
    temp12 = (uint8_t)(10*(.5-temp12_raw*(3.3/1023))/0.01);
    temp5_raw = analogRead(16);
    temp5 = (uint8_t)(10*(.5-temp5_raw*(3.3/1023))/0.01);;
    
    odrv1curr_raw = analogRead(24);
    odrv1curr = (uint8_t)(odrv1curr_raw*(5/122)*10);
    odrv2curr_raw = analogRead(25);
    odrv2curr = (uint8_t)(odrv2curr_raw*(5/122)*10);
    odrv3curr_raw = analogRead(26);
    odrv3curr = (uint8_t)(odrv3curr_raw*(5/122)*10);

    main_curr_raw = analogRead(27);
    main_curr = (uint8_t)(main_curr_raw*(5/122)*10);
    
    Serial.println("Sensor Values Taken");
    read_millis.restart();
  }

  if (send_millis.hasPassed(SEND_TIME)){
    memcpy(buf1, &PSBATT, 2);
    memcpy(buf1+2, &PS12volt, 1);
    memcpy(buf1+3, &PS5volt, 1);
    memcpy(buf1+4, &PS12curr, 1);
    memcpy(buf1+5, &PS5curr, 1);
    memcpy(buf1+6, &temp12, 1);
    memcpy(buf1+7, &temp5, 1);
    writeBus(can, buf1, DATA_PACKET1_ID, 8);
    Serial.println("Packet 1 Sent");
    
    memcpy(buf2, &odrv1curr, 1);
    memcpy(buf2+1, &odrv2curr, 1);
    memcpy(buf2+2, &odrv3curr, 1);
    memcpy(buf2+3, &main_curr, 1);
    writeBus(can, buf2, DATA_PACKET2_ID, 8);
    Serial.println("Packet 2 Sent");
    
    send_millis.restart();
  }
    
  
}

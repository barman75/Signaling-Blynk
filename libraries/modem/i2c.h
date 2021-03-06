//**********************************************************
//************************** I2C ***************************
//**********************************************************
bool esp8266_enable = false;
bool sim800_enable = false;

#define I2C_OFF   digitalWrite(A3,LOW);esp8266_enable=false;
#define I2C_ON    digitalWrite(A3,HIGH);
#define I2C_RESET {I2C_OFF;delay(2000);I2C_ON}

#include <Wire.h>

#define I2C_INIT i2c_init();i2c_rx_buf=text;i2c_tx_buf=email_buffer;i2c_enable=1;dtmf0=DTMF;I2C_RESET

TEXT* i2c_tx_buf;
TEXT* i2c_rx_buf;

uint8_t i2c_enable;
uint16_t* dtmf0;

extern MY_SENS *sensors;

// function that executes whenever data is received from master
void receiveEvent(int numBytes)
{
  char buf[BUFFER_LENGTH];
  uint8_t i = 0;
  uint8_t count = 0;
  uint8_t size;
  char* p;
  bool sens_flag = false;  
  i2c_cmd *cmd;
  uint16_t val;

  while (0 < Wire.available() && i < BUFFER_LENGTH) {
    buf[i] = (uint8_t)Wire.read();
    //DEBUG_PRINT(' ');DEBUG_PRINT(buf[i]);DEBUG_PRINT('|');DEBUG_PRINT(buf[i],HEX);
    i++;
  }

  count = i;
  i = 0;

  while(i < count)
  {
    if(count - i >= sizeof(i2c_cmd))
    {
      cmd = (i2c_cmd*)&buf[i]; 

      if(cmd->type == I2C_DATA)
      {
        i+=sizeof(i2c_cmd);

        switch (cmd->cmd)
        {
          case I2C_SENS_VALUE:
            i2c_sens_value sens_value;
            sens_value.type = cmd->type;
            sens_value.index = cmd->index;
            sens_value.value = sensors->GetValueByIndex(cmd->index);
            sens_value.enable = sensors->GetEnable(cmd->index);
            size = sizeof(i2c_sens_value);
            p = (char*)&sens_value;
            for(i = 0; i < size; i++)
            {
              i2c_tx_buf->AddChar(*(p+i));
            }      
          break; 
          case I2C_SENS_INFO:
            if(GET_FLAG(GUARD_ENABLE))
            {
              i2c_tx_buf->AddText_P(PSTR(I2C_NAME));            
              for(uint8_t k = 0; k < sensors->size; k++)
              {
                i2c_tx_buf->AddChar('%');
                i2c_tx_buf->AddText(sensors->GetNameByIndex(k));              
              }
              i2c_tx_buf->AddChar('\n');
            } 
          break;
          case I2C_FLAG_NAMES:
            i2c_tx_buf->AddText_P(PSTR(I2C_FLAG));
            i2c_tx_buf->AddChar('%');           
            i2c_tx_buf->AddText_P(PSTR(ALARM_NAME));
            i2c_tx_buf->AddChar('%');
            i2c_tx_buf->AddText_P(PSTR(GUARD_NAME));
#if SIRENA_ENABLE
            i2c_tx_buf->AddChar('%');
            i2c_tx_buf->AddText_P(PSTR(SIREN_NAME));
            i2c_tx_buf->AddChar('%');
            i2c_tx_buf->AddText_P(PSTR(SIREN2_NAME));      
#endif          
            if (sim800_enable)
            {
              i2c_tx_buf->AddChar('%');
              i2c_tx_buf->AddText_P(PSTR(EMAIL_NAME));
              i2c_tx_buf->AddChar('%');
              i2c_tx_buf->AddText_P(PSTR(SMS_NAME));
              i2c_tx_buf->AddChar('%');
              i2c_tx_buf->AddText_P(PSTR(RING_NAME));
            }
            i2c_tx_buf->AddChar('\n');                    
          break;
          case I2C_FLAGS:
            if(GET_FLAG(GUARD_ENABLE))
            {
              if(bitRead(cmd->index,GUARD_ENABLE)==0)
              {
                bitWrite(cmd->index,GUARD_ENABLE,HIGH);
                *dtmf0 = GUARD_OFF;
              } 
            }
            else
            if(bitRead(cmd->index,GUARD_ENABLE))
            {
              bitWrite(cmd->index,GUARD_ENABLE,LOW);
              *dtmf0 = GUARD_ON;            
            }
            flags = cmd->index;
          break;
          case I2C_SENS_ENABLE:
            val = cmd->index;
            p = (uint8_t*)&val;
          //  Serial.print(F("val=")); Serial.print(val, HEX);
          //  Serial.print(F("index=")); Serial.print((int)*(p+1), HEX);
          //  Serial.print(F(" enable=")); Serial.println((int)*p, HEX);
            sensors->SetEnable(*(p+1), (bool)*p);
          break;
          case I2C_DTMF:
            *dtmf0 = cmd->index;           
        }
        continue;
      }
    }
    i2c_rx_buf->AddChar(buf[i++]);    
  }
}

// function that executes whenever data is requested from master
void requestEvent()
{
  uint8_t i;
  uint8_t k;
  uint8_t l;
  char buf[32];

  esp8266_enable = true;

  memset(buf, 0, 32);

  for(i = 0; i < 2; i++) buf[i] = START_BYTE;
  
  buf[i++] = flags;
  //memcpy(buf+i, &flags, sizeof(flags));
  //i+=sizeof(flags);

  i+=i2c_tx_buf->GetBytes(buf+i, 26, false);

  for(k = 0; k < 2; k++) buf[i++] = END_BYTE;

  //memset(buf+i, 0, BUFFER_LENGTH-i);

  Wire.write(buf, 32);
}

void i2c_init()
{ 
  pinMode(A3,OUTPUT);
  digitalWrite(A3, HIGH); // Включаем ESP8266
  Wire.begin(ARDUINO_I2C_ADDR); /* join i2c bus with address */
  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent); /* register request event */
}

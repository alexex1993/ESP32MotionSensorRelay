#include <TM1637Display.h>
#include <FastBot.h>
#include <config.h> //add config.h


#define CLK  21 
#define DIO  20

#define Sensor 0
#define Relay 1


#define WIFI_SSID ENV_WIFI_SSID
#define WIFI_PASS ENV_WIFI_PASS
#define BOT_TOKEN ENV_BOT_TOKEN
#define BOT_CHAT_ID ENV_BOT_CHAT_ID

FastBot bot(BOT_TOKEN);

TM1637Display display = TM1637Display(CLK, DIO);

TaskHandle_t SensorReadTask;
TaskHandle_t RelayHandlerTask;

int duration = 0;
int duration_clear = 0;
bool Detection = false;

void connectWiFi() {
  delay(2000);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
}

void setup() 
{
  Serial.begin(115200);

  connectWiFi();

  display.clear();
  display.setBrightness(5); // set the brightness to 7 (0:dimmest, 7:brightest)


  pinMode(Sensor, INPUT_PULLDOWN);        // Указываем вывод, как вход  
  pinMode(Relay, OUTPUT_OPEN_DRAIN);

  xTaskCreatePinnedToCore(
                  SensorReadTaskCode,   /* Task function. */
                  "SensorReadTask",     /* name of task. */
                  10000,       /* Stack size of task */
                  NULL,        /* parameter of the task */
                  1,           /* priority of the task */
                  &SensorReadTask,      /* Task handle to keep track of created task */
                  0);          /* pin task to core 0 */  

  xTaskCreatePinnedToCore(
                    RelayHandlerTaskCode,   /* Task function. */
                    "RelayHandlerTask",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &RelayHandlerTask,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */

}

  void SensorReadTaskCode(void * pvParameters )
  {
    Serial.print("SensorReadTask running on core ");
    Serial.println(xPortGetCoreID());
    for(;;)
    {
      Detection = digitalRead(Sensor); // Считываем показания с порта
      delay(1000);
    }
  }

  void RelayHandlerTaskCode(void * pvParameters )
  {
    Serial.print("RelayHandlerTask running on core ");
    Serial.println(xPortGetCoreID());
    
    for(;;)
    {
      if(Detection == HIGH){
        Serial.println("Motion detected! Duration: " + String(duration) + " sec"); 
        display.showNumberDec(duration);
        digitalWrite(Relay, LOW);
        duration++;
        duration_clear = 0;
      }                    

      if(Detection == LOW){                  
        Serial.println("Clear");
        digitalWrite(Relay, HIGH);
        
        
        if (duration_clear == 0){
          bot.sendMessage("Motion was detected! Duration: " + String(duration), BOT_CHAT_ID);
        }
        duration = 0;
        duration_clear++;
      }             
      delay(1000);
      display.clear();
    }

  }

void loop() {}
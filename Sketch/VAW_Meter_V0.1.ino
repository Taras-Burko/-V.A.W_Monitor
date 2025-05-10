#include <Arduino.h>
#include "esp32-hal-ledc.h" // бібліотека esp32 для PWM
#include "INA228.h"
INA228 INA(0x40); // Адреса INA228
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "PNG_file.h" // файл з зображеннями  PNG для візуалізації
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
#include "AiEsp32RotaryEncoder.h"
#include <EEPROM.h>

#include "NotoSansBold15.h"     //Файл з плавними шрифтами
#include "NotoSansBold36.h"     //Файл з плавними шрифтами
#include "Unicode_Test_72.h"    //Файл з плавними шрифтами


// Stock font and GFXFF reference handle
#define GFXFF 1
#define FF18 &FreeSans12pt7b

// Custom are fonts added to library "TFT_eSPI\Fonts\Custom" folder
// a #include must also be added to the "User_Custom_Fonts.h" file
// in the "TFT_eSPI\User_Setups" folder. See example entries.
#define CF_OL24 &Orbitron_Light_24
#define CF_OL32 &Orbitron_Light_32
#define CF_RT24 &Roboto_Thin_24
#define CF_S24  &Satisfy_24
#define CF_Y32  &Yellowtail_32

//                        Налаштування  роботи  Охолодження 
//====================================================================================
 int Temp_min = 29 ; // Минимальная температура при которой начнет работать ШИМ вентилятора.
 int Temp_max = 60 ; // Температура при которой скорость вентилятора будет максимальной.
 int pwm_min =20;    // мінімальне значення шим при якому  починає обертитись вентилятор. 
 int pwm_max =255;   // максимальне значення для вентилятора. 
 int PWM_freq=5000;    // Частота PWM. при високих частотах вентилятор може некореткно працювати. Частоту потрібно підбирати індивідуально.

//                          Налаштування  Терморезистора  
//====================================================================================
 #define B 3950 // B-коєфіцієнт терморезистора 
 #define SERIAL_R 100 // Опір резистора R14 на платі, 100 кОм
 #define THERMISTOR_R 100// Номінальний опір терморезистора , 100 кОм
 #define NOMINAL_T 25 // номінальна  температура (при якій  терморезистор = 100 кОм)
  


#define ROTARY_ENCODER_A_PIN 25
#define ROTARY_ENCODER_B_PIN 26
#define ROTARY_ENCODER_BUTTON_PIN 35
#define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */

#define ROTARY_ENCODER_STEPS 4
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, -1, ROTARY_ENCODER_STEPS);
void IRAM_ATTR readEncoderISR(){
	rotaryEncoder.readEncoder_ISR();
  }

 
  float cicle;
  int x,y;
  char PWM0;
  int PWM,PWM_power;
  uint32_t ms_display,ms_led,ms_PWM,ms_SP,Time;
  bool flag_led,Accumulation_flag,flag_enc_but,flag_enc;
  float Voltage,Current,Power,Temperature,Charge,Energy,Voltege_fil,Curent_fil,Set_voltege;
  char Voltage_display[10]; 
  char Current_display[10]; 
  char Power_display[8]; 
  char Temperature_display[8]; 
  char Charge_display[12];
  char Energy_display[12];
  char PWM_display[6];
  char Set_voltege_display[10];
  char cicle_display[10];
 


// #define Enc_A_pin       25    // Енкодер фаза А
 //#define Enc_B_pin       26    // Енкодер фаза В
// #define Enc_but_pin     35    // Кнопка енкодера
 #define button_pin      14    // Кнопка 
 #define led_pin         34     // індикація світлодіода
 #define temp_pin        32    // пін для терморезистора.
 #define PWM_pin         33    // порт для керування PWM вентилятора
 #define relay_pin       12    // реле

//====================================================================================
//                                    Setup
//====================================================================================
void setup() {
 Serial.begin(115200);
 Wire.begin(); 
  if (!INA.begin() ) // Ініціалізація INA228
  {
    Serial.println("INA228 not connect."); // Якщо ініціалізація відсутня виводити помилку
    while(1);
  }

//                        Налаштування  роботи  INA228
//====================================================================================

 INA.setMode(0x0F); // Режим роботи, безперевне вимірювання напруги , шунта ,тепмератури = 0x0F
 INA.setBusVoltageConversionTime(2);   // Час перетворення  АЦП вольтметра us  50=0,84=1,150=2,280=3,540=4,1052=5,2074=6,4120=7 us 
 INA.setShuntVoltageConversionTime(2); // Час перетворення  АЦП Амперметра  50,84,150,280,540,1052,2074,4120 us
 INA.setTemperatureConversionTime(2);   // Час перетворення  АЦП температури 50,84,150,280,540,1052,2074,4120 us
 INA.setAverage(6); // Кількість вибірок 1=0,4=1,16=2,64=3,128=4,254=5,512=6,1024=7 
 INA.setMaxCurrentShunt(12, 0.0024923); // Задати  значчення шунта ( ток , опір шунта);
 //INA.setADCRange(0);  // flag = 0 => 164 мВ, 1 => 41 мВ
 INA.setTemperatureCompensation(1); //Увімкнути температурну компенсацію шунта. 0 - Вимкнути , 1-Увімкнути .
 INA.setShuntTemperatureCoefficent(50); // Коєфіцієнт температурної компенсації щунта, визначається в  ppm/°C. Його можна знайти в мануалі на шунт.  Переважно це значення від 50 ppm/°C до 200 ppm/°C взалежності від типу шунта який використовується. 
 
//====================================================================================


 tft.init();                   // ініціалізація дисплею . 
 tft.setRotation(3);           // Орієнтація  дисплею.
 tft.fillScreen(TFT_BLACK);    // Залити дисплей .

 pinMode(led_pin , OUTPUT);    // LED індикації статусу роботи 
 pinMode(relay_pin , OUTPUT);  // Дискретний сигнал на вихід. 
 //pinMode(Enc_but_pin, INPUT );  // Вхід кнопки.
 pinMode(temp_pin, INPUT );    // Вхід АЦП терморезистора.
 attachInterrupt(button_pin, Reset_Accumulation, FALLING);  // Переривання для скидання лічильника є мності та енергії.Кнопка енкодера.
 ledcAttach(PWM_pin, PWM_freq, 8);                           // Налаштовуємо PWM (канал , частота , розширеня 1-16)
 //ledcAttachChannel(led_pin, 0); //// Налаштовуємо PWM (порт PWM виходу , канал)
 Accumulation_flag =  0;
analogReadResolution(11); // Розщирення АЦП ESP32
 
 rotaryEncoder.begin();
 rotaryEncoder.setup(readEncoderISR);
 rotaryEncoder.setBoundaries(0, 1000, false); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
 rotaryEncoder.setAcceleration(250);

 Display_ramka();
 EEPROM.begin(512);
 //Set_voltege = EEPROM.read(0);  // Чтение данных
 EEPROM.get(0, Set_voltege);// - читает данные по адресу и сам записывает их в данные - указанную переменную
}
//====================================================================================
//                                    Loop
//====================================================================================
void loop() {
  if ( Accumulation_flag ==1  ){  // Якщо кнопка нажата  скидати  лічильник.
   INA.setAccumulation(1);
   Serial.println("setAccumulation");
   Accumulation_flag =0; 
  }
  Voltage=INA.getBusVoltage();
  Current=INA.getCurrent();
  Power=INA.getPower();
  Charge=INA.getCharge();
  Charge/=3.6 ;     // Конвертуємо кулони в mA/h
  Energy=INA.getWattHour();
  //Energy=INA.getMicroJoule();

  //if(Voltege < 0.00099) {Voltege=0;}
  //if(Curent < 0.00099) {Curent=0;}

  //Voltege_fil += (Voltege - Voltege_fil) * 0.05; // Фільтрація напруги 
  //Voltege = Voltege_fil ;
  //Curent_fil += (Curent - Curent_fil) * 0.05;    // Фільтрація току 
  //Curent = Curent_fil ;
  

 dtostrf( Voltage    , 6, 3, Voltage_display);
 dtostrf( Current    , 6, 3, Current_display);
 dtostrf( Power      , 7, 3, Power_display);
 dtostrf( Temperature, 4, 1, Temperature_display);
 dtostrf(  Charge    , 8, 2, Charge_display);
 dtostrf(  Energy    , 8, 2, Energy_display);
 dtostrf( PWM_power  , 5, 0, PWM_display);
 dtostrf( Set_voltege, 6, 2, Set_voltege_display);
 dtostrf( cicle      , 5, 1, cicle_display);
  S_P();          // Вивід даних в СОМ порт.
  Display();      // Візуалізація на  дисплей 
  //Led_blink();    // LED Статус роботи
  speed_fan (); //  Керування охолодженям.
  rotary_loop();
  led_cicle();
  if ( Voltage<=Set_voltege && flag_enc_but==1){
   digitalWrite(relay_pin, HIGH);
  }
   else{digitalWrite(relay_pin, LOW);
   }

   if (flag_enc==1 ){
     EEPROM.put(0, Set_voltege);  // Запись данных
     EEPROM.commit();   // Сохранение изменений
     //EEPROM.end();  // Освобождение ресурсов
     flag_enc=0;
    }

}

void Reset_Accumulation () {
  Accumulation_flag =1;
}

void Led_blink () {
  if (( millis()- ms_led ) >= 200) {
   flag_led = !flag_led;
   digitalWrite(led_pin, flag_led);
   ms_led=millis();
  } 
}

void speed_fan (){

if (( millis()- ms_PWM ) >= 1000) {
 uint32_t t = analogReadMilliVolts( temp_pin ); // Зчитуємо  значення AЦП на порту ESP32.
 Temperature = t * SERIAL_R /(3300 - t )  ; // Опір терморезистора 
 //Serial.println   ( Temperature );
 Temperature = log(Temperature / THERMISTOR_R); // ln(R/Ro)
 Temperature /= B; // 1/B * ln(R/Ro)
 Temperature += 1.0 / (NOMINAL_T + 273.15); // + (1/To)
 Temperature = 1.0 / Temperature; // Invert
 Temperature -= 273.15;
     //Розраховуємо начення ШИМ вентилятора  в залежності від температури терморезистора. 
  if (Temperature >= Temp_min && Temperature <= Temp_max )  {
   PWM =  ((Temperature - Temp_min ) * (pwm_max - pwm_min) / ( Temp_max - Temp_min )) + pwm_min ; }
   else if (Temperature < Temp_min)  { PWM = 0;}
   else if (Temperature >= Temp_max)  {PWM = 255;}
   ms_PWM=millis();
   ledcWrite(PWM_pin, PWM); // (канал , значення PWM)
   if (PWM>0){
     PWM_power  = 100*(PWM-pwm_min)/(pwm_max-pwm_min); // Розрахунок  потущності ШИМ для візуалізації.
    }
  }
}
void led_cicle(){
 cicle=1000/((millis()-Time));
 Time = millis();
 //char cicle_display[6];
}
void Display(){
  //if (( millis()- ms_display ) >= 100) {
   // We can set the text datum to be Top, Middle, Bottom vertically and Left, Centre
   // and Right horizontally. These are the text datums that can be used:
   // TL_DATUM = Top left (default)
   // TC_DATUM = Top centre
   // TR_DATUM = Top right
   // ML_DATUM = Middle left
   // MC_DATUM = Middle centre <<< This is used below
   // MR_DATUM = Middle right
   // BL_DATUM = Bottom left
   // BC_DATUM = Bottom centre
   // BR_DATUM = Bottom right
   // L_BASELINE = Left character baseline (Line the 'A' character would sit on)
   // C_BASELINE = Centre character baseline
   // R_BASELINE = Right character baseline

   tft.setTextDatum(TL_DATUM);   // Позиція тексту
   //tft.fillScreen(TFT_BLACK);  // Заповнити чорним
   tft.setTextColor(TFT_WHITE, TFT_BLACK); // Колір тексту  та Колір фону
   
   tft.loadFont(Unicode_Test_72);  // Активація шрифту  
  
   if( Voltage < 0 ){ x = 54;}
   else if ( Voltage >= 10 ){ x = 38;
   }
   else { x = 60 ;
   tft.fillRect(46,12,14,55,ILI9341_BLACK);  // перекриття залишків символів
    }
   tft.setTextColor(TFT_BLUE, TFT_BLACK, true);
   tft.setCursor(x,10);         // 
   tft.println(Voltage_display);
   
   if( Current < 0 ){ x = 54;}
   else if ( Current >= 10 ){ x = 38;}
   else { x = 60 ; }
   
   tft.setTextColor(TFT_RED, TFT_BLACK, true);
   tft.setCursor(x,78);         // 
   tft.println(Current_display);
   if ( 0 <= Current && Current < 10 ){           //Перекриття "-" 
   tft.fillRect(50,100,25,20,ILI9341_BLACK ); 
   }

   tft.loadFont( NotoSansBold36 );    // Активація шрифту  
  
    tft.setTextColor(TFT_YELLOW, TFT_BLACK, true);
   tft.drawString(Power_display, 110, 141);
   tft.unloadFont();

   tft.setTextColor(TFT_WHITE, TFT_BLACK);
    
   tft.drawString(Charge_display, 45, 174, 2);// Print the test text in the custom font
   
   tft.drawString(Energy_display, 202,174, 2);// Print the test text in the custom font
   
   tft.drawString(Temperature_display, 33,210, 2);// Print the test text in the custom font
   
   tft.drawString(PWM_display, 100,210, 2);// Print the test text in the custom font
   
   tft.drawString(cicle_display, 180,210, 2);// Print the test text in the custom font

    if (flag_enc_but ==1 ) {
       tft.drawString(Set_voltege_display, 250 ,210, 2);// Print the test text in the custom font
   tft.drawString("V", 300, 210, 2);// Print the string name of the font
    }
    else {
      
      tft.fillRect(240, 200, 70, 30, TFT_BLACK);//x, y, w, h, color
    }
  
  


}

void Display_ramka(){
tft.drawWideLine(0, 0, 320,0,2, TFT_WHITE, TFT_BLACK);
    tft.drawWideLine(0, 0, 0,320 ,2, TFT_WHITE, TFT_BLACK);
    tft.drawWideLine(0, 238, 320,238,2, TFT_WHITE, TFT_BLACK);
    tft.drawWideLine(318, 0, 318,238,2, TFT_WHITE, TFT_BLACK);

     tft.drawWideLine(0, 197, 320,195,2, TFT_WHITE, TFT_BLACK);

     tft.drawString("%", 92, 213, 2);// Print the string name of the font
     //tft.drawString("°C", 50, 213, 2);// Print the string name of the font
     tft.drawString("Wh", 260, 174, 2);// Print the string name of the font
     tft.drawString("mA/h", 100, 174, 2);// Print the string name of the font

    tft.loadFont( NotoSansBold36 );
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.drawString("V",275,20);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("A",275,90);
    tft.unloadFont();
    tft.loadFont( NotoSansBold15 );
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString("W",240,155);
    tft.unloadFont();
    tft.setSwapBytes(true);
    tft.pushImage(145,203,30, 30, setting_image);
    tft.pushImage(2,203,30, 30, celsius_image);
    tft.pushImage(70,203,30, 30, fan_image);
}
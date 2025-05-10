 void S_P () {
  if (( millis()- ms_SP ) >= 1000) {
    
    Serial.print("Voltege="); 
    Serial.print(Voltage );
    Serial.print("    "); 
    
    Serial.print("Curent=");
    Serial.print(Current, 3);
    Serial.print("   ");
  
    Serial.print("Power=");
    Serial.print(Power, 3);
    Serial.print("   "); // Serial.print("ShuntVoltage=");

    Serial.print("Volt_Shunt=");
    Serial.print(INA.getShuntMilliVolt());
    Serial.print("   "); // Serial.print("ShuntVoltage=");

    
    Serial.print("MaxCurrent=");
    Serial.print(INA.getMaxCurrent(),5);
    Serial.print("   "); // Serial.print("ShuntVoltage="); 

    Serial.print("Shunt=");
    Serial.print( INA.getShunt(), 5);
    Serial.print("   ");
     
    
    Serial.print("ADCRange=");
    Serial.print( INA.getADCRange());
    Serial.print("   ");

    Serial.print("CurrentLSB=");
    Serial.print( INA.getCurrentLSB(),12 );
    Serial.print("   ");
     
     Serial.print("Charge=" );
    Serial.print(Charge,4  );
    Serial.print("   ");

    Serial.print("Energy=");
    Serial.print(Energy),5 ;
    Serial.println("   ");
    
    ms_SP=millis(); 
  }
 }
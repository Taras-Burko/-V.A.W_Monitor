void rotary_onButtonClick()
{
	static unsigned long lastTimePressed = 0;
	//ignore multiple press in that time milliseconds
	if (millis() - lastTimePressed < 100)
	{
		return;
	}
	lastTimePressed = millis();
	Serial.print("button pressed ");
	Serial.print(millis());
	Serial.println(" milliseconds after restart");
}

void rotary_loop()
{
	//dont print anything unless value changed
	if (rotaryEncoder.encoderChanged() )
	{
		//Serial.print("Value: ");
		//Serial.println(rotaryEncoder.readEncoder());

		Set_voltege=rotaryEncoder.readEncoder();
		Set_voltege/=100;
		Set_voltege+=Encoder_memory;
		if(Set_voltege<0 ){
			Set_voltege=0;
		}
		flag_enc=1;
		
	}
	if (rotaryEncoder.isEncoderButtonClicked())
	{
		rotary_onButtonClick();
    flag_enc_but=!flag_enc_but;
	}
}


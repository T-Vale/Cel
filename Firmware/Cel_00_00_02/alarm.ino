void alarm_watcher()
{
  read_value();
  for(int adc = 0; adc < MAXIMUMNUMBEROFADCS; adc++)
  {
    if(actuationEnable_[adc] && adcValue_[adc] >= 500)
    {
      if(!adcAlarmState_[adc])
      {
        digitalWrite(actuationPinout_[adc], HIGH);
        delay(500);
        digitalWrite(actuationPinout_[adc], LOW);
        adcAlarmState_[adc] = true;
      }
    }
    else if(actuationEnable_[adc] && adcValue_[adc] < 500)
    {
      if(adcAlarmState_[adc])
      {
        adcAlarmState_[adc] = false;
      }
    }
  }
}


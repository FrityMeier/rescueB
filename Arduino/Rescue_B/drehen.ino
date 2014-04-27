int pos_alt;
int pos_neu;
int prev_rate = 0;
double Winkel=0;

void angle() {                                                    //aktuelle Drehposition bestimmen
  if(millis()-time > sampleTime){
    time = millis();
    gyro.read();
    rate=((int)gyro.g.z-dc_offset)/100;
    if (rate < -2 || rate > 2){
      Winkel += ((double)(prev_rate+ rate) * sampleTime) / 2000;
    }
  prev_rate = rate;
  if (Winkel < 0)
    Winkel += 360;
  else if (Winkel >= 360)
  Winkel-= 360;
  } 
}

void drehen_links(){
  angle();                                  
  pos_alt = Winkel;                                                 //Die Drehposition vor Beginn des Drehvorgangs sei pos_alt
  while (pos_alt - pos_neu < 80 && pos_alt - pos_neu > -80){        //Ist die Differenz die Differenz der beiden Positionen (pos_alt und pos_neu) kleiner als der Betrag von 90°, dreht sich der Roboter
    angle();                                                        //(80° statt 90°, da der Motor eine gewisse Zeit zum An- und Ausschalten benötigt)
    pos_neu = Winkel;
    dreheLinks(100);
  }
  Off(3);
}

void drehen_rechts(){
  angle();
  pos_alt = Winkel;
  while (pos_alt - pos_neu < 80 && pos_alt - pos_neu > -80){
    angle();
    pos_neu = Winkel;
    dreheRechts(100);
  }
  Off(3);
}

void drehen_herum(){
  angle();
  pos_alt = Winkel;
  while (pos_alt - pos_neu < 170 && pos_alt - pos_neu > -170){
    angle();
    pos_neu = Winkel;
    dreheRechts(100);
  }
  Off(3);
}

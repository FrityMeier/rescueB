#include <Wire.h>
#include <L3G.h>

L3G gyro;
long summe=0;
double noise=0;
unsigned long time;
int sampleTime=10;
int rate;
int dc_offset=0;
int abstand_rechts_hinten;
int abstand_rechts_vorne;
int abstand_diff_rechts;


void setup() {
  
  Serial.begin(57600);
  Wire.begin();
  if (!gyro.init()){
    Serial.println("Kein Gyro erkannt!");
    while (1);
  }
  gyro.enableDefault();
  stepper_begin();
  int sampleNum=500;
  for(int n=0;n<sampleNum;n++){
    gyro.read();
    dc_offset+=(int)gyro.g.z;
  }
  dc_offset=dc_offset/sampleNum;
  for(int n=0;n<sampleNum;n++){
  gyro.read();
  if((int)gyro.g.z-dc_offset>noise)
    noise=(int)gyro.g.z-dc_offset;
  else if((int)gyro.g.z-dc_offset<-noise)
    noise=-(int)gyro.g.z-dc_offset;
  }
  noise=noise/100;
  }

void loop() {
  if(Serial.available()){
    char c = Serial.read();   //'c' ist der Wert, den der Arduino über die serielle Schnittstelle empfängt
    if(c == 'r'){
      drehen_rechts();        //empfängt der Arduino 'r', dreht sich der Roboter um 90° nach rechts
    }
    if (c == 'l'){
      drehen_links();        //empfängt der Arduino 'l', dreht sich der Roboter um 90° nach links
    }
    if (c == 'u'){
      drehen_herum();        //empfängt der Arduino 'u', dreht sich der Roboter um 180°
    }
    if (c == 'v'){          
      feld_vor();            //empfängt der Arduino 'v', fährt er 30cm nach vorne
    }
    if (c == 'e'){
      erkunden();            //empfängt der Arduino 'e', erkundet er, ob er Wände um sich herum hat.
    }                        // 1. Wert --> linke Seite
                             // 2. Wert --> vorne                            
                             // 3. Wert --> rechte Seite
  }                          // '1' bedeutet Wand, '0' bedeutet keine Wand
}

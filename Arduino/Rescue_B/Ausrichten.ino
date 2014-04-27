void ausrichten_rechts(){
  int n = 0;
  int summe = 0;
  while(n < 2){
    summe += ir30Read(A6) - ir30Read(A7);
    delay (50);
    n++;
    }
  summe /= 3;
  while(summe < -5){
    dreheRechts(100);
    delay(30);
    summe += ir30Read(A6) - ir30Read(A7);
    }
  while(summe > 5){
    dreheLinks(100);
    delay(30);
    summe += ir30Read(A6) - ir30Read(A7);
    }
    Off(3);
  }
  
void ausrichten_links(){
  int n = 0;
  int summe = 0;
  while(n < 2){
    summe += ir30Read(A2) - ir30Read(A3);
    delay (50);
    n++;
    }
  summe /= 3;
  if(summe < -5){
    dreheLinks(100);
    delay(30);
    Off(3);
    }
  if(summe > 5){
    dreheRechts(100);
    delay(30);
    Off(3);
    }
  }

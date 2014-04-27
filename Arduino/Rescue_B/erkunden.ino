int wand_links = 0;
int wand_rechts = 0;
int wand_vorne = 0;
int dist_links = 0;
int dist_rechts = 0;

void erkunden(){
  dist_rechts = (ir30Read(A6)+ir30Read(A7))/2;
  dist_links = (ir30Read(A2)+ir30Read(A3))/2;
  if(dist_links < 180){
    wand_links = 1;
  }
  else wand_links = 0;
  
  if(dist_rechts < 180){
    wand_rechts = 1;
  }
  else wand_rechts = 0;
  
  if(usRead(A0) < 90){
    wand_vorne = 1;
  }
  else wand_vorne = 0;
  Serial.print(wand_links);
  Serial.print(wand_vorne);
  Serial.println(wand_rechts);
}
    
  

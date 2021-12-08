void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:


  //Check for Date

  //Schedule
  if (day ==1){
    move_liquid(enzyme, chamA, 0.06);
    move_liquid(chamA, 2.94);
  }

  else if (day ==2){
    move_liquid(revival, chamB, 20);
    move_liquid(chamB, waste, 20);
    move_liquid(chamB, waste, 20);
  }

    
  else if (day ==3):
    move_liquid(chamA, chamB, 2);
    move_liquid(buffer, chamA, 1.96);
    move_liquid(enzyme, chamA, 0.04);
  
  else if (day ==4){
    move_liquid(chamB, pre1, 4);
  }

  else if (day ==5):
    move_liquid(chamA, chamB, 2);
    move_liquid(medium, chamB, 2);
    move_liquid(buffer, chamA, 1.96);
    move_liquid(enzyme, chamA, 0.04);
  
  else if (day ==6):
    move_liquid(day, chamB, pre1, 4);

  else if (day ==7):
    move_liquid(chamA, chamB, 2);
    move_liquid(medium, chamB, 2);
    move_liquid(buffer, chamA, 1.96);
    move_liquid(enzyme, chamA, 0.04);

  else if (day ==8){
    move_liquid(day, chamB, pre1, 4);
    }

  else if (day ==9){
    move_liquid(chamA, chamB, 2);
    move_liquid(medium, chamB, 2);
    move_liquid(buffer, chamA, 1.96);
    move_liquid(enzyme, chamA, 0.04);

  else if (day ==10){
    move_liquid(day, chamB, pre1, 4);
  }
  

}

void move_liquid(source, target, volume) 
  {
    
  
  
  }

void valve_control(valve_name, id) 
  {
    
  
  
  }

  
}

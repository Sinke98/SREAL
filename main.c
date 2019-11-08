#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

  // Inicijalizacija programa
  
  FILE *f_prekidac;
  FILE *f_tasteri;
  FILE *f_ledovke;
  char *str;
  char pr1;
  char pritisnut1 = 0;
  char pritisnut2 = 0;
  char t0, t1, t2, t3;
  size_t br_bajtova = 6;
  float procenat = 0;
  long int perioda = 20000L;

  // Glavni deo

  while(1){

    // Ocitavanje vrednosti prekidaca "SW0"

    f_prekidac = fopen("/dev/switch", "r");

    if(f_prekidac == NULL){
      puts("Problem pri otvaranju /dev/switch");
      return -1;  
    }
  
    str = (char*)malloc(br_bajtova+1);

    getline(&str,&br_bajtova,f_prekidac);

    if(fclose(f_prekidac)){
      puts("Problem pri zatvaranju /dev/switch");
      return -1;
    }

    pr1 = str[2] - '0';

    free(str);

    // Ocitavanje vrednosti tastera "BTN0", "BTN1", "BTN2" i "BTN3"

    f_tasteri = fopen("/dev/button", "r");

    if(f_tasteri == NULL){
      puts("Problem pri otvaranju /dev/button");
      return -1;
    }

    str = (char*)malloc(br_bajtova+1);

    getline(&str,&br_bajtova,f_tasteri);

    if(fclose(f_tasteri)){
      puts("Problem pri zatvaranju /dev/button");
      return -1;
    }

    t0 = str[2] - '0';
    t1 = str[3] - '0';
    t2 = str[4] - '0';
    t3 = str[5] - '0';  
    
    free(str);

    // Impulsno sirinska modulacija

    // Ako je prekidac "SW0" aktivan
    if(pr1 == 1){

      f_ledovke = fopen("/dev/led", "w");

      if(f_ledovke == NULL){
        puts("Problem pri otvaranju /dev/led");
        return -1;
      }

      fputs("0xf\n",f_ledovke);

      // Ako je taster "BTN0" aktivan
      if(t0 == 1 && pritisnut1 == 0){
        
        pritisnut1 = 1;

        if(pritisnut1 == 1){

          // Povecanje faktora ispune za 20%
          procenat += 0.2;
        }
      }  
      // Ako je taster "BTN1" aktivan
      if(t1 == 1 && pritisnut2 == 0){
        
        pritisnut2 = 1;

        if(pritisnut2 == 1){

          // Smanjenje faktora ispune za 20%
          procenat -= 0.2;
        }
      }
      // Ako je pritisnut taster "BTN2"
      if(t2 == 1){
     
        // Postavi faktor ispune na 0%
        procenat = 0;
      }
      // Ako je taster "BTN3" aktivan
      if(t3 == 1){
       
        // Postavi faktor ispune na 100%
        procenat = 1;
      }

      usleep(procenat*perioda);

      fputs("0x0\n",f_ledovke);

      if(fclose(f_ledovke)){
        puts("Problem pri zatvaranju /dev/led");
        return -1;
      }

      usleep((1-procenat)*perioda);

      if(t0 == 0){
        
        pritisnut1 = 0;
      }

      if(t1 == 0){
        
        pritisnut2 = 0;
      }  

    }
      
  }

}

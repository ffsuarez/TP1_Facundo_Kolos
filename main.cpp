//===[Libraries]===
#include "mbed.h"
#include "arm_book_lib.h"

//===[Declaration and initialization of public global objects]===
AnalogIn sensorLDR(A0);
DigitalIn pushButton(D0);
DigitalOut focoDesk(D1);
DigitalOut lockSignal(D2,ON);
class Lockhandle {
    private:
        bool time1, time2;
    public:
        void update(){
            time1 = pushButton.read();
            wait_ms(100);//ms
            time2 = pushButton.read();
            if(time1 == time2){
                lockSignal = OFF;
            }
            else{
                lockSignal = ON;
            }            
        }
};
Lockhandle cerradura;
//===[Declaration and Initialization of public global variables]===
const int umbral = 100;
int lecturaLDR = 0;
//===[Declaration and Initialization of public functions]===
void inputsInit();
void outputsInit();
void sensorLEDUpdate();

//==[Main function]===
int main(){
    
    inputsInit();
    outputsInit();
    while(true){
        sensorLEDUpdate();
        cerradura.update();
    }
    return(0); 
}
//===[Implementation of public functions]===
void inputsInit(){
    pushButton.mode(PullUp);
}

void outputsInit(){
    focoDesk = OFF;
    lockSignal = ON;
}

void sensorLEDUpdate(){
    lecturaLDR = sensorLDR.read();
    if(lecturaLDR > umbral){
        focoDesk = ON;
    }
    else{
        focoDesk = OFF;
    }
}


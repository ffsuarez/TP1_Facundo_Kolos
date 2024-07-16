//===[Libraries]===
#include "mbed.h"
#include "arm_book_lib.h"

//===[Declaration and initialization of public global objects]===
AnalogIn sensorLDR(PA_0);
DigitalIn pushButton(D2);
DigitalOut focoDesk(D3);
DigitalOut lockSignal(D4,ON);
class Lockhandle {
    private:
        bool time1;
        bool time2;
    public:
        void update(){
            time1 = pushButton.read();
            wait_ms(100);//ms
            time2 = pushButton.read();
            if(time1 == time2){
                if(time1==OFF){
                    lockSignal = ON;
                }
                else{
                    lockSignal = OFF;
                }
            }
            else{
                lockSignal = OFF;
            }            
        }
};
Lockhandle cerradura;
Serial uartUsb(USBTX,USBRX,115200);
//===[Declaration and Initialization of public global variables]===
float umbral= 0.5;
float lecturaLDR;
//===[Declaration and Initialization of public functions]===
void inputsInit();
void outputsInit();
void sensorLEDUpdate();
void uartTask();
void availableCommands();
//==[Main function]===
int main(){
    
    inputsInit();
    outputsInit();
    umbral= sensorLDR.read();
    while(true){
        sensorLEDUpdate();
        cerradura.update();
        uartTask();
    }
    return(0); 
}
//===[Implementation of public functions]===
void inputsInit(){
    pushButton.mode(PullUp);
}

void outputsInit(){
    focoDesk = OFF;
    lockSignal = OFF;
}

void sensorLEDUpdate(){
    lecturaLDR = sensorLDR.read();
    if(lecturaLDR < umbral){
        focoDesk = OFF;
    }
    else{
        focoDesk = ON;
        uartUsb.printf("Sensor value: %f\r\n", lecturaLDR);

    }
}

void uartTask(){
    char receivedChar = '\0';
    if(uartUsb.readable()){
        receivedChar=uartUsb.getc();
        if(receivedChar=='1'){
            uartUsb.printf("focoDesk ON\r\n");
            focoDesk=ON;
            wait_ms(200);
        }
        else{
            availableCommands();
        }
    }
}

void availableCommands(){
    uartUsb.printf("Available commands1:\r\n");
    uartUsb.printf("Press '1' to turn ON the light\r\n");
}
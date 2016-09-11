#include <iostream>
#include <SDL.h>
#include <stdio.h>
#include <wiringPi.h>

using namespace std;

int main (void)
{
wiringPiSetup() ; // De esta forma se usaron los pines wiringPi

//Representación del teclado matricial
int matriz [4][4] = {{1,2,3,'A'},
                     {4,5,6,'B'},
                     {7,8,9,'C'},
                     {'*',0,'#','D'}};
//wPi                   Corresponden a los pines del header respectivamente
int fila [] = {0,1,2,3};  //11,12,13,15
int columna [] = {4,5,6,21}; // 16,18,22,29

for (int j=0; j<4; j++){
    pinMode (columna[j], OUTPUT);
    digitalWrite(columna[j],HIGH);
}
for(int i=0; i<4; i++){
    pinMode (fila[i], INPUT);
    pullUpDnControl(fila[i],PUD_UP);
}

    while(true){
        for (int j=0; j<4; j++){
            digitalWrite(columna[j],LOW);

            for(int i=0; i<4; i++) {
                if (digitalRead(fila[i])==0)
                    cout << matriz[i][j]<<endl;
                    while (digitalRead(fila[i])==0)
                        continue;
            }

            digitalWrite(columna[j],HIGH);
            }
    }
}





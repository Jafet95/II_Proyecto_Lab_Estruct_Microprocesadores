// SnakeGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <iostream>
#pragma comment(lib, "winmm.lib") 
using namespace std;

typedef struct Gusanito{	//Estructura de un "Vagón" del gusano
	int coord_x;
	int coord_y;
	Gusanito * Siguiente;
}h;

typedef Gusanito * PtrGus;

typedef struct comida{	//Estructura de una bolita de comida
	char forma;
	int pos_x;
	int pos_y;
	bool activo;
}c;

typedef struct Caras{	//Estructura de un "Vagón" del gusano
	char Face[60];
	Caras * Siguiente;
}x;
typedef Caras * PtrCar;

typedef struct JugadorA{	//Estructura de un "Nodo" del árbol de jugadores
	char nombre[21];
	int JPuntuacion;
	JugadorA * IZQ;
	JugadorA * DER;
}i;

typedef JugadorA * PtrJugA;

typedef struct JugadorL{	//Estructura de un "Nodo" del árbol de jugadores
	char nombre[21];
	int JPuntuacion;
	JugadorL * siguiente;
}s;

typedef JugadorL * PtrJugL;

//__________________________________________________________
//			Declaraciones Globales
//__________________________________________________________
HANDLE Consola;

bool Juego, Borrar = true, Borrar2 = true;
char FormaGus = 254;
char FormaEsc = 219;
char Direccion = 75, AuxDir = 75;
char Direccion2 = 'a', AuxDir2 = 'a';
PtrGus AuxGus = NULL;
PtrCar CaraGanar = NULL;
PtrCar CaraPerder = NULL;
double Dificultad = 10;
double Dificultad2 = 10;
int GusX, GusY;
int ContaBolitas = 0, ContaBolitas2, puntuacion = 0, Spuntuacion = 0, time1;
int Posibilidad_BOLIESP;
char Opc1, Opc2, Opc3;


//________________________________________________________________________________
//						Funciones
//________________________________________________________________________________
void gotoxy(int x, int y) //Define la funcion para poder mover el cursor por la pantalla.
{
	HANDLE hStdout;
	COORD pos;
	pos.X = x - 1;
	pos.Y = y - 1;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return;

	SetConsoleCursorPosition(hStdout, pos);
}
//____________________________________________CARAS_____________________________________
void AgregarFin(PtrCar &Lista, PtrCar &Nuevo)
{
	PtrCar Aux;
	Aux = Lista;
	if (Aux != NULL){
		while (Aux->Siguiente != NULL) Aux = Aux->Siguiente;
		Aux->Siguiente = Nuevo;
	}
	else Lista = Nuevo;
}

void Cargar(PtrCar &Lista1, PtrCar &Lista2){
	int conta = 0;
	PtrCar Nuevo;
	FILE *archivo;
	fopen_s(&archivo, "Caras.txt", "r");
	if (NULL == archivo){// Si no se encontro archivo
		printf("No se pudo abrir el archivo");
	}
	else{
		while (!feof(archivo)){
			Nuevo = new(Caras);
			fgets(Nuevo->Face, 60, archivo);
			Nuevo->Siguiente = NULL;
			if (conta < 26){
				AgregarFin(Lista1, Nuevo);
				conta++;
			}
			else AgregarFin(Lista2, Nuevo);
		}
	}
	fclose(archivo);
}

void ImprimirCara(PtrCar Lista1, char *Esp){
	int conta = 1;
	PtrCar Aux = Lista1;
	while (Aux != NULL) {
		cout << Esp << Aux->Face;
		Aux = Aux->Siguiente;
		conta++;
	}
}

void MatarCara(PtrCar &Cabeza){	//Elimina cada vagón del gusano cuando termina una partida
	PtrCar Verdugo;
	while (Cabeza != NULL){
		Verdugo = Cabeza;
		Cabeza = Cabeza->Siguiente;
		delete(Verdugo);
	}
}

//___________________________________________________________________________________________________

void InsertarCabeza(PtrGus &Cabeza, int x, int y){	//Crea una nueva cabeza (Vagón) para el gusano y la inserta al inicio
	PtrGus Nuevo;
	Nuevo = new(Gusanito);
	Nuevo->coord_x = x;
	Nuevo->coord_y = y;
	Nuevo->Siguiente = Cabeza;
	Cabeza = Nuevo;
}

void Crear_Gusano(PtrGus &Cabeza){	//Genera un gusano inicial y la imprime en el centro de la pantalla para comenzar a jugar
	for (int x = 40; x >= 38; x--){	//Crea tres "vagones" para el gusano inicial
		InsertarCabeza(Cabeza, x, 22);
	}
}

void Crear_GusVS(PtrGus &Cabeza1, PtrGus &Cabeza2){	//Genera un gusano inicial para el modo versus y la imprime en el centro de la pantalla para comenzar a jugar
	for (int x = 44; x <= 46; x++){	//Crea tres "vagones" para el gusano inicial
		InsertarCabeza(Cabeza1, x, 22);
	}

	for (int x = 34; x >= 32; x--){	//Crea tres "vagones" para el gusano inicial
		InsertarCabeza(Cabeza2, x, 22);
	}

}

void MatarGusano(PtrGus &Cabeza){	//Elimina cada vagón del gusano cuando termina una partida
	PtrGus Verdugo;
	while (Cabeza != NULL){
		Verdugo = Cabeza;
		Cabeza = Cabeza->Siguiente;
		gotoxy(Verdugo->coord_x, Verdugo->coord_y);
		cout << " ";
		delete(Verdugo);
	}
}

void primera_bolita(comida &bola, int posibilidad, PtrGus Gusano, PtrGus Excepcion){	//Genera la bolita de comida para el juego, esta es única, pero cambia de posición constantemente

	while (bola.activo == false){
		char s1 = 's', s2 = 's';
		AuxGus = Gusano;
		PtrGus AuxExc = Excepcion;
		bola.pos_x = rand() % 76 + 2;
		bola.pos_y = rand() % 21 + 2;
		while (AuxGus != NULL){
			if (bola.pos_x != AuxGus->coord_x || bola.pos_y != AuxGus->coord_y){
				AuxGus = AuxGus->Siguiente;
			}
			else {
				AuxGus = NULL;
				s1 = 'n';
				break;
			}
		}
		if (s1 == 's'){
			while (AuxExc != NULL){
				if (bola.pos_x != AuxExc->coord_x || bola.pos_y != AuxExc->coord_y){
					AuxExc = AuxExc->Siguiente;
				}
				else {
					AuxExc = NULL;
					s2 = 'n';
					break;
				}
			}
			if (s2 == 's'){
				bola.activo = true;
			}
		}

	}

	if (posibilidad <= 20) bola.forma = 1;//Evalúa la posibilidad para crear la bolita especial

	else bola.forma = 15;
}

int GetHora(){//Obtiene la hora del sistema y la retorna como un entero
	time_t hora;
	time(&hora);
	return hora;
}

void Puntuacion(comida &bola, int timeinicial)
{
	int time2 = GetHora();
	int tiempofinal = time2 - timeinicial;
	if (tiempofinal <= 5)
	{
		if (bola.forma == 1) puntuacion = 20;
		else puntuacion = 10;
	}
	if (tiempofinal > 5 && tiempofinal <= 10)
	{
		if (bola.forma == 1) puntuacion = 10;
		else puntuacion = 5;
	}
	if (tiempofinal > 10)
	{
		if (bola.forma == 1) puntuacion = 4;
		else puntuacion = 2;
	}
	Spuntuacion = Spuntuacion + puntuacion;
	time1 = GetHora();
}

//Funciones para el manejo de las puntuaciones de los jugadores (Se contemplan solo las 10 mejores)----------------------------------------------

PtrJugA CrearJugador(int PuntuacionX)
{
	system("cls");
	PtrJugA JugadorN = new(JugadorA);
	gotoxy(15, 12);
	cout << "Ingrese su nombre de jugador:";
	gotoxy(45, 12);
	cin >> JugadorN->nombre;
	JugadorN->JPuntuacion = PuntuacionX;
	JugadorN->IZQ = NULL;
	JugadorN->DER = NULL;
	return JugadorN;
}

void InsertarJugador(PtrJugA &Raiz, PtrJugA &Nuevo)// Inserta un votante en el árbol BB según su número de cédula
{
	if (Raiz == NULL)
	{
		Raiz = Nuevo;
		Raiz->IZQ = NULL;
		Raiz->DER = NULL;
	}
	else
	{
		if (Nuevo->JPuntuacion < Raiz->JPuntuacion) InsertarJugador(Raiz->IZQ, Nuevo);
		else InsertarJugador(Raiz->DER, Nuevo);
	}
}

void GuardarPuntuacion(PtrJugL Lista){/*Esta funcion permite guardar en un archivo de texto el
									  registro de datos*/
	FILE *archivo;//Crea un puntero para manipular un archivo
	fopen_s(&archivo, "PUNTUACIONES.txt", "w+"); //Abrir un archivo en modo de escritura

	if (NULL == archivo){//Si no se encontro ningun archivo para escribir
		printf("No se pudo abrir el archivo.");//... Imprime el error respectivo
	}
	else{//Si se encuentra el arhivo...
		PtrJugL AUX = Lista;// Asigna el contenido de lista al puntero aux
		int cont = 0;
		while (AUX != NULL && cont < 10){//Mientras no se llege el al final de la lista enlazada
			fprintf(archivo, "%s\n", AUX->nombre); /*Escribir los datos en el archivo de texto*/
			fprintf(archivo, "%i\n", AUX->JPuntuacion);
			AUX = AUX->siguiente;//Se continua evaluando el siguiente elemento
			cont++;
		}
	}
	fclose(archivo);//Cerrar archivo
	Sleep(1000);
}

void CargarPuntuacion(PtrJugA &Raiz)
{
	int cont = 0;
	FILE *archivo;
	fopen_s(&archivo, "PUNTUACIONES.txt", "r");
	if (NULL == archivo){
		printf("No se pudo abrir el archivo");
	}
	else
	{

		while (!feof(archivo) && cont < 10){
			PtrJugA AUX = new(JugadorA);
			fscanf_s(archivo, "%s\n", AUX->nombre, 21);
			fscanf_s(archivo, "%i\n", &AUX->JPuntuacion);
			AUX->IZQ = NULL;
			AUX->DER = NULL;
			InsertarJugador(Raiz, AUX);
			cont++;
		}
	}
	fclose(archivo);
}

void AgregarInicioLista(PtrJugL &Lista, PtrJugL &Nuevo)
{
	Nuevo->siguiente = Lista;
	Lista = Nuevo;
}

void LiberarLista(PtrJugL &Lista){
	PtrJugL AuxLista = NULL;
	while (Lista != NULL){
		AuxLista = Lista;
		Lista = Lista->siguiente;
		delete(AuxLista);
	}
}

void TreeToList(PtrJugA &Raiz, PtrJugL &Lista)//Esta función pasa los datos de un árbol a una lista para poder meterlos mediante GuadarPuntuacion a un .txt
{
	if (Raiz != NULL)
	{
		char NombreJug[21];
		TreeToList(Raiz->IZQ, Lista);
		PtrJugL Nuevo = new(JugadorL);
		strcpy_s(Nuevo->nombre, Raiz->nombre);
		Nuevo->JPuntuacion = Raiz->JPuntuacion;
		AgregarInicioLista(Lista, Nuevo);
		TreeToList(Raiz->DER, Lista);
	}
}

void Listar(PtrJugL &Lista)
{
	int Contador = 0;
	PtrJugL Aux;
	Aux = Lista;
	while (Aux != NULL && Contador < 10)
	{
		char m = 219;
		gotoxy(15, 4);
		for (int l = 0; l < 51; l++)	cout << m;
		gotoxy(30, 6);
		cout << "MEJORES PUNTUACIONES" << endl << endl;
		gotoxy(15, 8);
		for (int l = 0; l < 51; l++)	cout << m;
		gotoxy(25, 10 + Contador);
		cout << "    " << Contador + 1 << "- " << Aux->nombre;
		gotoxy(45, 10 + Contador);
		cout << "  (" << Aux->JPuntuacion << ")			" << endl << endl;
		gotoxy(15, 21);
		for (int l = 0; l < 51; l++)	cout << m;

		for (int l = 5; l < 21; l++)	{
			gotoxy(15, l);
			cout << m;
			gotoxy(65, l);
			cout << m;
		}
		Aux = Aux->siguiente;
		Contador++;
	}
	gotoxy(19, 23);
	cout << "Presione un tecla para regresar al menu . . .";
	_getch();
}

void NuevaPuntuacion(int NuevaPunt, PtrJugA &Raiz, PtrJugL &Lista){
	PtrJugA Jugador = CrearJugador(NuevaPunt);
	InsertarJugador(Raiz, Jugador);
	LiberarLista(Lista);
	TreeToList(Raiz, Lista);
	GuardarPuntuacion(Lista);
}

//----------------------------------------------------------------------------------------------------------------------------------------------

bool Interseccion(PtrGus Cabeza, comida bola){
	if (Cabeza->coord_x == bola.pos_x && Cabeza->coord_y == bola.pos_y){
		return true;
	}
	else return false;
}

void bolita_extra(comida &bola, PtrGus Cabeza1, PtrGus Cabeza2, int &Contador, bool &Borrar, PtrGus ExcepEsc){

	Posibilidad_BOLIESP = rand() % 101;
	Beep(5500, 60);
	Borrar = false;
	bola.activo = true;
	//Diagrama de Flujo
	bool invalido = true;

	while (invalido){
		bola.pos_x = rand() % 76 + 2;
		bola.pos_y = rand() % 21 + 2;
		//GlobalFree(AuxGus);
		PtrGus AuxExc = ExcepEsc;
		while (Cabeza1 != NULL){
			InsertarCabeza(AuxGus, Cabeza1->coord_x, Cabeza1->coord_y);
			Cabeza1 = Cabeza1->Siguiente;
		}
		while (Cabeza2 != NULL){
			InsertarCabeza(AuxGus, Cabeza2->coord_x, Cabeza2->coord_y);
			Cabeza2 = Cabeza2->Siguiente;
		}
		char s1 = 's';
		char s2 = 's';
		//-----------------------------------------------------------------------------------

		while (AuxGus != NULL){
			if (bola.pos_x != AuxGus->coord_x || bola.pos_y != AuxGus->coord_y){
				AuxGus = AuxGus->Siguiente;
			}//																					NO PUEDE CAER EN EL CUERPO DEL GUSANO
			else {
				AuxGus = NULL;
				s1 = 'n';
				break;
			}
		}
		//-----------------------------------------------------------------------------------
		//-----------------------------------------------------------------------------------
		if (s1 == 's'){
			while (AuxExc != NULL){

				if (bola.pos_x != AuxExc->coord_x || bola.pos_y != AuxExc->coord_y){
					AuxExc = AuxExc->Siguiente;
				}
				else {
					AuxExc = NULL;
					s2 = 'n';
					break;
				}

			}
			if (s2 == 's'){
				invalido = false;
			}
		}

	}
	if (Posibilidad_BOLIESP <= 5)//Evalúa la posibilidad para crear la bolita especial
	{
		bola.forma = 1;
	}
	else bola.forma = 15;
	Contador++;
}

void Recuadro(){	//Imprime un recuadro básico que demarca el área de juego
	system("color E");
	gotoxy(1, 1);
	cout << "É";
	for (int i = 0; i<77; i++){ cout << "Í"; }
	cout << "»" << endl;
	for (int i = 2; i < 24; i++){ cout << "º"; gotoxy(79, i); cout << "º" << endl; }
	cout << "È";
	for (int i = 0; i<77; i++){ cout << "Í"; }
	cout << "¼";
}

void Escenario1(){	//Imprime un escenario1 que demarca el área de juego
	Recuadro();
}

void Escenario2(PtrGus &Lista){	//Imprime un escenario1 que demarca el área de juego
	Recuadro();
	for (int i = 27; i < 33; i++){
		gotoxy(i, 5);
		cout << FormaEsc;
		InsertarCabeza(Lista, i, 5);
		gotoxy(i + 20, 5);
		cout << FormaEsc;
		InsertarCabeza(Lista, i + 20, 5);
		gotoxy(32, i - 22);
		cout << FormaEsc;
		InsertarCabeza(Lista, 32, i - 22);
		gotoxy(47, i - 22);
		cout << FormaEsc;
		InsertarCabeza(Lista, 47, i - 22);
		gotoxy(i, 11);
		cout << FormaEsc;
		InsertarCabeza(Lista, i, 11);
		gotoxy(i + 20, 11);
		cout << FormaEsc;
		InsertarCabeza(Lista, i + 20, 11);
	}
	for (int i = 12; i < 16; i++){
		gotoxy(37, i);
		cout << FormaEsc;
		InsertarCabeza(Lista, 37, i);
		gotoxy(42, i);
		cout << FormaEsc;
		InsertarCabeza(Lista, 42, i);
		gotoxy(25 + i, 15);
		cout << FormaEsc;
		InsertarCabeza(Lista, 25 + i, 15);
	}
	for (int i = 27; i < 53; i++){
		gotoxy(i, 20);
		cout << FormaEsc;
		InsertarCabeza(Lista, i, 20);
	}
	for (int i = 18; i < 20; i++){
		gotoxy(27, i);
		cout << FormaEsc;
		InsertarCabeza(Lista, 27, i);
		gotoxy(52, i);
		cout << FormaEsc;
		InsertarCabeza(Lista, 52, i);
	}
	gotoxy(41, 15);
	cout << FormaEsc;
	InsertarCabeza(Lista, 41, 15);
}

void Escenario3(PtrGus &Lista){	//Imprime un escenario1 que demarca el área de juego
	Recuadro();
	for (int i = 20; i < 61; i++){
		gotoxy(i, 5);
		cout << FormaEsc;
		InsertarCabeza(Lista, i, 5);
	}
	for (int i = 6; i < 13; i++){
		gotoxy(20, i);
		cout << FormaEsc;
		InsertarCabeza(Lista, 20, i);
		gotoxy(60, i);
		cout << FormaEsc;
		InsertarCabeza(Lista, 60, i);
		gotoxy(34, 8 + i);
		cout << FormaEsc;
		InsertarCabeza(Lista, 34, 8 + i);
		gotoxy(46, 8 + i);
		cout << FormaEsc;
		InsertarCabeza(Lista, 46, 8 + i);

	}
	for (int i = 0; i < 15; i++){
		gotoxy(20 + i, 13);
		cout << FormaEsc;
		InsertarCabeza(Lista, 20 + i, 13);
		gotoxy(46 + i, 13);
		cout << FormaEsc;
		InsertarCabeza(Lista, 46 + i, 13);
	}
}

void Escenario4(PtrGus &Lista){	//Imprime un escenario1 que demarca el área de juego
	Recuadro();
	for (int i = 8; i < 38; i++){
		gotoxy(i, 5);
		cout << FormaEsc;
		InsertarCabeza(Lista, i, 5);
		gotoxy(i + 35, 5);
		cout << FormaEsc;
		InsertarCabeza(Lista, i + 35, 5);
		gotoxy(i, 20);
		cout << FormaEsc;
		InsertarCabeza(Lista, i, 20);
		gotoxy(i + 35, 20);
		cout << FormaEsc;
		InsertarCabeza(Lista, i + 35, 20);
		gotoxy(i + 17, 9);
		cout << FormaEsc;
		InsertarCabeza(Lista, i + 17, 9);
		gotoxy(i + 17, 16);
		cout << FormaEsc;
		InsertarCabeza(Lista, i + 17, 16);
	}
	for (int i = 0; i < 5; i++){
		gotoxy(8, i + 6);
		cout << FormaEsc;
		InsertarCabeza(Lista, 8, i + 6);
		gotoxy(8, i + 15);
		cout << FormaEsc;
		InsertarCabeza(Lista, 8, i + 15);
		gotoxy(72, i + 6);
		cout << FormaEsc;
		InsertarCabeza(Lista, 72, i + 6);
		gotoxy(72, i + 15);
		cout << FormaEsc;
		InsertarCabeza(Lista, 72, i + 15);
		gotoxy(17, i + 10);
		cout << FormaEsc;
		InsertarCabeza(Lista, 17, i + 10);
		gotoxy(63, i + 10);
		cout << FormaEsc;
		InsertarCabeza(Lista, 63, i + 10);
	}
	gotoxy(17, 15);
	cout << FormaEsc;
	InsertarCabeza(Lista, 17, 15);
	gotoxy(63, 15);
	cout << FormaEsc;
	InsertarCabeza(Lista, 63, 15);
}

void Escenario5(PtrGus &Lista){	//Imprime un escenario1 que demarca el área de juego
	Recuadro();
	for (int i = 8; i < 19; i++){
		gotoxy(40, i);
		cout << FormaEsc;
		InsertarCabeza(Lista, 40, i);
	}
	for (int i = 20; i < 61; i++){
		gotoxy(i, 11);
		cout << FormaEsc;
		InsertarCabeza(Lista, i, 11);
		gotoxy(i, 15);
		cout << FormaEsc;
		InsertarCabeza(Lista, i, 15);
	}
	for (int i = 5; i < 11; i++){
		gotoxy(20, i);
		cout << FormaEsc;
		InsertarCabeza(Lista, 20, i);
		gotoxy(60, i);
		cout << FormaEsc;
		InsertarCabeza(Lista, 60, i);
		gotoxy(20, i + 11);
		cout << FormaEsc;
		InsertarCabeza(Lista, 20, i + 11);
		gotoxy(60, i + 11);
		cout << FormaEsc;
		InsertarCabeza(Lista, 60, i + 11);
	}
	for (int i = 30; i < 51; i++){
		gotoxy(i, 7);
		cout << FormaEsc;
		InsertarCabeza(Lista, i, 7);
		gotoxy(i, 19);
		cout << FormaEsc;
		InsertarCabeza(Lista, i, 19);
	}
	for (int i = 5; i < 7; i++){
		gotoxy(30, i);
		cout << FormaEsc;
		InsertarCabeza(Lista, 30, i);
		gotoxy(50, i);
		cout << FormaEsc;
		InsertarCabeza(Lista, 50, i);
		gotoxy(30, i + 15);
		cout << FormaEsc;
		InsertarCabeza(Lista, 30, i + 15);
		gotoxy(50, i + 15);
		cout << FormaEsc;
		InsertarCabeza(Lista, 50, i + 15);
	}
}

void EscenarioIns()
{
	system("CLS");
	gotoxy(25, 2);
	cout << "Seleccione el escenario de Juego";
	cout << endl << endl << endl;
	cout << "	   *	 						 *\n";
	cout << "	  ** 							 **\n";
	cout << "	 ************					************\n";
	cout << "	*************					*************\n";
	cout << "	 ************					************\n";
	cout << "	  **							 **\n";
	cout << "	   *	 						 *\n";
	cout << endl << "     Anterior Escenario" << "			****	" << "	Siguiente Escenario\n";
	cout << "		      		        ****\n";
	cout << "		      		        ****\n";
	cout << "		      		   *    ****\n";
	cout << "		      		  **    ****\n";
	cout << "		      		 ***********\n";
	cout << "		      		************\n";
	cout << "		      		 ***********\n";
	cout << "		      		  **\n";
	cout << "		      		   *\n";
	gotoxy(31, 24);
	cout << "ENTER: Seleccionar";


}

void Desencolar(PtrGus &Cabeza, bool &Borrar){	//Elimina la cola del gusano para retener el crecimiento del mismo, con excepción si se obtiene la bolita de comida
	if (Borrar){	//Condición que indica que no se obtuvo la bolita de comida
		AuxGus = Cabeza;
		while (AuxGus->Siguiente->Siguiente != NULL){ AuxGus = AuxGus->Siguiente; }
		gotoxy(AuxGus->Siguiente->coord_x, AuxGus->Siguiente->coord_y);
		cout << " ";
		delete(AuxGus->Siguiente);
		AuxGus->Siguiente = NULL;
	}
	Borrar = true;
}

void MoverGus(PtrGus &Cabeza){	//Función que combina las funciones InsertarCabeza y Desencolar para emular el movimiento
	//----Nueva cabeza-----
	HANDLE Consola;
	Consola = GetStdHandle(STD_OUTPUT_HANDLE);
	if (Direccion == 75) {
		if (AuxDir != 77){
			AuxDir = 75;
			InsertarCabeza(Cabeza, Cabeza->coord_x - 1, Cabeza->coord_y);
			Desencolar(Cabeza, Borrar);
		}
		else Direccion = 77;
	}
	if (Direccion == 77) {
		if (AuxDir != 75){
			AuxDir = 77;
			InsertarCabeza(Cabeza, Cabeza->coord_x + 1, Cabeza->coord_y);
			Desencolar(Cabeza, Borrar);
		}
		else Direccion = 75;
	}
	if (Direccion == 80) {
		if (AuxDir != 72){
			AuxDir = 80;
			InsertarCabeza(Cabeza, Cabeza->coord_x, Cabeza->coord_y + 1);
			Desencolar(Cabeza, Borrar);
		}
		else Direccion = 72;
	}
	if (Direccion == 72) {
		if (AuxDir != 80){
			AuxDir = 72;
			InsertarCabeza(Cabeza, Cabeza->coord_x, Cabeza->coord_y - 1);
			Desencolar(Cabeza, Borrar);
		}
		else Direccion = 80;
	}

	gotoxy(Cabeza->coord_x, Cabeza->coord_y);
	SetConsoleTextAttribute(Consola, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << FormaGus;
	SetConsoleTextAttribute(Consola, FOREGROUND_RED | FOREGROUND_INTENSITY);
	Sleep(Dificultad);

}

void MoverVS(PtrGus &Cabeza1, PtrGus &Cabeza2){	//Función que combina las funciones InsertarCabeza y Desencolar para emular el movimiento
	//----Nueva cabeza-----
	HANDLE Consola;
	Consola = GetStdHandle(STD_OUTPUT_HANDLE);
	if (Direccion == 75) {
		if (AuxDir != 77){
			AuxDir = 75;
			InsertarCabeza(Cabeza1, Cabeza1->coord_x - 1, Cabeza1->coord_y);
			Desencolar(Cabeza1, Borrar);
		}
		else Direccion = 77;
	}
	if (Direccion == 77) {
		if (AuxDir != 75){
			AuxDir = 77;
			InsertarCabeza(Cabeza1, Cabeza1->coord_x + 1, Cabeza1->coord_y);
			Desencolar(Cabeza1, Borrar);
		}
		else Direccion = 75;
	}
	if (Direccion == 80) {
		if (AuxDir != 72){
			AuxDir = 80;
			InsertarCabeza(Cabeza1, Cabeza1->coord_x, Cabeza1->coord_y + 1);
			Desencolar(Cabeza1, Borrar);
		}
		else Direccion = 72;
	}
	if (Direccion == 72) {
		if (AuxDir != 80){
			AuxDir = 72;
			InsertarCabeza(Cabeza1, Cabeza1->coord_x, Cabeza1->coord_y - 1);
			Desencolar(Cabeza1, Borrar);
		}
		else Direccion = 80;

	}
	gotoxy(Cabeza1->coord_x, Cabeza1->coord_y);
	SetConsoleTextAttribute(Consola, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << FormaGus;
	Sleep(Dificultad);
	//-------------------------------------------------------

	if (Direccion2 == 'a') {
		if (AuxDir2 != 'd'){
			AuxDir2 = 'a';
			InsertarCabeza(Cabeza2, Cabeza2->coord_x - 1, Cabeza2->coord_y);
			Desencolar(Cabeza2, Borrar2);
		}
		else Direccion2 = 'd';
	}
	if (Direccion2 == 'd') {
		if (AuxDir2 != 'a'){
			AuxDir2 = 'd';
			InsertarCabeza(Cabeza2, Cabeza2->coord_x + 1, Cabeza2->coord_y);
			Desencolar(Cabeza2, Borrar2);
		}
		else Direccion2 = 'a';
	}
	if (Direccion2 == 's') {
		if (AuxDir2 != 'w'){
			AuxDir2 = 's';
			InsertarCabeza(Cabeza2, Cabeza2->coord_x, Cabeza2->coord_y + 1);
			Desencolar(Cabeza2, Borrar2);
		}
		else Direccion2 = 'w';
	}
	if (Direccion2 == 'w') {
		if (AuxDir2 != 's'){
			AuxDir2 = 'w';
			InsertarCabeza(Cabeza2, Cabeza2->coord_x, Cabeza2->coord_y - 1);
			Desencolar(Cabeza2, Borrar2);
		}
		else Direccion2 = 's';
	}

	//-------------------------------------------------------


	gotoxy(Cabeza2->coord_x, Cabeza2->coord_y);
	SetConsoleTextAttribute(Consola, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << FormaGus;
	Sleep(Dificultad2);

	SetConsoleTextAttribute(Consola, FOREGROUND_RED | FOREGROUND_INTENSITY);


}

void Excepciones(PtrGus &Cabeza){	//Establece las coordenadas "prohibidas" que detienen el juego.
	AuxGus = Cabeza->Siguiente;
	if (Cabeza->coord_x == 1 || Cabeza->coord_x == 79 || Cabeza->coord_y == 1 || Cabeza->coord_y == 24) { //Coordenadas del Recuadro de juego
		Juego = false;
		return;
	}
	while (AuxGus != NULL){	//Coordenadas de intersección con el cuerpo del gusano
		if (Cabeza->coord_x == AuxGus->coord_x && Cabeza->coord_y == AuxGus->coord_y){
			Juego = false;
			break;
		}
		else AuxGus = AuxGus->Siguiente;
	}
}

void ExcepcionesEsc(PtrGus &Cabeza, PtrGus &Excepciones){
	AuxGus = Excepciones;
	while (AuxGus != NULL){	//Coordenadas de intersección con las paredes del escenario
		if (Cabeza->coord_x == AuxGus->coord_x && Cabeza->coord_y == AuxGus->coord_y){
			gotoxy(Cabeza->coord_x, Cabeza->coord_y);
			cout << FormaEsc;
			PtrGus Desecho = Cabeza;
			Cabeza = Cabeza->Siguiente;
			delete (Desecho);
			Juego = false;
			break;
		}
		else AuxGus = AuxGus->Siguiente;
	}

}

void RANDOM(void){
	time_t t;
	srand((unsigned)time(&t));
}

void Color_Azul(PtrGus Cabeza, comida bola){
	SetConsoleTextAttribute(Consola, FOREGROUND_RED | FOREGROUND_INTENSITY);
	gotoxy(bola.pos_x, bola.pos_y);
	cout << bola.forma;
	SetConsoleTextAttribute(Consola, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	while (Cabeza != NULL){
		gotoxy(Cabeza->coord_x, Cabeza->coord_y);
		cout << FormaGus;
		Cabeza = Cabeza->Siguiente;
	}
}

void Color_Verde(PtrGus Cabeza, comida bola){
	SetConsoleTextAttribute(Consola, FOREGROUND_RED | FOREGROUND_INTENSITY);
	gotoxy(bola.pos_x, bola.pos_y);
	cout << bola.forma;
	SetConsoleTextAttribute(Consola, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	while (Cabeza != NULL){
		gotoxy(Cabeza->coord_x, Cabeza->coord_y);
		cout << FormaGus;
		Cabeza = Cabeza->Siguiente;
	}
}

void ConteoInicial(){
	for (int t = 3; t > 0; t--){
		gotoxy(40, 3);
		cout << t;
		Beep(4000, 300);
		Sleep(700);
	}
	gotoxy(40, 3);
	cout << " ";
	Beep(5500, 300);
}

void QuemarTeclas(){
	while (_kbhit()){ _getch(); }
}

void MostrarGanador(PtrGus J1, PtrGus J2){

	PlaySound(TEXT("dk-stagestart(Ganador versus).wav"), NULL, SND_LOOP | SND_ASYNC);
	system("CLS");
	int conta1 = 0, conta2 = 0;
	while (J1 != NULL){
		conta1++;
		J1 = J1->Siguiente;
	}
	while (J2 != NULL){
		conta2++;
		J2 = J2->Siguiente;
	}

	if (conta1 > conta2){ SetConsoleTextAttribute(Consola, FOREGROUND_BLUE | FOREGROUND_INTENSITY); gotoxy(30, 10); cout << "GANA EL JUGADOR 1 !!!"; gotoxy(27, 13); cout << "LONGITUD ALCANZADA: " << conta1; }
	if (conta1 < conta2){ SetConsoleTextAttribute(Consola, FOREGROUND_GREEN | FOREGROUND_INTENSITY); gotoxy(30, 10); cout << "GANA EL JUGADOR 2 !!!"; gotoxy(27, 13); cout << "LONGITUD ALCANZADA: " << conta2; }
	if (conta1 == conta2){ SetConsoleTextAttribute(Consola, FOREGROUND_RED | FOREGROUND_INTENSITY); gotoxy(25, 10); cout << "LA PARTIDA HA QUEDADO EMPATADA"; gotoxy(27, 13); cout << "LONGITUD ALCANZADA: " << conta2; }
	Sleep(3000);

}
//________________________________________________________________________________

//________________________________________________________________________________
//				Menus
//________________________________________________________________________________

void SelDificultad(){
	system("cls");
	int Selector;
	cout << "DIFICULTAD DEL JUEGO" << endl << endl;
	cout << "	1- Facil" << endl << "	2- Medio" << endl << "	3- Dificil" << endl << endl;
	cout << "	Seleccione el nivel de dificultad: ";
	Selector = _getch();
	switch (Selector){
	case '1': {Dificultad = 90; break;	}
	case '2': {Dificultad = 60; break; }
	case '3': {Dificultad = 30; break; }
	default:{Dificultad = 90; break; }
	}
	system("CLS");
}

/*void Crear_escenario(int posicion, PtrGus Lista)
{
switch (posicion)
{
case 1:
Escenario1(); break;
case 2:
Escenario2(Lista); break;
case 3:
Escenario3(Lista); break;
case 4:
Escenario4(Lista); break;
case 5:
Escenario5(Lista); break;
}
}*/

void Escoge_Escenario(PtrGus &Lista)
{
TECLAZO:
	char tecla;
	int posicion = 1;//Para evaluar en cual escenario se está posicionado 
	EscenarioIns();
	tecla = _getch(); // Recibe una tecla ingresada por el usuario

	while (tecla != 13)  // Condicion de salida
	{
		system("cls");
		if (tecla != -32 && tecla != 13) { cout << "			TECLA INVALIDA!" << endl; Sleep(1000); system("CLS"); goto TECLAZO; }
		else {
			tecla = _getch();
			if (tecla == 77)
			{
				posicion++;
				if (posicion == 2) { MatarGusano(Lista); Escenario1(); }
				if (posicion == 3) { MatarGusano(Lista); Escenario2(Lista); }
				if (posicion == 4) { MatarGusano(Lista); Escenario3(Lista); }
				if (posicion == 5) { MatarGusano(Lista); Escenario4(Lista); }
				if (posicion == 6) { MatarGusano(Lista); Escenario5(Lista); }
				if (posicion == 7) { MatarGusano(Lista); EscenarioIns(); posicion = 1; }
			}
			if (tecla == 75)
			{
				posicion--;
				if (posicion == 0) { posicion = 6; }
				if (posicion == 1) { MatarGusano(Lista); EscenarioIns(); }
				if (posicion == 2) { MatarGusano(Lista); Escenario1(); }
				if (posicion == 3) { MatarGusano(Lista); Escenario2(Lista); }
				if (posicion == 4) { MatarGusano(Lista); Escenario3(Lista); }
				if (posicion == 5) { MatarGusano(Lista); Escenario4(Lista); }
				if (posicion == 6) { MatarGusano(Lista); Escenario5(Lista); }
			}
			tecla = _getch(); // Recibe una tecla ingresada por el usuario
		}
	}
}

void Gus2(int x){
	char o = 220, l = 219;
	gotoxy(x, 13);
	cout << "  " << o << o << o << "  ";
	gotoxy(x, 14);
	cout << "  " << l << " " << l << "  ";
	gotoxy(x, 15);
	cout << o << o << l << " " << l << o << o << o << " ";
}

void Gus1(int x){
	char o = 220;
	gotoxy(x - 2, 15);
	for (int i = 0; i<10; i++) cout << o;
}

void BorrarGus()
{
	for (int y = 1; y < 77; y++){
		gotoxy(y, 13); cout << " ";
		gotoxy(y, 14); cout << " ";
		gotoxy(y, 15); cout << " ";
	}
}

void FinalizarPartida(PtrGus &Cabeza, PtrGus &Excepciones){
	MatarGusano(Cabeza);
	MatarGusano(Excepciones);
	system("CLS");
	QuemarTeclas();
}

void Menu1(){
	system("CLS");
	cout << endl << endl << endl;
	cout << "		********   **    **   ********   **   **   ******	" << endl;
	cout << "		********   ***   **   ********   **  **    ******	" << endl;
	cout << "		**         ****  **   **    **   ** **     **		" << endl;
	cout << "		********   ** ** **   ********   ****      *****	" << endl;
	cout << "		      **   **  ****   ********   ** **     **		" << endl;
	cout << "		********   **   ***   **    **   **  **    ******	" << endl;
	cout << "		********   **    **   **    **   **   **   ******	" << endl << endl << endl << endl;
	cout << "\n\n\n		1- Modo Survival	4- Mostrar Ayuda" << endl << "		2- Modo Arcade		5- Mejores Puntuaciones" << endl << "		3- Modo Versus		6- Salir" << endl << endl;
	cout << "	Seleccione su opcion: ";
	for (int x = 5; x < 70; x = x + 3)
	{
		BorrarGus();
		Gus1(x);
		Sleep(500);
		BorrarGus();
		Gus2(x);
		Sleep(500);
		if (x == 69) x = 5;
		if (_kbhit()) {
			x = 70;
			Opc1 = _getch();
		}
	}
}

void LlamarMenu(PtrGus &Cabeza, PtrGus &Excep){
	QuemarTeclas();
	Juego = true;
	Spuntuacion = 0;
	ContaBolitas = 0;
	ContaBolitas2 = 0;
	Direccion = 75;
	AuxDir = Direccion;
	MatarGusano(Cabeza);
	MatarGusano(Excep);
	system("color 47"); //Colorcito
	//PlaySound(TEXT("Cancion1.wav"), NULL, SND_ASYNC);
	Opc1 = '4';
	Menu1();
	system("color E"); //Colorcito
}
//________________________________________________________________________________
//				Modos
//________________________________________________________________________________

void ModoSurvival(PtrGus &Gusano, comida &bolita, bool Sel_Dir, PtrGus &ExcepcionesEscenario){

	PlaySound(TEXT("Spyder(survival).wav"), NULL, SND_LOOP | SND_ASYNC);	//Cancioncita
	primera_bolita(bolita, Posibilidad_BOLIESP, Gusano, ExcepcionesEscenario);
	while (Juego){ //Inicia el juego
		gotoxy(15, 25);
		cout << "Bolitas = " << ContaBolitas;
		gotoxy(30, 25);
		cout << "Puntuacion: " << Spuntuacion;

		if (bolita.activo){//Si no hay bolita en pantalla, crea una nueva y la coloca
			gotoxy(bolita.pos_x, bolita.pos_y);
			SetConsoleTextAttribute(Consola, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << bolita.forma;
			bolita.activo = false;
		}

		if (_kbhit()){	//Escanea si el usuario generó un evento de teclado y de existir, lo interpreta
			Direccion = _getch();
			if (Direccion == -32)	Direccion = _getch();

			if (Direccion != 75 && Direccion != 77 && Direccion != 72 && Direccion != 80) Direccion = AuxDir;
			// Ajuste de la velocidad en cambio de ejes X-Y---------------
			if ((Direccion == 75 || Direccion == 77) && !Sel_Dir) {		//|
				Dificultad = Dificultad / 1.5;							//|
				Sel_Dir = true;											//|
			}															//|
			if ((Direccion == 72 || Direccion == 80) && Sel_Dir) {		//|
				Dificultad = Dificultad * 1.5;							//|
				Sel_Dir = false;										//|
			}															//|
		}																//|
		//-----------------------------------------------------------------

		MoverGus(Gusano);
		if (Interseccion(Gusano, bolita)){
			Puntuacion(bolita, time1);
			bolita_extra(bolita, Gusano, NULL, ContaBolitas, Borrar, ExcepcionesEscenario);
		}

		Excepciones(Gusano);
		ExcepcionesEsc(Gusano, ExcepcionesEscenario);

		if (!Juego){	//Si ocurre una excepción (Intersección de la cabeza del gusano con una coordenada "prohibida") acaba el juego
			//PlaySound(TEXT("WAH.wav"), NULL, SND_ASYNC);
			//Sleep(3750);
			FinalizarPartida(Gusano, ExcepcionesEscenario);
			ImprimirCara(CaraPerder, "          ");
			PlaySound(TEXT("smbdeath(muerte).wav"), NULL,SND_SYNC);

		}
	}
}

void ModoArcade(PtrGus &Gusano, comida &bolita, bool Sel_Dir, PtrGus &ExcepcionesEscenario){
	int ContaEsc = 0;
	int Vidas = 3;
	char corazon = 3;
	bool Ganador = false;
	bool CambioEscenario = true;
	Escenario1();
	primera_bolita(bolita, Posibilidad_BOLIESP, Gusano, ExcepcionesEscenario);
	PlaySound(TEXT("Bomberman_II_-_Level(primer nivel arcade).wav"), NULL, SND_LOOP | SND_ASYNC);	//Cancioncita
	ConteoInicial();
	while ((Juego && !Ganador) && Vidas > 0){ //Inicia el juego

		gotoxy(15, 25);
		cout << "Bolitas = " << ContaBolitas;
		gotoxy(30, 25);
		cout << "Puntuacion: " << Spuntuacion;
		gotoxy(50, 25);
		cout << "Vidas: ";
		for (int v = 1; v <= Vidas; v++) {
			cout << corazon << " ";
		}

		if (bolita.activo){//Si no hay bolita en pantalla, crea una nueva y la coloca
			gotoxy(bolita.pos_x, bolita.pos_y);
			SetConsoleTextAttribute(Consola, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << bolita.forma;
			bolita.activo = false;
		}

		if (_kbhit()){	//Escanea si el usuario generó un evento de teclado y de existir, lo interpreta
			Direccion = _getch();
			if (Direccion == -32)	Direccion = _getch();

			if (Direccion != 75 && Direccion != 77 && Direccion != 72 && Direccion != 80) Direccion = AuxDir;

			// Ajuste de la velocidad en cambio de ejes X-Y---------------

			if ((Direccion == 75 || Direccion == 77) && !Sel_Dir) {		//|
				Dificultad = Dificultad / 1.5;							//|
				Sel_Dir = true;											//|
			}															//|
			if ((Direccion == 72 || Direccion == 80) && Sel_Dir) {		//|
				Dificultad = Dificultad * 1.5;							//|
				Sel_Dir = false;										//|
			}															//|
		}																//|
		//-----------------------------------------------------------------


		if (Spuntuacion >= 0 && ContaEsc == -1)//Escenario 1
		{
			system("CLS");
			MatarGusano(ExcepcionesEscenario);
			ContaEsc++;
			MatarGusano(Gusano);
			Crear_Gusano(Gusano);
			Direccion = 75;
			AuxDir = Direccion;
			Escenario1();
			gotoxy(30, 23);
			if (CambioEscenario)	cout << "CAMBIO DE ESCENARIO!";
			CambioEscenario = true;
			PlaySound(TEXT("Bomberman_II_-_Level(primer nivel arcade).wav"), NULL, SND_LOOP | SND_ASYNC);	//Cancioncita
			ConteoInicial();
			gotoxy(30, 23);
			if (CambioEscenario)	cout << "                    ";
			QuemarTeclas();
			bolita_extra(bolita, Gusano, NULL, ContaBolitas, Borrar, ExcepcionesEscenario);
		}

		if (Spuntuacion >= 30 && ContaEsc == 0)//Escenario 2
		{
			PlaySound(TEXT("Adventure_Island_Level_1(segundo nivel arcade).wav"), NULL, SND_LOOP | SND_ASYNC);	//Cancioncita
			system("CLS");
			MatarGusano(ExcepcionesEscenario);
			ContaEsc++;
			MatarGusano(Gusano);
			Crear_Gusano(Gusano);
			Direccion = 75;
			AuxDir = Direccion;
			Escenario2(ExcepcionesEscenario);
			gotoxy(30, 23);
			if (CambioEscenario)	cout << "CAMBIO DE ESCENARIO!";
			CambioEscenario = true;
			ConteoInicial();
			gotoxy(30, 23);
			if (CambioEscenario)	cout << "                    ";
			QuemarTeclas();
			bolita_extra(bolita, Gusano, NULL, ContaBolitas, Borrar, ExcepcionesEscenario);
		}
		if (Spuntuacion >= 50 && ContaEsc == 1)//Escenario 3
		{
			PlaySound(TEXT("B2_Stage_6(tercer nivel arcade).wav"), NULL, SND_LOOP | SND_ASYNC);	//Cancioncita
			QuemarTeclas();
			system("CLS");
			MatarGusano(ExcepcionesEscenario);
			ContaEsc++;
			MatarGusano(Gusano);
			Crear_Gusano(Gusano);
			Direccion = 75;
			AuxDir = Direccion;
			Escenario3(ExcepcionesEscenario);
			gotoxy(30, 23);
			if (CambioEscenario)	cout << "CAMBIO DE ESCENARIO!";
			CambioEscenario = true;
			ConteoInicial();
			gotoxy(30, 23);
			if (CambioEscenario)	cout << "                    ";
			QuemarTeclas();
			bolita_extra(bolita, Gusano, NULL, ContaBolitas, Borrar, ExcepcionesEscenario);
		}
		if (Spuntuacion >= 70 && ContaEsc == 2)//Escenario 4
		{
			PlaySound(TEXT("Adventure_Island_-_Underworld (cuarto nivel arcade).wav"), NULL, SND_LOOP | SND_ASYNC);	//Cancioncita
			QuemarTeclas();
			system("CLS");
			MatarGusano(ExcepcionesEscenario);
			ContaEsc++;
			MatarGusano(Gusano);
			Crear_Gusano(Gusano);
			Direccion = 75;
			AuxDir = Direccion;
			Escenario4(ExcepcionesEscenario);
			gotoxy(30, 23);
			if (CambioEscenario)	cout << "CAMBIO DE ESCENARIO!";
			CambioEscenario = true;
			ConteoInicial();
			gotoxy(30, 23);
			if (CambioEscenario)	cout << "                    ";
			QuemarTeclas();
			bolita_extra(bolita, Gusano, NULL, ContaBolitas, Borrar, ExcepcionesEscenario);
		}
		if (Spuntuacion >= 90 && ContaEsc == 3)//Escenario 5
		{
			PlaySound(TEXT("shadow_colossus(final boss arcade).wav"), NULL, SND_LOOP | SND_ASYNC);	//Cancioncita
			QuemarTeclas();
			system("CLS");
			MatarGusano(ExcepcionesEscenario);
			ContaEsc++;
			MatarGusano(Gusano);
			Crear_Gusano(Gusano);
			Direccion = 75;
			AuxDir = Direccion;
			Escenario5(ExcepcionesEscenario);
			gotoxy(30, 23);
			if (CambioEscenario)	cout << "CAMBIO DE ESCENARIO!";
			CambioEscenario = true;
			ConteoInicial();
			gotoxy(30, 23);
			if (CambioEscenario)	cout << "                    ";
			QuemarTeclas();
			bolita_extra(bolita, Gusano, NULL, ContaBolitas, Borrar, ExcepcionesEscenario);
		}

		MoverGus(Gusano);
		if (Interseccion(Gusano, bolita)){
			Puntuacion(bolita, time1);
			bolita_extra(bolita, Gusano, NULL, ContaBolitas, Borrar, ExcepcionesEscenario);
		}
		Excepciones(Gusano);
		ExcepcionesEsc(Gusano, ExcepcionesEscenario);

		if ((Spuntuacion >= 100 && ContaEsc == 4)){ //GANADOR
			Ganador = true;
			Juego = false;
			Sleep(1000);
			system("ClS");
			gotoxy(1, 5);
			PlaySound(TEXT("Battlefield 4 Main Theme (8-Bit NES_FC Remix).wav"), NULL, SND_ASYNC);
			ImprimirCara(CaraGanar, "                ");
			Sleep(5000);
			system("ClS");
			gotoxy(32, 12);
			MatarGusano(Gusano);
			MatarGusano(ExcepcionesEscenario);
		}

		if (!Juego && !Ganador){	//Si ocurre una excepción (Intersección de la cabeza del gusano con una coordenada "prohibida") acaba el juego
			CambioEscenario = false;
			Vidas--;
			ContaEsc--;
			Juego = true;
		}
	}
	if (!Ganador){
		//PlaySound(TEXT("WAH.wav"), NULL, SND_ASYNC);
		Sleep(1000);
		system("CLS");
		PlaySound(TEXT("smbdeath(muerte).wav"), NULL, SND_ASYNC);
		ImprimirCara(CaraPerder, "          ");
		Sleep(5000);
		system("CLS");
		MatarGusano(Gusano);
		MatarGusano(ExcepcionesEscenario);
	}
}

void ModoVersus(PtrGus &Gusano1, comida &bolita, bool Sel_Dir, PtrGus &ExcepcionesEscenario){

	Consola = GetStdHandle(STD_OUTPUT_HANDLE);
	bool Sel_Dir_J1, Sel_Dir_J2 = true;
	PtrGus Gusano2 = NULL;
	Sel_Dir_J1 = Sel_Dir;
	char Tecla;
	int Comodin1 = -2, Comodin2 = -2;
	Crear_GusVS(Gusano1, Gusano2);
	Dificultad = (Dificultad - 10) / 2;
	Dificultad2 = Dificultad;
	PlaySound(TEXT("bmbr2battle (versus).wav"), NULL, SND_LOOP | SND_ASYNC);	//Cancioncita
	for (int y = 0; y < 5; y++){
		gotoxy(15, 4);
		SetConsoleTextAttribute(Consola, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "Player 2";
		gotoxy(60, 4);
		SetConsoleTextAttribute(Consola, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "Player 1";
		Sleep(250);
		gotoxy(15, 4);
		SetConsoleTextAttribute(Consola, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "Player 2";
		gotoxy(60, 4);
		SetConsoleTextAttribute(Consola, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "Player 1";
		Sleep(250);
	}
	gotoxy(15, 4);
	cout << "        ";
	gotoxy(60, 4);
	cout << "        ";
	primera_bolita(bolita, Posibilidad_BOLIESP, Gusano1, ExcepcionesEscenario);

	int Comida_restante = 25; ///// ------------------------------------>>>>>> CANTIDAD DE BOLITAS PARA ACABAR LA MEJENGA
	while (Comida_restante >= 0){

		gotoxy(25, 25);
		cout << "J1 = (" << ContaBolitas << "|" << Comodin1*-1 << ")" << "	J2 = (" << ContaBolitas2 << "|" << Comodin2*-1 << ")" << "	Restantes = " << Comida_restante << "   ";

		if (bolita.activo){//Si no hay bolita en pantalla, crea una nueva y la coloca
			gotoxy(bolita.pos_x, bolita.pos_y);
			SetConsoleTextAttribute(Consola, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << bolita.forma;
			Comida_restante--;
			bolita.activo = false;
		}

		if (_kbhit()){	//Escanea si el usuario generó un evento de teclado y de existir, lo interpreta
			Tecla = _getch();

			if (Tecla == 'a' || Tecla == 'd' || Tecla == 'w' || Tecla == 's'){ //Si fue el Jugador 2
				Direccion2 = Tecla;
				if ((Direccion2 == 'a' || Direccion2 == 'd') && !Sel_Dir_J2){
					Dificultad2 = Dificultad2 / 1.5;
					Sel_Dir_J2 = true;
				}
				if ((Direccion2 == 'w' || Direccion2 == 's') && Sel_Dir_J2){
					Dificultad2 = Dificultad2 * 1.5;							//|
					Sel_Dir_J2 = false;
				}
			}
			else if (Tecla == -32){
				Direccion = _getch();
				if (Direccion != 75 && Direccion != 77 && Direccion != 72 && Direccion != 80){
					Direccion = AuxDir;
				}
				// Ajuste de la velocidad en cambio de ejes X-Y---------------
				if ((Direccion == 75 || Direccion == 77) && !Sel_Dir_J1) {		//|
					Dificultad = Dificultad / 1.5;							//|
					Sel_Dir_J1 = true;											//|
				}															//|
				if ((Direccion == 72 || Direccion == 80) && Sel_Dir_J1) {		//|
					Dificultad = Dificultad * 1.5;							//|
					Sel_Dir_J1 = false;										//|
				}
			}
			//|
			else if (Tecla == 'q' && Comodin1 < 0){
				gotoxy(bolita.pos_x, bolita.pos_y);
				cout << " ";
				bolita_extra(bolita, Gusano1, Gusano2, Comodin1, Borrar, ExcepcionesEscenario);
				Comida_restante++;
				Borrar = true;
			}
			else if (Tecla == '0' && Comodin2 < 0){
				gotoxy(bolita.pos_x, bolita.pos_y);
				cout << " ";
				bolita_extra(bolita, Gusano1, Gusano2, Comodin2, Borrar2, ExcepcionesEscenario);
				Comida_restante++;
				Borrar2 = true;
			}
		}																//|
		//-----------------------------------------------------------------

		MoverVS(Gusano1, Gusano2);
		if (Interseccion(Gusano1, bolita)){
			bolita_extra(bolita, Gusano1, NULL, ContaBolitas, Borrar, ExcepcionesEscenario);
		}
		if (Interseccion(Gusano2, bolita)){
			bolita_extra(bolita, Gusano1, NULL, ContaBolitas2, Borrar2, ExcepcionesEscenario);
		}
		Excepciones(Gusano1);
		if (!Juego) {

			ContaBolitas = 0;
			MatarGusano(Gusano1);
			Crear_Gusano(Gusano1);
			Direccion = 75;
			AuxDir = Direccion;
			Recuadro();
			Color_Verde(Gusano2, bolita);
			Juego = true;
			//Sleep(500);
		}
		Excepciones(Gusano2);
		if (!Juego) {

			ContaBolitas2 = 0;
			MatarGusano(Gusano2);
			Crear_Gusano(Gusano2);
			Direccion2 = 'a';
			AuxDir2 = Direccion2;
			Recuadro();
			Color_Azul(Gusano1, bolita);
			Juego = true;
			//Sleep(500);
		}

		ExcepcionesEsc(Gusano1, ExcepcionesEscenario);
		if (!Juego) {

			ContaBolitas = 0;
			MatarGusano(Gusano1);
			system("color E");
			Crear_Gusano(Gusano1);
			Direccion = 75;
			AuxDir = Direccion;
			Color_Verde(Gusano2, bolita);
			Juego = true;
			//Sleep(500);
		}

		ExcepcionesEsc(Gusano2, ExcepcionesEscenario);
		if (!Juego) {

			ContaBolitas2 = 0;
			MatarGusano(Gusano2);
			system("color E");
			Crear_Gusano(Gusano2);
			Direccion2 = 'a';
			AuxDir2 = Direccion2;
			Color_Azul(Gusano1, bolita);
			Juego = true;
			//Sleep(500);
		}
	}
	MostrarGanador(Gusano1, Gusano2);

}

void Menu_Ayuda(){
	system("CLS");
	gotoxy(30, 5);
	cout << "Menu de Ayuda" << endl << endl << endl;
	cout << "		1- Modo Survival\n" << "		2- Modo Arcade\n" << "		3- Modo Versus\n" << "		4- Regresar\n";
	cout << "		Seleccione el apartado que desea ver: ";
	Opc2 = _getch();
	system("CLS");
	switch (Opc2){
	case '1': {
				  printf("Modo Survival\nObjetivo: Obtener la mayor puntuacion posible.\nEn este modo deberas seleccionar el nivel de dificultad deseado\n1 - Facil\n2 - Medio\n3 - Dificil\n\nLuego tendras que escoger el escenario que mas te guste, podras visualizar diversos escenarios con solo teclear las\ndireccionales derecha o izquierda en tu teclado, cuando encuentres el escenario deseado, deberas presionar la tecla 'ENTER'\n\n<- : Escenario anterior.\n-> : Escenario siguiente.\nENTER: Seleccionar escenario.\n\nEn este momento tu aventura comienza, deberas mover la serpiente de modo que no impactes contra ninguna parte del \nescenario o contigo mismo, mientras que intentas conseguir las bolitas que irán apareciendo por todo el escenario. Animate!!!, mientras\nmas rapido consigas las bolitas, mas puntuacion tendras!!!. \n\nPara poder controlar tu serpiente dispones de los siguientes\ncontroles:\n\n-> : Mover la serpiente hacia la derecha\n<- : Mover la serpiente hacia la izquierda\n ↑ : Mover la serpiente hacia la arriba\n ↓ : Mover la serpiente hacia la derecha");
				  break;
	}
	}
}


//____________________________________________________________________________________



void main(){

	//----------------Condiciones iniciales----------------------------------------------------
	system("title SnakeRevolution SuperPro");
	Consola = GetStdHandle(STD_OUTPUT_HANDLE);
	bool Salir = false;
	PtrGus ExcepcionEscenario = NULL;
	PtrGus Gusano = NULL;
	PtrJugA ArbolPunt = NULL;
	PtrJugL ListaPunt = NULL;
	CargarPuntuacion(ArbolPunt);
	Cargar(CaraPerder, CaraGanar);
	comida bolita;
	bool Sel_Dir = true; //Permite monitorear el eje de movimiento del gusano "X" o  "Y"
	bolita.activo = false;

	RANDOM();//Permite evitar predicciones en el número aleatorio
	Posibilidad_BOLIESP = rand() % 101;
Reproducir:

	PlaySound(TEXT("Dragon_ball_song(Menu).wav"), NULL, SND_LOOP | SND_ASYNC);
	//-----------------------------------------------------------------------------------------


	while (!Salir){
		LlamarMenu(Gusano, ExcepcionEscenario);

		if (Opc1 == '1'){ //Modo Survival
			SelDificultad();
			time1 = GetHora();
			Escoge_Escenario(ExcepcionEscenario);
			ConteoInicial();
			Crear_Gusano(Gusano);
			ModoSurvival(Gusano, bolita, Sel_Dir, ExcepcionEscenario);
			NuevaPuntuacion(Spuntuacion, ArbolPunt, ListaPunt);
			goto Reproducir;
		}

		if (Opc1 == '2'){//Modo Arcade
			SelDificultad();
			//PlaySound(TEXT("Cancion#.wav"), NULL, SND_LOOP | SND_ASYNC);	//Cancioncita
			time1 = GetHora();
			Crear_Gusano(Gusano);
			ModoArcade(Gusano, bolita, Sel_Dir, ExcepcionEscenario);
			goto Reproducir;
		}

		if (Opc1 == '3'){//Modo Versus
			SelDificultad();
			//PlaySound(TEXT("Cancion#.wav"), NULL, SND_LOOP | SND_ASYNC);	//Cancioncita
			time1 = GetHora();
			Escoge_Escenario(ExcepcionEscenario);
			Direccion = 77;
			AuxDir = Direccion;
			ModoVersus(Gusano, bolita, Sel_Dir, ExcepcionEscenario);
			goto Reproducir;
		}

		if (Opc1 == '4'){//Ayuda
			system("CLS");
			Menu_Ayuda();
		}

		if (Opc1 == '5'){
			system("CLS");
			LiberarLista(ListaPunt);
			TreeToList(ArbolPunt, ListaPunt);
			Listar(ListaPunt);
			LiberarLista(ListaPunt);
		}
		
		if (Opc1 == '6'){//Salir
			MatarGusano(Gusano);
			MatarGusano(ExcepcionEscenario);
			MatarCara(CaraGanar);
			MatarCara(CaraPerder);
			system("CLS");
			gotoxy(20, 10);
			cout << "Cerrando SnakeRevolution SuperPro";
			Sleep(3000);
			Salir = true;
		}
	}
}
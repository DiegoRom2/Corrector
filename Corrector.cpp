/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion. 
	(c) Diego Romero Gonzalez. 318048060
	
	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.
	
******************************************************************************************************************/


#include "stdafx.h"
#include <string.h>
#include "corrector.h"
#define TAMTOKEN 50
#include <stdio.h>
//Funciones publicas del proyecto
/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario	
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/

bool esSeparador(char ch) {
	return (ch == ' ' || ch == '\t' || ch == '\n' || ch == ',' || ch == ';' || ch == '.' || ch == '(' || ch == ')');
}
	
	void Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos)
	{
		// Abrir el archivo en modo lectura
		FILE* fp;
		fopen_s(&fp, szNombre, "r");

		// Verificar que el archivo se abrió correctamente
		if (fp == NULL) {
			return;
		}
		// Inicializar variables
		iNumElementos = 0;
		char palabra[TAMTOKEN];
		int k = 0;

		// Procesar el archivo caracter por caracter hasta el final
		int ch;
		while ((ch = fgetc(fp)) != EOF) {
			// Construir las palabras y realizar estadísticas
			if (k < TAMTOKEN - 1) {  // Evitar desbordamiento del buffer
				if (!esSeparador(ch)) {
					palabra[k++] = tolower(ch);
				}
				else if (k > 0) {
					// Finalizar la palabra y realizar estadísticas
					palabra[k] = '\0';
					iEstadisticas[iNumElementos] = 1;
					strcpy_s(szPalabras[iNumElementos], palabra);
					iNumElementos++;
					k = 0;

					// Eliminar duplicados y actualizar estadísticas
					int elementosFor = iNumElementos - 1;
					for (int i = 0; i < elementosFor; i++) {
						if (strcmp(szPalabras[elementosFor], szPalabras[i]) == 0) {
							iNumElementos--;
							iEstadisticas[i]++;
							iEstadisticas[elementosFor] = 0;
						}
					}
				}
			}
		}
		// Finalizar la última palabra y realizar estadísticas
		if (k > 0) {
			palabra[k] = '\0';
			iEstadisticas[iNumElementos]++;
			strcpy_s(szPalabras[iNumElementos], palabra);
			iNumElementos++;
		}
		// Ordenar alfabéticamente las palabras y sus estadísticas 
		for (int i = 0; i < iNumElementos - 1; i++) {
			for (int j = 0; j < iNumElementos - 1; j++) {
				if (strcmp(szPalabras[j], szPalabras[j + 1]) > 0) {
					// Intercambiar palabras y estadísticas
					char aux[TAMTOKEN];
					strcpy_s(aux, szPalabras[j]);
					strcpy_s(szPalabras[j], szPalabras[j + 1]);
					strcpy_s(szPalabras[j + 1], aux);
					int estadisticaTemp = iEstadisticas[j];
					iEstadisticas[j] = iEstadisticas[j + 1];
					iEstadisticas[j + 1] = estadisticaTemp;
				}
			}
		}

		// Cerrar el archivo
		fclose(fp);
    }

/*****************************************************************************************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/
	void	ListaCandidatas(
		char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
		int		iNumSugeridas,						//Lista de palabras clonadas
		char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
		int		iEstadisticas[],					//Lista de las frecuencias de las palabras
		int		iNumElementos,						//Numero de elementos en el diccionario
		char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
		int		iPeso[],							//Peso de las palabras en la lista final
		int& iNumLista)							//Numero de elementos en la szListaFinal
	{

		//Sustituya estas lineas por su código
		/* strcpy(szListaFinal[0], szPalabrasSugeridas[ 0] ); //la palabra candidata
		iPeso[0] = iEstadisticas[0];			// el peso de la palabra candidata

		iNumLista = 1;							//Una sola palabra candidata
		*/
		// Inicializar la lista final y el contador
		iNumLista = 0;

		// Filtrar palabras sugeridas y calcular pesos
		for (int i3 = 0; i3 < iNumElementos; i3++) {
			int palabraEncontrada = 0;

			for (int a = 0; a < iNumSugeridas && !palabraEncontrada; a++) {
				if (strcmp(szPalabras[i3], szPalabrasSugeridas[a]) == 0) {
					strcpy_s(szListaFinal[iNumLista], szPalabrasSugeridas[a]);
					iPeso[iNumLista] = iEstadisticas[i3];
					iNumLista++;
					palabraEncontrada = 1;
				}
			}
		}
	}

/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void	ClonaPalabras(
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
{
	//Sustituya estas lineas por su código
	strcpy(szPalabrasSugeridas[0], szPalabraLeida); //lo que sea que se capture, es sugerencia
	iNumSugeridas = 1;							//Una sola palabra sugerida
}

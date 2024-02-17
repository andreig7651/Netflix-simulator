/* Gavriliu Andrei - 313CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

typedef struct coada
{
    void* info;
    struct coada *urm;
    
} TCoada, *AQV;

typedef struct episod
{
	int durata;
    int vizionat;
    int vizionat_complet;
	
}TEpisod;

typedef struct sezon
{
	int nr_episoade;
	AQV episod;

}TSezon;

typedef struct serial
{
    int ID;
    char *nume;
    float rating;
    int nr_sezoane;
    int top;
	AQV sezon;
	
}TSerial;

int compara(void *element1, void *element2);
AQV InserareLG(AQV lg, void *ae);
AQV InserareSerial(AQV lg, void *ae, int *poz);
AQV InserareSerialTop(AQV lg, void *ae);
AQV InserareStiva(AQV lg, void *ae);
void AjustareCategorieTop(AQV lg);
int add_sez(char *nume, int nr_episoade, int *vector, AQV *v);
int add(void *element, int *vector, AQV *v);
int add_top(void *element, int *vector, AQV *v);
AQV later(AQV watch_later, char *nume, AQV *v);
int getPosition(AQV lg, char *nume);
AQV watch(AQV currently_watching, AQV *watch_later, AQV *history, char *nume, int durata, AQV *v, int *vizionat_complet);
void eliberareCategorie(AQV el);
void eliberareSerial(AQV el_serial);
void eliberareSezon(AQV el_sezon);
void eliberareEpisod(AQV el_episod);

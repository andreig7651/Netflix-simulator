/* Gavriliu Andrei - 313CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coada.h"

void eliberareMemorie(char *line, FILE *f, AQV *v, AQV watch_later, AQV currently_watching, AQV history) {
	if (line != NULL)
		free(line);
	if (f != NULL)
		fclose(f);
	if (v != NULL) {
		for (int i = 0; i < 4; i++) {
			eliberareCategorie(v[i]);
		}
		free(v);
	}
	eliberareCategorie(watch_later);
	eliberareCategorie(currently_watching);
	eliberareCategorie(history);
}

int main(int argc, char **argv) {	
	AQV *v = (AQV*)calloc(4, sizeof(AQV));
	FILE *f = NULL;

	AQV watch_later = NULL, currently_watching = NULL, history = NULL;

	char infilename[50], outfilename[50];
	if (argv[1] != NULL)//citire argumentelor din linia de comanda
		strcpy(infilename, argv[1]);
	else
		return 0;

	if (argv[2] != NULL)//citire argumentelor din linia de comanda
		strcpy(outfilename,argv[2]);
	else
		return 0;

	char *line = NULL;
	size_t len = 120; //dimensiunea maxima a unei linii in fisierul de intrare
	line = (char *)malloc(len * sizeof(char));
	if (line == NULL) {
		printf("eroare alocare buffer citire din fisierul de intrare\n");
		eliberareMemorie(line, f, v, watch_later, currently_watching, history);
		return 0;
	}

    f = fopen(infilename, "rt");//deschidere fisier pentru citire
	if (f == NULL) {
		printf("eroare deschidere fisier de intrare\n");
		eliberareMemorie(line, f, v, watch_later, currently_watching, history);
		return 0;
	}
	
	while (fgets(line, len, f) != NULL) {
        char *operatia = strtok(line, " ");//primul cuvant de pe fiecare linie din fisier

		if (strcmp(operatia, "add") == 0) {
			int ID = atoi(strtok(NULL, " "));
			char *nume = strtok(NULL, " ");
			float rating = atof(strtok(NULL, " "));
			int nr_sezoane = atoi(strtok(NULL, " "));
			int vector[80];
			int i = 0;
			char *token = strtok(NULL, " ");
			while (token != NULL) {
				vector[i] = atoi(token);
				token = strtok(NULL, " ");					
				i++;
			}
			TSerial *serial = (TSerial *)malloc(sizeof(TSerial));
			serial->ID = ID;
			serial->nume = strdup(nume);
			serial->rating = rating;
			serial->nr_sezoane = nr_sezoane;
			serial->top = 0;
			serial->sezon = NULL;
			int poz = add(serial, vector, v);
			if (poz > 0) {
				FILE *out = fopen(outfilename, "at");
				fprintf(out,"Serialul %s a fost adaugat la pozitia %d.\n", nume, poz);	
				fclose(out);
			}
		}

		if (strcmp(operatia, "add_sez") == 0) {
			char *nume = strtok(NULL, " ");
			int nr_episoade = atoi(strtok(NULL," "));
			int vector[80];
			int i = 0;
			char *token = strtok(NULL, " ");
			while (token != NULL) {
				vector[i] = atoi(token);
				token = strtok(NULL, " ");					
				i++;
			}
			if (add_sez(nume, nr_episoade, vector, v) == 1) {
				FILE *out = fopen(outfilename, "at");
				fprintf(out,"Serialul %s are un sezon nou.\n", nume);	
				fclose(out);
			}
		}

		if (strcmp(operatia, "add_top") == 0) {
			int poz = atoi(strtok(NULL, " "));
			char *nume = strtok(NULL, " ");
			float rating = atof(strtok(NULL, " "));
			int nr_sezoane = atoi(strtok(NULL, " "));
			int vector[80];
			int i = 0;
			char *token = strtok(NULL, " ");
			while (token != NULL) {
				vector[i] = atoi(token);
				token = strtok(NULL, " ");
				i++;
			}
			TSerial *serial = (TSerial *)malloc(sizeof(TSerial));
			serial->ID = 3;
			serial->nume = strdup(nume);
			serial->rating = rating;
			serial->nr_sezoane = nr_sezoane;
			serial->top = poz;
			serial->sezon = NULL;
			add_top(serial, vector, v);
			FILE *out = fopen(outfilename, "at");
			fprintf(out, "Categoria top10: [");
			for (AQV el = v[3]; el != NULL; el = el->urm){
				TSerial *serial = (TSerial *)el->info;
				if (el->urm == NULL)
					fprintf(out, "(%s, %.1f)", serial->nume, serial->rating);
				else
					fprintf(out, "(%s, %.1f), ", serial->nume, serial->rating);
			}
			fprintf(out, "].\n");
			fclose(out);
		}

		if (strcmp(operatia, "later") == 0) {
			char *nume = strtok(NULL, "\n");
			watch_later = later(watch_later, nume, v);
			if (watch_later != NULL) {
				int poz = getPosition(watch_later, nume);
				FILE *out = fopen(outfilename, "at");
				fprintf(out, "Serialul %s se afla in coada de asteptare pe pozitia %d.\n", nume, poz);
				fclose(out);
			}
		}

		if (strcmp(operatia, "watch") == 0) {
			char *nume = strtok(NULL, " ");
			int durata = atoi(strtok(NULL, " "));
			watch_later = later(watch_later, nume, v);//mai intai mutam serialul in coada wath_later
			int vizionat_complet = 1;
			currently_watching = watch(currently_watching, &watch_later, &history, nume, durata, v, &vizionat_complet);
			if (vizionat_complet == 1) {
				FILE *out = fopen(outfilename, "at");
				fprintf(out, "Serialul %s a fost vizionat integral.\n", nume);
				fclose(out);
			}
		}

		if (strcmp(operatia, "show") == 0) {
			char *categoria = strtok(NULL, "\n");
			FILE *out = fopen(outfilename, "at");
			AQV lg = NULL;
			if (strcmp(categoria, "1") == 0) {
				lg = v[0];
			}
			if (strcmp(categoria, "2") == 0) {
				lg = v[1];
			}
			if (strcmp(categoria, "3") == 0) {
				lg = v[2];
			}
			if (strcmp(categoria, "top10") == 0) {
				lg = v[3];
			}
			if (strcmp(categoria, "later") == 0) {
				lg = watch_later;
			}
			if (strcmp(categoria, "watching") == 0) {
				lg = currently_watching;
			}
			if (strcmp(categoria, "history") == 0) {
				lg = history;
			}
			fprintf(out, "Categoria %s: [", categoria);
			for (AQV el = lg; el != NULL; el = el->urm) {
				TSerial *serial = (TSerial*)el->info;
				if (el->urm == NULL)
					fprintf(out, "(%s, %.1f)", serial->nume, serial->rating);
				else
					fprintf(out, "(%s, %.1f), ", serial->nume, serial->rating);

			}
			fprintf(out, "].\n");
			fclose(out);
		}
    }

	eliberareMemorie(line, f, v, watch_later, currently_watching, history);
	
	return 0;
}



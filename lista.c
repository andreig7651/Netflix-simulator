/* Gavriliu Andrei - 313CB */
#include <string.h>
#include <stdlib.h>
#include "coada.h"

int compara(void *element1, void *element2) {
    TSerial *serial1 = (TSerial*) element1;
    TSerial *serial2 = (TSerial*) element2;
    if (strcmp(serial1->nume, serial2->nume) == 0)
        return 0;
    else
        return 1;
}

AQV InserareLG(AQV lg, void *ae) {
	AQV c = malloc(sizeof(TCoada));
	if (c == NULL)
		return NULL;
	c->info = ae;
	if (lg != NULL) {
		//adauga un element in coada listei existente
		AQV el = lg;
		while (el->urm != NULL) {
			el = el->urm;
		}
		el->urm = c;
		c->urm = NULL;
		return lg;//capul listei nu se modifica
	} else {
		//adauga primul element intr-o lista vida
		c->urm = NULL;
		return c;//capul listei devine primul element inserat
	}
}

AQV InserareSerial(AQV lg, void *ae, int *poz) {
	AQV c = malloc(sizeof(TCoada));
	if (c == NULL)
		return NULL;
	c->info = ae;
	if (lg == NULL) {
		//adauga primul serial din categoria lui
		c->urm = NULL;
		*poz = 1;
		return c;//capul listei devine primul element inserat
	}
	//cautam locul de inserare al serialului, respectand ordinea: rating descrescator, nume crescator
	TSerial *serial_nou = (TSerial *)ae;
	AQV el_anterior = NULL;
	int cnt = 0;
	for (AQV el = lg; el != NULL; el = el->urm) {
		TSerial *serial = (TSerial *)el->info;
		if (serial_nou->rating < serial->rating) {
			el_anterior = el;
			cnt++;
			continue;
		} else if (serial_nou->rating == serial->rating && strcmp(serial_nou->nume, serial->nume) > 0) {
			el_anterior = el;
			cnt++;
			continue;
		}
		if (el_anterior == NULL) {
			//serialul nou trebuie inserat la inceputul listei
			c->urm = el;
			*poz = cnt + 1;
			return c;//capul listei devine elementul inserat pe prima pozitie
		} else {
			//serialul nou trebuie inserat intre elementele el_anterior si el
			el_anterior->urm = c;
			c->urm = el;
			*poz = cnt + 1;
			return lg;//capul listei nu se modifica prin inserarea serialului in interiorul listei
		}
	}
	//serialul trebuie inserat la coada listei
	el_anterior->urm = c;
	c->urm = NULL;
	*poz = cnt + 1;
	return lg;//capul listei nu se modifica prin inserarea serialului l;a coada listei
}

AQV InserareSerialTop(AQV lg, void *ae) {
	AQV c = malloc(sizeof(TCoada));
	if (c == NULL)
		return NULL;
	c->info = ae;
	if (lg == NULL) {
		//adauga primul serial din categoria top (top ar trebui sa fie 1)
		c->urm = NULL;
		return c;//capul listei devine primul element inserat
	}
	//cautam locul de inserare al serialului, in functie de ordinea top
	TSerial *serial_nou = (TSerial *)ae;
	AQV el_anterior = NULL;
	int cnt = 0;
	for (AQV el = lg; el != NULL; el = el->urm) {
		TSerial *serial = (TSerial *)el->info;
		if (serial_nou->top > serial->top) {
			el_anterior = el;
			cnt++;
			continue;
		}
		if (el_anterior == NULL) {
			//serialul nou trebuie inserat la inceputul listei (top = 1)
			c->urm = el;
			return c;//capul listei devine elementul inserat pe prima pozitie
		} else {
			//serialul nou trebuie inserat intre elementele el_anterior si el
			el_anterior->urm = c;
			c->urm = el;
			return lg;//capul listei nu se modifica prin inserarea serialului in interiorul listei
		}
	}
	//serialul trebuie inserat la coada listei
	el_anterior->urm = c;
	c->urm = NULL;
	return lg;//capul listei nu se modifica prin inserarea serialului l;a coada listei
}

AQV InserareStiva(AQV lg, void *ae) {
	AQV c = malloc(sizeof(TCoada));
	if (c == NULL)
		return NULL;
	c->info = ae;
	c->urm = lg;
	return c;
}

void AjustareCategorieTop(AQV lg) {
	//corectam valorile top ale serialelor ca sa fie continue si pastram in lista maxim 10 seriale
	int cnt = 1;
	AQV el10 = NULL;//va contine referinta catre al 10-lea serial (daca lista are mai mult de 10 seriale)
	for (AQV el = lg; el != NULL; el = el->urm) {
		TSerial *serial = (TSerial *)el->info;
		serial->top = cnt;
		if (cnt == 10)
			el10 = el;
		cnt++;
	}
	if (el10 != NULL) {
		//stergem din lista serialele cu top > 10
		AQV el = el10->urm, el_aux;
		while (el != NULL) {
			el_aux = el->urm;
			eliberareSerial(el);
			el = el_aux;
		}
		el10->urm = NULL;//marcheaza sfarsitul listei top
	}
}

int add_sez(char *nume, int nr_episoade, int *vector, AQV *v) {
    for (int i = 0; i < 4; i++) {
        AQV prim = v[i];
        for (AQV el = prim; el != NULL; el = el->urm) {
            TSerial *serial = (TSerial*)el->info;
            if (strcmp(nume, serial->nume) == 0) {
				TSezon *sezon = (TSezon *)malloc(sizeof(TSezon));
				sezon->nr_episoade = nr_episoade;
				sezon->episod = NULL;
				serial->sezon = InserareLG(serial->sezon, sezon);
				if (serial->sezon == NULL)
					return 0;//operatia de inserare sezon a esuat
                for (int j = 0; j < nr_episoade; j++) {
                    TEpisod *episod = (TEpisod *)malloc(sizeof(TEpisod));
                    episod->durata = vector[j];
                    episod->vizionat = 0;
                    episod->vizionat_complet = 0;
					sezon->episod = InserareLG(sezon->episod, episod);
					if (sezon->episod == NULL)
						return 0;//operatia de inserare episod a esuat
				}
            }
		}
	}
    return 1;
}

int add(void *element, int *vector, AQV *v) {
    TSerial *serial = (TSerial*)element;
    AQV prim, el;
	prim = v[serial->ID - 1];//salvam inceputul listei
    for (el = prim; el != NULL; el = el->urm) {//verificam ca elementul sa nu fie deja inserat
		if (compara(el->info, element) == 0)
			return 0;//intoarcem 0 in caz ca elementul exista
    }
	int poz;
	AQV lg = InserareSerial(v[serial->ID - 1], element, &poz);
	if (lg == NULL)
		return 0;//intoarcem 0 cand operatia de inserare a esuat
	v[serial->ID - 1] = lg;
    int nr_episoade, s = 0, durata[50];
    for (int i = 0; i < serial->nr_sezoane; i++) {
		nr_episoade = vector[s];
        for (int j = 0; j < nr_episoade; j++)
            durata[j] = vector[s + j + 1];
		if (add_sez(serial->nume, nr_episoade, durata, v) == 0)
			return 0;//intoarcem 0 cand operatia de adaugarea sezon a esuat
        s = s + 1 + nr_episoade;
    }
    return poz;
}

int add_top(void *element, int *vector, AQV *v) {
	TSerial *serial = (TSerial *)element;
	AQV lg = InserareSerialTop(v[3], element);
	if (lg == NULL)
		return 0;//intoarcem 0 cand operatia de inserare a esuat
	v[3] = lg;
	int nr_episoade, s = 0, durata[50];
	for (int i = 0; i < serial->nr_sezoane; i++) {
		nr_episoade = vector[s];
		for (int j = 0; j < nr_episoade; j++)
			durata[j] = vector[s + j + 1];
		if (add_sez(serial->nume, nr_episoade, durata, v) == 0)
			return 0;//intoarcem 0 cand operatia de adaugarea sezon a esuat
		s = s + 1 + nr_episoade;
	}
	AjustareCategorieTop(v[3]);//corectam valorile top ale serialelor ca sa fie continue si pastram in lista maxim 10 seriale
	return 1;
}

AQV later(AQV watch_later, char *nume, AQV *v) {
	//mutam complet serialul, nu il copiem
    for (int i = 0; i < 4; i++) {
		AQV prim = v[i], pre = NULL;
		for (AQV el = prim; el != NULL; el = el->urm) {
			TSerial *serial = (TSerial *)el->info;
			if (strcmp(nume, serial->nume) == 0) {
				watch_later = InserareLG(watch_later, serial);
				if (watch_later == NULL) {
					return NULL;
				}
				if (pre == NULL) {
					//se sterge elementul din capul listei
					v[i] = el->urm;
				} else {
					pre->urm = el->urm;
				}
				free(el);
				if (i == 3) {
					AjustareCategorieTop(v[3]);
				}
				return watch_later;
			}
			pre = el;
		}
    }
	return watch_later;
}

int getPosition(AQV lg, char *nume) {
	int cnt = 1;
	for (AQV el = lg; el != NULL; el = el->urm) {
		TSerial *serial = (TSerial *)el->info;
		if (strcmp(nume, serial->nume) == 0) {
			return cnt;
		}
		cnt++;
	}
	return 0;
}

AQV watch(AQV currently_watching, AQV *watch_later, AQV *history, char *nume, int durata, AQV *v, int *vizionat_complet) {
	AQV el_serial = NULL; //elementul ce va contine serialul care se va viziona
	//il cautam  mai intai in stiva de currently_watching
	for (AQV el = currently_watching; el != NULL; el = el->urm) {
		TSerial *serial = (TSerial *)el->info;
		if (strcmp(nume, serial->nume) == 0) {
			el_serial = el;
		}
	}
	if (el_serial == NULL) {
		//serialul nu exista in stiva currently_watching, trebuie sa il mutam din coada watch_later (unde a fost mutat prin actiunea later anterioara)
		AQV pre = NULL;
		for (AQV el = *watch_later; el != NULL; el = el->urm) {
			TSerial *serial = (TSerial *)el->info;
			if (strcmp(nume, serial->nume) == 0) {
				currently_watching = InserareStiva(currently_watching, serial);
				if (currently_watching == NULL) {
					return NULL;
				}
				el_serial = currently_watching; //elementul nou introdus este pe primul loc in stiva
				if (pre == NULL) {
					//se sterge elementul din capul listei
					*watch_later = el->urm;
				}
				else {
					pre->urm = el->urm;
				}
				free(el);
				break;
			}
			pre = el;
		}
	}

	if (el_serial == NULL) {
		return NULL;
	}

	//vizionam serialul
	*vizionat_complet = 1; //presupunem ca serialul va fi vizionat complet
	TSerial *serial = (TSerial *)el_serial->info;
	for (int i = 0; i < serial->nr_sezoane; i++) {
		AQV el_sezon = serial->sezon;
		if (el_sezon != NULL) {
			TSezon *sezon = (TSezon *)el_sezon->info;
			for (int j = 0; j < sezon->nr_episoade; j++) {
				AQV el_episod = sezon->episod;
				if (el_episod != NULL) {
					TEpisod *episod = (TEpisod *)el_episod->info;
					int restDeVizionat = episod->durata - episod->vizionat;
					if (durata >= restDeVizionat) {
						episod->vizionat = episod->durata;
						episod->vizionat_complet = 1;
						durata = durata - restDeVizionat;
					} else {
						episod->vizionat = episod->vizionat + durata;
						durata = 0;
						*vizionat_complet = 0;
					}
				}
			}
		}
	}
	if (*vizionat_complet == 1) {
		//daca serialul a fost vizionat complet, il mutam in stiva history
		AQV pre = NULL;
		for (AQV el = currently_watching; el != NULL; el = el->urm) {
			if (el == el_serial) {
				TSerial *serial = (TSerial *)el->info;
				*history = InserareStiva(*history, serial);
				if (pre == NULL) {
					//se sterge elementul din capul listei
					currently_watching = el->urm;
				}
				else {
					pre->urm = el->urm;
				}
				free(el);
				break;
			}
			pre = el;
		}
	}

	return currently_watching;
}

void eliberareCategorie(AQV el) {
	AQV el_aux;
	while (el != NULL) {
		el_aux = el->urm;
		eliberareSerial(el);
		el = el_aux;
	}
}

void eliberareSerial(AQV el_serial) {
	TSerial *serial = (TSerial *)el_serial->info;
	AQV el_sezon = serial->sezon, el_aux;
	while (el_sezon != NULL) {
		el_aux = el_sezon->urm;
		eliberareSezon(el_sezon);
		el_sezon = el_aux;
	}
	free(serial->nume);
	free(serial);
	free(el_serial);
}

void eliberareSezon(AQV el_sezon) {
	TSezon *sezon = (TSezon *)el_sezon->info;
	AQV el_episod = sezon->episod, el_aux;
	while (el_episod != NULL) {
		el_aux = el_episod->urm;
		eliberareEpisod(el_episod);
		el_episod = el_aux;
	}
	free(sezon);
	free(el_sezon);
}

void eliberareEpisod(AQV el_episod) {
	TEpisod *episod = (TEpisod *)el_episod->info;
	free(episod);
	free(el_episod);
}

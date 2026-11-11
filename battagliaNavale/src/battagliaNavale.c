#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Sceglie casualmente quale dei due giocatori inizierà la partita. */
int scegliChiInizia()
{
	return rand() % 2 + 1;
}

/*
 * Inizializza entrambe le griglie: quella mostrata al giocatore e quella
 * interna, usata per registrare navi e colpi.
 */
void riempiGriglia(int griglia[10][10], int grigliaBackEnd[10][10])
{
	int i, j;
	i = 0;
	while (i < 10)
	{
		j = 0;
		while (j < 10)
		{
			griglia[i][j] = 0;
			grigliaBackEnd[i][j] = 0;
			j = j + 1;
		}
		i = i + 1;
	}
}

/* Stampa una griglia traducendo i valori numerici nei simboli del gioco. */
void stampaGriglia(int griglia[10][10])
{
	int i, j;
	char simbolo;

	printf("    |");
	j = 0;
	while (j < 10)
	{
		printf(" %c  |", 'A' + j);
		j = j + 1;
	}
	printf("\n");

	i = 0;
	while (i < 10)
	{
		printf("-----");
		j = 0;
		while (j < 10)
		{
			printf("-----");
			j = j + 1;
		}
		printf("\n%3d |", i + 1);

		j = 0;
		while (j < 10)
		{
			/* -2 = nave colpita, -1 = acqua, valori positivi = nave. */
			if (griglia[i][j] == -2)
			{
				simbolo = 'X';
			}
			else if (griglia[i][j] == -1)
			{
				simbolo = 'O';
			}
			else if (griglia[i][j] > 0)
			{
				simbolo = 'N';
			}
			else
			{
				simbolo = ' ';
			}
			printf(" %c  |", simbolo);
			j = j + 1;
		}
		printf("\n");
		i = i + 1;
	}
	printf("-----");
	j = 0;
	while (j < 10)
	{
		printf("-----");
		j = j + 1;
	}
	printf("\nN = nave, X = colpito, O = acqua\n");
}

/*
 * Fa posizionare tutte le navi a un giocatore. Le navi devono essere dritte,
 * avere la lunghezza prevista e non toccare le altre, nemmeno in diagonale.
 */
void posizionaNavi(int grigliaBackEnd[10][10], int giocatore, char nome[])
{
	/* Le posizioni dell'array corrispondono agli ID assegnati alle navi. */
	int lunghezze[] = {5, 4, 4, 3, 3, 3, 2, 2, 2, 2};
	int n, i, rigaIniziale, rigaFinale, rigaInizialeIndice, rigaFinaleIndice, colonnaInizialeIndice, colonnaFinaleIndice, valida, id = 1, max, min;
	int rInizio, rFine, cFine, cInizio, r, c;
	char colonnaIniziale, colonnaFinale;

	printf("Posizionamento navi di %s (giocatore %d)\n", nome, giocatore);
	n = 0;
	while (n < 10)
	{
		valida = 0;
		stampaGriglia(grigliaBackEnd);
		while (valida == 0)
		{
			printf("inserire la riga iniziale della nave da %d caselle (1-10): ", lunghezze[n]);
			scanf("%d", &rigaIniziale);

			printf("inserire la colonna iniziale della nave da %d caselle (A-J maiuscolo): ", lunghezze[n]);
			scanf(" %c", &colonnaIniziale);

			printf("inserire la riga finale della nave da %d caselle (1-10): ", lunghezze[n]);
			scanf("%d", &rigaFinale);

			printf("inserire la colonna finale della nave da %d caselle (A-J maiuscolo): ", lunghezze[n]);
			scanf(" %c", &colonnaFinale);

			colonnaInizialeIndice = colonnaIniziale - 'A';
			colonnaFinaleIndice = colonnaFinale - 'A';
			rigaInizialeIndice = rigaIniziale - 1;
			rigaFinaleIndice = rigaFinale - 1;

			/* Controlla che le coordinate siano valide e che la nave sia dritta. */
			valida = 1;
			if (rigaIniziale < 1 || rigaIniziale > 10 || colonnaIniziale < 'A' || colonnaIniziale > 'J' || rigaFinale < 1 || rigaFinale > 10 || colonnaFinale < 'A' || colonnaFinale > 'J' || ((rigaIniziale != rigaFinale) && (colonnaIniziale != colonnaFinale)))
			{
				valida = 0;
			}
			else if (rigaIniziale == rigaFinale)
			{
				if (colonnaInizialeIndice < colonnaFinaleIndice)
				{
					min = colonnaInizialeIndice;
					max = colonnaFinaleIndice;
				}
				else
				{
					min = colonnaFinaleIndice;
					max = colonnaInizialeIndice;
				}

				/* Gli estremi devono coprire esattamente la lunghezza richiesta. */
				if (max - min + 1 == lunghezze[n])
				{
					// rettangolo da controllare: nave + 1 casella di margine
					rInizio = rigaInizialeIndice - 1;
					rFine = rigaInizialeIndice + 1;
					cInizio = min - 1;
					cFine = max + 1;

					// clamp ai bordi della griglia
					if (rInizio < 0)
						rInizio = 0;
					if (rFine > 9)
						rFine = 9;
					if (cInizio < 0)
						cInizio = 0;
					if (cFine > 9)
						cFine = 9;

					r = rInizio;
					while (r <= rFine)
					{
						c = cInizio;
						while (c <= cFine)
						{
							if (grigliaBackEnd[r][c] != 0)
							{
								valida = 0;
							}
							c = c + 1;
						}
						r = r + 1;
					}
				}
				else
				{
					valida = 0;
				}
			}
			else
			{
				if (rigaInizialeIndice < rigaFinaleIndice)
				{
					min = rigaInizialeIndice;
					max = rigaFinaleIndice;
				}
				else
				{
					min = rigaFinaleIndice;
					max = rigaInizialeIndice;
				}

				/* Anche in verticale, la distanza tra gli estremi deve essere corretta. */
				if (max - min + 1 == lunghezze[n])
				{
					// rettangolo da controllare: nave + 1 casella di margine
					rInizio = min - 1;
					rFine = max + 1;
					cInizio = colonnaInizialeIndice - 1;
					cFine = colonnaInizialeIndice + 1;

					// clamp ai bordi della griglia
					if (rInizio < 0)
						rInizio = 0;
					if (rFine > 9)
						rFine = 9;
					if (cInizio < 0)
						cInizio = 0;
					if (cFine > 9)
						cFine = 9;

					r = rInizio;
					while (r <= rFine)
					{
						c = cInizio;
						while (c <= cFine)
						{
							if (grigliaBackEnd[r][c] != 0)
							{
								valida = 0;
							}
							c = c + 1;
						}
						r = r + 1;
					}
				}
				else
				{
					valida = 0;
				}
			}

			if (valida == 0)
			{
				printf("Posizione non valida, riprova.\n");
			}
		}

		/* Registra la nave usando lo stesso ID in tutte le sue caselle. */
		i = 0;
		while (i < lunghezze[n])
		{
			if (rigaInizialeIndice == rigaFinaleIndice)
			{
				/* Posizione orizzontale: parte dalla colonna più a sinistra. */
				if (colonnaInizialeIndice > colonnaFinaleIndice)
				{
					min = colonnaFinaleIndice;
				}
				else
				{
					min = colonnaInizialeIndice;
				}

				grigliaBackEnd[rigaInizialeIndice][min + i] = id;
			}
			else
			{
				/* Posizione verticale: parte dalla riga più in alto. */
				if (rigaInizialeIndice > rigaFinaleIndice)
				{
					min = rigaFinaleIndice;
				}
				else
				{
					min = rigaInizialeIndice;
				}

				grigliaBackEnd[min + i][colonnaFinaleIndice] = id;
			}
			i = i + 1;
		}
		/* Il prossimo posizionamento riceverà un ID distinto. */
		id = id + 1;
		n = n + 1;
	}
}

/* Legge i nomi, sceglie chi inizia e fa posizionare le navi a entrambi i giocatori. */
int main()
{
	/* Griglia interna di ogni giocatore: 0 = vuoto, valori positivi = ID della nave. */
	int grigliaGiocatore1[10][10];
	int grigliaGiocatore2[10][10];

	int backEndGrigliaGiocatore1[10][10];
	int backEndGrigliaGiocatore2[10][10];

	int giocatoreIniziale;
	char nomeGiocatore1[5];
	char nomeGiocatore2[5];

	/* Legge i nomi e impedisce che i due giocatori abbiano lo stesso nome. */
	printf("Inserisci il nome del giocatore 1 (massimo 4 caratteri): ");
	scanf("%4s", nomeGiocatore1);
	printf("Inserisci il nome del giocatore 2 (massimo 4 caratteri): ");
	scanf("%4s", nomeGiocatore2);

	while (strcmp(nomeGiocatore2, nomeGiocatore1) == 0)
	{
		printf("Inserire un nome diverso dal giocatore 1: ");
		scanf("%4s", nomeGiocatore2);
	}

	srand(time(NULL));
	giocatoreIniziale = scegliChiInizia();
	if (giocatoreIniziale == 1)
	{
		printf("Inizia %s!\n", nomeGiocatore1);
	}
	else
	{
		printf("Inizia %s!\n", nomeGiocatore2);
	}
	riempiGriglia(grigliaGiocatore1, backEndGrigliaGiocatore1);
	riempiGriglia(grigliaGiocatore2, backEndGrigliaGiocatore2);

	/* Il giocatore estratto per primo posiziona le navi per primo. */
	if (giocatoreIniziale == 1)
	{
		posizionaNavi(backEndGrigliaGiocatore1, 1, nomeGiocatore1);
		posizionaNavi(backEndGrigliaGiocatore2, 2, nomeGiocatore2);
	}
	else
	{
		posizionaNavi(backEndGrigliaGiocatore2, 2, nomeGiocatore2);
		posizionaNavi(backEndGrigliaGiocatore1, 1, nomeGiocatore1);
	}

	/* Mostra le griglie finali con tutte le navi posizionate. */
	printf("Griglia finale di %s:\n", nomeGiocatore1);
	stampaGriglia(backEndGrigliaGiocatore1);
	printf("Griglia finale di %s:\n", nomeGiocatore2);
	stampaGriglia(backEndGrigliaGiocatore2);

	return 0;
}

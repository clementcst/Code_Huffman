#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <time.h>

#include "file.h"

#define MALLOC(x)((x * ) malloc(sizeof(x)))


int main(int argc, char *argv[]){

   //Declaration des variables 
   int ocu[126];
   int choix;
   Ptrliste listeocutrie = MALLOC(Liste);
   PArbre arbrehuff = MALLOC(Arbre);
   Ptrlistenew newcode = MALLOC(Listenew);
   PLrecup data = MALLOC(Listerecup);
   char* sequencetmp = "sequence.txt";
   char* manueluser = "AIDEUSER.txt";
   clock_t t1, t2;
    
    
    while ( (choix = getopt(argc,argv,"dhc")) != -1){
        switch (choix){
            case 'c':                       //Cas de la copression de argv[2]
                t1 = clock();
                testfile(argv[2], argv[3], argv[4]);
                int v = taillefile(argv[2],1);
                initab(ocu);
                int cpt = nbrcaracteres(argv[2]);
                
                char *tab= malloc(cpt * sizeof(char*));
                tab = remplitab(argv[2],cpt);
                compteurocu(tab,ocu,cpt);
                int a = NbrcaracteresNnul(ocu);

                listeocutrie = ajoutocu(ocu);
                listeocutrie = Retireoccurencesnul(listeocutrie);
             
                PLArbre ListeArbHuf = CopieListe(listeocutrie);
                arbrehuff = remplirhuff(ListeArbHuf);
                complement(arbrehuff);

                Ptrlistenew newcodetmp = newcode;
                nouveauxcodage(newcodetmp,arbrehuff,listeocutrie);
        
                compressionhuffman(argv[2],sequencetmp,newcode);
                compressionhuffman2(sequencetmp,argv[4]);
                outilcompression(argv[3],listeocutrie,newcode,a,cpt);
                int w = taillefile(argv[4],0);
                remove(sequencetmp);
                t2 = clock();
                printf("Compression de %s...\n", argv[2]);
                printf("Fait (%fs).\n", (float)(t2-t1)/CLOCKS_PER_SEC);
                float pourcentage = ((w)*100)/v;
                printf("%d octets compressés en %d octets (%f %% ).\n",v,w,pourcentage);

                break;
            case 'd':              //Cas décompression de argv[2]
                testfile(argv[2], argv[3], argv[4]);
                t1 = clock();
                recupdata(data,argv[3]);
                recuperationseq(argv[2],sequencetmp);
                decompressionhuff(data,argv[4],sequencetmp,nbrcaracteres(sequencetmp));
                remove(sequencetmp);
                t2 = clock();
                printf("Decompression de %s...\n", argv[2]);
                printf("Fait (%fs).\n", (float)(t2-t1)/CLOCKS_PER_SEC);
                break;
            case 'h':             //Cas affichage du manuel
                affichagefile(manueluser);

                break;
            default:
                printf("-----------ERREUR DANS LA SAISIE DE LA COMMANDE-----------\n\n");
                printf("ENTREZ -c pour Compresser.\n");
                printf("ENTREZ -d pour Décopresser.\n");
                printf("ENTREZ -h pour Afficher une aide.\n");
        }
    } 
 
    return 0;
}



    

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <time.h>

#define MALLOC(x)((x * ) malloc(sizeof(x)))     //Raccourci allocation mémoire 

//--------------------------------------------------------------------------------------------------------------------------------------------------------

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~DECLARATION STRUCTURES/LISTE + PROCEDURES/FONCTIONS DE BASE ~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct l{                  // structure de liste caractère + occurences 
    char caracteres;
    int occurences;
    struct l* suivant;
}Liste;

typedef Liste* Ptrliste;

typedef struct arb{              // structure d'arbre caractère + occurences + fils (utilisation pour arbre de Huffman)
    char caracteres;
    int occurences;
    struct arb* fg;
    struct arb* fd;
    char equilibre[50];
}Arbre;

typedef Arbre* PArbre;


typedef struct larb{                //structure de liste pour construire l'arbre de Huffman 
    PArbre arb;
    struct larb* suivant;
}LArb;

typedef LArb* PLArbre;

typedef struct newl{                //structure de liste caractère + code (issue de l'arbre de Huffman)
    char caracteres;
    char code[50];
    struct newl* suivant;
}Listenew;

typedef Listenew* Ptrlistenew;
 
typedef struct lrecup{             //structure de liste pour aider décompression
    char caracteres;
    int occurences;
    char code[50];
    struct lrecup* suivant;
}Listerecup;

typedef Listerecup* PLrecup;



void initab(int tab[]){                //Initialise tableau int de taille 127 a 0  
    for(int i = 0 ; i < 128; i++){
        tab[i] = 0;
    }
}


Ptrliste creernoeud(char c,int x, Ptrliste suivant){           //création noeud pour Ptrliste 
	Ptrliste ptr = MALLOC(Liste);
	ptr->caracteres = c;
    ptr->occurences = x;
	ptr->suivant = suivant;
	return ptr;
}

PArbre creerarbre(char c, int x, PArbre fg, PArbre fd){      //création noeud pour PArbre
    PArbre ptr = MALLOC(Arbre);
    ptr->caracteres = c;
    ptr->occurences = x;
    ptr->fg = fg;
    ptr->fd = fd;
    strcpy(ptr->equilibre, "");
    return ptr;
}

PLArbre creerlarbre(char c, int x, PLArbre suiv){                   //création noeud pour PLArbre
    PLArbre ptr = MALLOC(LArb);
    PArbre arbre = creerarbre(c,x,NULL,NULL);
    ptr->arb = arbre;
    ptr->suivant = suiv;
    return ptr;
}

PLArbre creerlarbreavance(char c, int x, PArbre a, PArbre b, PLArbre suiv){                   //création noeud pour PLArbre avec fils
    PLArbre ptr = MALLOC(LArb);
    PArbre arbre = creerarbre(c,x,a,b);
    ptr->arb = arbre;
    ptr->suivant = suiv;
    return ptr;
}

Ptrlistenew creernoeudnew(char c, char code[], Ptrlistenew suiv){           //création noeud pour Ptrlistenew
    Ptrlistenew  liste = MALLOC(Listenew);
    liste->caracteres = c;
    strcpy(liste->code, "");
    liste->suivant = suiv;
    return liste;
}

int estvide(Ptrliste liste){              //test si Ptrliste est vide
	if (!liste){
		return 1;
	}return 0;	
}	

int estvidenew(Ptrlistenew liste){            //test si Ptrlistenew est vide
	if (!liste){
		return 1;
	}return 0;	
}	

int estvidearb(PArbre arb){              //test si PArbre est vide
    if(!arb){ 
        return 1;
    }return 0;
}

int estvidelarb(PLArbre plarb){            //test si PLArbre est vide
    if (!plarb){
        return 1;
    } return 0;
}

int estfeuille(PArbre a) {                 //test si PLArbre est feuille
return !(a->fg || a->fd);
}

int max(int a, int b) { return (a<b ? b : a); }          //retourne le max de 2 entier


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~PROCEDURES/FONCTIONS LISTES/TABLEAUX~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


Ptrliste ajoutliste(Ptrliste liste, char c, int x) {       //ajoute un noeud dans une liste chainé Ptrliste trié croissante en fonction de son nombre sa variable occurence
	Ptrliste ptr= creernoeud(c, x, NULL);
	Ptrliste p = liste;
	
    if (estvide(p)) {
	   return ptr;
	 } 
	if (p->occurences>x) { 
		ptr->suivant=p;
		return ptr;
	}
	while (p->suivant && p->suivant->occurences<x) {
		p=p->suivant; 
	}
	ptr->suivant = p->suivant; 
	p->suivant = ptr;
	return liste;
}

PLArbre AjoutListeArbre(PLArbre new, PLArbre plarb) {         //ajoute un noeud dans une liste chainé PLArbre trié croissante en fonction de son nombre sa variable occurence
	PLArbre p = plarb;
	
    if (estvidelarb(p)) {
	   return new;
	} 
	if (p->arb->occurences > new->arb->occurences) { 
		new->suivant= p ;
		return new;
	}
	while (p->suivant && p->suivant->arb->occurences < new->arb->occurences) {
		p = p->suivant; 
	}
	new -> suivant = p->suivant; 
	p->suivant = new;
	return plarb;
}

Ptrliste ajoutocu(int ocu[]){            //Remplie une liste chainé Ptrliste trié de façon croissante en fonction du nombre d'occurence d'un tableau int de taille 127
    Ptrliste ptr = NULL;
    for (int i = 0 ; i < 128; i++){
        ptr = ajoutliste(ptr,i, ocu[i]);
    }
    return ptr;
}


int nbrcaracteres(char*  nomFichier){        //Compte le nombre de caractère dans un fichier texte
    int cpt = 0;

    FILE *file = NULL;
    file = fopen(nomFichier, "r");
   if (file== NULL){
        printf("Impossible d'ouvrir le fichier %s\n",nomFichier);
        exit(1);
    }
    while (fgetc(file) != EOF) cpt ++;
    fclose(file);
    return cpt;
}

int taillefile(char* nomFichier,int a){          //Compte le nombre de caractère dans un fichier texte avec option pour fichier texte contenant des caractères binaires
    FILE *file = NULL;
    if (a==1) {
        file = fopen(nomFichier, "r");
    }else{
        file = fopen(nomFichier, "rb");
    }
    if (file== NULL){
        printf("Impossible d'ouvrir le fichier %s\n",nomFichier);
        exit(1);
    }
    int cpt = 0; int c;
    while ((c=fgetc(file)) != EOF) cpt ++;
    fclose(file);
    return cpt;
}

void affichagefile(char* nomFichier){
   char c;

   FILE *file = NULL;
   file = fopen(nomFichier, "r");
   if (file== NULL){
        printf("Impossible d'ouvrir le fichier %s\n",nomFichier);
        exit(1);
    }
    while ((c=fgetc(file)) != EOF) {
        printf("%c",c);
    }
    fclose(file);
}

char* remplitab(char* nomFichier, int cpt ){        //Remplie un tableau char* à partir d'un fichier texte 
    int c; 
    int i = 0;
    char *tabtexte = malloc(cpt * sizeof(char*));

    FILE *file = NULL;
    file = fopen(nomFichier, "r");
    if (file== NULL){
        printf("Impossible d'ouvrir le fichier %s\n",nomFichier);
        exit(1);
    }

    while ((c = fgetc(file)) != EOF){ 
        tabtexte[i] = (unsigned char)c;
        i++;
    }
    fclose(file);
    return tabtexte;
}

void testfile(char* file1, char* file2, char* file3){                     //Procedure qui vérifie si 3 fichier sont bien pris en paramètre
    if ((file1 == NULL) || (file2 == NULL) || (file3 == NULL)){
        printf("-------------------------------------------------------\n");
        printf("---ERREUR DANS L'EXPRESSION DE LA LIGNE DE COMMANDE----\n");
        printf("POUR ACCEDER A UNE AIDE ENTREZ LA COMMANDE ./Huffman -h\n");
        printf("-------------------------------------------------------\n");
        exit(1);
    }
}

void compteurocu(char *tab, int ocu[], int cpt){          //Remplie un tableau int en fonction du nombre d'occurence de tous les différents caractères dans un tableau de char
    int a;

    for(int i = 0; i < cpt; i++){
        a = tab[i];
        ocu[a]++;
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~PROCEDURES/FONCTIONS POUR ARBRE HUFFMAN ~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


Ptrliste Retireoccurencesnul(Ptrliste liste){                 //retire les noeud du liste chainé Ptrliste si sa varibale occurence = 0
    while(liste->occurences == 0) liste = liste->suivant;
    return liste;
}


PLArbre CopieListe(Ptrliste listeocu){                 //Copie une liste chaine de Ptrliste dans une liste PLArbre
    PLArbre ListeArbHuf = creerlarbre(listeocu->caracteres, listeocu->occurences, NULL);
    PLArbre ptr = ListeArbHuf;
    while(listeocu->suivant){
        ptr->suivant = creerlarbre(listeocu->suivant->caracteres, listeocu->suivant->occurences,NULL);
        ptr = ptr->suivant; listeocu = listeocu->suivant;
    }
    return ListeArbHuf;
}

 
PArbre remplirhuff(PLArbre plarb){             //Construction de l'arbre de HUffman en fonction d'une liste chainé de PLArbre 
    
    while(plarb->suivant->suivant){
       PLArbre tmp = creerlarbreavance(0,(plarb->arb->occurences) + (plarb->suivant->arb->occurences), plarb->arb, plarb->suivant->arb,NULL);
       plarb->suivant->suivant = AjoutListeArbre(tmp, plarb->suivant->suivant);
       plarb = plarb->suivant->suivant;
    }
    PLArbre tmpp = creerlarbreavance(0 ,(plarb->arb->occurences) + (plarb->suivant->arb->occurences), plarb->arb, plarb->suivant->arb,NULL);
    plarb = tmpp;
    return plarb->arb;
}  
 
int* remplirtab(int *tab, int x){                 //Place dans un tableau d'entier un entier x a sa premiere place non occupé.
    int i = 0;

    while(tab[i]){
        i++;
    }
    tab[i] = x;
    return tab;
}


void RechercheABR(PArbre a,PArbre cool, char x) {                   //Recherche dans un arbre le noeud qui contient le caratères x, et place la varibale code du noeud dans un autre noeud hors de l'arbre
   if (a){
       if (a->caracteres == x){
           cool->caracteres = a->caracteres;
           strcpy(cool->equilibre,a->equilibre);
       }else {
           RechercheABR(a->fg,cool,x);
           RechercheABR(a->fd,cool,x);
       }
   }
}   
  

int NbrcaracteresNnul(int ocu[]){          //retourne le nombre de caractère nul dans un tableau d'entier
    int a = 0;

    for (int i = 0 ; i < 127; i++){
        if (ocu[i] != 0) a++;
    }
    return a;
}


void complement(PArbre arbre){            //Donne a la valeur code de chaque noeud d'un arbre pere->code + 1 si fils droit (resp 0 si fils gauche)
    if (arbre){
        if (arbre->fg){
            strcpy(arbre->fg->equilibre,arbre->equilibre);
            strcat(arbre->fg->equilibre,"0");
            complement(arbre->fg);
        }
        if (arbre->fd){
            strcpy(arbre->fd->equilibre,arbre->equilibre);
            strcat(arbre->fd->equilibre,"1");
            complement(arbre->fd);
        }
        
    }
}


void nouveauxcodage(Ptrlistenew newcodetmp, PArbre arbrehuff, Ptrliste listeocu){      //cherche la valeur code d'un noeud contenant une valeur caractère special et la place dans une liste Ptrlistenew
    PArbre cool = MALLOC(Arbre);

    while(listeocu->suivant){
        RechercheABR(arbrehuff,cool,listeocu->caracteres);
        newcodetmp->caracteres = cool->caracteres;
        strcpy(newcodetmp->code,cool->equilibre);
        newcodetmp->suivant = MALLOC(Listenew);
        newcodetmp = newcodetmp->suivant;
        listeocu = listeocu->suivant;
    }
    RechercheABR(arbrehuff,cool,listeocu->caracteres);
    newcodetmp->caracteres = cool->caracteres;
    strcpy(newcodetmp->code,cool->equilibre);
}   
    

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~PROCEDURES/FONCTIONS AIDE A COMPRESION & DECOMPRESSION~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


char* cherchecode(Ptrlistenew liste, char carac){         //retourne dans une liste chaine Ptrlistenew la valeur codage du noeud contenant comme valeur caractère carac
    Ptrlistenew tmp = MALLOC(Listenew);
    tmp = liste;

    while(!estvidenew(tmp->suivant)){
        if (tmp->caracteres == carac){
            return tmp->code;
        }else {
            tmp = tmp->suivant;
        }  
    }
    return tmp->code;
}


char cherchecarac(PLrecup liste, char *find){         //cherche la chaine de caractère find dans une liste PLrecup et retourne a valeur caractère du noeud
    if (!liste) return '\0';
    if (!strcmp(find,liste->code)) return liste->caracteres;
    return cherchecarac(liste->suivant,find);

}


void ajoutab(char *tab, char c){        //Ajoute le caractère c a la premiere place vide d'un tableau de char
    int a = 0;
    while (tab[a] != '\0') a++;
    tab[a] = c;
}


int conversionbin(char *a){          //Retourne la convertion en int d'un nombre binaire pris en parametre
    char *str=malloc(2*sizeof(char));
    int b = 128; 
    int tot = 0; 
    int c;

    for(int i =  0; i < 8 ; i++){
        str[0]=a[i];
        str[1]='\0';
        c = atoi(str);
        tot = tot + c*b;
        b =b /2;
    }
    return tot;
}


void initio(char *tab){         //Initialise un tableau char de taille 7 au caractère vide
    for(int i = 0 ; i < 7; i++){
        tab[i] = '0';
    }
}

void initioo(char *tab){         //Initialise un tableau char de taille 20 au caractère vide
    for(int i = 0 ; i < 20; i++){
        tab[i] = '\0';
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~PROCEDURES COMPRESSION~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void compressionhuffman(char* origine, char* sequence, Ptrlistenew liste){             //procédure qui créer la sequence du texte en fonction des codes issue de l'arbre huffman                                              
    FILE* original = NULL;                                                    //et la copie dans un fichier texte
    FILE* nouveau = NULL; 
    char c; 
    char *tmp = malloc(sizeof(char*));

    original = fopen(origine,"r");
    if (original== NULL){
        printf("Impossible d'ouvrir le fichier %s\n",origine);
        exit(1);
    }
    nouveau = fopen(sequence,"w");
    if (nouveau == NULL){
        printf("Impossible d'ouvrir le fichier %s\n",sequence);
        exit(1);
    }
    while((c = fgetc(original)) != EOF){
        tmp = cherchecode(liste,c);
        fprintf(nouveau,"%s",tmp);
        tmp = malloc(sizeof(char*));
    }
    fclose(original); fclose(nouveau);
}


void compressionhuffman2(char* sequence, char* compress){                               //Procedure qui a partir du fichier contenant la sequence, recuprer par
    FILE* sequencefile = NULL;                                                          //bloque de 8 les '0' et '1', le convertie en binaire et l'insère dans notre fichier compressé
    FILE* nouveau = NULL;
    char c; 
    char *txt= malloc(7 * sizeof(char*)); 
    int d;
    int cpt = 0; 

    nouveau = fopen(compress,"wab");
    if (nouveau== NULL){
        printf("Impossible d'ouvrir le fichier %s\n",compress);
        exit(1);
    }
    sequencefile = fopen(sequence,"r");
    if (sequencefile == NULL){
        printf("Impossible d'ouvrir le fichier %s\n",sequence);
        exit(1);
    }
    while((c = getc(sequencefile)) != EOF){
       txt[cpt] = c;
       cpt++;
       if (cpt == 8 ){
           cpt = 0;
           d = conversionbin(txt); 
           initio(txt); 
           fwrite(&d,1,1,nouveau);
       }        
    }
    fclose(sequencefile); fclose(nouveau);
}
 
void outilcompression(char * aide, Ptrliste listeocu, Ptrlistenew newcode, int a,int cpt){     //Procedure que une fichier qui va être utile pour la decompression
    FILE* aidecompression = NULL;                                                              // contenant nombre de caractères (total et différents), les caractères avec occurences + codage 
    aidecompression = fopen(aide,"w");
    if (aidecompression == NULL){
        printf("Impossible d'ouvrir le fichier %s\n",aide);
        exit(1);
    }
    Ptrliste tmp = listeocu; 
    Ptrlistenew tmpp = newcode;
    fprintf(aidecompression,"%d <---------- Nombre de caractères différents codés.%c",a,10);
    fprintf(aidecompression,"%d <---------- Nombre total de caractères dans le fichier.%c",cpt,10);
    fprintf(aidecompression,"caractère(ascii) :     fréquence:       code issue arbre huffman:%c",10);
    while(!estvide(tmp->suivant) && !estvidenew(tmpp)){
        fprintf(aidecompression,"%d                    %d                    %s%c", tmp->caracteres, tmp->occurences, tmpp->code, 10);
        tmp = tmp->suivant;
        tmpp = tmpp->suivant;
    }
    fprintf(aidecompression,"%d                    %d                    %s%c", tmp->caracteres, tmp->occurences, tmpp->code, 10);
    fclose(aidecompression);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~PROCEDURES DECOMPRESSION~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void recupdata(PLrecup liste, char* info){                     //Procedure qui a partir du fichier d'aide pour la decompression, récupère les codages de tous les caractères
    FILE* data = NULL;                                         // et les insère dans une liste de type PLrecup contenant 
    PLrecup tmp = liste;                                        
    int numligne = 3;                                
    int cpt = 0;             
    int texte; 
    char ocu[7];
    char u[2]; 
    char c;
    char *code = malloc(sizeof(char*));  

    data = fopen(info, "r");
    if (data== NULL){
        printf("Impossible d'ouvrir le fichier %s\n",info);
        exit(1);
    } 
    fscanf(data,"%s",u);
    int a = atoi(u);    
    for(int i = 0 ; i < a ; i++){
        while(numligne != cpt){
            c = fgetc(data);
            if(c == '\n'){
                cpt++;
            }
        }
        fscanf(data, "%d", &texte);
        fseek(data,20,SEEK_CUR);
        fscanf(data, "%s", ocu);
        fseek(data,20,SEEK_CUR);
        fscanf(data, "%s", code);
        tmp->caracteres = texte;
        tmp->occurences = atoi(ocu);
        strcpy(tmp->code,code);
        if (!tmp->suivant) tmp->suivant = MALLOC(Listerecup);
        tmp = tmp->suivant;
        numligne++;
    }
    fclose(data);
}

void recuperationseq(char* compresse, char* new){                 //Procedure qui a partir du fichier compressé, copie dans un fichier texte une copie de la séquence initiale
    int c;
    int b;

    FILE * test = fopen(compresse,"rb");
    if (test == NULL){
        printf("Impossible d'ouvrir le fichier %s\n",compresse);
        exit(1);
    }
    FILE* recup = fopen(new,"w");
    if (recup == NULL){
        printf("Impossible d'ouvrir le fichier %s\n",new);
        exit(1);
    }
    while((c = fgetc(test)) != EOF){
        b = 128;
        for(int i = 0 ; i < 8; i++){
            if (c>=b){
                fprintf(recup,"1");
            }else {
                fprintf(recup,"0");
            }
            c = c%b;
            b = b/2;
        }
    }
    fclose(test);
    fclose(recup);

}


void decompressionhuff(PLrecup newcode, char* nouveautexte, char* recupseq,int x){                 //Procédure qui à partir du nouveau fichier texte de la séquence et des codages de tous les caractères
    char yenni[x];
    char codage[20];
    char c; 
    int i = 0;
    
    FILE* new = fopen(nouveautexte,"w");                                                           //recréait le texte de base et le copie dans un fichier texte
    if (new == NULL){
        printf("Impossible d'ouvrir le fichier %s\n",nouveautexte);
        exit(1);
    }
    FILE* newseq = fopen(recupseq, "r");
    if (newseq == NULL){
        printf("Impossible d'ouvrir le fichier %s\n",recupseq);
        exit(1);
    }
    while( (c=getc(newseq)) != EOF){
        yenni[i] = c; i++;
    }
    initioo(codage);
    for(int q = 0 ; q < x; q++){
        ajoutab(codage,yenni[q]);
        if (cherchecarac(newcode,codage) != '\0'){
            fprintf(new,"%c",cherchecarac(newcode,codage));
            initioo(codage);
        }
    }
    fclose(new); fclose(newseq);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------FICHIER README-------------------
--------------------------------------------------

Le code comprend un ensemble de 2 fichiers:
'file.h'
'huffman.c'

Les lignes de compilations des deux fichiers sont :
gcc -Wall -o file file.h
gcc -Wall -o huffman huffman.c

Les différentes options en ligne de commande sont :
Pour la compression :                    -c
Pour la décompression:                   -d
Pour affichier une aide d'utilisation :  -h

--------------
COMPRESSION : |
--------------
Exemple ligne de commande :
  
    ./huffman -c fichieroriginal.txt info.txt fichiercompresse.txt 

	fichieroriginal.txt  : Fichier texte du texte à compresser
        info.txt             : Fichier texte contenant informations nécéssaire à la décompression.
 	fichiercompresse.txt : Fichier texte  compressé.
 
	Le fichier compressé étant en binaire il n'est pas possible de lire en l'ouvrant simplement, pour l'afficher vous devez écrire 
	la commande suivante dans un terminal:   xxd -b fichiercompresse.txt 

---------------      
DECOMPRESION : |
--------------
Exemple ligne de commande :

     ./huffman -d fichiercompresse.txt info.txt nouveaufichier.txt 

	fichiercompresse.txt : Fichier texte  compressé.
        info.txt             : Fichier texte contenant informations nécéssaire à la décompression(obligatoirement le même généré lors de la compression).
        nouveaufichier.txt   : Fichier texte décompressé.

---------------------
AIDE D'UTILISATION : |
---------------------
Exemple ligne de commande :

      ./huffman -h

________________________________________________________________________________________________________________________________________________________________________
      
INFOS UTILES SUR LE PROGRAMME :

- Dans l'entré en ligne de commande des fichier.txt (compression ou décompression), ne pas nommer vos fichier 'README.txt' ou 'AIDEUSER.txt' sous peine d'écraser les 
  fichiers contenant les explications et les aides de compilation et d'éxecution du programme.

- Ce programme à été créée et sur la distribution Linux Debian, mais à été testé sur d'autre distribution Linux comme Ubuntu(et est fonctionnel).

- Après l'option -c ou -d entrez forcémment 3 fichier.txt correspondant au fichier expliqué au-dessus sinon il y aura une erreur.

- Après la compression, il est nécéssaire de ne pas supprimer le fichier "info.txt" (voir utilité au dessus) pour pouvoir par la suite.

- Pour que le programme fonctionnent correctement, il est nécéssaire d'être rigoureux dans quel ordre les fichier.txt sont écrit en ligne de commande.

__________________________________________________________________________________________________________________________________________________________________________











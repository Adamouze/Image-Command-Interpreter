#include <stdio.h>
#include <stdlib.h>
#include "projet.h"

enum direction{Nord,Est,Sud,Ouest};

struct contenu_ligne{
    char* cara_ligne; 
    ssize_t nbre_cara;
};

struct couleur{
    unsigned char Rouge;
    unsigned char Vert;
    unsigned char Bleu;
};

struct opacite{
    unsigned char Opacite;
};

struct pixel{
    couleur RVB;
    opacite O;
};

struct sceau_de_couleur{
    couleur RVB;
    list_couleur suiv_sdc; 
};

struct sceau_opacite{
    opacite o;
    list_opacite suiv_opacite;
};

struct image{
    ssize_t taille_image;
    pixel** cellule;
};

struct position{
    ssize_t courante[2];
    ssize_t marquee[2];
    enum direction dir;
};

struct pile_calque{
    ssize_t sommet;
    image contenu[Taille];
};

struct pile_pos{
    ssize_t sommet;
    ssize_t taille;
    position* contenu_pos;
};


void init_position(position* pos){
    ssize_t i;
    for(i=0;i<2;i+=1){
        pos->courante[i]=0;
        pos->marquee[i]=0;
    }
    pos->dir=Est;
}

pile_pos creer_pile_pos(ssize_t taille){
    pile_pos p_pos;
    p_pos.sommet=-1;
    p_pos.taille=taille*taille;
    p_pos.contenu_pos=malloc((taille*taille)*sizeof(position));
    return p_pos;
}

void empiler_pile_pos(pile_pos* p_pos, position pos){
    if(p_pos->sommet<((p_pos->taille)-1)){
        p_pos->sommet+=1;
        p_pos->contenu_pos[p_pos->sommet]=pos;
    }
}

position depiler_pile_pos(pile_pos* p_pos){
    if(p_pos->sommet>-1){
        position pos=p_pos->contenu_pos[p_pos->sommet];
        p_pos->sommet-=1;
        return pos;
    }
    else{
        exit(3);
    }
}

void free_pile_pos(pile_pos p_pos){
    free(p_pos.contenu_pos);
}

void mise_a_jour_position_marquee(position* pos){
    ssize_t i;
    for(i=0;i<2;i+=1){
        pos->marquee[i]=pos->courante[i];
    }
}

contenu_ligne fonction_contenu_une_ligne(){
    char* cara_ligne=NULL;
    size_t longueur=0;
    ssize_t nbre_cara=getline(&cara_ligne,&longueur,stdin);
    contenu_ligne c;
    c.cara_ligne=cara_ligne;
    c.nbre_cara=nbre_cara;
    return c;
}

image creer_image(ssize_t taille){
    image im;
    im.taille_image=taille;
    ssize_t i;
    im.cellule=(pixel**)malloc(im.taille_image*sizeof(pixel*));
    for(i=0;i<im.taille_image;i+=1){
        im.cellule[i]=(pixel*)malloc(im.taille_image*sizeof(pixel));
    }
    return im;
}

void afficher_image(image im){          
    ssize_t i;
    ssize_t j;
    for(i=0;i<im.taille_image;i+=1){
        for(j=0;j<im.taille_image;j+=1){
            printf("%c%c%c",im.cellule[i][j].RVB.Rouge,im.cellule[i][j].RVB.Vert,im.cellule[i][j].RVB.Bleu);
        }
    }
}

void free_image(image im){
    ssize_t i;
    for(i=0;i<im.taille_image;i+=1){
        free(im.cellule[i]);
    }
    free(im.cellule);
}

pile_calque creer_pile_calque(){
    pile_calque p_calc;
    p_calc.sommet=-1;
    return p_calc;
}

void ajouter_image_pile_calque(pile_calque* p_calc,image im){
    if((p_calc->sommet)<(Taille-1)){
        p_calc->sommet+=1;
        p_calc->contenu[p_calc->sommet]=im;
    }
}

image depiler_image_pile_calque(pile_calque *p_calc){
    if((p_calc->sommet)>-1){
        image im=p_calc->contenu[p_calc->sommet];
        p_calc->sommet-=1;
        return im;
    }
    else{
        exit(0);
    }

}
void decoupage_calque(pile_calque* p_calc){
    if(p_calc->sommet<=0){
        return;
    }
    else{
        image calc_masque=depiler_image_pile_calque(p_calc);
        image calc_a_decoupe=depiler_image_pile_calque(p_calc);
        ssize_t i;
        ssize_t j;
        opacite alphaO;
        for(i=0;i<calc_a_decoupe.taille_image;i+=1){
            for(j=0;j<calc_a_decoupe.taille_image;j+=1){
                alphaO.Opacite=calc_masque.cellule[i][j].O.Opacite;
                calc_a_decoupe.cellule[i][j].RVB.Rouge=(unsigned char)(calc_a_decoupe.cellule[i][j].RVB.Rouge*alphaO.Opacite/255);
                calc_a_decoupe.cellule[i][j].RVB.Vert=(unsigned char)(calc_a_decoupe.cellule[i][j].RVB.Vert*alphaO.Opacite/255);
                calc_a_decoupe.cellule[i][j].RVB.Bleu=(unsigned char)(calc_a_decoupe.cellule[i][j].RVB.Bleu*alphaO.Opacite/255);
                calc_a_decoupe.cellule[i][j].O.Opacite=(unsigned char)(calc_a_decoupe.cellule[i][j].O.Opacite*alphaO.Opacite/255);
            }
        }
        ajouter_image_pile_calque(p_calc,calc_a_decoupe);
        free_image(calc_masque);
    }
}

void fusion_calque(pile_calque* p_calc){
    if(p_calc->sommet<=0){
        return;
    }
    else{
        image calc_c0=depiler_image_pile_calque(p_calc);
        image calc_c1=depiler_image_pile_calque(p_calc);
        ssize_t i;
        ssize_t j;
        opacite alphaO;
        for(i=0;i<calc_c1.taille_image;i+=1){
            for(j=0;j<calc_c1.taille_image;j+=1){
                alphaO.Opacite=calc_c0.cellule[i][j].O.Opacite;
                calc_c1.cellule[i][j].RVB.Rouge=(unsigned char)(calc_c0.cellule[i][j].RVB.Rouge+(calc_c1.cellule[i][j].RVB.Rouge*(255-alphaO.Opacite)/255));
                calc_c1.cellule[i][j].RVB.Vert=(unsigned char)(calc_c0.cellule[i][j].RVB.Vert+(calc_c1.cellule[i][j].RVB.Vert*(255-alphaO.Opacite)/255));
                calc_c1.cellule[i][j].RVB.Bleu=(unsigned char)(calc_c0.cellule[i][j].RVB.Bleu+(calc_c1.cellule[i][j].RVB.Bleu*(255-alphaO.Opacite)/255));
                calc_c1.cellule[i][j].O.Opacite=(unsigned char)(calc_c0.cellule[i][j].O.Opacite+(calc_c1.cellule[i][j].O.Opacite*(255-alphaO.Opacite)/255));          
            }
        }
        ajouter_image_pile_calque(p_calc,calc_c1);
        free_image(calc_c0);
    }
}

void afficher_pile_calque(pile_calque p_calc){
    ssize_t i;
    for(i=0;i<(p_calc.sommet+1);i+=1){
        printf("%lde pile_calque :\n",i+1);
        afficher_image(p_calc.contenu[i]);
    }
}



list_couleur creer_sceau_de_couleur(){
    return NULL;
}

list_opacite creer_sceau_opacite(){
    return NULL;
}

void ajouter_sceau_de_couleur(list_couleur* sdc,couleur add){
    list_couleur nouveau_list_couleur=malloc(sizeof(struct sceau_de_couleur));
    nouveau_list_couleur->RVB=add;
    nouveau_list_couleur->suiv_sdc=*sdc;
    *sdc=nouveau_list_couleur;
}

void ajouter_sceau_opacite(list_opacite* so, opacite add){
    list_opacite nouveau_list_opacite=malloc(sizeof(struct sceau_opacite));
    nouveau_list_opacite->o=add;
    nouveau_list_opacite->suiv_opacite=*so;
    *so=nouveau_list_opacite;
}

ssize_t longueur_sceau_couleur(list_couleur sdc){
    ssize_t longueur=0;
    while(sdc!=NULL){
        longueur+=1;
        sdc=sdc->suiv_sdc;
    }
    return longueur;
} 

ssize_t longueur_sceau_opacite(list_opacite so){
    ssize_t longueur=0;
    while(so!=NULL){
        longueur+=1;
        so=so->suiv_opacite;
    }
    return longueur;
}

couleur moyenne_sceau_couleur(list_couleur sdc){
    ssize_t somme[3]={0,0,0};
    couleur moyenne;
    ssize_t longueur=longueur_sceau_couleur(sdc);
    while(sdc!=NULL){
        somme[0]+=(ssize_t)(sdc->RVB.Rouge);
        somme[1]+=(ssize_t)(sdc->RVB.Vert);
        somme[2]+=(ssize_t)(sdc->RVB.Bleu);
        sdc=sdc->suiv_sdc;
    }
    if(longueur>0){
        moyenne.Rouge=(unsigned char)(somme[0]/longueur);
        moyenne.Vert=(unsigned char)(somme[1]/longueur);
        moyenne.Bleu=(unsigned char)(somme[2]/longueur);
    }
    else{
        moyenne.Rouge=(unsigned char)(0);
        moyenne.Vert=(unsigned char)(0);
        moyenne.Bleu=(unsigned char)(0);
    }

    return moyenne;
}

opacite moyenne_sceau_opacite(list_opacite so){
    opacite moyenne;
    if(so==NULL){
        moyenne.Opacite=255;
        return moyenne;
    }

    else{
        ssize_t longueur=longueur_sceau_opacite(so);
        ssize_t somme;
        while(so!=NULL){
            somme+=(ssize_t)(so->o.Opacite);
            so=so->suiv_opacite;
        }
        moyenne.Opacite=(unsigned char)(somme/longueur);
        return moyenne;
    }
}

void vider_sceau_couleur(list_couleur* sdc){
    list_couleur tmp=creer_sceau_de_couleur();
    while((*sdc)!=NULL){
        tmp=(*sdc);
        *sdc=(*sdc)->suiv_sdc;
        free(tmp);
    }
}

void vider_sceau_opacite(list_opacite* o){
    list_opacite tmp=creer_sceau_opacite();
    while((*o)!=NULL){
        tmp=(*o);
        *o=(*o)->suiv_opacite;
        free(tmp);
    }
}

void afficher_sceau_couleur(list_couleur sdc){
    printf("sceau couleur :\n");
    while(sdc!=NULL){
        printf("[%hhu %hhu %hhu]->",sdc->RVB.Rouge,sdc->RVB.Vert,sdc->RVB.Bleu);
        sdc=sdc->suiv_sdc;
    }
    printf("[]\n");
}

void afficher_sceau_opacite(list_opacite so){
    printf("sceau opacite :\n");
    while(so!=NULL){
        printf("[%hhu]->",so->o.Opacite);
        so=so->suiv_opacite;
    }
    printf("[]\n");
}

pixel calcul_pixel_courant(list_couleur sdc, list_opacite so){
    pixel pi;
    pi.O=moyenne_sceau_opacite(so);
    couleur clr_courant=moyenne_sceau_couleur(sdc);
    pi.RVB.Rouge=(unsigned char)(clr_courant.Rouge*pi.O.Opacite/255);
    pi.RVB.Vert=(unsigned char)(clr_courant.Vert*pi.O.Opacite/255);
    pi.RVB.Bleu=(unsigned char)(clr_courant.Bleu*pi.O.Opacite/255);
    return pi;
}

int max(int a,int b){
    if(a>=b){
        return a;
    }
    else{
        return b;
    }
}

void trace_ligne(pile_calque* p_calc, list_couleur sdc, list_opacite so, position pos){
    image calc=depiler_image_pile_calque(p_calc);
    ssize_t delta_x=pos.courante[0]-pos.marquee[0];
    ssize_t delta_y=pos.courante[1]-pos.marquee[1];
    ssize_t d=max(abs(delta_x),abs(delta_y));
    ssize_t s;
    if((delta_x*delta_y)>0){
        s=0;
    }
    else{
        s=1;
    }
    ssize_t x=(ssize_t)(pos.marquee[0]*d+(d-s)/2);
    ssize_t y=(ssize_t)(pos.marquee[1]*d+(d-s)/2);
    ssize_t i;
    for(i=0;i<d;i+=1){
        calc.cellule[(ssize_t)(x/d)][(ssize_t)(y/d)]=calcul_pixel_courant(sdc,so);
        x+=delta_x;
        y+=delta_y;
    }
    calc.cellule[pos.courante[0]][pos.courante[1]]=calcul_pixel_courant(sdc,so);
    ajouter_image_pile_calque(p_calc,calc);
}

void remplissage(image* calc,position pos,pixel ancienne_couleur,pixel nouvelle_couleur){
    ssize_t taille=calc->taille_image;
    int** historique=malloc(taille*sizeof(int*));
    ssize_t i;
    ssize_t j;
    for(i=0;i<taille;i+=1){
        historique[i]=malloc(taille*sizeof(int));
        for(j=0;j<taille;j+=1){
            historique[i][j]=0;
        }
    }
    pile_pos p_pos=creer_pile_pos(taille);
    empiler_pile_pos(&p_pos,pos);
    while(p_pos.sommet>=0){
        position n=depiler_pile_pos(&p_pos);
        if(historique[n.courante[0]][n.courante[1]]==1){
            continue;
        }
        historique[n.courante[0]][n.courante[1]]=1;
        calc->cellule[n.courante[0]][n.courante[1]]=nouvelle_couleur;
        position nord=n;
        position sud=n;
        position est=n;
        position ouest=n;

        nord.courante[0]-=1;

        sud.courante[0]+=1;
        
        est.courante[1]+=1;
  
        ouest.courante[1]-=1;

        if(nord.courante[0]>=0 && nord.courante[0]<taille && nord.courante[1]>=0 && nord.courante[1]<taille && historique[nord.courante[0]][nord.courante[1]]==0 && calc->cellule[nord.courante[0]][nord.courante[1]].RVB.Rouge==ancienne_couleur.RVB.Rouge && calc->cellule[nord.courante[0]][nord.courante[1]].RVB.Vert==ancienne_couleur.RVB.Vert  && calc->cellule[nord.courante[0]][nord.courante[1]].RVB.Bleu==ancienne_couleur.RVB.Bleu && calc->cellule[nord.courante[0]][nord.courante[1]].O.Opacite==ancienne_couleur.O.Opacite){
            empiler_pile_pos(&p_pos,nord);
        }
        if(sud.courante[0]>=0 && sud.courante[0]<taille && sud.courante[1]>=0 && sud.courante[1]<taille && historique[sud.courante[0]][sud.courante[1]]==0 && calc->cellule[sud.courante[0]][sud.courante[1]].RVB.Rouge==ancienne_couleur.RVB.Rouge && calc->cellule[sud.courante[0]][sud.courante[1]].RVB.Vert==ancienne_couleur.RVB.Vert && calc->cellule[sud.courante[0]][sud.courante[1]].RVB.Bleu==ancienne_couleur.RVB.Bleu && calc->cellule[sud.courante[0]][sud.courante[1]].O.Opacite==ancienne_couleur.O.Opacite){
            empiler_pile_pos(&p_pos,sud);
        }
        if(est.courante[0]>=0 && est.courante[0]<taille && est.courante[1]>=0 && est.courante[1]<taille && historique[est.courante[0]][est.courante[1]]==0 && calc->cellule[est.courante[0]][est.courante[1]].RVB.Rouge==ancienne_couleur.RVB.Rouge && calc->cellule[est.courante[0]][est.courante[1]].RVB.Vert==ancienne_couleur.RVB.Vert && calc->cellule[est.courante[0]][est.courante[1]].RVB.Bleu==ancienne_couleur.RVB.Bleu && calc->cellule[est.courante[0]][est.courante[1]].O.Opacite==ancienne_couleur.O.Opacite){
            empiler_pile_pos(&p_pos,est);
        }
        if(ouest.courante[0]>=0 && ouest.courante[0]<taille && ouest.courante[1]>=0 && ouest.courante[1]<taille && historique[ouest.courante[0]][ouest.courante[1]]==0 && calc->cellule[ouest.courante[0]][ouest.courante[1]].RVB.Rouge==ancienne_couleur.RVB.Rouge && calc->cellule[ouest.courante[0]][ouest.courante[1]].RVB.Vert==ancienne_couleur.RVB.Vert && calc->cellule[ouest.courante[0]][ouest.courante[1]].RVB.Bleu==ancienne_couleur.RVB.Bleu && calc->cellule[ouest.courante[0]][ouest.courante[1]].O.Opacite==ancienne_couleur.O.Opacite){
            empiler_pile_pos(&p_pos,ouest);
        } 
    }
    free_pile_pos(p_pos);
    for(i=0;i<taille;i+=1){
        free(historique[i]);
    }
    free(historique);
}



int main(int argc, char* argv[]){
    if(argc!=1){
        fprintf(stderr,"L'usage est ainsi : %s < <chemin de votre fichier>\n",argv[0]);
        exit(1);
    }
    // Récupération de la première ligne -> taille du fichier
    contenu_ligne c=fonction_contenu_une_ligne();
    ssize_t taille_fichier=(ssize_t)strtoul(c.cara_ligne,NULL,10);

    // Etat de la machine initial
    image im=creer_image(taille_fichier);
    pile_calque p_calc=creer_pile_calque();
    ajouter_image_pile_calque(&p_calc,im);
    list_couleur sdc=creer_sceau_de_couleur();
    list_opacite so=creer_sceau_opacite();

    couleur add_couleur; //add_couleur et add_opacite vont servir d'intermédiaire afin d'ajouter une couleur/opacite à notre sceau correspondant
    opacite add_opacite;

    position pos; // pos va contenir ma position courante et marquee
    init_position(&pos); // permet d'initialiser les positions courante et marquee à (0,0) et la direction à l'Est

    

    while(c.nbre_cara!=-1){
        c=fonction_contenu_une_ligne(); // Permet de lire la ligne contenant toute les commandes
        if(c.nbre_cara==-1){ // Permet de revérifier la condition dans le while pour éviter de faire une boucle infinie
            break;
        }
        ssize_t a;
        for(a=0;a<c.nbre_cara;a+=1){ // Pour chaque caractère présent dans c.cara_ligne
            switch(c.cara_ligne[a]){ // Permet d'effectuer l'action correspondant à la commande du caractère d'où l'utilisation du switch
                case 'n':
                    add_couleur.Rouge=0;
                    add_couleur.Vert=0;
                    add_couleur.Bleu=0;
                    ajouter_sceau_de_couleur(&sdc,add_couleur);
                    break;

                case 'r':
                    add_couleur.Rouge=255;
                    add_couleur.Vert=0;
                    add_couleur.Bleu=0;
                    ajouter_sceau_de_couleur(&sdc,add_couleur);
                    break;
                
                case 'g':
                    add_couleur.Rouge=0;
                    add_couleur.Vert=255;
                    add_couleur.Bleu=0;
                    ajouter_sceau_de_couleur(&sdc,add_couleur);
                    break;

                case 'b':
                    add_couleur.Rouge=0;
                    add_couleur.Vert=0;
                    add_couleur.Bleu=255;
                    ajouter_sceau_de_couleur(&sdc,add_couleur);
                    break;

                case 'y':
                    add_couleur.Rouge=255;
                    add_couleur.Vert=255;
                    add_couleur.Bleu=0;
                    ajouter_sceau_de_couleur(&sdc,add_couleur);
                    break;

                case 'm':
                    add_couleur.Rouge=255;
                    add_couleur.Vert=0;
                    add_couleur.Bleu=255;
                    ajouter_sceau_de_couleur(&sdc,add_couleur);
                    break;

                case 'c':
                    add_couleur.Rouge=0;
                    add_couleur.Vert=255;
                    add_couleur.Bleu=255;
                    ajouter_sceau_de_couleur(&sdc,add_couleur);
                    break;

                case 'w':
                    add_couleur.Rouge=255;
                    add_couleur.Vert=255;
                    add_couleur.Bleu=255;
                    ajouter_sceau_de_couleur(&sdc,add_couleur);
                    break;

                case 't':
                    add_opacite.Opacite=0;
                    ajouter_sceau_opacite(&so,add_opacite);
                    break;

                case 'o':
                    add_opacite.Opacite=255;
                    ajouter_sceau_opacite(&so,add_opacite);
                    break;
                
                case 'i':
                    vider_sceau_couleur(&sdc);
                    vider_sceau_opacite(&so);
                    break;
                
                case 'v':
                    if(pos.dir==Est){                         //Si la position courante est EST
                        if(pos.courante[1]>=(ssize_t)(taille_fichier-1)){ //Si on se trouve au bout à droite de l'image et qu'on veut encore avancer -> on retourne au début de l'image
                            pos.courante[1]=0;   
                        }
                        else{
                            pos.courante[1]+=1;
                        }
                    }
                    else if(pos.dir==Ouest){         //Si la position courante est Ouest
                        if(pos.courante[1]<=0){      //Si on se trouve au bout à gauche de gauche de l'image et qu'on veut encore avancer -> on retourne tout à droite de l'image
                            pos.courante[1]=(ssize_t)(taille_fichier-1);
                        }
                        else{
                            pos.courante[1]-=1;
                        }   
                    }
                    else if(pos.dir==Nord){     //Si la position courante est Nord
                        if(pos.courante[0]<=0){ //Si on se trouve tout en haut de l'image et qu'on veut encore avancer -> on retourne tout en bas de l'image
                            pos.courante[0]=(ssize_t)(taille_fichier-1);
                        }
                        else{
                            pos.courante[0]-=1;
                        }
                    }
                    else if(pos.dir==Sud){                            //Si la position courante est Sud
                        if(pos.courante[0]>=(ssize_t)(taille_fichier-1)){ //Si on se trouve tout en bas de l'image et qu'on veut encore avancer -> au retourne tout en haut de l'image
                            pos.courante[0]=0;
                        }
                        else{
                        pos.courante[0]+=1;
                        }
                    }
                    break;
                
                case 'h':
                    if(pos.dir==Ouest){                         
                        pos.dir=Nord;
                    }
                    else{
                        pos.dir+=1;
                    }
                    break;

                case 'a':
                    if(pos.dir==Nord){
                        pos.dir=Ouest;
                    }
                    else{
                        pos.dir-=1;
                    }
                    break;

                case 'p':
                    mise_a_jour_position_marquee(&pos);
                    break;
                
                case 'l':
                    trace_ligne(&p_calc,sdc,so,pos);
                    break;
                
                case 'f':
                    image calque=depiler_image_pile_calque(&p_calc);
                    pixel pi_courant=calcul_pixel_courant(sdc,so);
                    remplissage(&calque,pos,calque.cellule[pos.courante[0]][pos.courante[1]],pi_courant);
                    ajouter_image_pile_calque(&p_calc,calque);
                    break;

                case 's':
                    if(p_calc.sommet<(Taille-1)){
                        image calque=creer_image(taille_fichier);
                        ajouter_image_pile_calque(&p_calc,calque);
                        break;
                    }
                    break;
                
                case 'e':
                    fusion_calque(&p_calc);
                    break;
                
                case 'j':
                    decoupage_calque(&p_calc);
                    break;

                default:
                    break;
            }
        }
    }
    // Affiche final du calque au sommet de la pile sur la sortie standard
    printf("P6\n%li %li\n255\n",taille_fichier,taille_fichier);
    image calc_final=depiler_image_pile_calque(&p_calc);
    afficher_image(calc_final);
    // Libère la mémoire de tout les éléments afin de ne pas avoir de fuite
    vider_sceau_couleur(&sdc);
    vider_sceau_opacite(&so);
    free_image(calc_final);
    free(c.cara_ligne);
    return 0;
}


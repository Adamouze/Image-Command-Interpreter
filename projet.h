#ifndef PROJET_H
#define PROJET_H

#define Taille 10

typedef struct contenu_ligne contenu_ligne;

typedef struct couleur couleur;

typedef struct opacite opacite;

typedef struct pixel pixel;

typedef struct sceau_de_couleur* list_couleur;

typedef struct sceau_opacite* list_opacite;

typedef struct image image;

typedef struct position position;

typedef struct pile_calque pile_calque;

typedef struct pile_pos pile_pos;

/*
@requires: Rien
@assigns: Rien
@ensures: Retourne une ligne d'un fichier
*/
contenu_ligne fonction_contenu_une_ligne();


/*
@requires: Rien
@assigns: initialise la position courante et marquée à (0,0)
@ensures: Rien
*/
void init_position(position* pos);

/*
@requires: taille > 0
@assigns: Rien
@ensures: Retourne une pile dynamique de taille : taille² 
*/
pile_pos creer_pile_pos(ssize_t taille);

/*
@requires: La pile de postion p_pos n'est pas pleine
@assigns: On va ajouter la position pos à la pile p_pos
@ensures: Rien
*/
void empiler_pile_pos(pile_pos* p_pos, position pos);

/*
@requires: La pile de postion p_pos n'est pas vide
@assigns: On va depiler l'image au sommet de la pile
@ensures: l'image au sommet de la pile
*/
position depiler_pile_pos(pile_pos* p_pos);

/*
@requires: Rien
@assigns: La fonction permet de libérer l'espace alloué à la pile de position
@ensures: Rien
*/
void free_pile_pos(pile_pos p_pos);


/*
@requires: Rien
@assigns: La position marquée prend les valeurs de la position courante
@ensures: Rien
*/
void mise_a_jour_position_marquee(position* pos);


/*
@requires: taille > 0
@assigns: Rien
@ensures: Retourne une image ayant tout ses pixels (couleur et opacité) nuls de la taille correspondante à celle mise en paramètre
*/
image creer_image(ssize_t taille);


/*
@requires: Rien
@assigns: Rien
@ensures: Retourne rien mais affiche sur la sortie standard la matrice d'image au format PPM
*/
void afficher_image(image im);      


/*
@requires: Rien
@assigns: La fonction permet de libérer l'espace alloué à l'image 
@ensures: Rien
*/
void free_image(image im);

/*
@requires: Rien
@assigns: Rien
@ensures: Retourne une pile de calque vide bien définie
*/
pile_calque creer_pile_calque();

/*
@requires: Une pile de calque non pleine
@assigns: Ajoute le calque im dans la pile de calque
@ensures: Rien
*/
void ajouter_image_pile_calque(pile_calque* p_calc,image im);


/*
@requires: une pile de calque non vide
@assigns: Retire le calque au sommet de la pile de calque
@ensures: Retourne le calque au sommet de la pile de calque
*/
image depiler_image_pile_calque(pile_calque *p_calc);

/*
@requires: Rien
@assigns: Affiche les matrices des calques contenues dans la pile
@ensures: Rien
*/
void afficher_pile_calque(pile_calque p_calc);

/*
@requires: une pile de calque avec au moins 2 calques à l'intérieur sinon on ne fait rien
@assigns: decoupage le calque en dessous du sommet de la pile de calque en prenant en masque le sommet de ce dernier, on libère aussi l'espace alloué au calque du sommet de la pile
@ensures: Rien
*/
void decoupage_calque(pile_calque* p_calc);


/*
@requires: une pile de calque avec au moins 2 calques à l'intérieur sinon on ne fait rien
@assigns: fusionne les 2 premiers calques au sommet de la pile de calque , on libère aussi l'espace alloué au calque du sommet de la pile
@ensures: Rien
*/
void fusion_calque(pile_calque* p_calc);

list_couleur creer_sceau_de_couleur();

list_opacite creer_sceau_opacite();

void ajouter_sceau_de_couleur(list_couleur* sdc,couleur add);

void ajouter_sceau_opacite(list_opacite* so, opacite add);

ssize_t longueur_sceau_couleur(list_couleur sdc);

ssize_t longueur_sceau_opacite(list_opacite so);

couleur moyenne_sceau_couleur(list_couleur sdc);

opacite moyenne_sceau_opacite(list_opacite so);

void vider_sceau_couleur(list_couleur* sdc);

void vider_sceau_opacite(list_opacite* o);

void afficher_sceau_couleur(list_couleur sdc);

void afficher_sceau_opacite(list_opacite so);

pixel calcul_pixel_courant(list_couleur sdc, list_opacite so);

int max(int a,int b);

void trace_ligne(pile_calque* p_calc, list_couleur sdc, list_opacite so, position pos);

void remplissage(image* calc,position pos,pixel ancienne_couleur,pixel nouvelle_couleur);

#endif
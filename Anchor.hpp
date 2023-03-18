#ifndef ANCHOR
#define ANCHOR

#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glut.h>
#include "Knot_Vector.hpp"

class Anchor {

protected:

	/** Variables ****************************************************************/

	int H_index;			// Indice vertical
	int V_index;			// Indice horizontal
	bool active;			// Statut (On / off)
	float* color;			// Couleur d'affichage associée
	Knot_Vector H_Knots;	// Le vecteur de noeud horizontal
	Knot_Vector V_Knots;	// Le vecteur de noeud vertical

public:

	/** Structeurs ***************************************************************/

	Anchor();							// Constructeur par défaut ...
	Anchor(int init_h, int init_v);		// ... Par indices
	~Anchor();							// Destructeur

	/** Méthodes *****************************************************************/

	void set_Color(float* new_c);					// Fixe la couleur associée
	void set_Active();								// Met le statut sur ON
	void set_Indexes(int new_h, int new_v);			// Mise à jour des indices
	void set_H_Knots(std::vector<float> new_Hk);	// Mise à jour du vecteur de noeuds horizontal
	void set_V_Knots(std::vector<float> new_Vk);	// Mise à jour du vecteur de noeuds vertical
	void set_H_Knots(Knot_Vector new_Hk);			// Mise à jour du vecteur de noeuds horizontal
	void set_V_Knots(Knot_Vector new_Vk);			// Mise à jour du vecteur de noeuds vertical
	void switch_State();							// Modification du statut de l'ancre

	int get_H_Index();					// Renvie l'indice horizontal
	int get_V_Index();					// Renvoie l'indice vertical
	bool is_Active();					// renvoie le statut courant
	float* get_Color();					// Renvoie la couleur de la zone d'influence
	float* get_Pt_Color();				// Renvoie la couleur du point et des courbes
	float get_Hk_midValue();			// Renvoie le noeud central du vecteur horizontal
	float get_Vk_midValue();			// Renvoie le noued central du vecteur vertical
	float get_HBF_Value(float t);		// Calcule la valeur en t des fonctions de base horizontales
	float get_VBF_Value(float t);		// Calcule la valeur en t des fonctions de base verticales
	Knot_Vector get_H_Knots();			// Renvoie le vecteur des noeuds horizontaux
	Knot_Vector get_V_Knots();			// Renvoie le vecteur des noeuds verticaux

};

#endif
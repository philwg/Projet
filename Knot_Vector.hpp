#ifndef KNOT_VECTOR
#define KNOT_VECTOR

#include <cmath>
#include "Knot_Intervals.hpp"

class Knot_Vector : public Knot_Intervals {

private:

	/** Variables ****************************************************************/

	float First_Value;				// La valeur de base du vecteur
	std::vector<float> K_Values;	// Le vecteur de toutes ses valeurs

	/** Méthodes *****************************************************************/

	void init_Knots(int init_size);		// Initialisation des valeurs
	void update_Knots();				// Mise à jour des valeurs

	int get_Multi();								// Renvoie la multiplicté du vecteur
	float BasisFunction(int i, int d, float t);		// Le calcul récursif général des fonctions de base

public:

	/** Structeurs ***************************************************************/

	Knot_Vector();									// Constructeur par défaut ...
	Knot_Vector(int init_size);						// ... par taille
	Knot_Vector(std::vector<float> Init_Values);	// ... par valeurs
	~Knot_Vector();									// Destructeur

	/** Méthodes *****************************************************************/

	int get_Size();										// Renvoie la taille du vecteur
	float mini();										// Renvoie le premier élément
	float maxi();										// Renvoie le dernier élément
	float get_Value_at(int index);						// Renvoie la valeur associée à l'indice
	float BasisFunction(float t);						// Le calcul des fonction de base pour un Mesh Borné

	void change_Interval_at(int index, float delta);	// Modifie la valeur à l'indice donné
	void insert_Knot_at(int index);						// Insère un nouveau noeud
	void remove_Knot_at(int index);						// Supprime un noeud

};

#endif
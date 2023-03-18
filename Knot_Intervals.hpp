#ifndef KNOT_INTERVALS
#define KNOT_INTERVALS

#include <iostream>
#include <vector>

class Knot_Intervals {

protected:

	/** Variables ****************************************************************/

	int n_elem;						// Le nombre d'intervalles
	std::vector<float> KI_Values;	// Les valeurs des intervalles

	/** Méthodes *****************************************************************/

	void init(int init_size);							// Initialisation
	void change_Interval_at(int index, float delta);	// Modification d'un intervalle
	void insert_Interval_at(int index, float value);	// Insertion d'un intervalle
	void remove_Interval_at(int index);					// Suppression d'un intervalle
	
	float get_Interval_at(int index);					// Accès à la valeur d'un intervalle

public:

	/** Structeurs ***************************************************************/

	Knot_Intervals();				// Constructeur par défaut ...
	Knot_Intervals(int init_size);	// ... ou par taille
	~Knot_Intervals();				// Destructeur

};

#endif
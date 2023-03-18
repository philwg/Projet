#ifndef EDGE_LINE
#define EDGE_LINE

#include <iostream>
#include <vector>

class Edge_Line {

private:

	/** Variables ****************************************************************/

	int n_elem;					// Le nombre d'éléments
	std::vector<bool> Edges;	// Le vecteur d'arêtes

	/** Méthodes *****************************************************************/

	void init(int init_size, bool init_value);	// Initialisation

public:

	/** Structeurs **************************************************************/

	Edge_Line();								// Constructeur par défaut
	Edge_Line(int init_size);					// ... par taille
	Edge_Line(int init_size, bool init_value);	// ... par taille et valeur
	~Edge_Line();								// Destructeur

	/** Méthodes ****************************************************************/

	bool at(int index);						// Accès aux valeurs
	void switch_at(int index);				// Modification d'une valeur
	void insert_at(int index, bool value);	// Insertion d'une nouvelle valeur
	void remove_at(int index);				// Suppression d'une valeur
	
};

#endif
#ifndef EDGE_MESH
#define EDGE_MESH

#include "Edge_Grid.hpp"
#include "Knot_Vector.hpp"

class Edge_Mesh : public Edge_Grid {

protected:

	/** Variables ****************************************************************/

	int H_degree;			// Le degré horizontal du Mesh
	int V_degree;			// Le degré vertical du Mesh

	Knot_Vector H_Knots;	// Le vecteur des noeuds horizontaux
	Knot_Vector V_Knots;	// Le vecteur des noeuds verticaux

	/** Méthodes *****************************************************************/

	void init_Mesh(int init_width, int init_height);	// Initialisation

public:

	/** Structeurs ***************************************************************/

	Edge_Mesh();								// Constructeur par défaut
	Edge_Mesh(int init_width, int int_height);	// Constructeur par dimensions
	~Edge_Mesh();								// Destructeur

	/** Méthodes *****************************************************************/

	void insert_HEdges_at(int V_index);		// Insérer une ligne d'arêtes horizontales
	void insert_VEdges_at(int H_index);		// Insérer une ligne d'arêtes verticales

	void remove_HEdges_at(int V_index);		// Supprimer une ligne d'arêtes horizontales
	void remove_VEdges_at(int H_index);		// Supprimer une ligne d'arêtes verticales

};

#endif
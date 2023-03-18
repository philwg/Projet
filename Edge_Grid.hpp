#ifndef EDGE_GRID
#define EDGE_GRID

#include "Edge_Line.hpp"

class Edge_Grid {

protected:

	/** Variables ****************************************************************/

	int width;							// Nombre d'arêtes horizontales
	int height;							// Nombre d'arêtes verticales
	
	std::vector<Edge_Line> H_Edges;		// Le vecteur des lignes d'arêtes hoizontales
	std::vector<Edge_Line> V_Edges;		// Le vecteur des lignes d'arêtes verticales

	/** Méthodes *****************************************************************/

	void init(int init_width, int init_height);	// Initialisation de la grille

	/** Dans les méthodes qui suivent h et v sont les indices des jonctions de la grille
	/*  qui vont donc de 0 à width inclus horizontalement et de 0 à height inclus verticalement
	*/
	bool is_Up_TJunction(int h, int v);			// teste la présence d'un coin ou d'un T barre en haut
	bool is_Down_TJunction(int h, int v);		// teste la présence d'un coin ou d'un T barre en bas
	bool is_Left_TJunction(int h, int v);		// teste la présence d'un coin ou d'un T barre à gauche
	bool is_Right_TJunction(int h, int v);		// teste la présence d'un coin ou d'un T barre à droite
	bool is_VEdge(int h, int v);				// teste la présence d'une arête verticale
	bool is_HEdge(int h, int v);				// teste la présence d'une arête horizontale

	/** Les méthodes ci-dessous insèrent des arêtes en préservant les jonctions et arêtes déjà présentes
	/*  dans la grille détectées grâce aux méthodes précédentes
	*/
	void insert_HEdges_at(int V_index);			// Insérer une ligne d'arêtes horizontales
	void insert_VEdges_at(int H_index);			// Insérer une ligne d'arêtes verticales

	void remove_HEdges_at(int V_index);			// Supprimer une ligne d'arêtes horizontales
	void remove_VEdges_at(int H_index);			// Supprimer une ligne d'arêtes verticales

public:

	/** Structeurs ***************************************************************/

	Edge_Grid();								// Constructeur par défaut ...
	Edge_Grid(int init_width, int init_height);	// ... et un autre par nombres d'arêtes
	~Edge_Grid();								// Destructeur

};

#endif
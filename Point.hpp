#ifndef POINT
#define POINT

#include <cmath>

class Point {

protected:

	/** Variables ****************************************************************/
	
	float x;		// Abscisse du Point
	float y;		// Ordonnée du Point
	float z;		// Cote du Point
	float w;		// Poids du Point

	int H_index;	// Indice horizontal
	int V_index;	// indice Vertical

	/** Méthodes *****************************************************************/

	/* Initialisation / Mise à jour */
	void set(float new_x, float new_y, float new_z, float new_w, int new_h, int new_v);

public:

	/** Structeurs ***************************************************************/

	Point();																// Constructeur par defaut ...
	Point(float init_x, float init_y);										// ... Par (x,y)
	Point(float init_x, float init_y, int init_h, int init_v);				// ... Par (x,y) et indices
	Point(float init_x, float init_y, float init_z, int init_h, int init_v);// ... Par (x,y,z) et indices
	~Point();																// Destructeur

	/** Methodes *****************************************************************/

	int get_H_Index();				// Renvoie l'indice horizontal
	int get_V_Index();				// Renvoie l'indice vertical

	void set_Weight(float new_w);					// Fixe le poids du point
	void change_Indexes(int delta_H, int delta_V);	// Change les indices

	float get_X();					// Renvoie l'abscisse
	float get_Y();					// Renvoie l'ordonnée								
	float get_Z();					// Renvoie la cote
	float get_Weight();				// Renvoie le poids

	float distTo(Point P);			// Renvoie la distance euclidienne à un autre point
	float* get_Vertex();			// Renvoie une référence sur un tableau contenant les coordonnées

	Point add_X(float delta_x);									// Renvoie un point modifé en abscisse
	Point add_Y(float delta_y);									// Renvoie un point modifié en ordonnée
	Point add_Z(float delta_z);									// Renvoie un point modifié en cote
	Point add(float delta_x, float delta_y, float delta_z);		// Renvoie un point modifié sur les trois coordonnées

};

#endif
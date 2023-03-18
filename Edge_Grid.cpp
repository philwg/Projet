#include "Edge_Grid.hpp"

using namespace std;

/************************************************/
/*				Constructeurs					*/
/************************************************/

Edge_Grid::Edge_Grid()
{
	this->init(1, 1);
}

Edge_Grid::Edge_Grid(int init_width, int init_height)
{
	this->init(init_width, init_height);
}

/************************************************/
/*				Destructeur						*/
/************************************************/

Edge_Grid::~Edge_Grid()
{
	this->width = 0;
	this->height = 0;
	this->H_Edges.clear();
	this->V_Edges.clear();
}

/************************************************/
/*				Méthodes protégées				*/
/************************************************/

void Edge_Grid::init(int init_width, int init_height)
{
	this->width = init_width;
	this->height = init_height;
	Edge_Line L;

	// Initialisation du vecteur des lignes horizontales
	this->H_Edges.clear();
	for(int k=0; k <= init_height; k++) {
		// La première et la dernière sont entièrement actives ...
		if ((k == 0) || (k == init_height)) L = Edge_Line(init_width, true);
		// ... les autres ne le sont pas du tout
		else L = Edge_Line(init_width, false);
		this->H_Edges.push_back(L);
	}

	//Initialisation du vecteur des lignes verticales
	this->V_Edges.clear();
	for(int k=0; k <= init_width; k++) {
		// La première et la dernière sont entièrement actives ...
		if ((k == 0) || (k == init_width)) L = Edge_Line(init_height, true);
		// ... les autres ne le sont pas du tout
		else L = Edge_Line(init_height, false);
		this->V_Edges.push_back(L);
	}
}

/** Dans les méthodes xxx_TJunction(h,v) on teste en fait la présence de deux coins.
/*  La jonction serait vraiment un T si et seulement si les deux coins étaient présents.
/*  Mais pour notre objectif ici, la présence de l'un ou de l'autre suffit.
*/

bool Edge_Grid::is_Up_TJunction(int h, int v)
{
	bool west = ((h>0)&&(h<=this->width)) ? this->H_Edges.at(v).at(h-1) : true;
	bool east = ((h>=0)&&(h<=this->width)) ? this->H_Edges.at(v).at(h) : true;
	bool south = ((v>0)&&(v<=this->height)) ? this->V_Edges.at(h).at(v-1) : true;
	return (west && south)||(east && south);
}

bool Edge_Grid::is_Down_TJunction(int h, int v)
{
	bool west = ((h>0)&&(h<=this->width)) ? this->H_Edges.at(v).at(h-1) : true;
	bool east = ((h>=0)&&(h<this->width)) ? this->H_Edges.at(v).at(h) : true;
	bool north = ((v>=0)&&(v<this->height)) ? this->V_Edges.at(h).at(v) : true;
	return (west && north)||(east && north);
}

bool Edge_Grid::is_Left_TJunction(int h, int v)
{
	bool east = ((h>=0)&&(h<this->width)) ? this->H_Edges.at(v).at(h) : true;
	bool north = ((v>=0)&&(v<this->height)) ? this->V_Edges.at(h).at(v) : true;
	bool south = ((v>0)&&(v<=this->height)) ? this->V_Edges.at(h).at(v-1) : true;
	return (east && north)||(east && south);
}

bool Edge_Grid::is_Right_TJunction(int h, int v)
{
	bool west = ((h>0)&&(h<=this->width)) ? this->H_Edges.at(v).at(h-1) : true;
	bool north = ((v>=0)&&(v<this->height)) ? this->V_Edges.at(h).at(v) : true;
	bool south = ((v>0)&&(v<=this->height)) ? this->V_Edges.at(h).at(v-1) : true;
	return (west && north)||(west && south);
}

bool Edge_Grid::is_VEdge(int h, int v)
{
	bool north = ((v>=0)&&(v<this->height)) ? this->V_Edges.at(h).at(v) : true;
	bool south = ((v>0)&&(v<=this->height)) ? this->V_Edges.at(h).at(v-1) : true;
	return (north && south);
}

bool Edge_Grid::is_HEdge(int h, int v)
{
	bool west = ((h>0)&&(h<=this->width)) ? this->H_Edges.at(v).at(h-1) : true;
	bool east = ((h>=0)&&(h<this->width)) ? this->H_Edges.at(v).at(h) : true;
	return (west && east);
}

/* Méthode d'insertion d'une ligne d'arêtes horizontales *************************/
void Edge_Grid::insert_HEdges_at(int V_index)
{
	Edge_Line EL;

	// Si on est dans la grille (sauf la première ligne qui reste fixe)
	if ((V_index > 0) && (V_index <= this->height)) {

		// Dans chaque ligne verticale ...
		for (int k=0; k <= this->width; k++) {
			bool value = false;
			if ((k <= 0) || (k >= this->width)) value = true;
			else if (this->is_VEdge(k, V_index)) value = true;
			else if (this->is_Down_TJunction(k, V_index-1) || this->is_Up_TJunction(k, V_index)) value = true;
			// ... on insère une arête cohérente avec la ligne à la bonne hauteur
			this->V_Edges.at(k).insert_at(V_index-1, value);
		}

		// On rajoute également une nouvelle ligne horizontale
		EL = Edge_Line(this->width, false);
		this->H_Edges.insert(this->H_Edges.begin() + V_index, EL);

		// On adapte la hauteur des grilles
		this->height += 1;
	}
}

/* Méthode d'insertion d'une ligne d'arêtes verticales ***************************/
void Edge_Grid::insert_VEdges_at(int H_index)
{
	Edge_Line EL;

	// Si on est dans la grille (sauf la première ligne qui reste fixe)
	if ((H_index > 0) && (H_index <= this->width)) {

		// Dans chaque ligne horizontale ...
		for (int k=0; k <= this->height; k++) {
			bool value = false;
			if ((k <= 0)||(k >= this->height)) value = true;
			else if (this->is_HEdge(H_index, k)) value = true;
			else if (this->is_Left_TJunction(H_index-1, k) || this->is_Right_TJunction(H_index, k)) value = true;
			// ... on insère une arêtes cohérente avec la ligne à la bonne position en largeur
			this->H_Edges.at(k).insert_at(H_index-1, value);
		}

		// On rajoute également une nouvelle ligne verticale
		EL = Edge_Line(this->height, false);
		this->V_Edges.insert(this->V_Edges.begin() + H_index, EL);

		// On adapte la largeur des grilles
		this->width += 1;
	}
}

/* Méthode de suppression d'une ligne d'arêtes horizontales ***********************/
void Edge_Grid::remove_HEdges_at(int V_index)
{
	// Si on est dans la grille (sauf la première ligne ou dernière ligne)
	if ((V_index > 0) && (V_index < this->height)) {

		// Dans chaque ligne verticale on supprime une arête à la bonne hauteur
		for (int k=0; k <= this->width; k++) this->V_Edges.at(k).remove_at(V_index-1);

		// On supprime également une nouvelle ligne horizontale
		this->H_Edges.erase(this->H_Edges.begin() + V_index);
		this->H_Edges.shrink_to_fit();

		// On adapte la hauteur des grilles
		this->height -= 1;
	}
}

/* Méthode d'insertion d'une ligne d'arêtes verticales ***************************/
void Edge_Grid::remove_VEdges_at(int H_index)
{
	// Si on est dans la grille (sauf la première ligne ou dernière ligne)
	if ((H_index > 0) && (H_index < this->width)) {

		// Dans chaque ligne horizontale, on supprime une arête à la bonne position en largeur
		for (int k=0; k <= this->height; k++) this->H_Edges.at(k).remove_at(H_index-1);

		// On supprime également une nouvelle ligne verticale
		this->V_Edges.erase(this->V_Edges.begin() + H_index);
		this->V_Edges.shrink_to_fit();

		// On adapte la largeur des grilles
		this->width -= 1;
	}
}
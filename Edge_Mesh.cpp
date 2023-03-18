#include "Edge_Mesh.hpp"

using namespace std;

/************************************************/
/*				Constructeurs					*/
/************************************************/

Edge_Mesh::Edge_Mesh()
{
	this->init_Mesh(1, 1);
}

Edge_Mesh::Edge_Mesh(int init_width, int init_height)
{
	this->init_Mesh(init_width, init_height);
}

/************************************************/
/*				Destructeur						*/
/************************************************/

Edge_Mesh::~Edge_Mesh()
{}

/************************************************/
/*				Méthodes publiques				*/
/************************************************/

void Edge_Mesh::insert_HEdges_at(int V_index)
{
	Edge_Grid::insert_HEdges_at(V_index);	// Insère une ligne horizontale dans la grille
	this->V_Knots.insert_Knot_at(V_index);	// Insère la valeur de noeud verticale correspondante
}

void Edge_Mesh::insert_VEdges_at(int H_index)
{
	Edge_Grid::insert_VEdges_at(H_index);	// Insère une ligne verticale dans la grille
	this->H_Knots.insert_Knot_at(H_index);	// Insère la valeur de noeud horizontale correspondante
}

void Edge_Mesh::remove_HEdges_at(int V_index)
{
	Edge_Grid::remove_HEdges_at(V_index);	// Supprime une ligne horizontale dans la grille
	this->V_Knots.remove_Knot_at(V_index);	// Supprime la valeur de noeud verticale correspondante
}

void Edge_Mesh::remove_VEdges_at(int H_index)
{
	Edge_Grid::remove_VEdges_at(H_index);	// Supprime une ligne verticale dans la grille
	this->H_Knots.remove_Knot_at(H_index);	// Supprime la valeur de noeud horizontale correspondante
}

/************************************************/
/*				Méthodes protégées				*/
/************************************************/

/** Initialisation du Mesh
*/
void Edge_Mesh::init_Mesh(int init_width, int init_height)
{
	this->init(init_width, init_height);			// Création d'une grille de dimensions adéquates
	this->H_Knots = Knot_Vector(init_width+1);		// Création du vecteur de noeud horizontal correspondant
	this->V_Knots = Knot_Vector(init_height+1);		// Création du vecteur de noeud vertical correspondant
	this->H_degree = 1;								// Initialisation du dégré horizontal à 1
	this->V_degree = 1;								// Initialisation du degré vertical à 1
}
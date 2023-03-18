#include "Edge_Line.hpp"

using namespace std;

/************************************************/
/*				Constructeurs					*/
/************************************************/

Edge_Line::Edge_Line()
{
	this->init(1, false);
}

Edge_Line::Edge_Line(int init_size)
{
	this->init(init_size, false);
}

Edge_Line::Edge_Line(int init_size, bool init_value)
{
	this->init(init_size, init_value);
}

/************************************************/
/*				Destructeur						*/
/************************************************/

Edge_Line::~Edge_Line()
{
	this->Edges.clear();
	this->n_elem = 0;
}

/************************************************/
/*				Méthodes publiques				*/
/************************************************/

/** Renvoie la valeur on/off de l'arête ou off pour le hors-range
*/
bool Edge_Line::at(int index)
{
	if ((index < 0) || (index > this->n_elem)) return false;
	else return this->Edges.at(index);
}

/** Change la valeur on/off à une position donnée dans le range
*/
void Edge_Line::switch_at(int index)
{
	if ((index >= 0) && (index < this->n_elem)) {
		if (this->at(index)) this->Edges.at(index) = false;
		else this->Edges.at(index) = true;
	}
}

/** Insère une valeur à une position donnée dans le range
/*  On ne peut pas insérer en dernière position ...
*/
void Edge_Line::insert_at(int index, bool value)
{
	if ((index >= 0) && (index < this->n_elem)) {
		this->Edges.insert(this->Edges.begin() + index, value);
		this->n_elem += 1;
	}
}

/** Supprime une valeur à une position donnée dans le range
/*  On ne peut pas supprimer en première ni dernière position ...
*/
void Edge_Line::remove_at(int index)
{
	if ((index > 0) && (index < this->n_elem)) {
		this->Edges.erase(this->Edges.begin() + index);
		this->Edges.shrink_to_fit();
		this->n_elem -= 1;
	}
}

/************************************************/
/*				Méthodes privées				*/
/************************************************/

/** Initialisation à la taille et valeur commune d'une ligne complète
*/
void Edge_Line::init(int init_size, bool init_value)
{
	if (init_size < 1) init_size = 1;
	this->n_elem = init_size;
	this->Edges.clear();
	for (int k=0; k<init_size; k++) {
		this->Edges.push_back(init_value);
	}
}
#include "Knot_Intervals.hpp"

using namespace std;

/************************************************/
/*				Constructeurs					*/
/************************************************/

Knot_Intervals::Knot_Intervals()
{
	this->init(1);
}

Knot_Intervals::Knot_Intervals(int init_size)
{
	this->init(init_size);
}

/************************************************/
/*				Destructeur						*/
/************************************************/

Knot_Intervals::~Knot_Intervals()
{
	this->KI_Values.clear();
	this->n_elem = 0;
}

/************************************************/
/*				Méthodes protégées				*/
/************************************************/

/** Initialisation des valeurs à 1
*/
void Knot_Intervals::init(int init_size)
{
	this->n_elem = init_size;
	KI_Values.clear();
	for (int k=0; k < init_size; k++) KI_Values.push_back(1.0f);
}

/** Renvoie la valeur de l'intervalle ou zéro
*/
float Knot_Intervals::get_Interval_at(int index)
{
	if ((index < 0) || (index >= this->n_elem)) return 0.0f;
	else return this->KI_Values.at(index);
}

/** Si l'intervalle est dans le range, modifie sa valeur sans déscendre sous 0
*/
void Knot_Intervals::change_Interval_at(int index, float delta)
{
	if ((index >= 0) && (index < this->n_elem)) {
		this->KI_Values.at(index) += delta;
		if (this->KI_Values.at(index) < 0) this->KI_Values.at(index) = 0;
	}
}

/** Insère un nouvel intervalle
*/
void Knot_Intervals::insert_Interval_at(int index, float value)
{
	if ((index >= 0) && (index < this->n_elem)) {
		this->KI_Values.insert(this->KI_Values.begin()+index, value);
		this->n_elem += 1;
	}
}

/** Supprime un intervalle
*/
void Knot_Intervals::remove_Interval_at(int index)
{
	if ((index >= 0) && (index < this->n_elem)) {
		this->KI_Values.erase(this->KI_Values.begin()+index);
		this->KI_Values.shrink_to_fit();
		this->n_elem -= 1;
	}
}
#include "Knot_Vector.hpp"

using namespace std;

/************************************************/
/*				Constructeurs					*/
/************************************************/

Knot_Vector::Knot_Vector()
{
	this->init_Knots(1);
}

Knot_Vector::Knot_Vector(int init_size)
{
	this->init_Knots(init_size);
}

Knot_Vector::Knot_Vector(vector<float> Init_Values)
{
	// Calcul des intervalles à partir des valeurs de noeuds
	this->init(Init_Values.size() - 1);
	vector<float> Intervals;
	Intervals.clear();
	float i;
	for (vector<float>::iterator Iv=Init_Values.begin()+1; Iv!=Init_Values.end(); Iv++) {
		i = *(Iv)-*(Iv-1);
		Intervals.push_back((i>=0.0f) ? i : 0.0f);
	}

	// Initialisation
	this->First_Value = Init_Values.front();
	this->KI_Values = Intervals;
	this->update_Knots();
}

/************************************************/
/*				Destructeur						*/
/************************************************/

Knot_Vector::~Knot_Vector()
{
	this->n_elem = 0;
	this->K_Values.clear();
	this->KI_Values.clear();
}

/************************************************/
/*				Méthodes publiques				*/
/************************************************/

/** Renvoie la taille du vecteur de noeud (nombre d'intervalles +1)
*/
int Knot_Vector::get_Size()
{
	return this->n_elem + 1;
}

/** Renvoie la valeur minimum
*/
float Knot_Vector::mini()
{
	return this->K_Values.front();
}

/** Renvoie la valeur maximum
*/
float Knot_Vector::maxi()
{
	return this->K_Values.back();
}

/** Renvoie la valeur du noeud en complétant par extension :
/*  première valeur pour les indices < 0
/*  dernière valeur pour les indices > taille
*/
float Knot_Vector::get_Value_at(int index)
{
	if (index < 0) return this->K_Values.front();
	else if (index > this->n_elem) this->K_Values.back();
	else return this->K_Values.at(index);
}

/** Méthode par défaut utilisée pour un T_Mesh borné
*/
float Knot_Vector::BasisFunction(float t)
{
	return this->BasisFunction(this->get_Multi(), this->get_Size()-2, t);
}

/** Modifie une valeur d'intervalle repérée par un indice en lui ajoutant delta
/*  si l'indice est dans le range.
*/
void Knot_Vector::change_Interval_at(int index, float delta)
{
	if ((index > 0) && (index <= this->n_elem)) {
		Knot_Intervals::change_Interval_at(index-1, delta);
		this->update_Knots();
	}
}

/** Insère une nouvelle valeur de noeud à l'indice donné si il est dans le range
/*  L'intervalle précédent est réduit de moitié et un nouveau est créé pour compléter
*/
void Knot_Vector::insert_Knot_at(int index)
{
	if ((index > 0) && (index <= this->n_elem)) {
		float value = this->get_Interval_at(index-1);
		Knot_Intervals::change_Interval_at(index-1, -(value/2.0f));
		Knot_Intervals::insert_Interval_at(index-1, value/2.0f);
		this->update_Knots();
	}
}

/** Supprime une nouvelle valeur de noeud à l'indice donné si il est dans le range
/*  L'intervalle suivant est additionné
*/
void Knot_Vector::remove_Knot_at(int index)
{
	if ((index > 0) && (index < this->n_elem)) {
		float value = this->get_Interval_at(index-1);
		Knot_Intervals::change_Interval_at(index, value);
		Knot_Intervals::remove_Interval_at(index-1);
		this->update_Knots();
	}
}

/************************************************/
/*				Méthodes privées				*/
/************************************************/

/** Initialisation
*/
void Knot_Vector::init_Knots(int init_size)
{
	this->First_Value = 0.0f;	// Par défaut, la première valeur est 0
	this->init(init_size-1);
	this->update_Knots();
}

/** Mise à jour des noeuds
*/
void Knot_Vector::update_Knots()
{
	this->K_Values.clear();
	// On part de la valeur de base ...
	float value = this->First_Value;
	// ... Pour chaque valeur d'intervalle ...
	for (int k=0; k < this->n_elem; k++) {
		// On enregistre la valeur ...
		this->K_Values.push_back(value);
		// On ajoute l'intervalle ...
		value += this->KI_Values.at(k);
	}
	// On ajoute la dernière valeur.
	this->K_Values.push_back(value);
}

/** Renvoie la multiplicité d'un vecteur (ie le nombre de fois qu'une valeur se répète)
*/
int Knot_Vector::get_Multi()
{
	// On calcule la multiplicité 
	int m = 0;
	for (int i=1; i<this->get_Size(); i++) {
		if (this->get_Value_at(i-1) == this->get_Value_at(i)) m++;
	}

	// On en déduit le décalage de la fonction de base associée
	if (m>0) m--;

	// si c'est la première valeur qui se répète le plus on affecte un signe -
	if (this->get_Value_at(0) == this->get_Value_at(1)) m *= -1;
	return m;
}

/** Méthode récursive de calcul de la valeur des fonctions de base associées au vecteur
*/
float Knot_Vector::BasisFunction(int i, int d, float t)
{
	if (d == 0)	{
		if ((t < this->get_Value_at(i)) || (t >= this->get_Value_at(i+1))) return 0.0f;
		else return 1.0f;
	}
	else if (((this->get_Value_at(i+d)-this->get_Value_at(i)) != 0.0f) && ((this->get_Value_at(i+d+1)-this->get_Value_at(i+1)) != 0.0f))
		return (((t-this->get_Value_at(i))/(this->get_Value_at(i+d)-this->get_Value_at(i)))*BasisFunction(i, d-1, t))+(((this->get_Value_at(i+d+1)-t)/(this->get_Value_at(i+d+1)-this->get_Value_at(i+1)))*BasisFunction(i+1, d-1, t));
	else if (((this->get_Value_at(i+d)-this->get_Value_at(i)) == 0.0f) && ((this->get_Value_at(i+d+1)-this->get_Value_at(i+1)) == 0.0f))
		return 0.0f;
	else if ((this->get_Value_at(i+d)-this->get_Value_at(i)) == 0.0f)
		return ((this->get_Value_at(i+d+1)-t)/(this->get_Value_at(i+d+1)-this->get_Value_at(i+1)))*BasisFunction(i+1, d-1, t);
	else return ((t-this->get_Value_at(i))/(this->get_Value_at(i+d)-this->get_Value_at(i)))*BasisFunction(i, d-1, t);
}
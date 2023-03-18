#include "Point.hpp"

using namespace std;

/************************************************/
/*				Constructeurs					*/
/************************************************/

Point::Point()
{
	this->set(0.0f, 0.0f, 0.0f, 1.0f, 0, 0);
}

Point::Point(float init_x, float init_y)
{
	this->set(init_x, init_y, 0.0f, 1.0f, 0, 0);
}

Point::Point(float init_x, float init_y, int init_h, int init_v)
{
	this->set(init_x, init_y, 0.0f, 1.0f, init_h, init_v);
}

Point::Point(float init_x, float init_y, float init_z, int init_h, int init_v)
{
	this->set(init_x, init_y, init_z, 1.0f, init_h, init_v);
}

/************************************************/
/*				Destructeur						*/
/************************************************/

Point::~Point()
{}

/************************************************/
/*				Méthodes publiques				*/
/************************************************/

/** Renvoie l'index horizontal du point
*/
int Point::get_H_Index() 
{
	return this->H_index;
}

/** Renvoie l'index vertical du point
*/
int Point::get_V_Index()
{
	return this->V_index;
}

/** Fixe le poids du point
*/
void Point::set_Weight(float new_w)
{
	this->w = new_w;
}

/* Modifie les indices du point
*/
void Point::change_Indexes(int delta_H, int delta_V)
{
	this->H_index += delta_H;
	this->V_index += delta_V;
}

/** Renvoie l'abscisse
*/
float Point::get_X()
{
	return this->x;
}

/** Renvoie l'ordonnée
*/
float Point::get_Y()
{
	return this->y;
}

/** Renvoie la cote
*/
float Point::get_Z()
{
	return this->z;
}

/** Renvoie le poids dpoint
*/
float Point::get_Weight() {
	return this->w;
}

/** Renvoie la distance à un autre point
*/
float Point::distTo(Point P)
{
	return sqrt(pow((this->x)-P.x, 2) + pow((this->y)-P.y, 2) + pow((this->z)-P.z, 2));
}

/** Renvoie l'adresse d'un tableau contenant les trois coordonnées du point
*/
float* Point::get_Vertex()
{
	return new float[3] {this->x, this->y, this->z};
}

/** Renvoie un point modifié en abscisse
*/
Point Point::add_X(float delta_x)
{
	Point P;
	P = Point(this->x+delta_x, this->y, this->z, this->H_index, this->V_index);
	return P;
}

/** Renvoie un point modifié en ordonnée
*/
Point Point::add_Y(float delta_y)
{
	Point P;
	P = Point(this->x, this->y+delta_y, this->z, this->H_index, this->V_index);
	return P;
}

/** Renvoie un point modifié en cote
*/
Point Point::add_Z(float delta_z)
{
	Point P;
	P = Point(this->x, this->y, this->z+delta_z, this->H_index, this->V_index);
	return P;
}

/** Renvoie un point modifié sur les trois coordonnées
*/
Point Point::add(float delta_x, float delta_y, float delta_z)
{
	Point P;
	P = Point(this->x+delta_x, this->y+delta_y, this->z+delta_z, this->H_index, this->V_index);
	return P;
}

/************************************************/
/*				Méthodes protégées				*/
/************************************************/

/** Initilaisation/Mise à jour des valeurs du points
*/
void Point::set(float new_x, float new_y, float new_z, float new_w, int new_h, int new_v)
{
	this->x = new_x;
	this->y = new_y;
	this->z = new_z;
	this->w = new_w;
	this->H_index = new_h;
	this->V_index = new_v;
}
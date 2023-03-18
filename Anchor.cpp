#include "Anchor.hpp"

using namespace std;

/************************************************/
/*				Constructeurs					*/
/************************************************/

Anchor::Anchor()
{}

Anchor::Anchor(int init_h, int init_v)
{
	this->set_Indexes(init_h, init_v);
	// La couleur associée est générée aléatoirement avec de la transparence
	this->set_Color(new float[4] {	(float)(rand()%60)/100.0f+0.4f,
									(float)(rand()%60)/100.0f+0.4f,
									(float)(rand()%60)/100.0f+0.4f,
									0.12f });
	this->active = false;
}

/************************************************/
/*				Déstructeur						*/
/************************************************/

Anchor::~Anchor()
{}

/************************************************/
/*				Méthodes publiques				*/
/************************************************/

/** Calcule la valeur des fonctions de base pour le vecteur de noeud horizontal
*/
float Anchor::get_HBF_Value(float t)
{
	return this->H_Knots.BasisFunction(t);
}

/** Calcule la valeur des fonctions de base^pour le vecteur de noeud vertical
*/
float Anchor::get_VBF_Value(float t)
{
	return this->V_Knots.BasisFunction(t);
}

/** Fixe la couleur associée à l'ancre
*/
void Anchor::set_Color(float* new_c)
{
	this->color = new_c;
}

/** Active l'ancre
*/
void Anchor::set_Active()
{
	this->active = true;
}

/** Fixe les indices de l'ancre
*/
void Anchor::set_Indexes(int new_h, int new_v)
{
	this->H_index = new_h;
	this->V_index = new_v;
}

/** Fixe le vecteur de noeud horizontal
*/
void Anchor::set_H_Knots(std::vector<float> new_Hk)
{
	this->H_Knots = Knot_Vector(new_Hk);
}

/** Fixe le vecteur de noeud vertical
*/
void Anchor::set_V_Knots(std::vector<float> new_Vk)
{
	this->V_Knots = Knot_Vector(new_Vk);
}

/** Fixe le vecteur de noeud horizontal
*/
void Anchor::set_H_Knots(Knot_Vector new_Hk)
{
	this->H_Knots = new_Hk;
}

/** Fixe le vecteur de noeud vertical
*/
void Anchor::set_V_Knots(Knot_Vector new_Vk)
{
	this->V_Knots = new_Vk;
}

/** Modifie le statut (On/Off) de l'ancre
*/
void Anchor::switch_State()
{
	if (this->is_Active()) this->active = false;
	else this->active = true;
}

/** Renvoie l'indice horizontal de l'ancre
*/
int Anchor::get_H_Index()
{
	return this->H_index;
}

/** Renvoie l'indice vertical de l'ancre
*/
int Anchor::get_V_Index()
{
	return this->V_index;
}

/** Renvoie le statut courant (On / Off)
*/
bool Anchor::is_Active()
{
	return this->active;
}

/** Renvoie la couleur d'affichage de la zone d'influence du point
*/
float* Anchor::get_Color()
{
	return this->color;
}

/** Renvoie la couleur d'affichage du point et des courbes associées
/*  La couleur est obtenue par application d'un White Patch sur la couleur de la zone;
*/
float* Anchor::get_Pt_Color()
{
	float r = this->color[0];
	float g = this->color[1];
	float b = this->color[2];
	float coef = min(1.0f/r, min(1.0f/g, 1.0f/b));
	// La transparence est supprimée
	return new float[4] {r*coef, g*coef, b*coef, 1.0f};
}

/** Renvoie la valeur centrale du vecteur de noeud horizontal
*/
float Anchor::get_Hk_midValue()
{
	return this->H_Knots.get_Value_at(floor(this->H_Knots.get_Size()/2));
}

/** Renvoie la valeur centrale du vecteur de noeud vertical
*/
float Anchor::get_Vk_midValue()
{
	return this->V_Knots.get_Value_at(floor(this->V_Knots.get_Size()/2));
}

/** Renvoie le vecteur de noeud horizontal
*/
Knot_Vector Anchor::get_H_Knots()
{
	return this->H_Knots;
}

/** Renvoie le vecteur de noeud vertical
*/
Knot_Vector Anchor::get_V_Knots()
{
	return this->V_Knots;
}
#include "Mesh_2D.hpp"

#define PHI 0.73f

using namespace std;

/************************************************/
/*				Constructeurs					*/
/************************************************/

Mesh_2D::Mesh_2D()
{
	this->init_Mesh(8, 11);
	this->Anchors.clear();
	this->set_W_Size(1000, 1200);
}

Mesh_2D::Mesh_2D(int Wdw_Width, int Wdw_Height)
{
	this->init_Mesh(8, 11);
	this->Anchors.clear();
	this->set_W_Size(Wdw_Width, Wdw_Height);
}

Mesh_2D::Mesh_2D(int init_width, int init_height, int Wdw_Width, int Wdw_Height)
{
	this->init_Mesh(init_width, init_height);
	this->Anchors.clear();
	this->set_W_Size(Wdw_Width, Wdw_Height);
}

/************************************************/
/*				Destructeur						*/
/************************************************/

Mesh_2D::~Mesh_2D()
{}

/************************************************/
/*				Méthodes publiques				*/
/************************************************/

/** Mise à jour des dimensions de l'affichage
*/
void Mesh_2D::set_W_Size(int Wdw_Width, int Wdw_Height)
{
	this->W_Width = Wdw_Width;
	this->W_Height = Wdw_Height;
	this->update_GL();
}

/** Méthode d'affichage appelant toutes les parties de l'affichage
*/
void Mesh_2D::display()
{
	this->display_V_Edges();
	this->display_H_Edges();
	this->display_Anchors();
	this->display_V_Knots();
	this->display_H_Knots();
}

/** Méthodes de Picking spécialisées voir getPicked() *******************************/
/*  --> Arêtes (H et V)																*/
/*  --> Noeuds (H et V)																*/
/*  --> Ancres 																		*/
/************************************************************************************/

int Mesh_2D::getPicked_H_Edge(int mState, int x, int y)
{
	return this->getPicked(mState, x, y, this->H_Edges_Middles);
}

int Mesh_2D::getPicked_V_Edge(int mState, int x, int y)
{
	return this->getPicked(mState, x, y, this->V_Edges_Middles);
}

int Mesh_2D::getPicked_H_Knot(int mState, int x, int y)
{
	return this->getPicked(mState, x, y, this->H_Knots_I_Hooks);
}

int Mesh_2D::getPicked_V_Knot(int mState, int x, int y)
{
	return this->getPicked(mState, x, y, this->V_Knots_I_Hooks);
}

int Mesh_2D::getPicked_Anchor(int mState, int x, int y)
{
	// On crée dynamiquement le vecteur de Point pour les ancres
	vector<Point> Ancres;
	Ancres.clear();
	Point P;
	for(vector<Anchor>::iterator Ank=this->Anchors.begin(); Ank!=this->Anchors.end(); Ank++) {
		// L'ancre est affichée au point central de ses valeurs de noeuds
		P = Point(	this->H_Delta + Ank->get_Hk_midValue() * this->H_Span,
					this->V_Delta + Ank->get_Vk_midValue() * this->V_Span,
					Ank->get_H_Index(),
					Ank->get_V_Index());
		Ancres.push_back(P);
	}
	return this->getPicked(mState, x, y, Ancres);
}

/** Méthodes de modification de statut pour l'affichage *****************************/
/*  --> une arête (H ou V)															*/
/*  --> toutes les arêtes d'une ligne (H ou V)										*/
/*  --> une ancre (Point+Zone+Courbes OU (juste) Point vert)						*/
/************************************************************************************/

void Mesh_2D::switch_H_Edge(int selected)	// arête H
{
	Point P = this->H_Edges_Middles.at(selected);
	if ((P.get_V_Index() > 0) && (P.get_V_Index() < this->height)) {
		this->H_Edges.at(P.get_V_Index()).switch_at(P.get_H_Index());
	}
}

void Mesh_2D::switch_V_Edge(int selected)	// arête V
{
	Point P = this->V_Edges_Middles.at(selected);
	if ((P.get_H_Index() > 0) && (P.get_H_Index() < this->width)) {
		this->V_Edges.at(P.get_H_Index()).switch_at(P.get_V_Index());
	}
}

void Mesh_2D::switch_H_Line(int selected)	// Ligne H
{
	Point P = this->H_Edges_Middles.at(selected);
	if ((P.get_V_Index() > 0) && (P.get_V_Index() < this->height)) {
		for (int i=0; i < this->width; i++) {
			this->H_Edges.at(P.get_V_Index()).switch_at(i);
		}
	}
}

void Mesh_2D::switch_V_Line(int selected)	// Ligne V
{
	Point P = this->V_Edges_Middles.at(selected);
	if ((P.get_H_Index() > 0) && (P.get_H_Index() < this->width)) {
		for (int i=0; i < this->height; i++) {
			this->V_Edges.at(P.get_H_Index()).switch_at(i);
		}
	}
}

void Mesh_2D::switch_Anchor(int selected)	// Ancre
{
	this->Anchors.at(selected).switch_State();
}

/** Méthodes de modifiaction des noeuds *********************************************/
/*  --> Changement de valeur d'un intervalle (H et V)								*/
/*  --> Insertion de noeuds (H et V)												*/
/*  --> Suppression de noeuds (H et V)												*/
/************************************************************************************/

void Mesh_2D::change_H_Interval(int selected, float delta)	// Intervalle H
{
	Point P = this->H_Knots_I_Hooks.at(selected);
	// On limite le changement à 1 centième à la fois
	float dlt = (delta >= 0) ? 0.01f : -0.01f;
	this->H_Knots.change_Interval_at(selected + 1, dlt);
	this->update_GL();
}

void Mesh_2D::change_V_Interval(int selected, float delta)	// Intervalle V
{
	Point P = this->V_Knots_I_Hooks.at(selected);
	// On limite le changement à 1 centième à la fois
	float dlt = (delta >= 0) ? 0.01f : -0.01f;
	this->V_Knots.change_Interval_at(selected + 1, dlt);
	this->update_GL();
}

void Mesh_2D::insert_HKnot_at(int selected)					// Insertion H
{
	this->hide_Anchors();
	Point P = this->H_Knots_Val_Pts.at(selected+1);
	Edge_Mesh::insert_VEdges_at(P.get_H_Index());
}

void Mesh_2D::insert_VKnot_at(int selected)					// Insertion V
{
	this->hide_Anchors();
	Point P = this->V_Knots_Val_Pts.at(selected+1);
	Edge_Mesh::insert_HEdges_at(P.get_V_Index());
}

void Mesh_2D::remove_HKnot_at(int selected)					// Suppression H
{
	this->hide_Anchors();
	Point P = this->H_Knots_Val_Pts.at(selected);
	Edge_Mesh::remove_VEdges_at(P.get_H_Index());
}

void Mesh_2D::remove_VKnot_at(int selected)					// Suppression V
{
	this->hide_Anchors();
	Point P = this->V_Knots_Val_Pts.at(selected);
	Edge_Mesh::remove_HEdges_at(P.get_V_Index());
}

/** Méthodes de gestion des degrés de travail dans le Mesh
/*  On se limite aux valeurs impaires 1(par défaut) et 3
*/
void Mesh_2D::change_H_Degree(int delta)
{
	if ((delta > 0) && (this->H_degree < 3)) this->H_degree += 2;
	if ((delta < 0) && (this->H_degree > 1)) this->H_degree -= 2;  
}

void Mesh_2D::change_V_Degree(int delta)
{
	if ((delta > 0) && (this->V_degree < 3)) this->V_degree += 2;
	if ((delta < 0) && (this->V_degree > 1)) this->V_degree -= 2;  
}

/************************************************/
/*				Méthodes protégées				*/
/************************************************/

/** Mise à jour des paramètre d'affichage
*/
void Mesh_2D::update_GL()
{
	this->H_Delta = (1.0f-PHI)*(float)this->W_Width;
	this->V_Delta = (1.0f-PHI)*(float)this->W_Height;
	this->H_Span = PHI*(float)this->W_Width/((float)this->H_Knots.get_Value_at(this->width)+1.0f);
	this->V_Span = PHI*(float)this->W_Height/((float)this->V_Knots.get_Value_at(this->height)+1.0f);
}

/** Méthode de Mise à jour des ancres à partir des précédentes
*/
void Mesh_2D::Update_And_Merge_If_New(vector<Anchor> new_A)
{
	// Si le vecteur d'ancre n'est pas vide
	if (this->Anchors.size()>=1) {

		// On élimine les éléments qui ont disparu dans la nouvelle liste en testant à rebours ...
		bool found_k;
		for (int z=this->Anchors.size()-1; z>=0; z--) {
			found_k = false;
			for (vector<Anchor>::iterator Ank=new_A.begin(); Ank!=new_A.end(); Ank++) {
				// ... la présence d'une ancre aux mêmes valeurs nodales ...
				found_k = (this->Anchors.at(z).get_Hk_midValue() == Ank->get_Hk_midValue())
						   && (this->Anchors.at(z).get_Vk_midValue() == Ank->get_Vk_midValue());
				if (found_k) break;
			}
			// si on n'a pas trouvé, on supprime
			if (!(found_k)) this->Anchors.erase(this->Anchors.begin()+z);
		}
		// On ajuste le vecteur d'ancre à son contenu
		this->Anchors.shrink_to_fit();

		// On met à jour la liste restante à partir de la nouvelle liste sur le même principe
		int index;
		for (vector<Anchor>::iterator nA=new_A.begin(); nA!=new_A.end(); nA++) {
			found_k = false;
			index = 0;
			for (vector<Anchor>::iterator Ank=this->Anchors.begin(); Ank!=this->Anchors.end(); Ank++) {
				found_k = (nA->get_Hk_midValue() == Ank->get_Hk_midValue())
						   && (nA->get_Vk_midValue() == Ank->get_Vk_midValue());
				if (found_k) break;
				index++;
			}
			// si on l'a trouvé dans la liste existante, on met à jour ses données
			if (found_k) {
				this->Anchors.at(index).set_Indexes(nA->get_H_Index(), nA->get_V_Index());
				this->Anchors.at(index).set_H_Knots(nA->get_H_Knots());
				this->Anchors.at(index).set_V_Knots(nA->get_V_Knots());
			}
			// sinon on le rajoute à la liste
			else this->Anchors.push_back(*nA);
		}
	}

	// Sinon (liste de départ vide), on initialise avec la nouvelle liste
	else this->Anchors = new_A;
}

/** Méthode de detction des ancres sur la grille
*/
void Mesh_2D::update_Anchors()
{
	// Initialisation d'une ancre courante et d'un nouveau vecteur d'ancres
	Anchor crnt_A;
	vector<Anchor> New_Anchors;

	// Initialisation d'un vecteur pour les indices des ancres
	vector<int*> Anchors_Indexes;
	Anchors_Indexes.clear();

	// Initialisation d'un vecteur pour les indices actifs horizontaux
	vector<vector<int>> Anchors_H_Indexes;
	Anchors_H_Indexes.clear();

	// Initialisation d'un vecteur pour les indices actif verticaux 
	vector<vector<int>> Anchors_V_Indexes;
	Anchors_V_Indexes.clear();

	// Déclaration d'un vecteur d'indices verticaux 
	vector<int> V_indexes;

	// Iitialisation d'un vecteur d'indice horizontaux 
	vector<int> H_indexes;
	H_indexes.clear();

	// Pour chaque poistion dans le Mesh (intersections)
	for (int h=0; h <= this->width; h++) {
		Edge_Line VL = this->V_Edges.at(h);			// Récupération de la ligne Verticale associée
		V_indexes.clear();							// Initialisation des indices verticaux
		for (int v=0; v <= this->height; v++) {
			Edge_Line HL = this->H_Edges.at(v);		// Récupération de la ligne Horizontale associée

			// Initialisation de chaque vecteur des indices actifs horizontaux
			if (h==0) Anchors_H_Indexes.push_back(H_indexes);

			// Detection d'un arête verticale active
			bool Vb = false;
			if (v == 0) Vb = VL.at(0);
			else if (v == this->height) Vb = VL.at(this->height-1);
			else Vb = VL.at(v-1) || VL.at(v);

			// Détection d'un arête horizontale active
			bool Hb = false;
			if (h == 0) Hb = HL.at(0);
			else if (h == this->width) Hb = HL.at(this->width-1);
			else Hb = HL.at(h-1) || HL.at(h);

			// On ajoute l'indice de l'arête horizontale si elle est active
			if (Vb) Anchors_H_Indexes.at(v).push_back(h);

			// On enregistre l'indice de l'arête verticale si elle est active
			if (Hb) V_indexes.push_back(v);

			// Si les deux sont actives, on a une ancre dont on enregistre les indices
			if (Vb && Hb) Anchors_Indexes.push_back(new int[2] {h, v});
		}
		// On ajoute le vecteur des indices verticaux avant de passer au suivant
		Anchors_V_Indexes.push_back(V_indexes);
	}

	// Déclaration d'un vecteur de valeurs de noeuds et d'un vecteur d'ancres
	vector<float> Knot_Values;
	New_Anchors.clear();

	// On repère parmi les lignes avec des ancres le nombre minimum d'arêtes V ou H à traverser
	int Hmin = this->width;
	int Vmin = this->height;
	for (vector<int*>::iterator Ank=Anchors_Indexes.begin(); Ank!=Anchors_Indexes.end(); Ank++) {
		int hs = Anchors_H_Indexes.at((*Ank)[1]).size();
		int vs = Anchors_V_Indexes.at((*Ank)[0]).size();
		Hmin = min(hs, Hmin);
		Vmin = min(vs, Vmin);
	}
	// Si le nombre d'arêtes minimum à traverser est insuffisant au regard du degré, on diminue le degré
	if (Hmin < this->H_degree+1) change_H_Degree(-2);
	if (Vmin < this->V_degree+1) change_V_Degree(-2);

	// Pour chaque ancre identifiée par ses indices
	for (vector<int*>::iterator Ank=Anchors_Indexes.begin(); Ank!=Anchors_Indexes.end(); Ank++) {

		// On crée l'ancre correspondante
		int h = (*Ank)[0];	
		int v = (*Ank)[1];
		crnt_A = Anchor(h,v);

		// On récupère la taille du vecteur des valeurs de noeuds diponibles sur les mêmes lignes 
		int hs = Anchors_H_Indexes.at(v).size();
		int vs = Anchors_V_Indexes.at(h).size();

		// On initialise un compteur et le vecteur de valeurs de noeuds
		int k = 0;
		Knot_Values.clear();

		// On récupère les valeurs de noeuds horizontales en fonction du degré horizontal
		while (h!= Anchors_H_Indexes.at(v).at(k)) k++;
		for (int l=-(floor(this->H_degree/2)+1); l<=floor(this->H_degree/2)+1; l++) {
			if (k+l<=0) Knot_Values.push_back(this->H_Knots.get_Value_at(Anchors_H_Indexes.at(v).at(0)));
			else if (k+l>=hs) Knot_Values.push_back(this->H_Knots.get_Value_at(Anchors_H_Indexes.at(v).at(hs-1)));
			else Knot_Values.push_back(this->H_Knots.get_Value_at(Anchors_H_Indexes.at(v).at(k+l)));
		}
		sort(Knot_Values.begin(), Knot_Values.end());	// On les trie dans l'ordre au cas où ...
		crnt_A.set_H_Knots(Knot_Values);				// On ajoute le vecteur horizontal à l'ancre créée

		// On ré-initialise les variables
		k = 0;
		Knot_Values.clear();

		// On récupère les valeurs de noeuds verticales en fonction du degré vertical
		while (v!= Anchors_V_Indexes.at(h).at(k)) k++;
		for (int l=-(floor(this->V_degree/2)+1); l<=floor(this->V_degree/2)+1; l++) {
			if (k+l<=0) Knot_Values.push_back(this->V_Knots.get_Value_at(Anchors_V_Indexes.at(h).at(0)));
			else if (k+l>=vs) Knot_Values.push_back(this->V_Knots.get_Value_at(Anchors_V_Indexes.at(h).at(vs-1)));
			else Knot_Values.push_back(this->V_Knots.get_Value_at(Anchors_V_Indexes.at(h).at(k+l)));
		}
		sort(Knot_Values.begin(), Knot_Values.end());	// On les trie dans l'ordre au cas où ...
		crnt_A.set_V_Knots(Knot_Values);				// On ajoute le vecteur vertical à l'ancre créée
		
		// On l'ajoute au nouveau vecteur d'ancres
		New_Anchors.push_back(crnt_A);
	}

	// On renvoie le nouveau vecteur mettre à jour le vecteur d'ancres
	this->Update_And_Merge_If_New(New_Anchors);
}

void Mesh_2D::hide_Anchors()
{
	// Pour chaque ancre
	for (vector<Anchor>::iterator Ank=this->Anchors.begin(); Ank!=this->Anchors.end(); Ank++) {
		// On l'active ...
		Ank->set_Active();
		// ... et on change son statut --> du coup elle est désactivée !
		Ank->switch_State();
	}
}

/** Méthodes d'affichage ************************************************************/
/*  --> arêtes (H et V)																*/
/*  --> barres de noeuds (H et V)													*/
/*	--> ancres 																		*/
/************************************************************************************/

void Mesh_2D::display_H_Edges()
{
	Point P;
	// Initialisation des vecteurs de Picking horizontaux
	this->H_Edges_Middles.clear();
	this->H_Knots_I_Hooks.clear();
	this->H_Knots_Val_Pts.clear();

	// Pour chaque ligne horizontale : v = 0 --> height (inclus)
	for (int v=0; v <= this->height; v++) {
		Edge_Line H_Line = this->H_Edges.at(v);
		for (int h=0; h < this->width; h++) {

			// On calcule les coordonnées des extrémités de l'arête
			float yd = this->V_Delta+this->V_Knots.get_Value_at(v)*this->V_Span;
			float xd = this->H_Delta+this->H_Knots.get_Value_at(h)*this->H_Span;
			float xf = this->H_Delta+this->H_Knots.get_Value_at(h+1)*this->H_Span;
			
			// On renseigne le point central de Picking
			P = Point((xd+xf)/2.0f, yd, h, v);
			this->H_Edges_Middles.push_back(P);

			// Au premier passage
			if (v == 0) {
				// On en profite pour créer les crochets sur les intervalles de Noeuds
				P = Point((xd+xf)/2.0f, this->V_Delta/3.0f, h, v);
				this->H_Knots_I_Hooks.push_back(P);
				// On en profite pour remplir les positions des valeurs de Noeuds (même la dernière)
				P = Point(xd, this->V_Delta/3.0f, h, v);
				P.set_Weight(this->H_Knots.get_Value_at(h));
				this->H_Knots_Val_Pts.push_back(P);
				if (h == this->width-1)  {
					P = Point(xf, this->V_Delta/3.0f, this->width, v);
					P.set_Weight(this->H_Knots.get_Value_at(h+1));
					this->H_Knots_Val_Pts.push_back(P);
				}
			}

			// On affiche les arêtes inactives en pointillés orange foncé
			if (!H_Line.at(h)) {
				glLineWidth(1.0f);
				glColor3f(0.48f, 0.24f, 0.12f);
				glPushAttrib(GL_ENABLE_BIT); 
					glLineStipple(1, 0x8888);
					glEnable(GL_LINE_STIPPLE);
			}
			// et les actives en rouge
			else {
				glLineWidth(12.0f);
				glColor3f(0.96f, 0.12f, 0.12f);
			}
			glBegin(GL_LINES);
				glVertex3f(xd, yd, 0.0f);
				glVertex3f(xf, yd, 0.0f);
			glEnd();
			if(!H_Line.at(h)) glPopAttrib();
		}
	}
}

void Mesh_2D::display_V_Edges()
{
	// Initialisation des vecteurs de Picking verticaux
	Point P;
	this->V_Edges_Middles.clear();
	this->V_Knots_I_Hooks.clear();
	this->V_Knots_Val_Pts.clear();

	// Pour chaque ligne verticale : h = 0 --> width (inclus)
	for (int h=0; h <= this->width; h++) {
		Edge_Line V_Line = this->V_Edges.at(h);
		for (int v=0; v < this->height; v++) {

			// On calcule les coordonnées des extrémités de l'arête
			float xd = this->H_Delta+this->H_Knots.get_Value_at(h)*this->H_Span;
			float yd = this->V_Delta+this->V_Knots.get_Value_at(v)*this->V_Span;
			float yf = this->V_Delta+this->V_Knots.get_Value_at(v+1)*this->V_Span;

			// On renseigne le point central pour le Picking
			P = Point(xd, (yd+yf)/2.0f, h, v);
			this->V_Edges_Middles.push_back(P);

			// Au premier passage
			if (h == 0) {
				// On en profite pour remplir les crochets sur les intervalles de Noeuds
				P = Point(this->H_Delta/3.0f, (yd+yf)/2.0f, h, v);
				this->V_Knots_I_Hooks.push_back(P);
				// On en profite pour remplir les positions des valeurs de Noeuds (même la dernière)
				P = Point(this->H_Delta/3.0f, yd, h, v);
				P.set_Weight(this->V_Knots.get_Value_at(v));
				this->V_Knots_Val_Pts.push_back(P);
				if (v == this->height-1)  {
					P = Point(this->H_Delta/3.0f, yf, h, this->height);
					P.set_Weight(this->V_Knots.get_Value_at(v+1));
					this->V_Knots_Val_Pts.push_back(P);
				}
			}

			// On affiche les arêtes inactives en pointillés orange foncé
			if (!V_Line.at(v)) {
				glLineWidth(1.0f);
				glColor3f(0.48f, 0.24f, 0.12f);
				glPushAttrib(GL_ENABLE_BIT); 
					glLineStipple(1, 0x8888);
					glEnable(GL_LINE_STIPPLE);
			}
			// et les actives en rouge
			else {
				glLineWidth(12.0f);
				glColor3f(0.96f, 0.12f, 0.12f);
			}
			glBegin(GL_LINES);
				glVertex3f(xd, yd, 0.0f);
				glVertex3f(xd, yf, 0.0f);
			glEnd();
			if(!V_Line.at(v)) glPopAttrib();
		}
	}
}

void Mesh_2D::display_H_Knots()
{
	// On trace la ligne support (du premier au dernier point)
	glColor3f(0.96f, 0.48f, 0.24f);
	glBegin(GL_LINES);
		glVertex3fv(this->H_Knots_Val_Pts.front().get_Vertex());
		glVertex3fv(this->H_Knots_Val_Pts.back().get_Vertex());
	glEnd();

	// Pour chaque valeur nodale on affiche un trait vertical avec la valeur en dessous
	for (vector<Point>::iterator Pt=this->H_Knots_Val_Pts.begin(); Pt!=this->H_Knots_Val_Pts.end(); Pt++) {
		glBegin(GL_LINES);
			glVertex3fv(Pt->add_Y(6.0f).get_Vertex());
			glVertex3fv(Pt->add_Y(-12.0f).get_Vertex());
		glEnd();
		char writeBuffer[12] = {'\0'};
		sprintf(writeBuffer, "%.2f", Pt->get_Weight());
		this->display_String(Pt->get_X()-16.0f, Pt->get_Y()-36.0f, GLUT_BITMAP_HELVETICA_18, writeBuffer);
	}

	// Pour chaque intervalle, on affiche le crochet pour le modifier en son centre
	glColor3f(0.96f, 0.48f, 0.24f);
	glPointSize(7.0f);
	glBegin(GL_POINTS);
	for (vector<Point>::iterator Pt=this->H_Knots_I_Hooks.begin(); Pt!=this->H_Knots_I_Hooks.end(); Pt++) {
		glVertex3fv(Pt->get_Vertex());
	}
	glEnd();
}

void Mesh_2D::display_V_Knots()
{
	// On trace la ligne support (du premier au dernier point)
	glColor3f(0.96f, 0.48f, 0.24f);
	glBegin(GL_LINES);
		glVertex3fv(this->V_Knots_Val_Pts.front().get_Vertex());
		glVertex3fv(this->V_Knots_Val_Pts.back().get_Vertex());
	glEnd();

	// Pour chaque valeur nodale on affiche un trait horizontal avec la valeur à gauche	
	for (vector<Point>::iterator Pt=this->V_Knots_Val_Pts.begin(); Pt!=this->V_Knots_Val_Pts.end(); Pt++) {
		glBegin(GL_LINES);
			glVertex3fv(Pt->add_X(6.0f).get_Vertex());
			glVertex3fv(Pt->add_X(-12.0f).get_Vertex());
		glEnd();	
		char writeBuffer[12] = {'\0'};
		sprintf(writeBuffer, "%.2f", Pt->get_Weight());
		this->display_String(Pt->get_X()-60.0f, Pt->get_Y()-6.0f, GLUT_BITMAP_HELVETICA_18, writeBuffer);
	}

	// Pour chaque intervalle, on affiche le crochet pour le modifier en son centre	
	glColor3f(0.96f, 0.48f, 0.24f);
	glPointSize(7.0f);
	glBegin(GL_POINTS);
	for (vector<Point>::iterator Pt=this->V_Knots_I_Hooks.begin(); Pt!=this->V_Knots_I_Hooks.end(); Pt++) {
		glVertex3fv(Pt->get_Vertex());
	}
	glEnd();
}

void Mesh_2D::display_Anchors()
{
	// On met à jour les ancres
	this->update_Anchors();

	// Pour chaque ancre ...
	for (int k=0; k<this->Anchors.size(); k++) {
		Anchor A = this->Anchors.at(k);
		glPointSize(7.0f);

		// ... Si elle est active
		if (A.is_Active()) {

			// On affiche son point d'ancrage dans le T_Mesh dans sa couleur
			glColor4fv(A.get_Pt_Color());
			glBegin(GL_POINTS);
				glVertex3f(this->H_Delta+A.get_Hk_midValue()*this->H_Span,
					   	   this->V_Delta+A.get_Vk_midValue()*this->V_Span,
						   0.0f);
			glEnd();	

			// On affiche sa zone d'influence dans le T_Mesh en transparence		
			glColor4fv(A.get_Color());
			float Hmin = A.get_H_Knots().mini();
			float Hmax = A.get_H_Knots().maxi();
			float Vmin = A.get_V_Knots().mini();
			float Vmax = A.get_V_Knots().maxi();
			glBegin(GL_QUADS);
				glVertex3f(this->H_Delta+Hmin*this->H_Span, this->V_Delta+Vmin*this->V_Span, 0.0f);
				glVertex3f(this->H_Delta+Hmax*this->H_Span, this->V_Delta+Vmin*this->V_Span, 0.0f);
				glVertex3f(this->H_Delta+Hmax*this->H_Span,	this->V_Delta+Vmax*this->V_Span, 0.0f);
				glVertex3f(this->H_Delta+Hmin*this->H_Span, this->V_Delta+Vmax*this->V_Span, 0.0f);
			glEnd();

			// On trace la courbe d'influence horizontale sur la barre de noeud horizontale
			Point Prev;
			Point Crnt;
			Prev = Point(this->H_Delta+Hmin*this->H_Span,
						 this->V_Delta/3.0f+A.get_HBF_Value(Hmin)*this->V_Delta/3.0f);
			glColor4fv(A.get_Pt_Color());
			for (float t=Hmin+0.01f; t<=Hmax; t=t+0.01f) {
				Crnt = Point(this->H_Delta+t*this->H_Span,
						 	 this->V_Delta/3.0f+A.get_HBF_Value(t)*this->V_Delta/3.0f);
				glBegin(GL_LINES);
					glVertex3fv(Prev.get_Vertex());
					glVertex3fv(Crnt.get_Vertex());
				glEnd();
				Prev = Crnt;
			}

			// On trace la courbe d'influence verticale à droite de la barre de noeud verticale
			Prev = Point(this->H_Delta/3.0f+A.get_VBF_Value(Vmin)*this->H_Delta/3.0f,
						 this->V_Delta+Vmin*this->V_Span);
			glColor4fv(A.get_Pt_Color());
			for (float t=Vmin+0.01f; t<=Vmax; t=t+0.01f) {
				Crnt = Point(this->H_Delta/3.0f+A.get_VBF_Value(t)*this->H_Delta/3.0f,
							 this->V_Delta+t*this->V_Span);
				glBegin(GL_LINES);
					glVertex3fv(Prev.get_Vertex());
					glVertex3fv(Crnt.get_Vertex());
				glEnd();
				Prev = Crnt;
			}
		}

		// ... Sinon l'ancre est inactive
		else {

			// On affiche juste son point d'ancgrage dans le T_Mesh en vert
			glColor3f(0.12f, 0.60f, 0.12f);
			glBegin(GL_POINTS);
				glVertex3f(this->H_Delta+A.get_Hk_midValue()*this->H_Span,
					   	   this->V_Delta+A.get_Vk_midValue()*this->V_Span,
						   0.0f);
			glEnd();
		}

	}
}

/** Méthode d'écriture de chîne en Open GL
*/
void Mesh_2D::display_String(float x, float y, void *font, const char *string)
{
	const char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

/** Méthode générale de Picking sur un vecteur de Points
*/
int Mesh_2D::getPicked(int mState, int x, int y, vector<Point> Pts)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLuint selected;
	int vertex_Nb = Pts.size();
	if ((mState == GLUT_DOWN) && (vertex_Nb > 0)) {				
		GLuint select_Buffer[2 * vertex_Nb]; 					
		GLuint *BufferSelected_ptr;	
		GLint Selected_Nb; 							
		glSelectBuffer(2 * vertex_Nb, select_Buffer);			
		glRenderMode(GL_SELECT); 							
		glPushMatrix();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPickMatrix(x, viewport[3]-y, 24.0f, 24.0f, viewport); 
			glOrtho(0.0f, this->W_Width, 0.0f, this->W_Height, -1.0f, 1.0f);
			glInitNames();
			glPushName(2);
			for (int i=0; i < vertex_Nb; i++) {
				glLoadName(i);							
				glBegin(GL_POINTS);
					glVertex2fv(Pts.at(i).get_Vertex());
				glEnd();
			}
		glPopMatrix();
		glFlush();
		Selected_Nb = glRenderMode(GL_RENDER); 
		if (Selected_Nb) {
			BufferSelected_ptr = (GLuint *)select_Buffer; 
			BufferSelected_ptr += 3;		
			selected = *BufferSelected_ptr;
		}
		else selected = -1;
	}
	else selected = -1;
	return selected;
}
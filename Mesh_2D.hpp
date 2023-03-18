#ifndef MESH_2D
#define MESH_2D

#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include "Point.hpp"
#include "Edge_Mesh.hpp"
#include "Anchor.hpp"

class Mesh_2D : public Edge_Mesh {

protected:

	/** Variables ****************************************************************/

	int W_Width;							// La largeur de l'affichage en pixels
	int W_Height;							// La hauteur de l'affichage en pixels

	float H_Delta;							// Le décalage horizontal du T_Mesh
	float V_Delta;							// Le décalage vertical du T_Mesh
	float H_Span;							// Le nombre de pixels par unité (H)
	float V_Span;							// Le nombre de pixels par unité (V)

	std::vector<Point> H_Edges_Middles;		// Les points pour le picking des arêtes H
	std::vector<Point> V_Edges_Middles;		// Les points pour le picking des arêtes V
	std::vector<Point> H_Knots_I_Hooks;		// Les points pour le picking des intervalles H
	std::vector<Point> V_Knots_I_Hooks;		// Les points pour le picking des intervalles V
	std::vector<Point> H_Knots_Val_Pts;		// Les points pour l'affichage des noeuds H
	std::vector<Point> V_Knots_Val_Pts;		// Les points pour l'affichage des noeuds V

	std::vector<Anchor> Anchors;			// Les ancres du T_Mesh

	/** Méthodes *****************************************************************/

	void update_GL();				// Mise à jour des paramètres d'affichage
	
	/* Mise à jour des ancres à partir des précédentes*/
	void Update_And_Merge_If_New(std::vector<Anchor> new_A);
	void update_Anchors();			// Mise à jour complète des Ancres à partir de la grille
	void hide_Anchors();
	
	void display_H_Edges();			// Affichage des arêtes Horizontales
	void display_V_Edges();			// Affichage des arêtes verticales
	void display_H_Knots();			// Affichage de la barre de noeud horizontale
	void display_V_Knots();			// Affichage de la barre de noeuds verticale
	void display_Anchors();			// Affichage des ancres

	/* Méthode d'affichage d'une chaine de caractères */
	void display_String(float x, float y, void *font, const char *string);

	/* Méthode générale de picking dans un vecteur de points */
	int getPicked(int mState, int x, int y, std::vector<Point> Pts);

public:

	/** Structeurs ***************************************************************/

	Mesh_2D();
	Mesh_2D(int Wdw_Width, int Wdw_Height);
	Mesh_2D(int init_width, int init_height, int Wdw_Width, int Wdw_Height);
	~Mesh_2D();

	/** Méthodes *****************************************************************/

	void set_W_Size(int Wdw_Width, int Wdw_Height);		// Rafraichissement des dimensions de l'affichage
	void display();										// Méthode d'affichage complète

	int getPicked_H_Edge(int mState, int x, int y);		// Picking pour les arêtes horizontales
	int getPicked_V_Edge(int mState, int x, int y);		// Picking pour les arêtes verticales
	int getPicked_H_Knot(int mState, int x, int y);		// Picking pour les intervalles de noeuds H
	int getPicked_V_Knot(int mState, int x, int y);		// Picking pour les intervalles de noeuds V
	int getPicked_Anchor(int mState, int x, int y);		// Picking pour les ancres

	void switch_H_Edge(int selected);					// Changement de statut arête H
	void switch_V_Edge(int selected);					// Changement de statut arête V
	void switch_H_Line(int selected);					// Changement de statut Ligne H
	void switch_V_Line(int selected);					// Changement de statut Ligne V
	void switch_Anchor(int selected);					// Changement de statut Ancre

	void change_H_Interval(int selected, float delta);	// Modification d'une valeur d'intervalle H
	void change_V_Interval(int selected, float delta);	// Modification d'une valeur d'intervalle V

	void insert_HKnot_at(int selected);					// Insertion d'un noeud H
	void insert_VKnot_at(int selected);					// Insertion d'un noeud V

	void remove_HKnot_at(int selected);					// Suppression d'un noeud H
	void remove_VKnot_at(int selected);					// Suppression d'un noeud V

	void change_H_Degree(int delta);					// Changer le degré horizontal
	void change_V_Degree(int delta);					// Changer le degré vertical

};

#endif
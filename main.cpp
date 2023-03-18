#include <iostream>
#include <GL/glut.h>
#include "Mesh_2D.hpp"

using namespace std;

/** Variables ********************************************************************/

Mesh_2D T_Mesh;					// Le T_Mesh !

int dispWdwXpos		{60};		// Position de l'affichage en X
int dispWdwYpos		{60};		// Position de l'affichage en Y
int dispWdwHeight	{1200};		// Hauteur de l'affichage
int dispWdwWidth	{1000};		// Largeur de l'affichage
int zOrthoMin		{-1};		// Profondeur minimum de l'affichage
int zOrthoMax		{ 1};		// Profondeur maximum de l'affichage
int action 			{-1};		// Le selecteur d'action
int selected		{-1};		// Le selcteur de point "pické"
	
float bgColor[4]	= {0.12f, 0.12f, 0.12f, 0.0f};	// Couleur d'arrière plan

float old_x 	{0.0f};			// Mémorisation de x pour mousemotion()
float old_y 	{0.0f};			// Mémorisation de y pour mousemotion()

/** Prototypes des fonctions *****************************************************/

void display();											// Gestion de l'affichage
void initMenu();										// Initialisation du menu
void menu(int item);									// Gestion du menu
void keyboard(unsigned char touche, int x, int y);		// Gestion du clavier
void special(int key, int x, int y);					// Gestion des touches speciales
void mouse(int mButton, int mState, int x, int y);		// Gestion du clic souris
void mousemotion(int x, int y);							// Gestion du déplacement de la souris
void reshape(int width, int height);					// gestion du dimensionnement

/** Implémentation des fonctions *************************************************/

/** Gestion de l'affichage
*/
void display()
{
	// Effacement de l'image avec la couleur de fond
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);

	// Rappel des paramètres d'affichage
	glClear(GL_COLOR_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	reshape(dispWdwWidth, dispWdwHeight);

	// Objets à afficher
	glLoadIdentity();
	glPushMatrix();
		T_Mesh.display();
	glPopMatrix();
	
	// Nettoyage
	glFlush();
	glutSwapBuffers();
}

/** Définition des items du menu "clic-droit"
*/
void initMenu()
{
	// Sous-menu des arêtes horizontales
	int sm_he = glutCreateMenu(menu);
	glutAddMenuEntry(" Horizontal Edges ",  0);
	glutAddMenuEntry(" ________________ ",  0);
	glutAddMenuEntry("  switch Line     ", 11);
	glutAddMenuEntry("  switch Edge     ", 12);

	// Sous-menu des arêtes verticales
	int sm_ve = glutCreateMenu(menu);
	glutAddMenuEntry(" Vertical Edges   ",  0);
	glutAddMenuEntry(" ________________ ",  0);
	glutAddMenuEntry("  switch Line     ", 13);
	glutAddMenuEntry("  switch Edge     ", 14);

	// Sous-menu des noeuds horizontaux
	int sm_hk = glutCreateMenu(menu);
	glutAddMenuEntry(" Horizontal Knots ",  0);
	glutAddMenuEntry(" ________________ ",  0);
	glutAddMenuEntry("   Move           ", 21);
	glutAddMenuEntry("   Insert         ", 22);
	glutAddMenuEntry("   Delete         ", 23);

	// Sous-menu des noeuds verticaux
	int sm_vk = glutCreateMenu(menu);
	glutAddMenuEntry(" Vertical Knots   ",  0);
	glutAddMenuEntry(" ________________ ",  0);
	glutAddMenuEntry("   Move           ", 31);
	glutAddMenuEntry("   Insert         ", 32);
	glutAddMenuEntry("   Delete         ", 33);

	// Menu principal
	int main_menu = glutCreateMenu(menu);
	glutAddSubMenu  ("   Horiz. Edges   ", sm_he);
	glutAddSubMenu  ("   Vert.  Edges   ", sm_ve);
	glutAddMenuEntry("   Anchor switch  ",  4);
	glutAddMenuEntry(" ________________ ",  0);
	glutAddSubMenu	("   Horiz. Knots   ", sm_hk);
	glutAddSubMenu  ("   Vert.  Knots   ", sm_vk);
	glutAddMenuEntry(" ________________ ",  0);
	glutAddMenuEntry("   CANCEL         ",  8);
	glutAddMenuEntry("   EXIT           ",  9);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/** Gestion des actions liées aux items du menu
*/
void menu(int item)
{
	switch (item) {
	
		case 11:			//--------------------- Switch sur une ligne horizontale entière
			action = item;
			glutSetCursor(GLUT_CURSOR_INFO);
			break;
			
		case 12:			//--------------------- Switch sur une arête horizontale
			action = item;
			glutSetCursor(GLUT_CURSOR_INFO);
			break;
			
		case 13:			//--------------------- Switch sur une ligne verticale entière
			action = item;
			glutSetCursor(GLUT_CURSOR_INFO);
			break;
			
		case 14:			//--------------------- Switch sur une arête verticale
			action = item;
			glutSetCursor(GLUT_CURSOR_INFO);
			break;
			
		case 21:			//--------------------- Modifier un noeud horizontal
			action = item;
			glutSetCursor(GLUT_CURSOR_INFO);
			break;

		case 22:			//--------------------- Ajouter un noeud horizontal
			action = item;
			glutSetCursor(GLUT_CURSOR_HELP);
			break;

		case 23:			//--------------------- Supprimer un noeud horizontal
			action = item;
			glutSetCursor(GLUT_CURSOR_DESTROY);
			break;
		
		case 31:			//--------------------- Modifier un noeud vertical
			action = item;
			glutSetCursor(GLUT_CURSOR_INFO);
			break;
			
		case 32:			//--------------------- Insérer un noeud vertical
			action = item;
			glutSetCursor(GLUT_CURSOR_HELP);
			break;
			
		case 33:			//--------------------- Supprimer un noeud vertical
			action = item;
			glutSetCursor(GLUT_CURSOR_DESTROY);
			break;
			
		case 4:				//--------------------- Switch des Ancres
			action = item;
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
			break;
			
		case 8:				//--------------------- Annuler les opérations en cours
			action = 0;
			selected = -1;
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			glutPostRedisplay();
			break;
			
		case 9:				//-------------------- Quitter
			exit(0);
			break;		
	}
}

/** Gestion des saisies clavier
*/
void keyboard(unsigned char touche, int x, int y)
{
	switch(touche) {
	
		case 'H':			//--------------------- Augmentation du degré horizontal
			T_Mesh.change_H_Degree(2);
			break;
			
		case 'h':			//--------------------- Diminution du degré horizontal
			T_Mesh.change_H_Degree(-2);
			break;

		case 'V':			//--------------------- Augmentation du degré vertical
			T_Mesh.change_V_Degree(2);
			break;
			
		case 'v':			//--------------------- Diminution du degré vertical
			T_Mesh.change_V_Degree(-2);
			break;
			
		case 'q':			//--------------------- Quitter
			exit(0);
			break;
	}					
	glutPostRedisplay();
}

/** Gestion des saisies clavier spéciales
*/
void special(int key, int x, int y)
{
	switch(key) {
	
		case GLUT_KEY_LEFT:
			break;
			
		case GLUT_KEY_RIGHT:
			break;
			
		default :
			break;
	}	
}

/** Gestion du clic souris
*/
void mouse(int mButton, int mState, int x, int y)
{
	// On ne s'occupe que du clic gauche
	if (mButton == GLUT_LEFT_BUTTON) {

		switch (action) {

			case 11:			//------------------------- Switch d'une ligne horizontale
				selected = T_Mesh.getPicked_H_Edge(mState, x, y);
				if (selected > -1) T_Mesh.switch_H_Line(selected);
				break;

			case 12:			//------------------------- Switch d'une arête horizontale
				selected = T_Mesh.getPicked_H_Edge(mState, x, y);
				if (selected > -1) T_Mesh.switch_H_Edge(selected);
				break;
				
			case 13:			//------------------------- Switch d'une ligne verticale
				selected = T_Mesh.getPicked_V_Edge(mState, x, y);
				if (selected > -1) T_Mesh.switch_V_Line(selected);
				break;

			case 14:			//------------------------- Switch d'une arête verticale
				selected = T_Mesh.getPicked_V_Edge(mState, x, y);
				if (selected > -1) T_Mesh.switch_V_Edge(selected);
				break;
				
			case 21:			//------------------------- On modifie un noeud horizontal
				selected = T_Mesh.getPicked_H_Knot(mState, x, y);
				break;

			case 22:			//------------------------- On insère un noeud horizontal
				selected = T_Mesh.getPicked_H_Knot(mState, x, y);
				if (selected > -1) T_Mesh.insert_HKnot_at(selected);
				break;
				
			case 23:			//------------------------- On insère un noeud horizontal
				selected = T_Mesh.getPicked_H_Knot(mState, x, y);
				if (selected > -1) T_Mesh.remove_HKnot_at(selected);
				break;
				
			case 31:			//------------------------- On modifie un noeud vertical
				selected = T_Mesh.getPicked_V_Knot(mState, x, y);
				break;
				
			case 32:			//------------------------- On insère un noeud vertical
				selected = T_Mesh.getPicked_V_Knot(mState, x, y);
				if (selected > -1) T_Mesh.insert_VKnot_at(selected);
				break;

			case 33:			//------------------------- On insère un noeud vertical
				selected = T_Mesh.getPicked_V_Knot(mState, x, y);
				if (selected > -1) T_Mesh.remove_VKnot_at(selected);
				break;

			case 4:				//------------------------- On switche le statut d'une ancre
				selected = T_Mesh.getPicked_Anchor(mState, x, y);
				if (selected > -1) T_Mesh.switch_Anchor(selected);
				break;

			default:
				break;		
		}
		// Si on relâche le bouton alors on supprime la sélection active
		if (mState == GLUT_UP) selected = -1;					
		glutPostRedisplay();
	}
}

/** Gestion des déplacements de la souris
*/
void mousemotion(int x, int y)
{
	// Si une action est en cours avec le bouton gauche appuyé
	if (selected != -1) {

		// On convertit les coordonnées écran
		float new_x = (float)x;
		float new_y = (float)(dispWdwHeight - y);

		// Suivant l'action validée dans le menu ...
		if (action == 21) T_Mesh.change_H_Interval(selected, new_x-old_x);	// On modifie un intervalle de noeud H
		if (action == 31) T_Mesh.change_V_Interval(selected, new_y-old_y);	// On modifie un intervalle de noeud V
		
		// On enregistre la position pour évaluer le déplacement suivant et on ré-affiche
		old_x = new_x;
		old_y = new_y;
		glutPostRedisplay();
	}
}

/** Mise à jour de la définition et des modalités de l'affichage
*/ 
void reshape(int width, int height)
{
	glViewport(0, 0, width, height); 			
	dispWdwWidth = width;
	dispWdwHeight = height;
	T_Mesh.set_W_Size(width, height);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	glOrtho(0.0f, width, 0.0f, height, zOrthoMin, zOrthoMax); 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
}


/*********************************************************/
/*             	Lancement de l'application				 */
/*********************************************************/

int main(int argc, char** argv)
{
	//Initialisation de GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(dispWdwXpos, dispWdwYpos);
	glutInitWindowSize(dispWdwWidth, dispWdwHeight);
	glutCreateWindow("T-Mesh");
		
	// Initialisation des styles d'affichage
	glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Activation du T_Mesh
	T_Mesh = Mesh_2D(dispWdwWidth, dispWdwHeight);
	
	/* Enregistrement des fonctions de rappel */
	initMenu();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemotion);

	// Entrée dans la boucle OpenGL
	glutMainLoop();

	// Quitter sans erreurs
	return 0;
}
#include "doom_nukem.h"

/*
**	Antoine t'es un encule le cercle trigo EST INVERSE oublie pas, wtf t debile une grosse salope
pute pute pute pute pute pute pute pute pute pute pute pute pute pute pute 
**
**	Ajouts :
**	- Limite la recherche de mur a la taille de la hitbox ! opti
**	- Gliss sur les murs
**
**	s = perimetre / 2 = somme des trois cotes / 2
**	On melange 2 formules sur l'aire du triangle : base * h / 2 = sqrt(s(s-a)(s-b)(s-c))
**	Et boom on a h par rapport aux 3 cotes.
**
**
**	Nouveau systeme collisions :
**	-	Raycasting dans la direction physique du player
**	-	de la largeur de la hitbox si possible
**	-	Il faut inverser le cercle trigo pour que l'angle soit coherent
**			avec le repere.
** Espece de ptite salope a t'ecrire des notes persos t'es la reine des mouilles tu sais pas coder et t'es nul en math en plus */

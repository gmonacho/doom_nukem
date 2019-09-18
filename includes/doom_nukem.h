#ifndef DOOM_NUKEM_H
# define DOOM_NUKEM_H


# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <math.h>
# include <stdio.h>//////////////////////////////////////////////////////////////

# include "SDL.h"
# include "SDL_ttf.h"
# include "SDL_image.h"
// # include "SDL_mixer.h"
# include "libft.h"

# define MAX_SECTORS		25
# define NEXT_FACTOR		50
# define HEIGHT_WALL		150
# define RENDER_DISTANCE	HEIGHT_WALL * 200
# define SIDE_LEN			40
# define ID_SIZE            20
# define ANG_HOR_VER        5

# define _2_PI			    6.28318530718
# define _3_PI_2		    4.71238898038

# include "bordel_struct.h"
# include "bordel_proto.h"

#endif

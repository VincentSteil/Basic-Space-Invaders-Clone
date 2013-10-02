//SpaceInvaders.h

extern const unsigned char shoot[];

extern const unsigned char SmallExplosion[];
// introduction screen with the words "Space Invaders"
// width=128 by height=68 (so there can be space on top/bottom of screen for your name(s) or other text)
extern const unsigned char marquee[];

// enemy ship that starts at the top of the screen (arms/mouth closed)
// width=16 by height=10
extern const unsigned char SmallEnemy30PointA[];

// clear enemy ship
// width 16 by height 10
extern const unsigned char SmallEnemy30PointAClear[];

// enemy ship that starts at the top of the screen (arms/mouth open)
// width=16 by height=10
extern const unsigned char SmallEnemy30PointB[];

// enemy ship that starts in the middle of the screen (arms together)
// width=16 by height=10
extern const unsigned char SmallEnemy20PointA[];

// enemy ship that starts in the middle of the screen (arms apart)
// width=16 by height=10
extern const unsigned char SmallEnemy20PointB[];

// enemy ship that starts at the bottom of the screen (arms down)
// width=16 by height=10
extern const unsigned char SmallEnemy10PointA[];

// enemy ship that starts at the bottom of the screen (arms up)
// width=16 by height=10
extern const unsigned char SmallEnemy10PointB[];

// image of the player's ship
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// width=18 by height=8
extern const unsigned char PlayerShip0[]; 



// small, fast bonus enemy that occasionally speeds across the top of the screen after enough enemies have been killed to make room for it
// includes two blacked out columns on the left and right sides of the image to prevent smearing when moved 2 pixels to the left or right
// width=20 by height=7
extern const unsigned char SmallEnemyBonus0[];

// small shield floating in space to cover the player's ship from enemy fire (undamaged)
// width=18 by height=5
extern const unsigned char Bunker0[];

// small shield floating in space to cover the player's ship from enemy fire (moderate generic damage)
// width=18 by height=5
extern const unsigned char Bunker1[];

// small shield floating in space to cover the player's ship from enemy fire (heavy generic damage)
// width=18 by height=5
extern const unsigned char Bunker2[];


// blank space used to cover a bunker that is destroyed
// width=18 x height=5
extern const unsigned char Bunker3[];

// large explosion that can be used upon the demise of the player's ship (first frame)
// width=18 by height=8
extern const unsigned char BigExplosion0[];

// large explosion that can be used upon the demise of the player's ship (second frame)
// width=18 by height=8
extern const unsigned char BigExplosion1[];

// small explosion best used for the demise of an enemy
// width=16 by height=10
extern const unsigned char SmallExplosion0[];

// blank space following the small explosion for the demise of an enemy
// width=16 by height=10
extern const unsigned char SmallExplosion1[];

// a missile in flight
// includes one blacked out row on the top and bottom of the image to prevent smearing when moved 1 pixel up or down
// width=4 by height=9
extern const unsigned char Missile0[];

// blank space to cover a missile after it hits something
// width=4 x height=9
extern const unsigned char Missile1[];

// a laser burst in flight
// includes one blacked out row on the top and bottom of the image to prevent smearing when moved 1 pixel up or down
// width=2 by height=9
extern const unsigned char Laser0[];

// blank space to cover a laser after it hits something
// width=2 x height=9
extern const unsigned char Laser1[];

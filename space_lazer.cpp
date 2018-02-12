#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
    #include <cstdlib>
#else
#include <stdlib.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "Box2D/Box2D.h"


void bougervaisseau(b2Body*,SDL_Rect *position,int x,int y,double temps, b2Vec2 forceVaisseau);
bool Collision(SDL_Rect position_vaisseaux  ,SDL_Rect position_lazer );
bool Collision2( SDL_Rect position_vaisseaux ,SDL_Rect position_lazer );
void apparition_laser( SDL_Surface* lazer , int opacite);

//// vaisseau qui remonte alors que la barre ne bouge pas
int space_lazer ()
{
    int continuer = 1, touche,i=0,numeroimagevaiseau=0, numeroimagelazer=0 ,sortie_map=0, opacite=0;
    bool up=0,down=0,left=0,right=0,reset=1,testbool=0, testbool2=0 ;
    SDL_Surface *tableauimage[4],*tableauimage2[4], *tableaulazer[9];
    SDL_Surface *ecran= NULL, *lazer ;
    SDL_Rect position_vaisseaux, position_vaisseaux2, position_copie_vaisseaux,position_haut, position_lazer ;
    SDL_Event event;
    position_vaisseaux.x=600 ;
    position_vaisseaux.y=325;
    position_haut.x=0 ;
    position_haut.y=0;
    position_lazer.x=0;
    position_lazer.y=0;
    SDL_Init(SDL_INIT_VIDEO);
    ecran=SDL_SetVideoMode(1200, 650, 32, SDL_HWSURFACE| SDL_DOUBLEBUF );
    SDL_WM_SetCaption("SPACE_LAZER", NULL);
    // SDL_ShowCursor(SDL_DISABLE);
    SDL_EnableKeyRepeat(1, 1);

    // definition de la gravité
    b2Vec2 gravity(0.0,0);
    //définition des forces sur le vaisseau
    b2Vec2 forceVaisseau(0,0);
    // création du monde avec la gravité définis plus haut
    b2World world(gravity);
    // definition des futurs body
    b2BodyDef bodyDef;
    b2BodyDef groundBodyDef;
    b2BodyDef groundBodyDef2;
    // definition du corps en dynamique
    bodyDef.type = b2_dynamicBody;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef2.type = b2_staticBody;
    // positionement d'un body
    groundBodyDef.position.Set(0, 520);
    groundBodyDef2.position.Set(0, -476);
    bodyDef.position.Set(50,20);
    // création du body dans le monde
    b2Body* body = world.CreateBody(&bodyDef);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2Body* groundBody2 = world.CreateBody(&groundBodyDef2);
    // création d'une forme pour le corps
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(267,238);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(1, 1);
    // création d'une fixation
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1;
    fixtureDef.friction =0.0;
    // fixation attribué a un corps
    body->CreateFixture(&fixtureDef);
    groundBody->CreateFixture(&fixtureDef);
    groundBody2->CreateFixture(&fixtureDef);

    float32 timeStep = 1.0f / 60.0f;

    int32 velocityIterations = 6;
    int32 positionIterations = 2;


    position_vaisseaux2.x = 1250;
    //position_vaisseaux.x = -300;
    bool tableau[4];
    long reftemp=0;
    long reftemp2=0;
    long RefTempImage=0;
    long spawn_lazer=0;

    tableau[0]=0;
    tableau[1]=0;
    tableau[2]=0;
    tableau[3]=0;

 //   lazer = IMG_Load("image/lazer.bmp");

    tableauimage[0]=IMG_Load("image/vaisseaux.bmp");
    tableauimage[1]=IMG_Load("image/vaisseaux2.bmp");
    tableauimage[2]=IMG_Load("image/vaisseaux3.bmp");
    tableauimage[3]=IMG_Load("image/vaisseaux4.bmp");

    tableauimage2[0]=IMG_Load("image/vaisseauxB.bmp");
    tableauimage2[1]=IMG_Load("image/vaisseauxB2.bmp");
    tableauimage2[2]=IMG_Load("image/vaisseauxB3.bmp");
    tableauimage2[3]=IMG_Load("image/vaisseauxB4.bmp");

    tableaulazer[0] = IMG_Load("image/lazer.bmp");
    tableaulazer[1] = IMG_Load("image/lazer1.bmp");
    tableaulazer[2] = IMG_Load("image/lazer2.bmp");
    tableaulazer[3] = IMG_Load("image/lazer3.bmp");
    tableaulazer[4] = IMG_Load("image/lazer4.bmp");
    tableaulazer[5] = IMG_Load("image/lazer5.bmp");
    tableaulazer[6] = IMG_Load("image/lazer6.bmp");
    tableaulazer[7] = IMG_Load("image/lazer7.bmp");
    tableaulazer[8] = IMG_Load("image/lazer8.bmp");
    tableaulazer[9] = IMG_Load("image/lazer9.bmp");


    for(i=0;i<4;i++)
    {
        SDL_SetColorKey(tableauimage[i], SDL_SRCCOLORKEY, SDL_MapRGB(tableauimage[i]->format, 255, 255, 255));
        SDL_SetColorKey(tableauimage[i], SDL_SRCCOLORKEY, SDL_MapRGB(tableauimage2[i]->format, 255, 255, 255));
        reftemp=SDL_GetTicks();
        reftemp2=SDL_GetTicks();
        RefTempImage=SDL_GetTicks();
        spawn_lazer = SDL_GetTicks();
    while (continuer==1) // tant que continuer est égale a 1 le jeu ne doit pas ce quitter
    {

        if ( SDL_PollEvent(&event))
        {// en utilisant SDL_WaitEvent(&event) le jeu se met en pause en attendant chaque evenements donc ce n'est pas interresant
            switch(event.type) // ce switch regarde le type d'evenements surveiller
            {

                case SDL_QUIT: // cet evenement sert a surveiller si le joueur veut quitter le jeu
                    continuer = 0;
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                       case SDLK_DOWN:
                        tableau[0]=1;
                        break;
                        case SDLK_UP:
                        tableau[1]=1;
                        break;
                        case SDLK_RIGHT:
                        tableau[2]=1;
                        break;
                        case SDLK_LEFT:
                        tableau[3]=1;
                        break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_DOWN:
                        tableau[0]=0;
                        break;
                        case SDLK_UP:
                        tableau[1]=0;
                        break;
                        case SDLK_RIGHT:
                        tableau[2]=0;
                        break;
                        case SDLK_LEFT:
                        tableau[3]=0;
                        break;
                    }
                    break;
            }
        }
if(SDL_GetTicks()-reftemp>5)
{
    bougervaisseau(body,&position_vaisseaux,tableau[2]-tableau[3],tableau[0]-tableau[1],SDL_GetTicks()-reftemp, forceVaisseau);
    world.Step(1.0/200.0, velocityIterations, positionIterations);
    b2Vec2 position = body->GetPosition();
    position_vaisseaux.x = position.x *10.0;
    position_vaisseaux.y = position.y *10.0;
    b2Vec2 position2 = groundBody->GetPosition();
    position_haut.x = position2.x *10.0;
    position_haut.y = position2.y *10.0;
    printf("position 2dbox:");
    printf("%f", position2.x * 10);
    printf("_");
    printf("position BORD!!!:");
    printf("%f", position_haut.y);
    printf("_");

    // gestion de la sorie de la map a droite
    if ( position.x * 10 >= 900 )
    {
        printf("*****");
      //  sortie_map = 1 ;
        position_vaisseaux2.x = position.x *10 - 1205;
        position_vaisseaux2.y = position.y *10 ;
        if ( position.x * 10 > 1467 )
        {
            printf("+++");
            body->SetTransform( b2Vec2(position_vaisseaux2.x / 10,position.y), 0 );
            position_vaisseaux2.x = 1250;
            sortie_map = 0 ;
        }
         if ( position_vaisseaux2.x  < -220)
        {
            printf("------POSITIONVAISEAUX2------:");
            printf("%f", position_vaisseaux2.x );
            position_vaisseaux2.x = 1250;
            sortie_map = 0 ;
        }
    }
    else
    {
        // position_vaisseaux2.x = 1250;
    }

    // gestion de la sorie de la map a gauche
    if ( position.x * 10 < 20  )
    {
        printf("*****");
        //sortie_map = 1 ;
        position_vaisseaux2.x = position_vaisseaux.x + 1190;
        position_vaisseaux2.y = position_vaisseaux.y;
        if ( position_vaisseaux2.x < 1023 )
        {
            printf("---");
            body->SetTransform( b2Vec2(position_vaisseaux2.x / 10,position.y), 0 );
            position_vaisseaux2.x = 1250;
            sortie_map = 0 ;
        }
        if ( position_vaisseaux.x > 20)
        {
            position_vaisseaux2.x = 1250;
            sortie_map = 0 ;
        }
    }
     else
    {
       //  position_vaisseaux2.x = 1250;
    }

    testbool = Collision(position_vaisseaux2  , position_lazer );
    testbool2 = Collision2(position_vaisseaux  , position_lazer );
    if (testbool==true)
    {
        body->SetTransform( b2Vec2(50 , 20), 0 );
        position_vaisseaux2.x = 1250 ;
    }
    if (testbool2==true)
    {
        body->SetTransform( b2Vec2(50 , 20), 0 );
        position_vaisseaux2.x = 1250 ;
    }

    // gestion apparition des lasers


    // apparition_laser(lazer , opacite);

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 61, 61, 61));
    SDL_BlitSurface(tableauimage[numeroimagevaiseau], NULL, ecran, &position_vaisseaux);
    SDL_BlitSurface(tableauimage2[numeroimagevaiseau], NULL, ecran, &position_vaisseaux2);
    SDL_BlitSurface(tableaulazer[numeroimagelazer], NULL, ecran, &position_lazer);
    SDL_Flip(ecran);
    if(SDL_GetTicks()-RefTempImage>50)
        {
        numeroimagevaiseau+=1;
        numeroimagevaiseau%=4;
        RefTempImage=SDL_GetTicks();
        }
        if(SDL_GetTicks()-reftemp2>100)
        {
        numeroimagelazer+=1;
        numeroimagelazer%=10;
        reftemp2=SDL_GetTicks();
        }
                reftemp=SDL_GetTicks();
}

}
     for(i=0;i<4;i++)
    {
        SDL_FreeSurface(tableauimage[i]);
        SDL_FreeSurface(tableauimage2[i]);
    }
    for(i=0;i<11;i++)
    {
        SDL_FreeSurface(tableaulazer[i]);
    }
    SDL_FreeSurface(lazer);
    continuer=1;
    return continuer;

}
}

 void bougervaisseau(b2Body* body ,SDL_Rect *position,int x,int y,double temps, b2Vec2 forceVaisseau )
{
   // body->ApplyLinearImpulse( b2Vec2(x*100000000000.0f,y*10000000000), body->GetWorldCenter(),1 );
    /* body->GetLinearVelocity()
    printf("")*/
body->ApplyForce( b2Vec2(((float)x)*200000000.0f*temps,((float)y)*200000000.0f*temps), body->GetWorldCenter(), 1 );
//body->ApplyForce( forceVaisseau, body->GetWorldCenter(), 1 );
    //position->x+=x*temps;
    //position->y+=y*temps;
}

bool Collision(SDL_Rect position_vaisseaux  ,SDL_Rect position_lazer)

{

   if((position_lazer.x >= position_vaisseaux.x + (position_vaisseaux.w-100))      // trop à droite

    || ( position_lazer.x + position_lazer.w <= (position_vaisseaux.x+100) // trop à gauche

    || (position_lazer.y >= position_vaisseaux.y + position_vaisseaux.h-100)) // trop en bas

    || (position_lazer.y +  position_lazer.h <= position_vaisseaux.y))  // trop en haut

          return false;

   else

          return true;

}

bool Collision2(SDL_Rect position_vaisseaux  ,SDL_Rect position_lazer)

{

   if((position_lazer.x >= position_vaisseaux.x + (position_vaisseaux.w-100))      // trop à droite

    || ( position_lazer.x + position_lazer.w <= (position_vaisseaux.x+100) // trop à gauche

    || (position_lazer.y >= position_vaisseaux.y + position_vaisseaux.h-100)) // trop en bas

    || (position_lazer.y +  position_lazer.h <= position_vaisseaux.y))  // trop en haut

          return false;

   else

          return true;

}

void apparition_laser( SDL_Surface* lazer , int opacite)
{
    SDL_SetAlpha( lazer , SDL_SRCALPHA, opacite );
}


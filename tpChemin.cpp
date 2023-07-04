#include <iostream>
#include <deque>
#include <cstdlib>
#include <cmath>

static bool destinationReached = false;


struct Point
{
    Point(int _x, int _y) :x(_x), y(_y)
    {}
    int x = 0;
    int y = 0;
    bool passable = true;
    bool person = false; //verifie si le joueur est sur cette case
    bool dest = false;
    bool passed = false;
};

//Affichage des cases d'un deque simple
void print(std::deque<Point> deq)
{
    std::deque<Point>::iterator it; //creation d'un iterateur

    for (it = deq.begin(); it != deq.end(); ++it) //Parcourt tous les elements du deque et les affiche
        //std::cout << it->x << "," << it->y << " ";
       {
            if(it->passable==false)
            {
                std::cout << "X " ;
            }

            else if(it->person==true)
            {
                std::cout << "O ";
            }
            else if(it->dest==true)
            {
                std::cout << "D ";
            }
            else if (it->passed==true)
            {
                std::cout << "- ";
            }
            else
            {
                std::cout << ". " ;
            }
       }

    //std::cout << std::endl;
}

//Affichage des coordonnes des points d'un deque simple
void printCoord(std::deque<Point> deq)
{
    std::deque<Point>::iterator it; //creation d'un iterateur

    for (it = deq.begin(); it != deq.end(); ++it) //Parcourt tous les elements du deque et les affiche
        std::cout << it->x << "," << it->y << " ";

    std::cout << std::endl;
}

struct DbDeque
{
    DbDeque(int _nbLignes, int _nbColonnes) :nbLignes(_nbLignes), nbColonnes(_nbColonnes) //constructeur du double deque
    {
        for (int i = 0; i < nbLignes; i++)
        {
            std::deque<Point> deq;
            for (int j = 0; j < nbColonnes; j++)
            {
                deq.push_back(Point(i, j));
            }
            //Ajoute nbLignes deques simples de nbColonnes colonnes
            matrix.push_back(deq);
        }

    };
    void addLigneFront()  //Ajoute une ligne au debut du deque double
    {
        std::deque<Point> deq;
        for (int i = yMin; i <= yMax; i++)
        {
            deq.push_back(Point(xMin-1, i));
        }

        matrix.push_front(deq);
        nbLignes++;
        xMin--;

    }
    void addLigneBack() //Ajoute une ligne à la fin du deque double
    {
        std::deque<Point> deq;
        for (int i = yMin; i <= yMax; i++)
        {
            deq.push_back(Point(xMax+1, i));
        }
        matrix.push_back(deq);
        nbLignes++;
        xMax++;
    }
    void addColonneFront() //Ajoute une colonne au debut du deque double
    {
        std::deque< std::deque<Point> >::iterator it2d = matrix.begin();
        int i = xMin;
        while (it2d != matrix.end())
        {
            it2d->push_front(Point(i, yMin-1));
            it2d++;
            i++;
        }
        nbColonnes++;
        yMin--;
    }
    void addColonneBack() //Ajoute une colonne à la fin du deque double
    {
        std::deque< std::deque<Point> >::iterator it2d = matrix.begin();
        int i = xMin;
        while (it2d != matrix.end())
        {
            it2d->push_back(Point(i, yMax+1));
            it2d++;
            i++;
        }
        nbColonnes++;
        yMax++;
    }
    void addPoint(int x, int y) //ajoute un point non present dans la matrice
    {
        int nbX = abs(xMax - x);
        int nbY = abs(yMax - y);
        if (x > xMax)
        {
            for (int i = 0; i < nbX; i++)
                addLigneBack();
        }
        else if (x < xMin)
        {
            for (int i = 0; i <= abs(x-xMin); i++)
                addLigneFront();
        }
        if (y > yMax)
        {
            for (int i = 0; i < nbY; i++)
                addColonneBack();
        }
        if (y < yMin)
        {
            for (int i = 0; i <= abs(y-yMin); i++)
                addColonneFront();
        }
    };



    void affichageCoord() //Affiche la totalité du deque double
    {
        std::deque< std::deque<Point> >::iterator it2d = matrix.begin(); //creation et initialisation d'un iterateur
        while (it2d != matrix.end()) //Parcourt tous les deques simples et les affiche
        {
            printCoord(*it2d);
            it2d++;
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void affichage() //Affiche la totalité du deque double
    {
        std::deque< std::deque<Point> >::iterator it2d = matrix.begin(); //creation et initialisation d'un iterateur
        while (it2d != matrix.end()) //Parcourt tous les deques simples et les affiche
        {
            print(*it2d);
            it2d++;
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }


    std::deque<std::deque<Point>> matrix;
    int nbLignes, nbColonnes;
    int x0=0, y0=0;
    int xMin = 0, yMin = 0, xMax =nbLignes-1, yMax=nbColonnes-1; //coordonnees min et max des points de la matrice
    //xmin et ymin serviront pour la conversion coordonnees reels et coordonnes dans la matrice
};

float distance(int xa, int ya, int xb, int yb)
{
    return sqrt(pow(xb-xa,2)+pow(yb-ya,2));
}



//determination de la case suivante où deplacer le personnage
Point* successor(Point* actual, Point* destination, DbDeque* dbDeque)
{
    int xD=actual->x - 1, yD=actual->y - 1;

    for(int x = actual->x-1; x<=actual->x+1; x++)
    {
        for(int y = actual->y-1; y<=actual->y+1; y++)
        {
            if(x>dbDeque->xMax || x<dbDeque->xMin || y<dbDeque-> yMin || y>dbDeque->yMax)
            {
                dbDeque->addPoint(x,y); //ajoute des lignes et colonnes si le point n'existe pas dans la matrice
            }
            if(!(x== actual->x && y == actual->y) && (dbDeque->matrix[x-dbDeque->xMin][y-dbDeque->yMin].passable==true) )
            {
                  if(x==destination->x && y==destination->y)
                  {
                        std::cout << "Arrive" << std::endl;
                        xD = destination->x; yD=destination->y;
                       //incrementation des indices pour sortir des boucles
                        destinationReached = true;
                        y+=4;
                        x+=4;
                  }
                  else if (distance(x, y, destination->x, destination->y) < distance(xD, yD, destination->x, destination->y)) //cherche le point le plus proche de la destination
                  {
                        xD = x; yD = y;
                  }
            }
        }
    }
    //pointer vers le point du dbDeque correspondant à ces coordonnées

    actual= &dbDeque->matrix[xD-dbDeque->xMin][yD-dbDeque->yMin];
    return actual;
}



int main()
{
    DbDeque dbDeque(10, 10);

    int x0 = 0, y0 = 4; //coordonnes de depart
    int xd = 8, yd = 9; //coordonnes de la destination

    std::cout << "Dimensions : " << dbDeque.nbLignes << "*" << dbDeque.nbColonnes << std::endl;

    //transforme quelques cases en non passables pour creer des obstacles
    for(int j=3; j<6; j++)
    {
        dbDeque.matrix[3-dbDeque.xMin][j-dbDeque.yMin].passable = false;
    }

    for(int i=6; i<9; i++)
    {
        dbDeque.matrix[i-dbDeque.xMin][7-dbDeque.yMin].passable = false;
    }

    //place le personnage sur sa case de depart
    dbDeque.matrix[x0-dbDeque.xMin][y0-dbDeque.yMin].person = true;

    //definit la case de destination
    dbDeque.matrix[xd-dbDeque.xMin][yd-dbDeque.yMin].dest = true;

    dbDeque.affichageCoord();

    dbDeque.affichage();

    //initialisation des points de position actuelle du personnage et de la destination
    Point* actual = &dbDeque.matrix[x0-dbDeque.xMin][y0-dbDeque.yMin];
    Point* destination = &dbDeque.matrix[xd-dbDeque.xMin][yd-dbDeque.yMin];

    //boucle tant que le personnage n'a pas atteint la destination
    while (destinationReached ==false)
    {
        actual->person=false;
        actual->passed = true;
        actual = successor(actual, destination, &dbDeque);
        actual->person=true;
        dbDeque.affichage();
    }


    return 0;


}




#include "Generator/voronoi/voronoi.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <algorithm>

#include "Generator/map/map.hpp"

#include "Generator/voronoi/parabola.hpp"
#include "Generator/voronoi/edge.hpp"

#define DRAND(min, max) (min) + ((double)std::rand() / RAND_MAX) * ((max) - (min))

namespace VOR
{

Voronoi::Voronoi() :
    _tempEdges(),
    _events(),
    _deleted(),
    _sweepLine(0),
    _root(NULL)
{
    _step = FILL;
}

Voronoi::~Voronoi()
{
}

void    Voronoi::run()
{
    generateRandomSites();
    //generateTestSites();

    std::cout << "[[[[[BEGIN FORTUNE]]]]]" << std::endl;
    fortuneAlgo();

    /*
    std::cout << "[[[[[END]]]]]" << std::endl;
    LloydRelaxation(); // en gros equilibrer les point et relancer fortune
    std::cout << "[[[[[BEGIN FORTUNE]]]]]" << std::endl;
    fortuneAlgo();*/
    std::cout << "[[[[[END]]]]]" << std::endl;



    computeFinalMap();

    std::cout << "Done" << std::endl;
}

void    Voronoi::generateRandomSites()
{
    for (unsigned int i = 0; i < _map->zoneNumber(); ++i)
    {
        MAP::Zone *zone = new MAP::Zone(DRAND(0, _map->xMax()), DRAND(0, _map->yMax()));
        _events.push(new Event(zone));
        _tempZones.push_back(zone);
    }
}

/* DEPRECATED
void    Voronoi::generateTestSites()
{
    addSite(230, 400);
    addSite(400, 300);
    addSite(50, 300);
    addSite(250, 200);
    addSite(120, 150);
    addSite(200, 100);
    addSite(30, 80);
    addSite(100, 50);
} */

void    Voronoi::fortuneAlgo()
{
    _root = NULL;
    Parabola::indexMax = 0;
    for(std::vector<Edge *>::iterator it = _tempEdges.begin(); it != _tempEdges.end(); ++it)
        delete (*it);
    _tempEdges.clear();

    while (!_events.empty())
    {
        Event *event = _events.top();
        _events.pop();

        _sweepLine = event->y;

        if(_deleted.find(event) != _deleted.end())
        {
            delete(event);
            _deleted.erase(event);
            continue;
        }

        std::cout << std::endl << "<event type : " << event->type << "; y = " << event->y << ">" << std::endl;
        if (event->type == Event::POINT)
            addParabola(event->site);
        else
            removeParabola(event);
    }

    finishEdge(_root);
    for(std::vector<Edge *>::iterator it = _tempEdges.begin(); it != _tempEdges.end(); ++it)
    {
        if( (*it)->neighbour)
        {
            (*it)->start = (*it)->neighbour->end;
            delete (*it)->neighbour;
        }
    }
}

void    Voronoi::LloydRelaxation() // NOT WORKING FOR NOW
{
    // filling neighbors temporarly.. ugly for now. will have to remove totally Zone from this algorythme
    for (const auto &e: _tempEdges)
    {
        if (!e->left->haveNeighbor(e->right))
            e->left->neighbors.push_back(e->right);
        if (!e->right->haveNeighbor(e->left))
            e->right->neighbors.push_back(e->left);
    }

    for (const auto &z: _tempZones)
    {
        z->point.x = 0;
        z->point.y = 0;
        for (const auto &n: z->neighbors)
        {
            z->point.x += n->point.x;
            z->point.y += n->point.y;
        }
        z->point.x /= z->neighbors.size();
        z->point.y /= z->neighbors.size();
        z->neighbors.clear();
        _events.push(new Event(z));
    }
}

void	Voronoi::finishEdge(Parabola * p)
{
    if(p->isLeaf) {delete p; return;}

    double mx;
    if(p->edge->direction.x > 0.0)
        mx = std::max(_map->xMax(), p->edge->start.x + 10 );
    else
        mx = std::min(0.0, p->edge->start.x - 10);


    p->edge->end.x = mx;
    p->edge->end.y = mx * p->edge->f + p->edge->g;


    finishEdge(p->left());
    finishEdge(p->right());
    delete p;
}

void        Voronoi::computeFinalMap()
{
    for (const auto &e: _tempEdges)
    {
        MAP::CrossedEdge    *edge = new MAP::CrossedEdge();
        MAP::Corner *c0;
        MAP::Corner *c1;

        // Ugly workaround
        if (!_map->zones().count(e->left->index))
            _map->zones().insert(std::pair<int, MAP::Zone *>(e->left->index, e->left));
        if (!_map->zones().count(e->right->index))
            _map->zones().insert(std::pair<int, MAP::Zone *>(e->right->index, e->right));

        if (!e->left->haveNeighbor(e->right))
            e->left->neighbors.push_back(e->right);
        if (!e->right->haveNeighbor(e->left))
            e->right->neighbors.push_back(e->left);

        edge->z0 = e->left;
        edge->z1 = e->right;
        e->left->borders.push_back(edge);
        e->right->borders.push_back(edge);

        if (!(c0 = checkCorner(e->left, e->start)))
        {
            if (!(c0 = checkCorner(e->right, e->start)))
            {
                c0 = new MAP::Corner();
                _map->corners().insert(std::pair<int, MAP::Corner *>(c0->index, c0));

                c0->point.x = e->start.x;
                c0->point.y = e->start.y;
                e->right->corners.push_back(c0);
                c0->faces.push_back(e->right);
            }
            e->left->corners.push_back(c0);
            c0->faces.push_back(e->left);
        }

        if (!(c1 = checkCorner(e->left, e->end)))
        {
            if (!(c1 = checkCorner(e->right, e->end)))
            {
                c1 = new MAP::Corner();
                _map->corners().insert(std::pair<int, MAP::Corner *>(c1->index, c1));

                c1->point.x = e->end.x;
                c1->point.y = e->end.y;
                e->right->corners.push_back(c1);
                c1->faces.push_back(e->right);
            }
            e->left->corners.push_back(c1);
            c1->faces.push_back(e->left);
        }


        edge->c0 = c0;
        edge->c1 = c1;
        c0->edges.push_back(edge);
        c1->edges.push_back(edge);
        c0->adjacent.push_back(c1);
        c1->adjacent.push_back(c0);

        _map->edges().insert(std::pair<int, MAP::CrossedEdge *>(edge->index, edge));
    }
}

MAP::Corner *Voronoi::checkCorner(MAP::Zone *z, Point &p)
{
    for (std::vector<MAP::Corner *>::iterator it = z->corners.begin(); it != z->corners.end(); ++it)
        if ((*it)->point.x == p.x && (*it)->point.y == p.y)
            return (*it);
    return NULL;
}


void    Voronoi::addParabola(MAP::Zone *site)
{
    std::cout << "<===== addParabola(" << *site << ") =====>" << std::endl << std::endl;
    if (!_root) { _root = new Parabola(site); return; }

    // Nop
    /*if (_root->isLeaf && _root->site->point.y - site->point.y < 1)
    {
        _root->isLeaf = false;
        _root->setLeft(new Parabola(_root->site));
        _root->setRight(new Parabola(site));
        Point   s((site->point.x + _root->site->point.x) / 2, _yMax);

        if (site->point.x  > _root->site->point.x)
            _root->edge = new Edge(s, _root->site, site);
        else
            _root->edge = new Edge(s, site, _root->site);

        _tempEdges.push_back(_root->edge);

        return;
    }*/

    Parabola    *topParabola = getParabolaAtX(site->point.x); // get first parabola above the new site
    if (topParabola->cEvent) // Event annulé car bouffé par une nouvelle parabole
    {
        _deleted.insert(topParabola->cEvent);
        topParabola->cEvent = NULL;
    }

    Point   start(0, 0);
    start.x = site->point.x;
    start.y = getY(topParabola->site->point, site->point.x);

    Edge    *el = new Edge(start, topParabola->site, site);
    Edge    *er = new Edge(start, site, topParabola->site);
    el->neighbour = er;
    _tempEdges.push_back(el);
    topParabola->edge = er;
    topParabola->isLeaf = false;

    Parabola    *p0 = new Parabola(topParabola->site); // Gauche
    Parabola    *p1 = new Parabola(site);              // Nouvelle, celle qui coupe
    Parabola    *p2 = new Parabola(topParabola->site); // Droite

    topParabola->setRight(p2);
    topParabola->setLeft(new Parabola());
    topParabola->left()->edge = el;
    topParabola->left()->setLeft(p0);
    topParabola->left()->setRight(p1);

    checkCircle(p0);
    checkCircle(p2);

}

void    Voronoi::removeParabola(Event *e)
{
    std::cout << "<===== removeParabola(" << *e->arch << ") =====>" << std::endl << std::endl;
    Parabola    *p1 = e->arch;

    Parabola    *pLeft = Parabola::getLeftParent(p1);
    Parabola    *pRight = Parabola::getRightParent(p1);
    Parabola    *p0 = Parabola::getLeftChild(pLeft);
    Parabola    *p2 = Parabola::getRightChild(pRight);

    if (p0 == p2)
        std::cout << "error : same focal point\n";

    if (p0->cEvent)
    {
        _deleted.insert(p0->cEvent);
        p0->cEvent = NULL;
    }
    if (p2->cEvent)
    {
        _deleted.insert(p2->cEvent);
        p2->cEvent = NULL;
    }

    Point   p(0, 0);
    p.x = e->intersect.x;
    p.y = getY(p1->site->point, e->intersect.x); //

    pLeft->edge->end.x = p.x;
    pLeft->edge->end.y = p.y;
    pRight->edge->end.x = p.x;
    pRight->edge->end.y = p.y;

    std::cout << "pLeft : " << *pLeft << "pRight" << *pRight << std::endl;

    Parabola * higher;
    Parabola * par = p1;
    while(par != _root)
    {
        par = par->parent;
        std::cout << *par << std::endl;
        if(par == pLeft) higher = pLeft;
        if(par == pRight) higher = pRight;
    }

    higher->edge = new Edge(p, p0->site, p2->site);
    _tempEdges.push_back(higher->edge);

    Parabola * gparent = p1->parent->parent;
    if(p1->parent->left() == p1)
    {
        if(gparent->left()  == p1->parent) gparent->setLeft ( p1->parent->right() );
        if(gparent->right() == p1->parent) gparent->setRight( p1->parent->right() );
    }
    else
    {
        if(gparent->left()  == p1->parent) gparent->setLeft ( p1->parent->left()  );
        if(gparent->right() == p1->parent) gparent->setRight( p1->parent->left()  );
    }

    delete p1->parent;
    delete p1;

    std::cout << " gp final : " << *gparent << " gp left : " << *gparent->left() << " gp right : " << *gparent->right() << std::endl;
    checkCircle(p0);
    checkCircle(p2);
}

double      Voronoi::getXofEdge(Parabola *p, double y)
{
    const Point &sLeft = Parabola::getLeftChild(p)->site->point;
    const Point &sRight = Parabola::getRightChild(p)->site->point;

    std::cout << "<- getXofEdge(" << *p << ", " << y << ") ->" << std::endl;
    std::cout << "fils gauche : " << *Parabola::getLeftChild(p) << "; fils droite : " << *Parabola::getRightChild(p) << std::endl;
    std::cout << "site fils gauche : " << sLeft << "; site fils droite : " << sRight << std::endl;

    // justifier
    double dp;

    // 1 = parabole gauche ; 2 = parabole droite ; vide = intersection
    // ax² + bx + c
    double a, a1, a2, b, b1, b2, c, c1, c2;

    // b² - 4ac, resolution d'equation du second defrée
    double   delta, x1, x2;

    double result;

    // REVERIFIER
    dp = 2.0 * (sLeft.y - y);
    a1 = 1.0 / dp;
    b1 = -2.0 * sLeft.x / dp;
    c1 = y + dp / 4 + sLeft.x * sLeft.x / dp;

    dp = 2.0 * (sRight.y - y);
    a2 = 1.0 / dp;
    b2 = -2.0 * sRight.x / dp;
    c2 = _sweepLine + dp / 4 + sRight.x * sRight.x / dp; // why _sweepline ?

    a = a1 - a2;
    b = b1 - b2;
    c = c1 - c2;

    delta = b*b - 4 * a * c;
    x1 = (-b + sqrt(delta)) / (2*a);
    x2 = (-b - sqrt(delta)) / (2*a);

    if(sLeft.y < sRight.y)
        result = std::max(x1, x2);
    else
        result = std::min(x1, x2);

    std::cout << "x1 : " << x1 << "; x2 : " << x2 << "; result : " << result << std::endl;
    return result;
}

Parabola    *Voronoi::getParabolaAtX(double nx)
{
    std::cout << "<--- getParabolaAtX(" << nx << ") --->" << std::endl;
    Parabola *p = _root;
    double x = 0.0;

    while(!p->isLeaf)
    {
        x = getXofEdge(p, _sweepLine);
        if (x > nx)
            p = p->left();
        else
            p = p->right();
    }
    std::cout << "parabola founded = " << *p << std::endl;
    return p;
}

double      Voronoi::getY(const Point &s, double x)
{
    // Formule d'intersection, à reviser
    double dp = 2 * (s.y - _sweepLine);
    double a1 = 1 / dp;
    double b1 = -2 * s.x / dp;
    double c1 = _sweepLine + dp / 4 + s.x * s.x / dp;

    double result = a1*x*x + b1*x + c1;

    std::cout << "<- getY(" << s << ", " << x << ") : " << result << " ->" << std::endl;
    return(result);
}

void        Voronoi::checkCircle(Parabola *b)
{
    std::cout << std::endl << "<--- checkCircle(" << *b << ") --->" << std::endl;
    Parabola    *leftParent = Parabola::getLeftParent(b);
    Parabola    *rightParent = Parabola::getRightParent(b);

    Parabola    *a = Parabola::getLeftChild(leftParent);
    Parabola    *c = Parabola::getRightChild(rightParent);

    std::cout << "leftParent : ";
    if (leftParent) std::cout << *leftParent;
    else std::cout << "Nan";
    std::cout << "; rightParent : ";
    if (rightParent) std::cout << *rightParent;
    else std::cout << "Nan";
    std::cout << std::endl << "leftChild site : ";
    if (a) std::cout << *a->site;
    else std::cout << "Nan";
    std::cout << "; rightChild site : ";
    if (c) std::cout << *c->site;
    else std::cout << "Nan";
    std::cout << std::endl;

    if (!a || !c || a->site == c->site)
        return;

    Point s(0, 0);
    if (!getEdgeIntersection(leftParent->edge, rightParent->edge, s))
        return;

    // Trouver la future distance de la sweepline quand intersection
    double dx = a->site->point.x - s.x;
    double dy = a->site->point.y - s.y;
    double d = sqrt( (dx * dx) + (dy * dy) );
    if(s.y - d >= _sweepLine)
        return;
    s.y = s.y - d;

    Event *e = new Event(b, s);

    b->cEvent = e;

    std::cout << "Added a circle event at x = " << e->intersect.x << " y = " << e->y << std::endl;
    _events.push(e);
}

bool Voronoi::getEdgeIntersection(Edge *a, Edge *b, Point &result)
{
    std::cout << "<- getEdgeIntersection ->" << std::endl;
    std::cout << *a << std::endl << *b << std::endl;

    double x = (b->g - a->g) / (a->f - b->f);
    double y = a->f * x + a->g;

    std::cout << "result : " << x << "; " << y << std::endl;

    if((x - a->start.x)/a->direction.x < 0) return false;
    if((y - a->start.y)/a->direction.y < 0) return false;
    if((x - b->start.x)/b->direction.x < 0) return false;
    if((y - b->start.y)/b->direction.y < 0) return false;

    std::cout << "Validated !" << std::endl;
    result.x = x;
    result.y = y;
    return true;
}

}

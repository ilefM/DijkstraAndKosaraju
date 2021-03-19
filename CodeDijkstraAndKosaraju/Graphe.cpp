/**
 * \file Graphe.cpp
 * \brief Implémentation d'un graphe orienté.
 * \author IFT-2008 & ?
 * \version 0.1
 * \date mars 2021
 *
 *  Travail pratique numéro 2
 *
 */

#include "Graphe.h"
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{
    using namespace std;
	//Mettez l'implémentation de vos méthodes ici.

    Graphe::Graphe(size_t nbSommets)
    {
        this->nbSommets = nbSommets;
        nbArcs = 0;
        noms = vector<string>(nbSommets);
        listesAdj = vector<list<Arc>>(nbSommets);
    }

    Graphe::~Graphe()
    {
        //Ce sont des conteneurs de la stl ils ont leur propre destructeur
    }

    void Graphe::resize(size_t nouvelleTaille)
    {
        nbSommets = nouvelleTaille;
        nbArcs = 0;
        noms = vector<string>(nbSommets);
        listesAdj = vector<list<Arc>>(nbSommets);
    }

    void Graphe::nommer(size_t sommet, const std::string& nom)
    {
        if(sommet > nbSommets) throw logic_error("Le sommet entré est supérieur au nombre de sommets");
        noms[sommet] = nom;
    }

    void Graphe::ajouterArc(size_t source, size_t destination, float duree, float cout)
    {
        if(source > nbSommets || destination > nbSommets) throw logic_error("La source ou la destination est supérieur au nombre de sommets");

        list<Arc> arcsSource = listesAdj[source];

        for(list<Arc>::iterator i = arcsSource.begin(); i != arcsSource.end(); i++)
            if(i->destination == destination) throw logic_error("L'arc existe déjà");

        listesAdj[source].push_back(Arc(destination, Ponderations(duree, cout)));
        nbArcs++;
    }

    void Graphe::enleverArc(size_t source, size_t destination)
    {
        if(source > nbSommets || destination > nbSommets) throw logic_error("La source ou la destination est supérieur au nombre de sommets");

        list<Arc> arcsSource = listesAdj[source];

        for(list<Arc>::iterator i = arcsSource.begin(); i != arcsSource.end(); i++)
        {
            if(i->destination == destination)
            {
                listesAdj[source].erase(i);
                nbArcs--;
                return;
            }
        }

        throw logic_error("peut pas retirer un arc qui n'existe pas");


    }

    bool Graphe::arcExiste(size_t source, size_t destination) const
    {
        if(source > nbSommets || destination > nbSommets) throw logic_error("La source ou la destination est supérieur au nombre de sommets");

        list<Arc> arcsSource = listesAdj[source];
        for(list<Arc>::iterator i = arcsSource.begin(); i != arcsSource.end(); i++)
            if(i->destination == destination) return true;

        return false;
    }

    std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet) const
    {
        if(sommet > nbSommets) throw logic_error("Le sommet entré est supérieur au nombre de sommets");

        list<Arc> arcsSommet = listesAdj[sommet];
        vector<size_t> sommetsAdjacents;
        for(list<Arc>::iterator i = arcsSommet.begin(); i != arcsSommet.end(); i++)
            sommetsAdjacents.push_back(i->destination);

        return sommetsAdjacents;
    }

    std::string Graphe::getNomSommet(size_t sommet) const
    {
        return noms[sommet];
    }

    size_t Graphe::getNumeroSommet(const string& nom) const
    {
        size_t i = 0;
        for(string n : noms)
        {
            if(nom == noms[i]) return i;
            i++;
        }

        throw logic_error("Le nom n'existe pas dans le graphe");
    }


    int Graphe::getNombreSommets() const
    {
        return noms.capacity();
    }

    int Graphe::getNombreArcs() const
    {
        return nbArcs;
    }

    Ponderations Graphe::getPonderationsArc(size_t source, size_t destination) const
    {
        if(source > nbSommets || destination > nbSommets) throw logic_error("La source ou la destination est supérieur au nombre de sommets");

        list<Arc> arcsSource = listesAdj[source];

        for(list<Arc>::iterator i = arcsSource.begin(); i != arcsSource.end(); i++)
        {
            if(i->destination == destination)
            {
                return i->poids;
            }
        }

        throw logic_error("peut pas retirer un arc qui n'existe pas");
    }

    /*
    vector<string> Graphe::getNomsSommets() const
    {
        return noms;
    }
     */
}//Fin du namespace

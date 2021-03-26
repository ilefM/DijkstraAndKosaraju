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
	//Mettez l'implémentation de vos méthodes ici.

    /**
     * \fn	Graph::Graph(size_t nbSommets)
     * \brief constructeur avec parametres. initialisation de l'attribut nbSommets parametre nbSommets.
     * \param[in] nbSommets (nombre de sommets)
     */
    Graphe::Graphe(size_t nbSommets)
    {
        this->nbSommets = nbSommets;
        nbArcs = 0;
        noms = std::vector<std::string>(nbSommets);
        listesAdj = std::vector<std::list<Arc>>(nbSommets);
    }

    /**
     * \fn	Graph::~Graph()
     * \brief destructeur de l'objet. Ici on ne fait rien car les attribut sont des conteneurs de la STL (noms et listesAdj) qui ont leur propre destructeur
     */
    Graphe::~Graphe() = default;


    /**
     * \fn	Graph::resize(size_t nouvelleTaille)
     * \brief Change la taille du graph en changant le nombre de sommets
     * \param[in] nouvelleTaille (nombre de sommets)
     */
    void Graphe::resize(size_t nouvelleTaille)
    {
        nbSommets = nouvelleTaille;
        nbArcs = 0;
        noms = std::vector<std::string>(nbSommets);
        listesAdj = std::vector<std::list<Arc>>(nbSommets);
    }

    /**
     * \fn	Graph::nommer(size_t sommet, const std::string& nom)
     * \brief Change un nom du sommet
     * \param[in] sommet, nom
     * \exception Le sommet entré est supérieur au nombre de sommets
     */
    void Graphe::nommer(size_t sommet, const std::string& nom)
    {
        if(sommet > nbSommets) throw std::logic_error("Le sommet entré est supérieur au nombre de sommets");
        noms[sommet] = nom;
    }

    /**
     * \fn	Graph::ajouterArc(size_t source, size_t destination, float duree, float cout)
     * \brief Ajout un arc dans le graph entre deux sommets (source et destination)
     * \param[in] source, destination, duree, cout
     * \exception La source ou la destination est supérieur au nombre de sommets
     * \exception L'arc existe déjà
     */
    void Graphe::ajouterArc(size_t source, size_t destination, float duree, float cout)
    {
        if(source > nbSommets || destination > nbSommets) throw std::logic_error("La source ou la destination est supérieur au nombre de sommets");

        std::list<Arc> arcsSource = listesAdj[source];

        for(std::list<Arc>::iterator i = arcsSource.begin(); i != arcsSource.end(); i++)
            if(i->destination == destination) throw std::logic_error("L'arc existe déjà");

        listesAdj[source].push_back(Arc(destination, Ponderations(duree, cout)));
        nbArcs++;
    }

    /**
     * \fn	Graph::enleverArc(size_t source, size_t destination)
     * \brief Supprime un arc dans le graph entre deux sommets (source et destination)
     * \param[in] source, destination
     * \exception La source ou la destination est supérieur au nombre de sommets
     * \exception Peut pas retirer un arc qui n'existe pas
     */
    void Graphe::enleverArc(size_t source, size_t destination)
    {
        if(source > nbSommets || destination > nbSommets) throw std::logic_error("La source ou la destination est supérieur au nombre de sommets");

        std::list<Arc> arcsSource = listesAdj[source];

        for(std::list<Arc>::iterator i = arcsSource.begin(); i != arcsSource.end(); i++)
        {
            if(i->destination == destination)
            {
                listesAdj[source].erase(i);
                nbArcs--;
                return;
            }
        }
        throw std::logic_error("Peut pas retirer un arc qui n'existe pas");
    }

    /**
     * \fn	Graph::arcExiste(size_t source, size_t destination) const
     * \brief Verifie si un arc existe dans le graph entre deux sommets (source et destination)
     * \param[in] source, destination
     * \return vrai si l'arc existe, faux dans le cas contraire
     * \exception La source ou la destination est supérieur au nombre de sommets
     */
    bool Graphe::arcExiste(size_t source, size_t destination) const
    {
        if(source > nbSommets || destination > nbSommets) throw std::logic_error("La source ou la destination est supérieur au nombre de sommets");

        std::list<Arc> arcsSource = listesAdj[source];
        for(std::list<Arc>::iterator i = arcsSource.begin(); i != arcsSource.end(); i++)
            if(i->destination == destination) return true;

        return false;
    }

    /**
     * \fn	Graph::listerSommetsAdjacents(size_t sommet) const
     * \brief Retourne la liste d'adjacence d'un sommet (sommets connectes au sommet en parametre)
     * \param[in] sommet
     * \return vector<size_t> (liste d'adjacence)
     * \exception Le sommet entré est supérieur au nombre de sommets
     */
    std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet) const
    {
        if(sommet > nbSommets) throw std::logic_error("Le sommet entré est supérieur au nombre de sommets");

        std::list<Arc> arcsSommet = listesAdj[sommet];
        std::vector<size_t> sommetsAdjacents;
        for(std::list<Arc>::iterator i = arcsSommet.begin(); i != arcsSommet.end(); i++)
            sommetsAdjacents.push_back(i->destination);

        return sommetsAdjacents;
    }

    /**
     * \fn	Graph::getNomSommet(size_t sommet) const
     * \brief Avoir le nom d'un sommet a partir de son numero
     * \param[in] sommet
     * \return string (nom d'un sommet)
     * \exception Le sommet est supérieur au nombre de sommets
     */
    std::string Graphe::getNomSommet(size_t sommet) const
    {
        if(sommet > nbSommets) std::logic_error("Le sommet est supérieur au nombre de sommets");
        return noms[sommet];
    }

    /**
     * \fn	Graph::getNumeroSommet(const std::string& nom) const
     * \brief Avoir le numero d'un sommet partir de son nom
     * \param[in] nom
     * \return size_t (numero d'un sommet)
     * \exception Le numero n'existe pas dans le graphe
     */
    size_t Graphe::getNumeroSommet(const std::string& nom) const
    {
        size_t i = 0;
        for(std::string n : noms)
        {
            if(nom == noms[i]) return i;
            i++;
        }
        throw std::logic_error("Le numero n'existe pas dans le graphe");
    }

    /**
     * \fn	Graph::getNombreSommets() const
     * \brief Avoir le nombre du sommet du graph
     * \return int (nombre de sommets du graph)
     */
    int Graphe::getNombreSommets() const
    {
        return noms.capacity();
    }

    /**
    * \fn	Graph::getNombreArcs() const
    * \brief Avoir le nombre d'arcs du graph
    * \return int (nombre d'arcs du graph)
    */
    int Graphe::getNombreArcs() const
    {
        return nbArcs;
    }

    /**
    * \fn	Graph::getPonderationsArc(size_t source, size_t destination)
    * \brief Avoir la ponderation(duree et cout) d'un trajet entre la ville source et la ville destination
    * \param source, destination
    * \return ponderation de l'arc
    * \exception La source ou la destination est supérieur au nombre de sommets.
     *\exception Peut pas avoir la ponderation d'un arc qui n'existe pas
    */
    Ponderations Graphe::getPonderationsArc(size_t source, size_t destination) const
    {
        if(source > nbSommets || destination > nbSommets) throw std::logic_error("La source ou la destination est supérieur au nombre de sommets");

        for(Arc arc : listesAdj[source])
        {
            if(arc.destination == destination)
            {
                return arc.poids;
            }
        }
        throw std::logic_error("Peut pas avoir la ponderation d'un arc qui n'existe pas");
    }
}//Fin du namespace

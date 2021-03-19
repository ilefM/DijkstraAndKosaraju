/**
 * \file ReseauInterurbain.cpp
 * \brief Implémentattion de la classe ReseauInterurbain.
 * \author IFT-2008 & ?
 * \version 0.1
 * \date mars 2021
 *
 *  Travail pratique numéro 2
 *
 */
#include <sstream>
#include <fstream>
#include "ReseauInterurbain.h"
#include <limits>
#include <algorithm>
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{
    using namespace std;

    ReseauInterurbain::ReseauInterurbain(std::string nomReseau, size_t nbVilles)
    {
        unReseau = Graphe(nbVilles);
        this->nomReseau = nomReseau;
    }

    ReseauInterurbain::~ReseauInterurbain()
    {
    }

    void ReseauInterurbain::resize(size_t nouvelleTaille)
    {
        unReseau.resize(nouvelleTaille);
    }


    // Méthode fournie
    void ReseauInterurbain::chargerReseau(std::ifstream & fichierEntree)
    {
        if (!fichierEntree.is_open())
            throw std::logic_error("ReseauInterurbain::chargerReseau: Le fichier n'est pas ouvert !");

        std::string buff;

        getline(fichierEntree, nomReseau);
        nomReseau.erase(0, 20); // Enlève: Reseau Interurbain:

        int nbVilles;

        fichierEntree >> nbVilles;
        getline(fichierEntree, buff); //villes

        unReseau.resize(nbVilles);

        getline(fichierEntree, buff); //Liste des villes

        size_t i = 0;

        getline(fichierEntree, buff); //Premiere ville

        while(buff != "Liste des trajets:")
        {
            unReseau.nommer(i, buff);
            getline(fichierEntree, buff);
            i++;
        }

        while(!fichierEntree.eof())
        {
            getline(fichierEntree, buff);
            std::string source = buff;
            getline(fichierEntree, buff);
            std::string destination = buff;

            getline(fichierEntree, buff);
            std::istringstream iss(buff);

            float duree;
            iss >> duree;

            float cout;
            iss >> cout;

            unReseau.ajouterArc(unReseau.getNumeroSommet(source), unReseau.getNumeroSommet(destination), duree, cout);
        }
    }

    Chemin ReseauInterurbain::rechercheCheminDijkstra(const std::string &origine, const std::string &destination, bool dureeCout) const
    {
        if(!verifieAbsence(origine) || !verifieAbsence(destination)) throw logic_error("si origine et/ou destination absent du réseau");

        //Initialisation
        int nbSommets = unReseau.getNombreSommets();

        vector<double> poidsCourt = vector<double>(nbSommets);
        for(int p = 0; p < nbSommets; p++) poidsCourt[p] = numeric_limits<double>::max();

        vector<size_t> villesRestantes = vector<size_t>(nbSommets);
        for(int v = 0; v < nbSommets; v++) villesRestantes[v] = v;

        vector<size_t> parents = vector<size_t>(nbSommets);


        poidsCourt[unReseau.getNumeroSommet(origine)] = 0;

        //Commencer l'algorithme
        while(!villesRestantes.empty())
        {
            int sourceIndex = minValueIndex(poidsCourt);

            vector<size_t>::iterator it = villesRestantes.begin() + sourceIndex;
            villesRestantes.erase(it);

            vector<size_t> villesVoisines = unReseau.listerSommetsAdjacents(sourceIndex);

            for(int i = 0; i < villesVoisines.size(); i++)
            {
                size_t destIndex = villesVoisines[i];
                Ponderations ponderation = unReseau.getPonderationsArc(sourceIndex, destIndex);
                if(dureeCout)
                {
                    double tempsTrajet = ponderation.duree;
                    double sommeDureeTrajet = tempsTrajet + poidsCourt[sourceIndex];
                    if(sommeDureeTrajet < poidsCourt[destIndex])
                    {
                        poidsCourt[destIndex] = sommeDureeTrajet;
                        parents[destIndex] = sourceIndex;
                    }
                }
                else
                {
                    double coutTrajet = ponderation.cout;
                    double sommeCoutTrajet = coutTrajet + poidsCourt[sourceIndex];
                    if(sommeCoutTrajet < poidsCourt[destIndex])
                    {
                        poidsCourt[destIndex] = sommeCoutTrajet;
                        parents[destIndex] = sourceIndex;
                    }
                }
            }

        }

        //Creation du chemin
        int courantIndex = unReseau.getNumeroSommet(destination);
        int stopIndex = unReseau.getNumeroSommet(origine);
        Chemin chemin = Chemin();
        do
        {
            chemin.listeVilles.push_back(unReseau.getNomSommet(courantIndex));
            courantIndex = parents[courantIndex];
        }
        while(courantIndex != stopIndex);
        chemin.listeVilles.push_back(unReseau.getNomSommet(courantIndex));
        if(dureeCout) chemin.dureeTotale = poidsCourt[courantIndex];
        return chemin;
    }

    std::vector<std::vector<std::string>> ReseauInterurbain::algorithmeKosaraju()
    {
        return std::vector<std::vector<std::string>>();
    }

    int ReseauInterurbain::minValueIndex(const vector<double> poidsCourt) const
    {
        int minValue = poidsCourt[0];
        int indexMin = 0;
        for(int i = 1; i < poidsCourt.size(); i++ )
        {
            if(poidsCourt[i] < minValue)
            {
                minValue = poidsCourt[i];
                indexMin = i;
            }
        }
        return indexMin;
    }

    bool ReseauInterurbain::verifieAbsence(const string &ville) const
    {
        bool existe = false;
        for(int i = 0; i < unReseau.getNombreSommets(); i++)
        {
            if(ville == unReseau.getNomSommet(i))
            {
                existe = true;
            }
        }
        return existe;
    }

}//Fin du namespace

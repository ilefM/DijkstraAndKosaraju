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
//#include <fstream>
#include "ReseauInterurbain.h"
#include <limits>
#include <algorithm>
#include <stack>
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{
    using namespace std;

    ReseauInterurbain::ReseauInterurbain(std::string nomReseau, size_t nbVilles)
    {
        unReseau = Graphe(nbVilles);
        this->nomReseau = nomReseau;
    }

    ReseauInterurbain::~ReseauInterurbain() = default;//Ce sont des conteneurs de la stl ils ont leur propre destructeur

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
        vector<double> poidsCourt(unReseau.getNombreSommets(), numeric_limits<double>::max());
        vector<size_t> villesRestantes(unReseau.getNombreSommets());
        for(int v = 0; v < unReseau.getNombreSommets(); v++) villesRestantes[v] = v;

        vector<size_t> parents(unReseau.getNombreSommets(), numeric_limits<size_t>::max());

        poidsCourt[unReseau.getNumeroSommet(origine)] = 0;

        //Commencer l'algorithme
        while(!villesRestantes.empty())
        {
            size_t sourceIndex = minValueIndex(poidsCourt, villesRestantes);
            if(sourceIndex == numeric_limits<int>::max()) break;

            villesRestantes.erase(remove(villesRestantes.begin(), villesRestantes.end(), sourceIndex), villesRestantes.end());

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
        chemin.reussi = true;
        vector<string> cheminInverse = vector<string>();

        int maxIterations = unReseau.getNombreSommets();
        do
        {
            if(maxIterations == 0 || courantIndex == numeric_limits<size_t>::max())
            {
                chemin.reussi = false;
                return chemin;
            }
            cheminInverse.push_back(unReseau.getNomSommet(courantIndex));
            courantIndex = parents[courantIndex];
            maxIterations--;
        }while(courantIndex != stopIndex);
        cheminInverse.push_back(unReseau.getNomSommet(courantIndex));

        for(int i = cheminInverse.size() - 1; i >= 0; i--)chemin.listeVilles.push_back(cheminInverse[i]);

        if(dureeCout) chemin.dureeTotale = poidsCourt[unReseau.getNumeroSommet(destination)];
        else chemin.coutTotal = poidsCourt[unReseau.getNumeroSommet(destination)];
        return chemin;
    }

    std::vector<std::vector<std::string>> ReseauInterurbain::algorithmeKosaraju()
    {
        //Partie 1 DFS
        vector<size_t> villesDFS1 = parcoursProfondeur();

        //Partie 2 Inverser le graph
        vector<vector<size_t>> graphInverse = inverserGraph();

        //Partie 3 DFS avec listeVilles
        vector<bool> dejaVisite(unReseau.getNombreSommets(), false);
        vector<vector<string>> cFC;

        for(int i = villesDFS1.size() - 1; i >= 0; i --)
        {
            if(!dejaVisite[i])
            {
                auto ville = villesDFS1[i];
                //villesDFS1.pop();
                stack<size_t> pileSommets;
                pileSommets.push(ville);
                dejaVisite[ville] = true;
                vector<string> composante;

                while (!pileSommets.empty())
                {
                    auto villeATraiter = pileSommets.top();
                    pileSommets.pop();
                    //villesDFS1.pop();
                    composante.push_back(unReseau.getNomSommet(villeATraiter));

                    for(size_t villeVoisine : graphInverse[villeATraiter])
                    {
                        if(!dejaVisite[villeVoisine])
                        {
                            dejaVisite[villeVoisine] = true;
                            pileSommets.push(villeVoisine);
                        }
                    }
                }
                cFC.push_back(composante);
            }
        }
        return cFC;
    }

    int ReseauInterurbain::minValueIndex(const vector<double> & poidsCourt, const vector<size_t> & villesRestantes) const
    {
        vector<double> poidRestant = vector<double>(villesRestantes.size());
        for(int i = 0; i < poidRestant.size(); i++)
        {
            int nSommet = villesRestantes[i];
            poidRestant[i] = poidsCourt[nSommet];
        }
        double minValue = *min_element(poidRestant.begin(), poidRestant.end());
        if(minValue == numeric_limits<double>::max()) return numeric_limits<int>::max();
        for(int i = 0; i < poidsCourt.size(); i++)
        {
            if(minValue == poidsCourt[i])
            {
                return i;
            }
        }
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

    vector<size_t> ReseauInterurbain::parcoursProfondeur()
    {
        vector<bool> dejaVisite(unReseau.getNombreSommets(), false);
        vector<size_t> sommetsParcourus;

        for(int i = 0; i < unReseau.getNombreSommets(); i++)
        {
            if(!dejaVisite[i])
            {
                stack<size_t> pileSommets;
                pileSommets.push(i);
                dejaVisite[i] = true;

                vector<size_t> parcours;

                while(!pileSommets.empty())
                {
                    auto sommetATraiter = pileSommets.top();
                    pileSommets.pop();
                    //parcours.push_back(sommetATraite);
                    sommetsParcourus.push_back(sommetATraiter);

                    for(size_t villeVoisine : unReseau.listerSommetsAdjacents(sommetATraiter))
                    {
                        if(!dejaVisite[villeVoisine])
                        {
                            dejaVisite[villeVoisine] = true;
                            pileSommets.push(villeVoisine);
                        }
                    }
                }
            }
        }
        return sommetsParcourus;
    }


    std::vector<std::vector<size_t>> ReseauInterurbain::inverserGraph()
    {
        vector<vector<std::size_t>> graphInverse(unReseau.getNombreSommets());
        for(int i = 0; i < unReseau.getNombreSommets(); i++)
            for(size_t villeVoisine : unReseau.listerSommetsAdjacents(i))
                graphInverse[villeVoisine].push_back(i);
        return graphInverse;
    }

    std::vector<size_t> ReseauInterurbain::parcoursProfondeurrrr(size_t debut)
    {
        vector<bool> dejaVisite(unReseau.getNombreSommets(), false);
        stack<size_t> pileSommets;
        pileSommets.push(debut);
        dejaVisite[debut] = true;

        vector<size_t> parcours;

        while(!pileSommets.empty())
        {
            auto sommetATraite = pileSommets.top();
            pileSommets.pop();
            parcours.push_back(sommetATraite);

            for(size_t villeVoisine : unReseau.listerSommetsAdjacents(sommetATraite))
            {
                if(!dejaVisite[villeVoisine])
                {
                    dejaVisite[villeVoisine] = true;
                    pileSommets.push(villeVoisine);
                }
            }
        }
        return parcours;
    }

}//Fin du namespace
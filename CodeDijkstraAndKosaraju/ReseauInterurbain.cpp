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

    /**
     * \fn	ReseauInterurbain::ReseauInterurbain(std::string nomReseau, size_t nbVilles)
     * \brief constructeur avec parametres. initialisation de l'attribut nomReseau et nbVille avec les parametres.
     * \param[in] nomReseau, nbVilles
     */
    ReseauInterurbain::ReseauInterurbain(std::string nomReseau, size_t nbVilles)
    {
        unReseau = Graphe(nbVilles);
        this->nomReseau = nomReseau;
    }

    /**
     * \fn	ReseauInterurbain::~ReseauInterurbain()
     * \brief destructeur de l'objet. Ici on ne fait rien car les attribut sont des conteneurs de la STL qui ont leur propre destructeur
     */
    ReseauInterurbain::~ReseauInterurbain() = default;

    /**
     * \fn	ReseauInterurbain::resize(size_t nouvelleTaille)
     * \brief Change la taille du graph en appelant la fonction de la classe Graph
     * \param[in] nouvelleTaille
     */
    void ReseauInterurbain::resize(size_t nouvelleTaille)
    {
        unReseau.resize(nouvelleTaille);
    }


    /**
     * \fn	ReseauInterurbain::chargerReseau(std::ifstream & fichierEntree)
     * \brief Charge le graph avec les aretes
     * \param[in] fichierEntree
     * \exception ReseauInterurbain::chargerReseau: Le fichier n'est pas ouvert !
     */
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

    /**
     * \fn	ReseauInterurbain::rechercheCheminDijkstra(const std::string &origine, const std::string &destination, bool dureeCout)
     * \brief recherche du chemin le plus court entre un sommet origin et une destination en utilisant l'algorithme de Dijkstra et calculer la duree du trajet ou son cout dependemment de la valeur bool
     * \param[in] origine, destination, dureeCout
     * \return Chemin
     * \exception Origine et/ou destination absent du réseau
     */
    Chemin ReseauInterurbain::rechercheCheminDijkstra(const std::string &origine, const std::string &destination, bool dureeCout) const
    {
        if(!verifieAbsence(origine) || !verifieAbsence(destination)) throw std::logic_error("Origine et/ou destination absent du réseau");

        //Initialisation
        std::vector<double> poidsCourt(unReseau.getNombreSommets(), std::numeric_limits<double>::max());
        std::vector<size_t> villesRestantes(unReseau.getNombreSommets());
        for(int v = 0; v < unReseau.getNombreSommets(); v++) villesRestantes[v] = v;

        std::vector<size_t> parents(unReseau.getNombreSommets(), std::numeric_limits<size_t>::max());

        poidsCourt[unReseau.getNumeroSommet(origine)] = 0;

        //Commencer l'algorithme
        while(!villesRestantes.empty())
        {
            size_t sourceIndex = minValueIndex(poidsCourt, villesRestantes);
            if(sourceIndex == std::numeric_limits<int>::max()) break;

            villesRestantes.erase(remove(villesRestantes.begin(), villesRestantes.end(), sourceIndex), villesRestantes.end());

            std::vector<size_t> villesVoisines = unReseau.listerSommetsAdjacents(sourceIndex);

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
        std::vector<std::string> cheminInverse = std::vector<std::string>();

        int maxIterations = unReseau.getNombreSommets();
        do
        {
            if(maxIterations == 0 || courantIndex == std::numeric_limits<size_t>::max())
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

    /**
     * \fn	ReseauInterurbain::algorithmeKosaraju()
     * \brief trouver les composantes fortement connexes en utilisant l'algorithme de Kosaraju.
     * \return std::vector<std::vector<std::string>> des composantes
     */
    std::vector<std::vector<std::string>> ReseauInterurbain::algorithmeKosaraju()
    {
        //Partie 1 DFS
        std::vector<size_t> villesDFS1 = parcoursProfondeur();

        //Partie 2 Inverser le graph
        std::vector<std::vector<size_t>> graphInverse = inverserGraph();

        //Partie 3 DFS avec listeVilles
        std::vector<bool> dejaVisite(unReseau.getNombreSommets(), false);
        std::vector<std::vector<std::string>> cFC;

        for(int i = villesDFS1.size() - 1; i >= 0; i --)
        {
            if(!dejaVisite[i])
            {
                auto ville = villesDFS1[i];
                //villesDFS1.pop();
                std::stack<size_t> pileSommets;
                pileSommets.push(ville);
                dejaVisite[ville] = true;
                std::vector<std::string> composante;

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

    /**
     * \fn	ReseauInterurbain::minValueIndex(const std::vector<double> & poidsCourt, const std::vector<size_t> & villesRestantes)
     * \brief Trouve la valeur minimale de poidsCourt a chaque iteration de l'algorithme de Dijkstra. Si il y a deux valeur minimales, retourne le premier rencontrer
     * \param[in] poidsCourt, villesRestantes
     * \return L'index de l'element le plus petit du vecteur
     */
    int ReseauInterurbain::minValueIndex(const std::vector<double> & poidsCourt, const std::vector<size_t> & villesRestantes) const
    {
        std::vector<double> poidRestant = std::vector<double>(villesRestantes.size());
        for(int i = 0; i < poidRestant.size(); i++)
        {
            int nSommet = villesRestantes[i];
            poidRestant[i] = poidsCourt[nSommet];
        }
        double minValue = *min_element(poidRestant.begin(), poidRestant.end());
        if(minValue == std::numeric_limits<double>::max()) return std::numeric_limits<int>::max();
        for(int i = 0; i < poidsCourt.size(); i++)
        {
            if(minValue == poidsCourt[i])
            {
                return i;
            }
        }
    }

    /**
     * \fn	ReseauInterurbain::minValueIndex(const std::vector<double> & poidsCourt, const std::vector<size_t> & villesRestantes)
     * \brief verifie si la ville est absente dans le graph
     * \param[in] ville
     * \return vrai si la ville est absente, faux dans le cas contraire
     */
    bool ReseauInterurbain::verifieAbsence(const std::string &ville) const
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

    /**
     * \fn	ReseauInterurbain::parcoursProfondeur()
     * \brief effectue un parcours en profondeur du graph
     * \return un vecteur des sommet parcours en ordre dans le parcours en profondeur
     */
    std::vector<size_t> ReseauInterurbain::parcoursProfondeur()
    {
        std::vector<bool> dejaVisite(unReseau.getNombreSommets(), false);
        std::vector<size_t> sommetsParcourus;

        for(int i = 0; i < unReseau.getNombreSommets(); i++)
        {
            if(!dejaVisite[i])
            {
                std::stack<size_t> pileSommets;
                pileSommets.push(i);
                dejaVisite[i] = true;

                std::vector<size_t> parcours;

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

    /**
     * \fn	ReseauInterurbain::inverserGraph()
     * \brief Inverse tous les arcs du graph
     * \return la liste d'adjacence du graph inverse
     */
    std::vector<std::vector<size_t>> ReseauInterurbain::inverserGraph()
    {
        std::vector<std::vector<std::size_t>> graphInverse(unReseau.getNombreSommets());
        for(int i = 0; i < unReseau.getNombreSommets(); i++)
            for(size_t villeVoisine : unReseau.listerSommetsAdjacents(i))
                graphInverse[villeVoisine].push_back(i);
        return graphInverse;
    }

}//Fin du namespace
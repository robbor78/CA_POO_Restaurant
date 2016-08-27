#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

/*****************************************************
 * Compléter le code à partir d'ici
 *****************************************************/

#include <iomanip>

class Produit {
private:
  string nom;
  string unite;

public:
  Produit(string nom, string unite = "") : nom(nom), unite(unite) {
    std::cout.precision(6);
  }

  string getNom() const { return nom; }
  string getUnite() const { return unite; }

  virtual string toString() const { return nom; }

  virtual Produit const * adapter(double n) const {
    return this;
  }

  virtual double quantiteTotale(string const &nomProduit) const {
    double sum = 0.0;
    if (getNom() == nomProduit) {
      sum = 1.0;
    }
    return sum;
  }
};

class Ingredient {
private:
  Produit const &produit;
  double quantite;

public:
  Ingredient(Produit const &produit, double quantite) : produit(produit), quantite(quantite) {}

  Produit const & getProduit() const { return produit; }
  double getQuantite() const { return quantite; }

  void setQuantite(double quantite) { this->quantite = quantite; }

  string descriptionAdaptee() {
    stringstream ss;
    auto pa = produit.adapter(quantite);
    ss << fixed << getQuantite() << " " << pa->getUnite() << " de " << pa->toString(); //TODO
    return ss.str();
  }

  double quantiteTotale(string const &nomProduit) const {
    double sum = produit.quantiteTotale(nomProduit) * quantite;
    return sum;
  }
};

class Recette {
private:
  string nom;
  double nbFois;
  vector<Ingredient> ingredients;

public:
  Recette(string nom, double nbFois = 1.0) : nom(nom), nbFois(nbFois) {}

  void ajouter(Produit const  &p, double quantite) {
    ingredients.push_back(Ingredient(p, quantite*nbFois));
  }

  Recette adapter(double n) const {
    Recette r(*this);

    r.nbFois *= n;
    for (auto &i : r.ingredients) {
      i.setQuantite(i.getQuantite()*n);
    }
    return r;
  }

  string toString() const {
    stringstream ss;
    ss << "Recette \"" << nom << "\" x " << nbFois << ":" << endl;
    int count = 1;
    for (auto i : ingredients) {
      ss << count++ << ". ";
      ss << i.descriptionAdaptee();
      if (count <= ingredients.size()) { ss << endl; }
    }
    return ss.str();
  }

  double quantiteTotale(string const &nomProduit) const {
    double sum = 0.0;
    for (auto i : ingredients) {
      sum += i.quantiteTotale(nomProduit);// *nbFois;
    }
    return sum;
  }
};

class ProduitCuisine : public Produit {
private:
  Recette recette;
public:
  ProduitCuisine(string nom) : Produit(nom, "portion(s)"), recette(nom) {}

  void ajouterARecette(Produit const &produit, double quantite) {
    recette.ajouter(produit, quantite);
  }

  ProduitCuisine const * adapter(double n) const override {
    ProduitCuisine *pc = new ProduitCuisine(*this);
    pc->recette = recette.adapter(n);
    return pc;
  }

  string toString() const override {
    stringstream ss;
    ss << Produit::toString() << endl;
    ss << recette.toString();
    return ss.str();
  }

  double quantiteTotale(string const &nomProduit) const override {
    double sum = 0.0;
    if (getNom() == nomProduit) {
      sum = 1.0;
    }
    else {
      sum = recette.quantiteTotale(nomProduit);
    }
    return      sum;
  }

};






/*******************************************
 * Ne rien modifier après cette ligne.
 *******************************************/
void afficherQuantiteTotale(const Recette& recette, const Produit& produit)
{
  string nom = produit.getNom();
  cout << "Cette recette contient " << recette.quantiteTotale(nom)
       << " " << produit.getUnite() << " de " << nom << endl;
}

int main()
{
  // quelques produits de base
  Produit oeufs("oeufs");
  Produit farine("farine", "grammes");
  Produit beurre("beurre", "grammes");
  Produit sucreGlace("sucre glace", "grammes");
  Produit chocolatNoir("chocolat noir", "grammes");
  Produit amandesMoulues("amandes moulues", "grammes");
  Produit extraitAmandes("extrait d'amandes", "gouttes");

  ProduitCuisine glacage("glaçage au chocolat");
  // recette pour une portion de glaçage:
  glacage.ajouterARecette(chocolatNoir, 200);
  glacage.ajouterARecette(beurre, 25);
  glacage.ajouterARecette(sucreGlace, 100);
  cout << glacage.toString() << endl;

  ProduitCuisine glacageParfume("glaçage au chocolat parfumé");
  // besoin de 1 portions de glaçage au chocolat et de 2 gouttes
  // d'extrait d'amandes pour 1 portion de glaçage parfumé

  glacageParfume.ajouterARecette(extraitAmandes, 2);
  glacageParfume.ajouterARecette(glacage, 1);
  cout << glacageParfume.toString() << endl;

  Recette recette("tourte glacée au chocolat");
  recette.ajouter(oeufs, 5);
  recette.ajouter(farine, 150);
  recette.ajouter(beurre, 100);
  recette.ajouter(amandesMoulues, 50);
  recette.ajouter(glacageParfume, 2);

  cout << "===  Recette finale  =====" << endl;
  cout << recette.toString() << endl;
  afficherQuantiteTotale(recette, beurre);
  cout << endl;

  // double recette
  Recette doubleRecette = recette.adapter(2);
  cout << "===  Recette finale x 2 ===" << endl;
  cout << doubleRecette.toString() << endl;

  afficherQuantiteTotale(doubleRecette, beurre);
  afficherQuantiteTotale(doubleRecette, oeufs);
  afficherQuantiteTotale(doubleRecette, extraitAmandes);
  afficherQuantiteTotale(doubleRecette, glacage);
  cout << endl;

  cout << "===========================\n" << endl;
  cout << "Vérification que le glaçage n'a pas été modifié :\n";
  cout << glacage.toString() << endl;

  return 0;
}

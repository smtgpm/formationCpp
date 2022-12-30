#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "chap5_heritages.h"

void heritages() {
  // p47_heritages();
  p51_heritageMultiple();
}

class Ouvrage {
public:
  std::string m_nom;
  Ouvrage(std::string n) :m_nom{ n } {}
  virtual void afficher() { std::cout << m_nom; }
  // Le mot cl� "virtual" indique qu'il faut aller chercher si il n'y a pas une autre version de cette fonction dans les classes fille,
  // et les appellera si possible, sinon on appelle celle-ci. La fonciton devient un peu plus lourde et un peu plus lente d'ou le fait de 
  // pas le mettre toujours. autre utilit�, si l'auteur ne veut pas que cette fonction soit red�finie, on ne met pas virtual
  Ouvrage(Ouvrage &&src) : m_nom(src.m_nom) {}
  virtual ~Ouvrage() {} // Necessaire lorsuq'on a des classes filles multiples en diamant
};

class Livre :virtual public Ouvrage {
  // Le mot cl� public ici indique que la classe fille va h�riter de mani�re publique tout ce qui est publique de la m�re, et protected ce qui est protected.
  // Si on avait mis protected, tout ce dont la fille h�rite sera protected
  // Si on avait mis private, tout ce dont la fille h�rite sera private
public:
  std::string m_auteur;
  Livre(std::string n, std::string a) :Ouvrage(n), m_auteur{ a } {}  // Standard: mettre en arguments d'abord les plus anciens (classe m�re) puis les nouveaux
  void afficher() noexcept override {
    Ouvrage::afficher();
    std::cout << " de " << m_auteur;
  }
  // Depuis c++ 11, il est possible d'ajouter le mot cl� 'final' apr�s la d�claration de la fonction pour faire en sorte qu'apr�s cette d�f,
  // il n'est plus possible de redefinir la fonction (en gros bloque le 'virtual' � partir d'ici

  // Autre mot cl�, on peut ajouter le mot cl� 'override'. Ce mot cl� permet de checker 1:que cette fonction existe bien (bon check contre les fautes d'orthographe)
  // et de 2: elle v�rifie que la fonciton m�re est bel et bien virtual.
  Livre(Livre &&src) :Ouvrage(std::move(src)), m_auteur(src.m_auteur) {}
  // Ici on a du appeller le std::move car le src n'est pas une rvalue, donc on Ouvrages(src) n'appellerai pas le constructeur Ouvrages(Ouvrages &&src), et donc comportement non voulu
};

class Cd :virtual public Ouvrage {
  // On ajoute 'virtual' pour chaque classe qui aura une sous-couche suivante, cela permet d'�viter d'avoir que la classe mere soit instanci�e plus d'une fois.
public:
  unsigned m_duree;
  Cd(std::string n, unsigned d) :Ouvrage(n), m_duree(d) {}
  void afficher() noexcept override {
    Ouvrage::afficher();
    std::cout << " durant " << m_duree << "s";
  }
};

class LivreAudio :  // Heritage multiple
  public Livre,
  public Cd{
public:
  std::string m_langue;
  LivreAudio(std::string n, std::string a, unsigned d, std::string l) :Ouvrage(n), Livre(n, a), Cd(n, d), m_langue(l) {}
  void afficher() noexcept override {
    Livre::afficher();
    std::cout << " , ";
    Cd::afficher();
    std::cout << " , ";
    std::cout << " en " << m_langue;
  }
};

void p47_heritages() {
  std::vector<std::unique_ptr<Ouvrage>> v;  // On autorise � mettre des Livres � la place de ouvrages car tout ce qui sera demand� � ouvrage sera faisable par livre aussi (pour l'instant, tant que tout est public)
  v.push_back(std::make_unique<Livre>("Tintin", "Unknown"));
  v.push_back(std::make_unique<Livre>("Gros minet", "Pas Titi"));
  v.push_back(std::make_unique<Ouvrage>("l'ethymologie du mot Poule"));
  for (auto &vv:v){
    vv->afficher();
    std::cout << std::endl;
  }
}

void p51_heritageMultiple() {
  LivreAudio la("Fais Dodo", "auteur", 1500, "FR");
  Cd cd("blahblah", 1000);
  la.afficher();
  std::cout << std::endl;
  std::cout << "taille Ouvrage " << sizeof(Ouvrage) << std::endl;
  std::cout << "taille Livre " << sizeof(Livre) << std::endl;
  std::cout << "taille Cd " << sizeof(Cd) << std::endl;
  std::cout << "taille LivreAudio " << sizeof(LivreAudio) << std::endl;
}

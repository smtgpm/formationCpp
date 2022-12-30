#pragma once

// p19
class Chariot
{
public:
  Chariot(float v) :m_vitesse{ v } {}
protected:
  Chariot() :Chariot(0.0F) {};
  void setVitesse() {
    m_vitesse = 0.F;
  }
  void setVitesse(float v) {  // surcharge d'une méthàde autorisée, il fautt que le nombre d'arguments et/ou le type d'arguments soient différents
    m_vitesse = v;
  }
private:
  float m_vitesse;
};

class ChariotElectrique : public Chariot {
  // lorsuq'on hérite d'un classe, on hérite de l'ensemble des méthodes publiques ou protected, mais pas les constructeurs. les 3 constructeurs par défaults sont créés (pas d'args, copie et 
public:
  using Chariot::setVitesse;  // ça permet de remettre en public la fonction protected de la classe mère, et donc on peut l'utiliser en dehors de la classe 
  using Chariot::Chariot;     // 9a permet de récupérer le/les constructeurs dela classe mère et le met dans la fille (ça renomme automatiquement par contre)

private:
  void setVitesse() {  // Ceci ne peut pas être appelé en dehors de la classe, le using n'est pas prioritaire
    setVitesse(2.0F);
  }
};

// Lorsqu'on implémente un des 3 de la regle des 3, on fait tous les autres (constructeur par recopie, le egal et le destructeur)
class TabEnt {
public:
  TabEnt() :m_taille{ 0 }, m_tabEnt{ nullptr } {};  // constr par défault, nécessaire à redéfinir vu qu'on a dféfinit celui justee en dessous
  TabEnt(unsigned n) : m_taille{ n }, m_tabEnt(new int[n]) {};  // Un new a été appelé, il faut un delete
  TabEnt(const TabEnt& src): TabEnt(src.m_taille) {
    for (int i = 0; i < m_taille; i++) {
      m_tabEnt[i] = src.m_tabEnt[i];
    }
  }
  TabEnt &operator=(const TabEnt &src) {
    if (this != &src)  // permet de gérer le cas où _l'utilisateur souhaite faire un t2 = t2 (en gros égalité sur lui-même). Alors on fais rien
    {
      if (m_tabEnt != nullptr) // Au cas ou le tableau existait déjà
      {
        delete[] m_tabEnt;
      }
      m_taille = src.m_taille;
      m_tabEnt = new int[m_taille];
      for (int i = 0; i < m_taille; i++) {
        m_tabEnt[i] = src.m_tabEnt[i];
      }
    }
    return *this;  // toujours un return *this dans l'operateur =
  }
  // Note: le contructeur par copie ainsi que le = sont lents en cas de gros gros objets car ça fait une copie. DU coup, on ajoute au 3 constructeurs par defaults 2 additionnels avec des rvalues
  TabEnt(TabEnt &&src) : m_taille{ src.m_taille }, m_tabEnt{ src.m_tabEnt } {  // Ici on copie simplement la taille, et le pointeur vers le tableau
    src.m_tabEnt = nullptr; // Ceci est necessaire pour éviter que le delete du src (appelé lors du destructeur) ne casse le m-tabEnt de this
  }
  TabEnt &operator=(TabEnt &&src) {
    if (this != &src)
    {
      m_taille = src.m_taille;
      m_tabEnt = src.m_tabEnt;
      src.m_tabEnt = nullptr;
    }
    return *this;  // toujours un return *this dans l'operateur =
  }

  virtual ~TabEnt() { delete[] m_tabEnt; };  // delete sans les crochets sert à détruire un element, delete[] detruit l'ensemble de la structire

  int *getValues() { return m_tabEnt; }
private:
  int *m_tabEnt;
  unsigned m_taille;
};

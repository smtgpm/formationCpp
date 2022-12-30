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
  void setVitesse(float v) {  // surcharge d'une m�th�de autoris�e, il fautt que le nombre d'arguments et/ou le type d'arguments soient diff�rents
    m_vitesse = v;
  }
private:
  float m_vitesse;
};

class ChariotElectrique : public Chariot {
  // lorsuq'on h�rite d'un classe, on h�rite de l'ensemble des m�thodes publiques ou protected, mais pas les constructeurs. les 3 constructeurs par d�faults sont cr��s (pas d'args, copie et 
public:
  using Chariot::setVitesse;  // �a permet de remettre en public la fonction protected de la classe m�re, et donc on peut l'utiliser en dehors de la classe 
  using Chariot::Chariot;     // 9a permet de r�cup�rer le/les constructeurs dela classe m�re et le met dans la fille (�a renomme automatiquement par contre)

private:
  void setVitesse() {  // Ceci ne peut pas �tre appel� en dehors de la classe, le using n'est pas prioritaire
    setVitesse(2.0F);
  }
};

// Lorsqu'on impl�mente un des 3 de la regle des 3, on fait tous les autres (constructeur par recopie, le egal et le destructeur)
class TabEnt {
public:
  TabEnt() :m_taille{ 0 }, m_tabEnt{ nullptr } {};  // constr par d�fault, n�cessaire � red�finir vu qu'on a df�finit celui justee en dessous
  TabEnt(unsigned n) : m_taille{ n }, m_tabEnt(new int[n]) {};  // Un new a �t� appel�, il faut un delete
  TabEnt(const TabEnt& src): TabEnt(src.m_taille) {
    for (int i = 0; i < m_taille; i++) {
      m_tabEnt[i] = src.m_tabEnt[i];
    }
  }
  TabEnt &operator=(const TabEnt &src) {
    if (this != &src)  // permet de g�rer le cas o� _l'utilisateur souhaite faire un t2 = t2 (en gros �galit� sur lui-m�me). Alors on fais rien
    {
      if (m_tabEnt != nullptr) // Au cas ou le tableau existait d�j�
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
  // Note: le contructeur par copie ainsi que le = sont lents en cas de gros gros objets car �a fait une copie. DU coup, on ajoute au 3 constructeurs par defaults 2 additionnels avec des rvalues
  TabEnt(TabEnt &&src) : m_taille{ src.m_taille }, m_tabEnt{ src.m_tabEnt } {  // Ici on copie simplement la taille, et le pointeur vers le tableau
    src.m_tabEnt = nullptr; // Ceci est necessaire pour �viter que le delete du src (appel� lors du destructeur) ne casse le m-tabEnt de this
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

  virtual ~TabEnt() { delete[] m_tabEnt; };  // delete sans les crochets sert � d�truire un element, delete[] detruit l'ensemble de la structire

  int *getValues() { return m_tabEnt; }
private:
  int *m_tabEnt;
  unsigned m_taille;
};

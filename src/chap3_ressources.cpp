#include <iostream>
#include <string>
#include <memory>
#include "chap3_ressources.h"

void ressources() {
  // Les ressources c'est ce � quoi notre code est associ�. (lien vers un r�seau, imprimante, morceau de m�moire, composant graphique...)
  // Ce qui est conseill� aujourd'hui en c++ c'est un design pattern assez classique: raii (ressource aquisition is initialization)
  // en gros dans les constructeurs on cr�� le lien vers la ressource, dans les destructeurs on abandonne la ressource
  // il faut �viter dans des classes avoir une fonction open et close pour les ressources. Il faut utiliser les constructeurs/destructeurs.

  // En C, on a l'habitudde d'avoir un pointeur vers un objet associ� � la ressource extzerne. On le d�clare ptr *, on fait un new puis un delete.
  // En c++, on peut toujours l'utiliser, mais il y a des risques de fuite de m�moire, buffer overflow ... Pour palier � �a, il y a un syst�me alternatif
  // en utilisant un synthaxe diff�rente de la classique qui ajoute des protections. Ces classes sont d�sign�es par le terme smart_pointers. 
  // p33_pointeursUniques();
  // p34_ptrUniqueTableaux();
  //p35_sharedPointers();

  p37_weakPointers();
}

void p33_pointeursUniques() {
  struct Usager {
    std::string nom;
    int age;
    Usager(std::string n, int a) :nom{ n }, age{ a }{}
    void afficher() { std::cout << nom << " " << age << std::endl; }
    virtual ~Usager() { std::cout << "kill " << nom << std::endl; }
  };

  // "ancienne" facon
  Usager * u1 = new Usager("Dede", 50);
  u1->afficher();
  delete u1;
  // Probleme ici, ca demande bcp de code, et il faut appeler le new et delete.

  // new version utilisant la classe unique_ptr
  std::unique_ptr<Usager> u2(new Usager("JC1", 44));
  u2->afficher();
  // Destructeur est appel� automatiquement en fin de fonction. "Le pointeur est responsable de la vie de son contenu.
  // En gros, u2 sera d�truit � la fin de la fonction, mais aussi son contenu
  // Ce n'est pas le seul avantage. Le principal avantage c'est qu'il est unique, il ne peut pas y avoir un deuxi�me pointeur qui pointe vers la m�me donn�e :
  // std::unique_ptr<Usager> u3{ u2 }; ==> NEIN ! le constructeur par recopie n'existe pas.
  // de m�me que :
  // std::unique_ptr<Usager> u4;
  // u4 = u2; ==> NEIN l'op�rateur '=' a aussi �t� enlev�
  // Du coup le == n'est jamais vrai, le != est toujours vrai

  struct S1 {
    void montrer(std::unique_ptr<Usager> u)   { u->afficher(); };
    void montrer2(std::unique_ptr<Usager> *u) { (*u)->afficher(); }  // �a marche mais �a outre passe le principe de unique pointer
    void montrer3(std::unique_ptr<Usager> &u) { u->afficher(); }  // OK comme syntaxe, on donne directement l'objet, y'a pas de copie.
    void montrer4(Usager *u) {
      u->afficher();
      delete u; // n�cessaire car on a pris la responsabilit� de l'objet, vu qu'on a appel� avec le release
    }
    void montrer5(std::unique_ptr<Usager> &&u) { u->afficher(); }
  };
  S1 s1;
  // s1.montrer(u2); ==> NEIN, on ne peut pas copier le pointeur, et en le donnant � la fonction on le copie.
  s1.montrer2(&u2);
  s1.montrer3(u2);
  s1.montrer4(u2.release()); // Le unique_ptr.release() signifie "abandonne le contenu et renvoi le". En gros lee contenu de l'objet de l'unique pointer est renvoy�, on enl�ve le c�t� unique.
  // u2 maintenant n'est plus utilisable...
  // u2->afficher()  => NEIN car le contenu n'est plus associ� � u2
  s1.montrer5(std::unique_ptr<Usager>(new Usager("JC2", 44)));  // C'estr linteret de montrer5 (qui attend la valeur � droite), �a permet de donner un objet directement sans l'avoir cr�� avant. 
  // Le destructeur du Usager cr�� ci-dessus est appel� � la fin de la fonction montrer5
  std::unique_ptr<Usager> u5(new Usager("JC3", 44));
  s1.montrer5(std::move(u5));
  // Par contre, une fois l'appel au dessus fait, on ne doit plus utiliser u5 car le delete du contenu de u5 a �t� d�truit: quand une fonction 
  // recoit une valeur � droite, elle est responable de ses donn�es. Le delete du contenu de u5 est appel� � la fin de la fonction, donc on ne doit plus l'utiliser.
  u5->afficher();  // C'est compilable mais on ne peut pas pr�voir ce que �a va faire, parce que la donn�e peut potentiellement etre remplac�e...

  std::unique_ptr<Usager> u6 = std::make_unique<Usager>("Franck", 7);  // => �a permet de pas appeler le new. C'est la meme choses qu'avant, juste que par symmetrie on appelle pas plus de new que de delete
  // plus besoin d'appeler de "new" en gros avec cette syntaxe
  u6->afficher();
}

void p34_ptrUniqueTableaux() {
  std::unique_ptr<double[]> t1(new double[4]);
  std::unique_ptr<double[]> t2 = std::make_unique<double[]>(4);  // Les 2 synthaxes donnent le m�me r�sulta, un unique pointer vers un tableau
  std::memset(&t1[0], 0, 4);
  t1[0] = 10.1;  // l'op�rateur '[]' est aussi red�finit dans les unique_ptr
  t1[1] = 0.1;
  t1[2] = 120.1;
  t1[3] = 102;
  struct S1 {
    double m_somme;
    void set_sum(const std::unique_ptr<double[]> &t1, int sz) {
      m_somme = 0.0;
      for (int i = 0; i < sz; i++)
        m_somme += t1[i];
    }
  };
  S1 s1;
  s1.set_sum(t1, 4);
  std::cout << " somme de t1 : " << s1.m_somme << std::endl;
}

void p35_sharedPointers() {
  std::shared_ptr<std::string> s1{ new std::string("Hello") };
  std::shared_ptr<std::string> s2 = std::make_shared<std::string>("guten tag");
  // Note, on peut utiliser auto � gauche ce qui est plus simple...

  std::cout << (*s1) << " " << (*s2) << std::endl;

  // Ce qui change par rapport � l'unique ptr:
  {
    std::shared_ptr<std::string> s3{ s1 };
    std::shared_ptr<std::string> s4;
    s4 = s2;
    std::cout << (*s3) << " " << (*s4) << std::endl;
    // Le shared_ptr a un compteur interne du nombre d'instances cr��es, et le dernier qui sera supprim� ira delete le contenu.
    // Ici, si on d�truit s3 ou s4, le contenu de s1 ou s2 ne sera pas d�truit.
    std::cout << s1.use_count() << std::endl;  // 2
  }
  std::cout << s1.use_count() << std::endl;  // 1

  // Il est possible de vider un pointeur partag� :
  auto s5 = s1;
  if (!s5)
    std::cout << "s5 is empty" << std::endl;  // doesn't show
  std::cout << s1.use_count() << std::endl;  // 2
  s5.reset();
  if (!s5)
    std::cout << "s5 is empty" << std::endl; // shows
  std::cout << s1.use_count() << std::endl;  // 1
}

void p37_weakPointers() {
  struct Chat {
    std::string m_name;
    std::shared_ptr<Chat> m_enemy;
    Chat(std::string n) :m_name{ n }, m_enemy{} {}
    virtual ~Chat() { std::cout << "killed " << m_name << std::endl; }
    // en vrai faudrait ajouter l'operateur =
  };
  {
    std::shared_ptr<Chat> c1 = std::make_shared<Chat>("pipi");
    std::shared_ptr<Chat> c2 = std::make_shared<Chat>("pupu");
    std::shared_ptr<Chat> c3 = std::make_shared<Chat>("popo");
    c1->m_enemy = c2;
    c2->m_enemy = c3;
    c3->m_enemy = c1;
  }; // ==> les chat sont pas but�s parce qu'il y a un rebouclage, donc le use_count est toujours � 2. On a ici une r�f�rence circulaire

  // c'est ici que vienne en jeu les weak_ptr. C'est cr�� � partir d'un shared pointer (depuis un shared on peut obtenir un weak), mais � n'emp�che pas la suppression.
  struct Chaton {
    std::string m_name;
    std::weak_ptr<Chaton> m_enemy;
    Chaton(std::string n) :m_name{ n }, m_enemy{} {}
    virtual ~Chaton() { std::cout << "killed " << m_name << std::endl; }
    // en vrai faudrait ajouter l'operateur =
  };
  {
    std::shared_ptr<Chaton> c1 = std::make_shared<Chaton>("mini pipi");
    std::shared_ptr<Chaton> c2 = std::make_shared<Chaton>("mini pupu");
    std::shared_ptr<Chaton> c3 = std::make_shared<Chaton>("mini popo");
    c1->m_enemy = c2;
    c2->m_enemy = c3;
    c3->m_enemy = c1;
    // Pour utiliser le contenu du weakpointer, il faut utiliser le lock.
    std::cout << c1->m_name << "'s ennemy is " << c1->m_enemy.lock()->m_name << std::endl;

    // Par contre, pour acc�der � un weakpointer, c'est � nous de v�rifier si il n'a pas expir� avant d'y faire appel. Donc au dessus il faudrait en fait faire:
    if(!c2->m_enemy.expired())
      std::cout << c2->m_name << "'s ennemy is " << c2->m_enemy.lock()->m_name << std::endl;

  }; // ==>  les destructeurs sont bien appel�s.
}
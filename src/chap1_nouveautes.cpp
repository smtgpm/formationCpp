#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <initializer_list>  // les <> cherchent en priorité dans la lib interne
#include "chap1_nouveautes.h"      // les "" chechent en priorité dans les dossiers locaux
#include "classes.h"

#define NDEBUG  // cette définition permet de désactiver les assertions classique du C. En gros on ajoute cette définition quand on veut pas d'assert
#include <cassert>

void nouveautes() {
  std::cout << "Nouveautes:" << std::endl;
  //p7_ptr_null();
  //p8_sizes();
  //p9_initialisations();
  //p11_initializer_list();
  //p13_enumerations();
  //p14_alignements_memoire();
  //p15_autos();
  //p16_declarations();
  //p17_methodesAutomatiques();
  //p18_delegationConstructeurs();
  //p19_methodeHeritees();
  //p20_aliases();
  //p20_assertions();
  p22_expressionsConstantes();
}

void p7_ptr_null() {
  bool *a = NULL;  // à ne plus utiliser
  bool *b = nullptr;
  long long ai = (long long)(a);
  long long bi = (long long)(b);
  std::cout << ai << std::endl;
  std::cout << bi << std::endl;
  long long ai2 = NULL;
  //long long bi2 = nullptr;  ==> interdit !
  std::cout << ai2 << std::endl;
  //std::cout << bi2 << std::endl;
}

void p8_sizes() {
  int x;        // size >= 2 octets
  long y;       // size >= 4 octets
  long long z;  // size >= 8 octets
  std::cout << sizeof(x) << " - " << sizeof(y) << " - " << sizeof(z) << std::endl;
  // win 64  : 4 - 4 - 8 
  // linux64 : 4 - 8 - 8 
  // les types sont définis de facon imprecises pour pouvoir être utilisés sur toutes les machines

  char * p;
  std::cout << sizeof(p) << std::endl;  // 4 en 32 bits, 8 en 64 bits
  // en 32 bits, on peut pas accéder à plus de 4Go de RAM. au dela il faut du 64 bits forcément. la compil pourrait etre un peu plus lente sur des machines 64 bits
  // Compiler en 32 bits permet de marcher sur toutes machines (64 et 32bits), le contraire n'est pas vrai

  // types à taille fixe
  int16_t s16_a = 3;
  uint64_t u32_b = 3;
  int_fast16_t c = INT_FAST16_MAX; // le plus rapide possible, mais au moins 16 bits (en fonction du processaur ça peut erte plus)
  int_least8_t d = 3; // au moins 16 bits (2o), mais le plus petit possible (quasi toujours 16 bits, mais il est possible que des machines acceptent pas sur du 16 bits)
                      // Le least peut parfois etre utile lors de tableaux pour les alignements de taille...
  std::cout << c << std::endl;  // c donne max sint32, et non int16. Le compilo estime que sur un win64 le 32 bits va plus vite
  std::cout << d << std::endl;

  intmax_t e = INTMAX_MAX;  // max possible des entiers de la machine
  intptr_t f = INTPTR_MAX; // taille d'un pointeur sur la machine

  e = 10; // base dix
  e = 012; // (c'est de l'octal)  => 10
  e = 0xA; // hexa
  e = 0b1010; // binaire
  e = 100'000'000;
}

void p9_initialisations() {
  int a = 12;
  int b(12);  // identique, l'idée c'était d'uniformiser...
  // dans les 2 cas il y a des fois des cas impossibles Pour uniformiser, il y a une synthaxe finale :
  int c{ 12 };  // les accolades, lorsqu'elles suivent un intitulé et comporte que des valeurs, alors c'est une init
  // ces accolades permettent de s'appliquer à d'autres types, ou des classes

  // un structure en c++ c'est comme une classe, mais tout est public
  struct S1 { S1(int n) {} };
  S1 d{ 12 }; // accolade possible ici aussi
  S1 e{ c };  // valide, pareil que juste au dessus
  S1 f{ d };  // quand on fait une classe juste avec un constructeur, elle créé automatiquement un constructeur de copie. Du coup f devient une copie de d (en gros on a S1(S1))
  int g();  // !!! ca compile, mais c'est en fait une pré-déclaration de fonction. Ca prépare une future fonction g avec pas d'arguments. 
  // g = 3 ne fonctionne pas
  int h{}; // h = 0 --> les accolade initialize avec la valeur par défault.

  // TABLEAUX
  double t1[3]{ 1.2, 5.5, 464.2 };
  double t2[] { 1.2, 5.5, 464.2 };  // avec les accolades, pas besoin d'indiquer la taille du tableau. taille fixe, pareil qu'au dessus au final
  double t3[8]{ 10.1, 4.5 };  // autorisé, les valeurs données sont les premieres du tableau, le restu du tableau reçoit les valeurs pas défault ( 10.1, 4.5, 0.0, 0.0, 0.0 ...)
  std::vector<double> v1{ 10.1, 4.5 };
  std::vector<double> v2{ 8 };  // ça créé un vector de taille 1 avec la valeur 8
  // pour créer un vector de taille 8, il faut les parenthèses
  std::vector<double> v3(8);  // ça créé un vector de taille 8 initialisé avec les valeurs pas défault
  std::cout << v3[7] << std::endl; // v2[7] ouvre une assert, v3[7] donne 0
  std::vector<double> v4(8, 3);  // ça créé un vector de taille 8 initialisé des 3

  // MAPS
  std::map<std::string, float> m1 {
    {"lundi", -18.4f},
    {"mardi", -6.2f}
  }; // initialization avec des imbrication d'accolades pour plusieurs dimensions...
  std::cout << m1["mardi"] << std::endl;
}

// si on veut faire une classe qui est capable d'etre appelle avec un nombre libre de valeurs utilisant des accolades, il faut créer un constructeur qui contient en argument un initializer_list:
void p11_initializer_list() {
  struct S1 {
    float total{ 0.0 };
    S1(std::initializer_list<float> l) {
      for (std::initializer_list<float>::iterator it = l.begin(); it != l.end(); ++it)
      {
        total += (*it);
      }
    };
  };
  S1 lt{ 4.0,2.1,565,44.0,4.0 };  // ici on peut mettre autant de valeurs qu'on veut dans le constructeur de la classe
  std::cout << "S1 : " << lt.total << std::endl;
  // en cas d'ambiguité, c'est le constructeur initializer list qui passe en priorité. A noter que l'initializer list foncitonne qu'avec les accolades, donc si je créé un constructeur
  // qui prend 4 valeurs en entrée et un avec l'initializer_list, si j'appelle une instance avec 4 valeurs  A{ 1,2,3,4}, alors l'initializer_list sera pris car prioritaire, mais A(1,2,3,4) prendra l'autre car parentheses

  // p12 boucle d'iteration alternative qui peut etre utilisée pour lire des containeurs ayant un type spécifique (vector, map)
  std::vector<int> v1(5, 1);  // vecto de 5 elts, valeurs 1
  for (double v : v1) {
    // pour utiliser cette synthaxe, il faut que la classe v1 possède un begin, un end et que ce qui est renvoyé par les iterateurs de cette classe disposent de l'opérateur ++ et d'un * (et donc on peut l'utiliser pour autre que les vactors)
    std::cout << v << std::endl;
  }
  // du coup on peut refaire la classe S1 en réduit:
  struct S2 {
    float total{ 0.0 };
    S2(std::initializer_list<float> l) {
      for (float vi : l) {
        total += vi;
      }
    };
  };
  S2 lt2{ 4.0,2.1,565,44.0,4.0 };  // ici on peut mettre autant de valeurs qu'on veut dans le constructeur de la classe
  std::cout << "S2 : " << lt2.total << std::endl;
}

void p13_enumerations() {
  enum Jours{Lundi, Mardi, Mercredi};
  Jours J1 = Mardi;
  enum Couleurs {Rouge, Bleu, Jaune};
  Couleurs C1 = Bleu;
  if (J1 == C1) {
    std::cout << "compparaison entre Jour et couleur pareil" << std::endl;  // Probleme ici, on peut comparer des choses qui n'ont rien à voir. Ca teste les valeurs (ici, 1==1)
  }
  // enum Albums{EnPassant, Minoritaire, Rouge};  // ==> compile pas car Rouge est égal à 0 et à 2, pas possible
  // Ces 2 problèmes vont être résolus par la classe d'enum:
  enum class Albums { EnPassant, Minoritaire, Rouge };
  Albums A1 = Albums::Rouge;
  //if (A1 == Rouge) {  => ne marche pas, les 2 ont rien à voir.
  //  std::cout << "pareil" << std::endl;  // Probleme ici, on peut comparer des choses qui n'ont rien à voir. Ca teste les valeurs (ici, 1==1)
  //}
  if (int(A1) >= 1) {
    std::cout << "Ca, ca marche" << std::endl;
  }
  // même chose:
  if (A1 >= Albums::EnPassant) {};  // tester directement sur les valeurs

  enum class other {element1, element2, element3=5, element4};
  // ===> ici, e1 = 0, e2 = 1, e3 = 5, e4 = 6

  // Toujours utiliser des enum class, jamais des enum.
}

void p14_alignements_memoire() {
  int32_t x, y;
  std::cout << "addr X " << intptr_t(&x) << std::endl;
  std::cout << "addr Y " << intptr_t(&y) << std::endl;
  // L'écart des addresses entre X et Y est généralement un multiple de 4. Ils s'alignent sur des multiples de 4. En gros, le compilateur va 
  // mettre chaque objet sur des valeurs d'adresses qui ont des multiples de 4, et donc on peut potentiellement perdre un peu d'espace, mais ca permet une meilleur opti.

  // Il y a un mot clé qui permet d'obesrver l'alignement des objets dans la mémoire: alignof
  std::cout << "alignof X " << alignof(decltype(x)) << std::endl;
  std::cout << "alignof Y " << alignof(decltype(y)) << std::endl;
  // Alignof nous donne la valeur du multiple sur lequel ce type en particulier sera espacé. Ici ca renvoi 4, donc tout les int32_t seront écrit forcément sur des adresses 
  // ayant des multiples de 4.

  // Serialisation:
  struct S1 {
    int32_t a;
    int8_t  b, c, d;
    int32_t e;
  };
  S1 s1;
  std::cout << "addr a " << intptr_t(&s1.a) << std::endl;
  std::cout << "addr b " << intptr_t(&s1.b) << std::endl; // +4
  std::cout << "addr c " << intptr_t(&s1.c) << std::endl; // +1
  std::cout << "addr d " << intptr_t(&s1.d) << std::endl; // +1
  std::cout << "addr e " << intptr_t(&s1.e) << std::endl; // +2 ==> car alignement sur mulltiple de 4 pour le int32_t.
  // addr: |a|a|a|a|b|c|d| |e|e|e|e| avec le premier a qui est sur un multible de 4

  std::cout << "alignement classe complete : " << alignof(decltype(s1)) << std::endl;  // = 4 :  MAX alignement des elements de S 

  // pour jouer sur les alignements, on peut utiliser le mot clé alignas(<multiple de 2 supérieur à l'alignement de l'elt donné>)
  // Serialisation:
  struct alignas(16) S2 {
    int32_t a;
    int8_t  b, c, d;
    int32_t e;
  };
  S2 s2;
  std::cout << "addr s2 a " << intptr_t(&s2.a) << std::endl;
  std::cout << "addr s2 b " << intptr_t(&s2.b) << std::endl; // +4
  std::cout << "addr s2 c " << intptr_t(&s2.c) << std::endl; // +1
  std::cout << "addr s2 d " << intptr_t(&s2.d) << std::endl; // +1
  std::cout << "addr s2 e " << intptr_t(&s2.e) << std::endl; // +2 ==> car alignement sur mulltiple de 4 pour le int32_t.
  // addr: |a|a|a|a|b|c|d| |e|e|e|e| mais le prmier a est sur un multiple de 16

  std::cout << "alignement classe s2 complete : " << alignof(decltype(s2)) << std::endl;  // = 16 :  MAX alignement des elements de S 

}

void p15_autos() {
  int32_t a = 8;
  auto b{ a };  // b aura le type de a
  std::cout << sizeof(b) << std::endl;
  auto c = 3 - 14;
  std::cout << sizeof(c) << std::endl;  // c est un int car les valeurs numériques sans virgules sont par défault un int

  // auto d;
  // d = a;  => interdit car une cdéclaration avec auto doit etre définis tout de suite

  std::vector<bool> v1{ true, true, false, true };
  for (auto v : v1) {  // cette boucle for est générique, utilisable quasiment partout
    std::cout << v << std::endl;
  }

  int32_t &ra = a;  // changer ra modifie aussi a. Ils sont égaux mais à la meme adresse donc changer a changera ra, et inversement
  auto ra2 = ra; 
  std::cout << "a " << a << "     ra : " << ra << "     ra2 : " << ra2 << std::endl;
  ra2++;
  std::cout << "a " << a << "     ra : " << ra << "     ra2 : " << ra2 << std::endl;
  // on contate que ni a, ni ra ont incrémenté. ra2 et ra1 sont donc indépendants. auto fait la copie du type de données, pas les indicateurs (on perd la référence, de même que const)

// Pour résoudre le problème ci dessus, il faudrait :
  auto &ra3 = ra;

  // perte du const :
  const int i = 5;
  auto ia = i;
  ia++;  // possible
  const auto ia2 = i;
  // ia2++; ==> NEIN

  // auto ne peut pas etre utilisé pour les arguments de fonction
  struct S1 {
    // void f1(auto x) { x = 0; } ==> NEIN

    // par contre, c'est autorisé en type de return
    auto f2(double n) { return n * 3; }

    auto f3(double n) {
      if (n < 2)
        return 3;
      else
        // return 4.0;  ==> NEIN, tous les return doivent donner un type identique si la fonction retuourne auto.
        return 4;  //   ==> OK
    };
  };
  S1 s1;
  std::cout << s1.f2(8.2) << std::endl;
}


void p16_declarations() {
  int a = 18;
  // auto ça reprend le type de qqch, et reprend la valeur.
  // Mais si on veut récupérer le type, mais pas la valeur, on utilise decltype:
  decltype(a) b = 27;
  
  decltype(sin(3)) c = 0;
  // ==> c aura le type du retour de la ofnction sin, mais on n'appelle pas sin(3), on gagne ce temps la. decltype(sin) ne fonctionne pas car il faut donner ce qui ets attendendu par le/les constructeurs
  // en gros l'utilité peut etre ssi on connais pas le type de retour d'une fonction, ou préparer un objet qui va stocker le retour d'une fonction avant d'avoir eu à appeler la fonction

  decltype(auto) d = a; // se comporte comme un auto classique. L'interet ici est uniquement si a est un template, ici ca permet de contourner ce soucis car auto d = <template> ne foncitonne pas
}

void p17_methodesAutomatiques() {
  // Lorsqu'on créé une classe sans constructeur, 3 trucs sont automatiquement générés
  // Un constructeur par défault, le destructeur et l'opératzeur =
  struct S1 {
    std::string message;
  }; // On pourra créer des S1 avec aucune entrées
  S1 s1{}; // ==> OK

  // lorsqu'on créé un constructeur, on enlève automatiquement celui qui est générés tout seul. Si on veut garder le généré, il faut ajouter constr() = default
  struct S2 {
    std::string message;
    S2(std::string m) :message{ m } {};
    S2() = default;
  };
  S2 s2{}; // OK grace au constructeur default, sinon on pourrait pas
  s2.message = "OK!";

  S2 s2_b;
  s2_b = s2;  // OK car le constructeur par copie est automatiquement généré dans une classe
  // Si on veut refuser on peut redéfinir la fonction opérateur, ou ajouter delete:
  struct S3 {
    std::string message;
    S3(std::string m) :message{ m } {};
    S3() = default;
    S3& operator=(const S3 src) = delete;  // neutralise l'opérateur =
  };
  S3 s3{};
  S3 s3_b;
  // s3_b = s3;   ==> NEIN
}

void p18_delegationConstructeurs() {
  // delegué : terme générique pour désigner une fonction qui en appelle une autre
  struct Usager {
    std::string prenom, nom;
    uint32_t age, qi;
    Usager(std::string p, std::string n, uint32_t a) : prenom{ p }, nom{ n }, age{ a } { }
    Usager(std::string p, std::string n) : Usager(p, n, 0) {};
    Usager(std::string p) : Usager(p, "Inconnu") {};
    Usager() : Usager("Inconnu") {};  // on a créé un constructeur qui se base sur un constructeur existant.
    void afficher() {
      std::cout << "prenom : " << prenom << std::endl;
      std::cout << "nom    : " << nom << std::endl;
      std::cout << "age    : " << age << std::endl;
    }
  };
  Usager u1("Bob", "Martin", 32);
  u1.afficher();
  Usager u2 {};
  u2.afficher();
  Usager u3("gege");
  u3.afficher();
}

void p19_methodeHeritees() {
  Chariot c1(10.7f);
  ChariotElectrique c2;
  // c2.setVitesse(); ==> Ne fonctionne pas car le using n'est pas prioritaire, et donc la setVitesse est privée
  c2.setVitesse(10.7f);  // oui car la fonction privée est uniquement celle sans arguments, le using par contre à récupéré les 2 versions de setVitesse de la classe mère.

  ChariotElectrique c3(0.1F); // OK grace au using du constructeur.
  c3.setVitesse(10.0F);
}

template <typename T> using PtVector = std::vector<T>*;
void p20_aliases() {
  unsigned int * p1;
  // mettons on veut utiliser ce type souvent, et comme c'est long à écrire, on peut utiliser ce qu'on appelle un alias
  typedef unsigned int * PtNaturel;
  PtNaturel p2;
  // p1 et p2 sont de même types

  using PtNaturel2 = unsigned int *; // nouveau moyen de l'écrire : conseillé de l'écrire comme ça
  PtNaturel2 p3;
  // autre avantage du using, c'est qu'il est utilisable avec des templates.

  std::vector<int> v1{ 0, 4, 5, 2 };
  using vectInt = std::vector<int>;
  vectInt v2 = v1;
  // avec un typedef ça aurait marché aussi. Mais, si vous voulez créer un alias qui est lui-même une template ça ne marche pas (cf PtVector):
  PtVector<int> v3 = &v1;
  int a = (*v3)[2];
  std::cout << a << std::endl;
}

void p20_assertions() {
  std::cout << "quel est votre age ? " << std::endl;
  int age;
  std::cin >> age;
  // on peut utiliser les assert pour etre sur que age a ubne bonne valeur
  assert((age > 0) && (age < 100));

  const int MAX_AGE = 70;
  static_assert((MAX_AGE > 10) && (MAX_AGE < 200)); // c++17
  // static asserts sont checké à la compilation. Possible que sur des const, ce code est supprimé à la compil donc ne prend rien en plus pour l"'"execution, mais permet d'avoir un code plus propre/pluys sur
}

void p22_expressionsConstantes() {
  int x = 11;
  //int t1[x]; // NEIN ==> en principe, ceci est interdit. Entre les crochets du tableau, il faut une expression constante.
  int t2[3]; // OK car 3 est une expression constante
  int t3[2 + 7]; // OK , idem
  const int y = 7;
  int t4[y]; // OK
  int t5[5 * y]; // OK, à la compilation il va résoudre toute l'expression en avance, et va voir que c'est const
  struct S1 {
    const int getV() { return 8; }
  };
  S1 s1;
  // int t6[s1.getV()]; NEIN : le compilo ne fait pas l'appel de getV, et donc ne sait pas que c'est une constrexpr
  int z = y + 2;
  // int t7[z];  ==> NEIN. Il faut vraiment que l'élt soit définit comme une constexpr

  // utilisation de constrexpr. Pour une variable c'est exactement comme un const:
  constexpr int z2 = y + 2;
  int t8[z2];  // OK
  // L'utilité de constexpr par rapport à const est dans les fonciton par exemple :
  struct S2 {
    constexpr int getV() { return 8; }
  };
  S2 s2;
  int t9[s2.getV()];   // OK
  // Note, le constexpr est un peu plus léger que le const. L'expression est constante, pas forcément la valeur ? pas sur sur ici de l'interet de constexpr vs const...

  struct S3 {
    constexpr int get1() { int x = 1;  return x; }  // OK
    //constexpr int get2() { int x; x = 2;  return x; }  ==> NEIN, le compilo arrive pas à voir que x sera toujours égal à 2
    constexpr int get3(int n) { return n * 2; }   // OK
  };
  S3 s3;
  int t10[s3.get3(10)]; // OK
  // int t11[s3.get3(x)]; // NEIN
  const int w = 5;
  int t11[s3.get3(w)]; //  OK  

  // Note, les static_assert necessitent des constexpr
}




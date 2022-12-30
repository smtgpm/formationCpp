#include <iostream>
#include <string>
#include "chap6_progFonctionnelle.h"

#include <functional>

void prog_fonctionnelle() {
  //p56_variableDeFonction();
  //p57_curryfication();
  //p58_adaptationDeRefs();
  p59_lambdas();
}

// La programmation fonctionnelle revien à tout mettre en fonction. Il n'y a pas d'affectation ( pas de = )
// Un des avantages, c'est qu'il est généralement très optimisable. C'es facile de tout découper en multiples threads.

// 3 fonctions suivantes retournent le nombre de livres rendus par mois dans une librairie selon l'age :
int calculEnfant(int mois) noexcept {
  return (3 * mois - 2);
}

int calculAdo(int mois) noexcept {
  return (2 * mois + 2);
}

int calculAdulte(int mois) noexcept {
  return (2 * mois);
}
// Les 3 fonctions ont la même signature (parametre = int, return = int), mais un algo différent


void p56_variableDeFonction() {
  int choix;
  std::cout << "Enfant/ado/adulte ? 0/1/2" << std::endl;
  std::cin >> choix;
  // Maintenant, au lieu de se souvenir du choix pour ensuite appeler les fonctions, on peut stocker directement les fonctions :
  std::function<int(int)> fcalcul;
  switch(choix){
  case 0:
    fcalcul = calculEnfant;
    break;
  case 1:
    fcalcul = calculAdo;
    break;
  case 2:
    fcalcul = calculAdulte;
    break;
  default:
    throw std::range_error("choix doit etre 0,1 ou 2 !");
    break;
  }
  if (fcalcul)
    std::cout << "pour 6 mois, donner " << fcalcul(6) << " livres" << std::endl;;
  // On voit que l'opérateur () a été redéfinie sur l'objet fcalcul, qui va faire appel à l'opérateur () de la fonction en question
  
  struct S1 {
    unsigned m_mois;
    S1(unsigned m) :m_mois(m) {}
    void affiche() {
      std::cout << "j'auria lu " << m_mois * 3 << " livres" << std::endl;
    }
    void affiche(unsigned m) {
      std::cout << "j'aurai lu " << m * 3 << " livres" << std::endl;
    }
  };
  std::function<void(S1)> faffiche = std::mem_fn<void()>(&S1::affiche);  // mem_fn est utilisée que pour des fonctions dans des classes.
  // td::function<type de fonction (type de parametre)>
  // mem_fn<type de fonction>(ref de la fonction voulu)
  faffiche(S1{ 10 });

  std::function<void(S1, unsigned)> faffiche2 = std::mem_fn<void(unsigned)>(&S1::affiche);
  // td::function<type de fonction (type de parametre1 (l'objet passé en premier), param 2...>
  // mem_fn<type de fonction(arguments)>(ref de la fonction voulu)
  faffiche2(S1{ 10 }, 20);
  // ne pas viser de contructeurs ou destructeurs avec cette methode. On ne peut pas mettre un truc privé ou protected non plus
}

int livresGroupe(int enfants, int adultes, int mois) {
  // bat lec de ce qu'on veut faire, c'est une fonction quelconque qui a plusieurs variables
  return mois * (enfants * 4 + adultes * 3 - enfants * adultes / 2);
}

void p57_curryfication() {
  // ou appels préparés...
  std::function<int(int, int, int)> ftout = livresGroupe;
  std::cout << ftout(3, 1, 2) << std::endl;
  std::function<int()> f = std::bind(ftout, 2, 0, 1);  // ==> on a construit une fonction à 0 paramètres en fixant les params de ftout.
  std::cout << f() << std::endl;

  // Maintenant, on peut aussi fixer que certains params et en garder d'autres ouvertes :
  std::function<int(int, int)> fea = std::bind(ftout, std::placeholders::_1, 4, std::placeholders::_2);
  std::cout << fea(5, 1) << std::endl;

  std::function<int(int)> fe = std::bind(fea, std::placeholders::_1, 1);
  std::cout << fe(5) << std::endl;

  // On peut aussi changer l'ordre des arguments. (par exemple f(mois, enfants))
  std::function<int(int, int)> fea2 = std::bind(ftout, std::placeholders::_2, 0, std::placeholders::_1);
  std::cout << fea2(12, 1) << std::endl;

}

void augmenter(int &n) {
  n++;
}

void p58_adaptationDeRefs() {
  std::string ok("ok");
  std::vector<std::string> v1{ ok, ok, ok };
  for (std::string s : v1) { // on fait un for par copie, donc les ok auront des '!' mais pas le contenu de v. Il faudrait ajouter la ref
    s += '!';
  }
  std::cout << v1[0] << "," << ok << std::endl;
  for (std::string &s : v1) { // avec la ref, le contenu de v1 sera bel et bien changé. La chaine initiale ok ne sera pas changée par contre.
    s += '!';
  }
  std::cout << v1[0] << "," << ok << std::endl;


  // Si on veut que tout pointe vers le meme ok, et avoir ok!!!
  // pas la meilleure facon à prioris, mais ça marche, avec des pointeurs.
  std::vector<std::string*> v2{ &ok, &ok, &ok };
  for (std::string *s : v2) {
    *s += '!';
  }
  std::cout << *v2[0] << "," << ok << std::endl;

  // La bonne manière avec des références:
  std::vector<std::reference_wrapper<std::string>> v3{ std::ref(ok), std::ref(ok), std::ref(ok) };
  for (std::string &s : v3) {
    s += '!';
  }
  std::cout << v3[0].get() << "," << ok << std::endl;
  // Ici on a pu créer un vector avec des refs. On ne travaille plus que sur une seule chaine de characteres (ok), et non pas plusieurs copies.

  // On peut utiliser cette methode dans le bind aussi pour donner les refs
  int a = 8;
  auto f1 = std::bind(augmenter, a); // ==> a ne sera pas augmenté parce qu'avec bind on donne une copie...
  f1();
  std::cout << a << std::endl;
  auto f2 = std::bind(augmenter, std::ref(a)); // ==> ici on a bien donné la ref.
  f2();
  std::cout << a << std::endl;
}

auto lireF() {
  int n{ 1 };
  auto f = [&n] { std::cout << n << std::endl; }; // n par référence est dangereux ici car l'appel de lireF va cout 'n' alors que n peut ne plus exister car on est sorti de la fonction.
  n = 3;
  return f;
}

void p59_lambdas() {
  // Permet de crééer des fonctions sur le tas, pas besoin d'en avoir une de définie...
  std::function<double(double)> fois3 = [](double x)->double {return x * 3.0; };
  // crochets : [] ils servent de communication au moment de la déclaration.
  // (paramètres, il peut y en avoir plusieurs, séparés par des virgules)->valeur de retour
  // entre les accolades on met ce qu'on veut comme code. Mais généralement on reserve un code assez court pour éviter un bordel
  std::cout << " 3 * 5.2 = " << fois3(5.2) << std::endl;
  auto ditBonjour = [] {std::cout << "Bonjour !" << std::endl; }; // on peut ne pas mettre le void
  ditBonjour();

  auto i = 5;
  // auto affichei = [] {std::cout << i << std::endl; }; // ==> NEIN, il ne connait pas 'i'
  // Pour qu'il connaisse i, voici la solution :
  auto affichei = [i] {std::cout << i << std::endl; };
  // Ici, l' [i] veut dire, "trouves i". elle va chercher i elle-même, pas besoin de donner i lors de l'appel par contre :
  affichei();
  i = 8;
  affichei(); // On voit ici que i n'a pas changé vers 8, ça affiche toujours 5. En gros, lorsqu'on a mis [i], ça passe i par valeur (copie) au moment de la déclaration.

  i = 5;
  auto affichei2 = [&i] {std::cout << i << std::endl; };
  // Ici on ne fait pas par copie, on affiche le i tel quel
  affichei2();
  i = 8;
  affichei2();
  // Dans les crochets, on peut mettre plusieurs variuables si on veut (séparé par virgule).. Cf p60
  // On peut aussi mettre [&]  . Ca veut dire que le lambda à accés à tout son environnement par référence.
  // De même, on peut mettre  [=]  . Ca donne accès à tout l'environnement mais par valeur.
  // On peut aussi mettre [=, &<param>]  : tout par valeur sauf le param qui est par référence
  lireF()();

  // Hors sujet :
  // Static en C = "je conserve la valeur d'un appel à l'autre". En gros ca devient une valeur globale, mais la portée reste uniquement dans le scope ou c'est défini.
  // en gros, si je défini un static n dans une fonction, à chaque appel de cette fonction, n gardera sa valeur. En dhors de la fonciton par contre n n'existe pas
};
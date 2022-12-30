#include <iostream>
#include "chap2_move_semantics.h"
#include "classes.h"

void move_semantics() {
  //p24_valeursGD();
  //p25_donneesTemporaires();
  //p26_deplacements();
  //p27_move();
  p30_deplacementDeThis();
}

void p24_valeursGD() {
  // Left values/ right values (ie: rvalue, lvalue)
  int x = 8;  // x <==> lvalue    8 <==> rvalue
  // Une lvalue est qqch qu'on peut mettre � gauche d'un �gal. 8 ne peut pas, c'est une rvalue.
  int y = x;  // x ET y sont des lvalues.

  int z = x - y;  // z, x et y sont des lvalues. Mais "x-y" est une rvalue

  struct S1 {
    int f1() { return 2; }
    int& f2(int& x) { return x; }
  };
  S1 s1;
  int r1 = s1.f1();  // r1 lvalue, s1.f1() est une rvalue.
  // G�n�ralement, une fonction est une rvalue. Sauf que elle renvoit une r�f�rence
  int r2 = s1.f2(x); // r2 et s1.f2(x) sont des lvalues:
  s1.f2(x) = 11;
  std::cout << x << std::endl;
}

void p25_donneesTemporaires() {
  int x = 3;
  int& y = x;  // on dit que y est une r�f�rence � gauche sur un entier (left value reference)
  struct S1 {
    void f1(int n) { n++; }
    void f2(int &n) { n++; }
    int f3() { return 8; }
    int f4(int n) { return n + 2; }
  };
  S1 s1;
  s1.f1(x);
  std::cout << x << std::endl;  // 3
  s1.f2(x);
  std::cout << x << std::endl;  // 4
  
  // s1.f2(15); ==> pas possible. C'(est pour �a qu'on appelle �a des left value r�f�rence, car il faut que ca fasse une r�f�rence � un left value. 15 �tait rvalue, �a marche pas.
  int v = s1.f3(); // s1.f3 est une rvalue
  // int &v2 = s1.f3(); ==> NEIN car le & est une left value reference, et doit faire ref � une lvalue. 
  const int &v2 = s1.f3(); // NOTE: Ceci est autoris� car on indique qu'on changera pas la lvalue

  // int &v3 = s1.f4(x);  ==> non plus, f4 est une rvalue. Ca passe si on ajoute const comme juste au dessus
  const int &v3 = s1.f4(x);

  // c++ a maintenant ajout� des r�f�rences � droite. La syntaxe est :
  int &&v4 = s1.f3();
  // lorsqu'on fait v = s1.f3() , f3 renvoit 8 qui va etre stock� dans v, mais le 8 de f3 va disparaitre juste apr�s l'appel. 
  // L'utilit� de ces ref�rences � droite c'est que le 8 va rester en m�moire. Ici v4 = s1.f3(), il n'y a pas eu besoin de faire une copie du 8, c'est directement un pointage vers ce 8.
  // On peut ici faire :
  v4++;
  std::cout << v4 << std::endl;

  int &&v5 = s1.f3() * 6;  // OK. ici, le 8 de f3 va disparaitre, le 6 ausis, mais le 48 r�sultant du 6 * 8 va rester en m�moire et v5 pointe vers ce 48.
  v5 += s1.f3() + 2;
  std::cout << v5 << std::endl;
  // A noter que v5 est un lvalue
  // int &&v6 = v5; ==> interdit.

  struct S2 {
    void f1(int x)  { std::cout << "int   " << x << std::endl; }
    void f1(int &x) { std::cout << "int&  " << x << std::endl; }  
    // ceci est interdit, car il y a ambiguit�. Ce serait bien de pouvoir avoir cet overload en fonction de l'argument.
    // L'ambiguit� vient du fait que le premier f1 peut prendre une rvalue ou un lvalue, le second que du rvalue.
  };
  S2 s2;
  // s2.f1(x); ==> NEIN ! 

  // ce souci est r�solu si on enl�ve l'ambiguit�:
  struct S3 {
    void f1(int &&x) { std::cout << "int&& (rvalue)  " << x << std::endl; }  // attends une rvalue
    void f1(int &x) { std::cout << "int&  (lvalue) " << x << std::endl; }  // attends un lvalue
    // Pas d'ambiguit� possible pour les 2 f1
  };
  S3 s3;
  s3.f1(x);  // lvalue
  s3.f1(3);  // rvalue
  s3.f1(3+2);  // rvalue
  s3.f1(s1.f3());  // rvalue
  s3.f1(v5); // lvalue  => c'est pas parce qu'on d�finit qqch comme un r�f�rence de rvalue que lui-m�me est rvalue. En effet, v5 est d�fini � gauche du �gal ...
  s3.f1(v5 + 0); // rvalue => astuce pour transformer un nombre lvalue en rvalue. Mais il y a une solution g�n�rique pour �a :
  s3.f1(std::move(v5)); // ==> transforme la lvalue en rvalue
  // CF example suivant
}

void p26_deplacements() {
  // cf la classe TabEnt
  TabEnt t1(3);
  int *vs = t1.getValues();
  vs[0] = 4;
  vs[2] = 12;
  TabEnt t2{ t1 };
  int *vs2 = t2.getValues();
  vs2[2] = 15;
  TabEnt t3;
  t3 = t1;
  int *vs3 = t3.getValues();
  vs3[2] = 35;
  std::cout << vs[2] << std::endl;
  std::cout << vs2[2] << std::endl;
  std::cout << vs3[2] << std::endl;
}

void p27_move() {
  struct S1 {
    TabEnt f1() { return TabEnt(10000); }
  };
  S1 s1;
  TabEnt t4;
  t4 = s1.f1();  // Ce qu'on fait ici, on cr�� un objet avec f1, qui ensuite est copi� dans t4. On fait une copie qui ne sert pas, ce serait mieux de directement prendre l''objet cr�� et le donner � t4.
  TabEnt t5;
  t5 = std::move(t4);  // appelle le operator=(&&)
  int *vs5 = t5.getValues();
  vs5[2] = 15;
  std::cout << t5.getValues()[2] << std::endl;


  // p28 swap :
  int a = 2;
  int c = 19;
  std::swap(a, c);
}

void p30_deplacementDeThis() {
  struct S1 {
    int x;
    S1(int x) : x{ x } {};
    void afficherXXXX() const & {
      int y = x * x * x * x;
      std::cout << y << std::endl;
    }
    void afficherXXXX() && {  // plus rapide, mais plus destructrice car l'objet n'est plus utilisable apr�s
      x = x * x * x * x;
      std::cout << x << std::endl;
    }
  };
  S1 s1{ 4 };
  s1.afficherXXXX();  // 256

  S1{ 4 }.afficherXXXX();  // 256. Ici l'objet est directement d�truit apr�s l'appel. Alors que s1 n'est d�truit qu'a la fin de p30_deplacementDeThis
  // Ici, dans ce cas, on a le droit de d�truire this dans S1{ 4 }.afficherXXXX() puisqu'il n'est pas utilis� apr�s. On pourrait donc optimiser un peu  (cf la 2eme fonction)


  // NC'est en ajoutant le & et le && apr�s les fonctions qu'on dit � la classe que lorsque afficheXXXX est appel�, si l'objet � gauche du point est un rvalue on appelle le && car peut etre d�truit, et si
  // l'objet � gauche de afficherXXXX est un lvalue alors on appelle la version &.

  // en gros, si j'ai le droit de casser this, alors faire une fonction avec la version && ce qui permet d'optimiser un peu le contenu car on peut d�truire l'objet
}



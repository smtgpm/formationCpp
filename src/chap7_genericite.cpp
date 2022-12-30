#include <iostream>
#include <string>
#include <cassert>
#include "chap7_genericite.h"

void genericite() {
  //p62_basesDeTemplate();
  p72_traits();
}

// templates de fonctions :
template<typename T> T nPlusA(T x, T y) { return x + y + x * y; }

// sp�cialisation (sp�cialisation d'un autre template)
template<>char nPlusA(char x, char y) { return x + y; }
// en gros, nPlus a prendra le premier, sauf si on donne des char.

// Note: si un type sp�cialis� compile une erreur, le compilo va ignorer la specialisation et utiliser la version g�n�rique

// pour completer les specialisations, on peut aussi faire:
template<> long nPlusA(long, long) = delete;
// Ceci enl�ve un template sp�cifique. Le nPlusA n'est pas utilisable avec des long

// p63 template de const
template<typename T> const T PI = T(3.14159679);
// Ceci est plus rapide que de caster (si on fait un define 3.14159679 puis cast � chaque appel)

// p64 genericite multiples
template<typename T1, typename T2> decltype(auto) nPlusA2(T1 x, T2 y) noexcept { return  x + y + x * y; }
// ici on utilise decltype(auto) parce qu'on sait pas de quel type le return sera, et cette mani�re permet de laisser le choix 
// On met le decltype parce qu'il y a un probl�me d'ordre de r�solution entre template et auto, le decltype r�soud ce soucis

// p64 sp�cialisation de classe
template<typename T1, typename T2> struct Tuple { T1 x; T2 y; };
template<typename T2> struct Tuple<float, T2> { float x; T2 y; };
template<> struct Tuple<float, float> { float x; float y; };

// p66 - 67 templates non-types
template<typename T, int8_t V> T diviseur(T x) { return x / V; }

// p68 templating + h�ritage
template<typename T> struct A { T x; };
// 1: on peux h�riter et sp�cialiser en meme temps :
struct B1 :A<int> {}; // B1 h�rite de la version int de A
// 2: on peux h�riter sans sp�cialiser
template<typename T> struct B2 :A<T> {};
// 3: on peut h�riter et devenir plus g�n�rique encore
template<typename T1, typename T2> struct B3 :A<T1> { T2 y; };
// 4:
template<typename T> struct B4 :A<int> { T y; };


// p74 traits
// comment faire en sorte qu'on ai une fonction qui a 2 codes diff�rent, en fonction du type qu'on demande ? 
template<typename T, typename std::enable_if_t<std::is_unsigned<T>::value>* = nullptr> T vSiSansSigne(T v) { return v; };
template<typename T, typename std::enable_if_t<!std::is_unsigned<T>::value>* = nullptr> T vSiSansSigne(T v) { return 0; };
// le enableIf va, si jamais le value est true, g�n�rer le typename* et donc le typename*=nullptr va r�ussir � compiler. Par contre, si jamais ca passe pas,
// ca va g�n�rer un void, et donc on aura void* = nullptr ce qui fait peter la compil, donc on ignorera ce template et prendra l'autre.
// en gros c'est un sorte de hack pour que ca compile l'un ou l'autre en fonction du typename qu'on donne... version optimis�e...


void p62_basesDeTemplate() {
  int a{ 2 }, b{ 5 };
  std::cout << nPlusA(a, b) << std::endl;
  float c{ 2.4f }, d{ 36.1 };
  std::cout << nPlusA(c, d) << std::endl;
  // On peut aussi forcer le type attendu:
  std::cout << nPlusA<double>(c, d) << std::endl;
  // std::cout << nPlusA(a, c) << nPlusA(" ", " ") << std::endl;  ==> double NEIN. Le premier est interdit car 2 types diff�rents. Le 2eme �a marche pas tout simplement, mais il va renvoyer l'erreur au template

  // template de const :
  std::cout << PI<float> << std::endl;
  std::cout << PI<double> << std::endl;
  std::cout << PI<char> << std::endl;

  // genericite multiples
  std::cout << nPlusA2(2, 5.69) << std::endl;

  // sp�cialisation de classe
  Tuple<float, int> t1;  // ==> ira cherche le 2eme des 3 d�finitions de template

  // Templates avec non types
  std::cout << diviseur<double, 4>(13.2) << std::endl;
  // attention, � la place du 4 il faut forc�ment une constexpr. 
  int8_t i = 2;
  // std::cout << diviseur<double, i>(13.2) << std::endl; // ==> NEIN
  constexpr int8_t j = 2;
  std::cout << diviseur<double, j>(13.2) << std::endl; // ==> JA

}

void p72_traits() {
  // Un trait c'est une particulatit� d'un classe. IE: une certain classe, est-ce qu'elle a telle particularit�. (il y en a plein, exemple : isintegral, isfunction, isconst ...)
  if (std::is_unsigned<int>::value)
  {
    std::cout << "int is unsigned " << std::endl;
  }
  else
  {
    std::cout << "int is not unsigned " << std::endl;
  }
  // du coup c'est std::is_'TRAIT'<type ou classe>::value avec value qui ets le bool true/false
  // Cf les tmeplate vSiSansSigne
  std::cout << vSiSansSigne(3.4f) << " , " << vSiSansSigne(unsigned(4)) << std::endl;  // On veut 0, 4
  
}
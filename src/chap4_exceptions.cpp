#include "chap4_exceptions.h"
#include <iostream>

void exceptions() {
  // les excpetions sont assez brutales, donc il ne fait pas en mettre partout. Si on peut le handle de mani�re diff�rente il faut...
  // Par exemple les input des users, on met des ifs, pas des exceptions
  // De meme, les exceptions ne sont pas la pour detecter les erreurs de code... les assert sont la pour �a
  // Il y a certaines r�gles aussi, par exemple, ne jamais mettre d'exceptions dans un destructeur.. (cf p40)
  p41_exceptions();

}

void p41_exceptions() noexcept {  
  // Cette fonction ne laisse partir aucune exception, on en est sur, alors il faut ajouter le mot cl� noexcept.
  // Ce mot cl� permet � l'utilisateur de pas avoir � mettre de try catch autour de cette fonction
  // si jamais le mot cl� noexcept est mis sur une fonction qui raise une exception, si jamais l'exception est raised, �a fait crash le programme!
  if(0)
  {
    int age;
    std::cout << " age ? " << std::endl;
    std::cin >> age;
    int vies_par_an = 1 / age;
    std::cout << "vies par an " << vies_par_an << std::endl;
    // suivant les compilo, si on met 0 dans une div, le comportement n'est pas le m�me. Visual raise une divByZero exception...
  }
  try {
    int age;
    std::cout << " age ? " << std::endl;
    std::cin >> age;
    if (age == 0) {
      throw 0; // lors d'un throw, il faut envoyer un objet de n'importe quel type (pas de new par exemple)
    }
    else if (age < 0) {
      throw std::range_error("Age negatif");
    }
    else {}
    int vies_par_an = 1 / age;
    std::cout << "vies par an " << vies_par_an << std::endl;
    std::cout << "A pu voter : " << maxElectionsLesgislatives(age) << " fois." << std::endl;
    // On constate ici que le "A pu voter" est print avant que l'exception soit raised si jamai son met un age < 18
  }
  catch (int &exc) {  // une exception est toujours attrap�e par r�f�rence.
    // Le catch est sur un type uniquement, pas sur sa valeur.On doit ensuite faire les tests sur la valeurs si on le souhaite dans le catch
    if (exc == 0)
      std::cout << "0 exception caught !" <<  std::endl;
    else
      std::cout << "other than 0 int exception caught :" << exc << std::endl;
  }
  catch (std::range_error &exc) {
    std::cout << "range error exception. error message : " << exc.what() << std::endl;
  }
  catch (...) { // (...) means any other exception (=other than any defined above this catch)
    std::cout << "any other exception caught..." << std::endl;
  }
}

void verificationAge(int age) {
  // fonction qui l�ve une exception si l'age est incorrect
  if (age < 18) {
    throw std::range_error("Age < 18");
  }
}

int maxElectionsLesgislatives(int age) {
  // fonction qui compte le maximum d'elections l�gislatives qu'une personnai d'un age 'age' ai pu voter (tous les 5 ans � partir de 18 ans)
  verificationAge(age);
  // Il est possible de v�rifier que la fonction utilis�e a �t� d�clar�e noexcept ou pas en utilisant :
  if (noexcept(verificationAge(age))) {
    // call function
  }
  return (1 + (age - 18) / 5);
}

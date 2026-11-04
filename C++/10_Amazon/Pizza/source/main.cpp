/**
 * Pizza
 */

#include "Pizza.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
  Pizza pizza(Base::Thin, Size::Large);
  pizza.addTopping(Topping::Cheese);
  pizza.addTopping(Topping::Olives);

  std::cout << "Price: " << pizza.getPrice() << std::endl;

  return 0;
}

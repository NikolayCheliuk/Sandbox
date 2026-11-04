/**
 * Pizza.cpp
 */

#include "Pizza.hpp"

Pizza::Pizza(Base b, Size s) :
  base(b),
  size(s)
{

}

void Pizza::addTopping(Topping topping)
{
  toppings.push_back(topping);
}

double Pizza::getPrice() const
{
  double price = base_price[(size_t)base][(size_t)size];

  for (auto topping : toppings)
  {
      price += topping_price[(size_t)topping][(size_t)size];
  }

  return price;
}

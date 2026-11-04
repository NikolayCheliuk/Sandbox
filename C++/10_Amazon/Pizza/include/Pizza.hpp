/**
 * Pizza.hpp
 */

#pragma once

#include <vector>

enum class Base
{
  Thin,
  Regular,
  Cheesy
};

enum class Size
{
  Small,
  Medium,
  Large
};

enum class Topping
{
  Cheese,
  Mushrooms,
  Olives
};

class Pizza
{
public:
  Pizza(Base b, Size s);

  void addTopping(Topping topping);

  double getPrice() const;

private:
  Base base;
  Size size;
  std::vector<Topping> toppings;

  // Price Table for Base: Base × Size
  static inline double base_price[3][3] =
  {
    { 5.0, 6.5, 8.0 },   // Thin
    { 6.0, 7.5, 9.0 },   // Regular
    { 7.0, 8.5, 10.0}    // Cheesy
  };

  // Price Table for Toppings: Topping × Size
  static inline double topping_price[3][3] =
  {
    { 1.0, 1.0, 1.0 },   // Cheese (fixed price)
    { 1.5, 2.0, 2.5 },   // Mushrooms (depends)
    { 2.0, 2.5, 3.0 }    // Olives (depends)
  };
};

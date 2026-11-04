/**
 * Filmography
 */

#include "Filmography.hpp"

#include <iostream>
#include <string>

int main(int argc, const char * argv[])
{
    Movie m1;
    m1._cast_list.push_back({ "Sean Connery", "James Bond" });
    m1._cast_list.push_back({ "Ursula Andress", "Honey Ryder" });

    Movie m2;
    m2._cast_list.push_back({ "Roger Moore", "James Bond" });
    m2._cast_list.push_back({ "Jane Seymour", "Solitaire" });

    Movie m3;
    m3._cast_list.push_back({ "Sean Connery", "James Bond" });

    std::vector<Movie> movies = { m1, m2, m3 };

    auto actors = FindActorsForCharacter(movies, "James Bond");
    std::cout << "All James Bond actors:\n";

    for (auto & a : actors)
    {
      std::cout << " - " << a << "\n";
    }

    auto top = TopActorsForCharacter(movies, "James Bond", 2);
    std::cout << "\nTop James Bond actors:\n";

    for (auto & [actor, count] : top)
    {
        std::cout << actor << " (" << count << " times)\n";
    }

  return 0;
}

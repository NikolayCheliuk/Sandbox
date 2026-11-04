/**
 * Filmography.hpp
 */

#pragma once

#include <list>
#include <string>
#include <vector>

class CastCredit;
class CrewCredit;

class Movie
{
public:
  std::vector<CastCredit> _cast_list;
  std::vector<CrewCredit> _crew_list;
};

class CastCredit
{
public:
  std::string _name;
  std::string _character_name;
};

class CrewCredit
{
public:
  std::string _name;
  std::string _role;
};

std::vector<std::string> FindActorsForCharacter(const std::vector<Movie> & movies, const std::string & character_name);
std::vector<std::pair<std::string, int>> TopActorsForCharacter(const std::vector<Movie>& movies, const std::string& character_name, int N);

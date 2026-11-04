/**
 * Magic Spells
 *
   Sample Input:
   3
   fire 5
   AquaVitae 999 AruTaVae
   frost 7

   Sample Output:
   Fireball: 5
   6
   Frostbite: 7
 *
 */

#include <iostream>
#include <vector>
#include <string>

class Spell {
  private:
    std::string scrollName;

  public:
    Spell(): scrollName("")
    {
    }

    Spell(std::string name) :
      scrollName(name)
    {
    }

    virtual ~Spell()
    {
    }

    std::string revealScrollName()
    {
        return scrollName;
    }
};

class Fireball :
  public Spell
{
  private:
    int power;

  public:
    Fireball(int power) :
      power(power)
    {
    }

    void revealFirepower()
    {
      std::cout << "Fireball: " << power << std::endl;
    }
};

class Frostbite :
  public Spell
{
  private:
    int power;

  public:
    Frostbite(int power) :
      power(power)
    {
    }

    void revealFrostpower()
    {
      std::cout << "Frostbite: " << power << std::endl;
    }
};

class Thunderstorm : public Spell
{
  private:
    int power;

  public:
    Thunderstorm(int power) :
      power(power)
    {
    }

    void revealThunderpower()
    {
      std::cout << "Thunderstorm: " << power << std::endl;
    }
};

class Waterbolt : public Spell
{
  private:
    int power;

  public:
    Waterbolt(int power): power(power)
    {
    }

    void revealWaterpower()
    {
      std::cout << "Waterbolt: " << power << std::endl;
    }
};

class SpellJournal
{
  public:
    static std::string journal;

    static std::string read()
    {
        return journal;
    }
};

std::string SpellJournal::journal = "";

void counterspell(Spell *spell)
{
  /* Enter your code here */

  Fireball      * fireball      = dynamic_cast<Fireball *>(spell);
  Frostbite     * frostbite     = dynamic_cast<Frostbite *>(spell);
  Thunderstorm  * thunderstorm  = dynamic_cast<Thunderstorm *>(spell);
  Waterbolt     * waterbolt     = dynamic_cast<Waterbolt *>(spell);

  if (fireball)
  {
    fireball->revealFirepower();
  }
  else if(frostbite)
  {
    frostbite->revealFrostpower();
  }
  else if(thunderstorm)
  {
    thunderstorm->revealThunderpower();
  }
  else if(waterbolt)
  {
    waterbolt->revealWaterpower();
  }
  else
  {
    std::string a = spell->revealScrollName();
    std::string b = SpellJournal::read();

    int n = a.size();
    int m = b.size();

    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    for (int i = 1; i < n + 1; i++)
    {
      for (int j = 1; j < m + 1; j++)
      {
        if (a[i - 1] == b[j - 1])
        {
          dp[i][j] = dp[i - 1][j - 1] + 1;
        }
        else
        {
          dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }
      }
    }

    std::cout << dp[n][m] << std::endl;
  }
}

class Wizard {
  public:
    Spell *cast()
    {
      Spell * spell;
      std::string s;
      int power;

      std::cin >> s;
      std::cin >> power;

      if(s == "fire")
      {
        spell = new Fireball(power);
      }
      else if(s == "frost")
      {
        spell = new Frostbite(power);
      }
      else if(s == "water")
      {
        spell = new Waterbolt(power);
      }
      else if(s == "thunder")
      {
        spell = new Thunderstorm(power);
      }
      else
      {
        spell = new Spell(s);
        std::cin >> SpellJournal::journal;
      }

      return spell;
    }
};

int main(int argc, const char * argv[])
{
  int T;
  Wizard Arawn;

  std::cin >> T;

  while(T--)
  {
    Spell *spell = Arawn.cast();
    counterspell(spell);
  }

  return 0;
}

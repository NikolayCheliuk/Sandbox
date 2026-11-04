/**
 * CloneGraph
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


// Definition for a Node.

class Node
{
  public:

    int val;
    std::vector<Node *> neighbors;

    Node()
    {
      val = 0;
      neighbors = std::vector<Node *>();
    }

    Node(int _val)
    {
      val = _val;
      neighbors = std::vector<Node *>();
    }

    Node(int _val, std::vector<Node *> _neighbors)
    {
      val = _val;
      neighbors = _neighbors;
    }
};


class Solution
{
  public:

    std::unordered_map<Node *, Node *> nodes;

    Node * cloneGraph(Node * node)
    {
      if (!node)
      {
        return node;
      }

      if (nodes[node] == nullptr)
      {
        nodes[node] = new Node(node->val);

        for (size_t i = 0; i < node->neighbors.size(); i++)
        {
          Node * clonned_neighbor = cloneGraph(node->neighbors[i]);
          nodes[node]->neighbors.push_back(clonned_neighbor);
        }
      }

      return nodes[node];
    }
};

void printGraph(Node * node)
{
  if (!node)
  {
    return;
  }

  std::unordered_map<Node *, bool> visited;
  std::queue<Node *> q;

  q.push(node);
  visited[node] = true;

  while (!q.empty())
  {
    Node * cur = q.front();
    q.pop();

    std::cout << cur->val << ": ";

    for (Node * n : cur->neighbors)
    {
      std::cout << n->val << " ";
      if (!visited[n])
      {
        visited[n] = true;
        q.push(n);
      }
    }

    std::cout << std::endl;
  }
}

int main(int argc, const char * argv[])
{
  // создаём узлы
  Node * n1 = new Node(1);
  Node * n2 = new Node(2);
  Node * n3 = new Node(3);
  Node * n4 = new Node(4);

  // связи (undirected)
  n1->neighbors = { n2, n4 };
  n2->neighbors = { n1, n3 };
  n3->neighbors = { n2, n4 };
  n4->neighbors = { n1, n3 };

  std::cout << "Original graph:" << std::endl;
  printGraph(n1);

  Solution s;
  Node * clone = s.cloneGraph(n1);

  std::cout << std::endl << "Cloned graph:" << std::endl;
  printGraph(clone);

  return 0;
}

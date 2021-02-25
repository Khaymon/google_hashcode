#include <deque>
#include <fstream>
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class Graph {
 private:
  int edges;
  std::vector<std::vector<std::tuple<int, int, std::string>>> graph;

 public:
  Graph() {}

  Graph(int vertex_count) {
    graph.assign(vertex_count,
                 std::vector<std::tuple<int, int, std::string>>());
  }

  Graph(int vertex_count, int edges) {
    graph.assign(vertex_count,
                 std::vector<std::tuple<int, int, std::string>>());
    this->edges = edges;
  }

  ~Graph() = default;

  void addEdge(int from, int to, int w, const std::string& name) {
    graph[from].emplace_back(to, w, name);
    graph[to].emplace_back(from, w, name);
  }

  bool isEdge(int from, int to) const {
    for (int i = 0; i < graph[from].size(); i++)
      if (get<0>(graph[from][i]) == to) return true;
  }

  int getEdgesCount() const { return edges; }

  int Size() const { return graph.size(); }

  const vector<tuple<int, int, string>>& operator[](int node) const {
    return graph[node];
  }
};

struct Car {
  std::vector<std::pair<int, int>> paths_;
  int full_time_;
};

enum class Color { WHITE, GRAY, BLACK };

void DFSUtil(const Graph& graph, int vertex, std::vector<Color>& color,
             int& general_time) {
  std::vector<int> parent(graph.Size(), vertex);
  std::stack<int> vStack;
  vStack.push(vertex);

  while (!vStack.empty()) {
    auto curr = vStack.top();

    if (color[curr] == Color::WHITE) {
      color[curr] = Color::GRAY;
      for (const auto& to : graph[curr]) {
        if (color[std::get<0>(to)] == Color::WHITE) {
          parent[std::get<0>(to)] = curr;
          vStack.push(std::get<0>(to));
        }
      }
    } else if (color[curr] == Color::GRAY) {
      vStack.pop();
      color[curr] = Color::BLACK;
      for (int i = 0; i < graph[parent[curr]].size(); ++i) {
        if (std::get<0>(graph[parent[curr]][i]) == curr) {
          general_time += std::get<1>(graph[parent[curr]][i]);
          break;
        }
      }
    } else {
      vStack.pop();
    }
  }
}

int ComputeCarTime(const Car& car, const Graph& graph) {
  int result = 0;
  for (const auto& path : car.paths_) {
    for (int i = 0; i < graph[path.first].size(); ++i) {
      if (std::get<0>(graph[path.first][i]) == path.second) {
        result += std::get<1>(graph[path.first][i]);
        break;
      }
    }
  }
  return result;
}

int main() {
  std::cout << "Enter data set name: \n";
  std::string filename;
  std::cin >> filename;
  int sim_duration = 0;
  int intersection_num = 0;
  int num_of_streets = 0;
  int num_of_cars = 0;
  int points = 0;

  ifstream input_file;
  input_file.open(filename);

  input_file >> sim_duration >> intersection_num >> num_of_streets >>
      num_of_cars >> points;

  int a = 0;
  int b = 0;
  std::string street_name;
  int time = 0;

  Graph graph{intersection_num, num_of_streets};
  std::unordered_map<std::string, std::tuple<int, int, int>> city(
      num_of_streets);

  std::vector<int> input_flow(intersection_num, 0);

  for (int i = 0; i < num_of_streets; ++i) {
    input_file >> a >> b >> street_name >> time;
    std::cout << street_name << '\n';
    graph.addEdge(a, b, time, street_name);
    city[street_name] = std::tie(a, b, time);
    ++input_flow[b];
  }

  std::vector<Car> cars(num_of_cars);
  Car car;
  int path_size = 0;

  for (int i = 0; i < num_of_cars; ++i) {
    input_file >> path_size;
    for (int j = 0; j < path_size; ++j) {
      input_file >> street_name;
      car.paths_.emplace_back(std::get<0>(city[street_name]),
                              std::get<1>(city[street_name]));
      car.full_time_ = ComputeCarTime(car, graph);
      if (car.full_time_ <= sim_duration) {
        cars[i] = car;
      }
    }
  }

  auto compare = [](const Car& lhs, const Car& rhs) {
    return lhs.full_time_ < rhs.full_time_;
  };

  std::set<Car, decltype(compare)> sorted_cars(compare);

  return 0;
}

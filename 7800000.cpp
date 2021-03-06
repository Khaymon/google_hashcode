#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>
#include <stack>

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

    void addEdge(int from, int to, int w, const std::string &name) {
        graph[from].emplace_back(to, w, name);
        graph[to].emplace_back(from, w, name);
    }

    bool isEdge(int from, int to) const {
        for (int i = 0; i < graph[from].size(); i++)
            if (get<0>(graph[from][i]) == to) return true;
    }

    int getEdgesCount() const { return edges; }

    int Size() const { return graph.size(); }

    const vector<tuple<int, int, string>> &GetNeighbours(int node) const {
        return graph[node];
    }

    const vector<tuple<int, int, string>> &operator[](int node) const {
        return graph[node];
    }
};


enum class Color {
    WHITE, GRAY, BLACK
};

void DFSUtil(const Graph &graph, int vertex, std::vector<Color> &color) {
    std::vector<int> parent(graph.Size(), vertex);
    std::stack<int> vStack;
    vStack.push(vertex);

    while (!vStack.empty()) {
        auto curr = vStack.top();

        if (color[curr] == Color::WHITE) {
            color[curr] = Color::GRAY;

//            for (const auto& to : graph[curr]) {
//                if (color[to] == Color::WHITE) {
//                    parent[to] = curr;
//                    vStack.push(to);
//                }
//            }
        } else if (color[curr] == Color::GRAY) {
            vStack.pop();
            color[curr] = Color::BLACK;
            //Visit(graph, curr, parent);
        } else {
            vStack.pop();
        }
    }
}

struct Car {
    std::vector<std::pair<int, int>> paths_;
    int full_time_;
};

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
    for (int i = 0; i < 6; ++i) {
        std::string filename = "a.txt";
        filename[0] = 'a' + i;
        int sim_duration = 0;
        int intersection_num = 0;
        int num_of_streets = 0;
        int num_of_cars = 0;
        int points = 0;

        ifstream input_file;
        input_file.open(filename);

        input_file >> sim_duration >> intersection_num >> num_of_streets >>
                   num_of_cars >> points;
        std::vector<int> number(intersection_num, 0);
        std::vector<std::vector<string>> names(intersection_num);
        vector<vector<pair<bool, int>>> times(num_of_streets, vector<pair<bool, int>>(num_of_streets, make_pair(false, 0)));
        std::string filename_output = "output-";
        filename_output += filename;
        ofstream output;
        output.open(filename_output);
        output << intersection_num ;
        int a = 0;
        int b = 0;
        std::string street_name;
        int time = 0;

        Graph graph{intersection_num, num_of_streets};
        std::unordered_map<std::string, std::tuple<int, int, int>> city(
                num_of_streets);

        for (int i = 0; i < num_of_streets; ++i) {
            input_file >> a >> b >> street_name >> time;

            graph.addEdge(a, b, time, street_name);
            number[b]++;
            names[b].push_back(street_name);


            city[street_name] = std::tie(a, b, time);
        }

        std::vector<Car> cars(num_of_cars);
        Car car;
        int path_size = 0;

        for (int i = 0; i < num_of_cars; ++i) {
            input_file >> path_size;
            int all_time = 0;
            for (int j = 0; j < path_size; ++j) {
                input_file >> street_name;
                car.paths_.emplace_back(std::get<0>(city[street_name]),
                                        std::get<1>(city[street_name]));
                if (!times[std::get<1>(city[street_name])][std::get<0>(city[street_name])].first) {
                    all_time += std::get<2>(city[street_name]);
                times[std::get<1>(city[street_name])][std::get<0>(city[street_name])].first = true;
                times[std::get<1>(city[street_name])][std::get<0>(city[street_name])].second += std::get<2>(city[street_name]);
                }
                car.full_time_ = ComputeCarTime(car, graph);
                if (car.full_time_ <= sim_duration) {
                    cars[i] = car;
                }
            }
        }

        for (int j = 0; j < intersection_num; ++j) {
            output << '\n' << j ;
            output << '\n'<< number[j] ;
            for (int k = 0 ; k < number[j]; k++) {
                output << '\n'<< names[j][k] << " " << max(times[j][k].second, 1);
            }
        }
        input_file.close();
    }

    //std::cout << sim_duration << intersection_num << num_of_streets << num_of_cars << points << std::endl;
    return 0;
}

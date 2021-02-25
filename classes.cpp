#include <iostream>
#include <vector>

class Street {
  std::string index;
  int from;
  int to;
  int time;
};

class Intersection {
  std::vector<int> input_streets_; // indexes of streets
  std::vector<int> output_streets_;
  std::vector< std::pair<int, int> > lights_; // first - index of the street, second - index of light
};

class Light {
  int time_;
  int street_index_;
};

class Schedule {
  std::vector<int> lights_; // indexes of lights;
};

class Car {
  std::vector<int> path_streets_; // indexes of streets in path
  int start_street_; // index of starting street
};
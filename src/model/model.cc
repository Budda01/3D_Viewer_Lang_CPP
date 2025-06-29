#include "model.h"

namespace s21 {

Model::Model()
    : count_of_vertices(0),
      count_of_facets(0),
      rotation_x(0.0),
      rotation_y(0.0),
      rotation_z(0.0) {}

Model::~Model() { ClearData(); }

void Model::CountVerticesAndFacets(const std::string& file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
  std::string line;
  count_of_vertices = 0;
  count_of_facets = 0;
  while (std::getline(file, line)) {
    if (line.substr(0, 2) == "v ") {
      count_of_vertices++;
    } else if (line.substr(0, 2) == "f ") {
      std::istringstream iss(line.substr(2));
      int count_vertices_in_facets = 0;
      std::string token;
      while (iss >> token) {
        count_vertices_in_facets++;
      }
      if (count_vertices_in_facets == 2) {
        count_of_facets += count_vertices_in_facets - 1;
      } else if (count_vertices_in_facets > 2) {
        count_of_facets += count_vertices_in_facets - 2;
      }
    }
  }
  file.close();
}

void Model::ParseModelData(const std::string& file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
  std::string line;
  int vertex_index = 1, facet_index = 1, count_vertex = 0;
  double x = 0, y = 0, z = 0;
  matrix_3d.resize(count_of_vertices + 1, std::vector<double>(3, 0.0));
  polygons.resize(count_of_facets + 1);
  while (std::getline(file, line)) {
    if (line.substr(0, 2) == "v ") {
      std::istringstream iss(line.substr(2));
      std::vector<double> coords;
      double coord = 0;
      while (iss >> coord) {
        coords.push_back(coord);
      }
      if (coords.size() != 3) {
        throw std::runtime_error("Each vertex must have exactly 3 coordinates");
      }
      x = coords[0];
      y = coords[1];
      z = coords[2];
      if (vertex_index > count_of_vertices) {
        throw std::runtime_error("Vertex index out of bounds");
      }
      matrix_3d[vertex_index][0] = x;
      matrix_3d[vertex_index][1] = y;
      matrix_3d[vertex_index][2] = z;
      vertex_index++;
      count_vertex++;
    } else if (line.substr(0, 2) == "f ") {
      std::istringstream iss(line.substr(2));
      std::string token;
      int count_vertex_in_facets = 0;
      while (iss >> token) {
        int current_vertex_index = std::stoi(token);
        if (current_vertex_index < 0) {
          current_vertex_index = count_vertex + 1 + current_vertex_index;
        }
        if (current_vertex_index == 0 || current_vertex_index > vertex_index) {
          throw std::runtime_error("Invalid vertex index");
        } else {
          polygons[facet_index].vertices.push_back(current_vertex_index);
          count_vertex_in_facets++;
        }
      }
      polygons[facet_index].count_vertices_in_facets = count_vertex_in_facets;
      if (count_vertex_in_facets > 1) {
        facet_index++;
      }
    }
  }
  file.close();
}

void Model::RotateModel(double step, char xyz) {
  switch (xyz) {
    case 'x':
      rotation_x = step;
      break;
    case 'y':
      rotation_y = step;
      break;
    case 'z':
      rotation_z = step;
      break;
    case 'X':
      rotation_x = -step;
      break;
    case 'Y':
      rotation_y = -step;
      break;
    case 'Z':
      rotation_z = -step;
      break;
    default:
      break;
  }
}

void Model::RotatePoint(std::vector<double>& point, double angle, char xyz) {
  double cos_result = cos(angle);
  double sin_result = sin(angle);
  double temp = 0;
  switch (xyz) {
    case 'x':
      temp = point[1] * cos_result - point[2] * sin_result;
      point[2] = point[1] * sin_result + point[2] * cos_result;
      point[1] = temp;
      break;
    case 'y':
      temp = point[0] * cos_result + point[2] * sin_result;
      point[2] = -point[0] * sin_result + point[2] * cos_result;
      point[0] = temp;
      break;
    case 'z':
      temp = point[0] * cos_result - point[1] * sin_result;
      point[1] = point[0] * sin_result + point[1] * cos_result;
      point[0] = temp;
      break;
  }
}

void Model::ApplyRotation() {
  for (int i = 1; i <= count_of_vertices; i++) {
    RotatePoint(matrix_3d[i], rotation_x, 'x');
    RotatePoint(matrix_3d[i], rotation_y, 'y');
    RotatePoint(matrix_3d[i], rotation_z, 'z');
  }
  rotation_x = 0.0;
  rotation_y = 0.0;
  rotation_z = 0.0;
}

void Model::MoveModel(double distance, char xyz) {
  switch (xyz) {
    case 'x':
      for (int i = 1; i <= count_of_vertices; i++) {
        matrix_3d[i][0] += distance;
      }
      break;
    case 'y':
      for (int i = 1; i <= count_of_vertices; i++) {
        matrix_3d[i][1] += distance;
      }
      break;
    case 'z':
      for (int i = 1; i <= count_of_vertices; i++) {
        matrix_3d[i][2] += distance;
      }
      break;
    case 'X':
      for (int i = 1; i <= count_of_vertices; i++) {
        matrix_3d[i][0] -= distance;
      }
      break;
    case 'Y':
      for (int i = 1; i <= count_of_vertices; i++) {
        matrix_3d[i][1] -= distance;
      }
      break;
    case 'Z':
      for (int i = 1; i <= count_of_vertices; i++) {
        matrix_3d[i][2] -= distance;
      }
      break;
    default:
      break;
  }
}

void Model::CenterModel() {
  const auto& vertices = this->GetMatrix3D();
  if (vertices.empty()) {
    throw std::runtime_error("Empty model");
  }

  double center_x = 0.0;
  double center_y = 0.0;
  double center_z = 0.0;

  for (const auto& vertex : vertices) {
    center_x += vertex[0];
    center_y += vertex[1];
    center_z += vertex[2];
  }

  int vertex_count = this->GetVertexCount();
  center_x /= vertex_count;
  center_y /= vertex_count;
  center_z /= vertex_count;

  this->MoveModel(-center_x, 'x');
  this->MoveModel(-center_y, 'y');
  this->MoveModel(-center_z, 'z');
}

void Model::ScaleModelToFit(double scale_factor) {
  const auto& vertices = this->GetMatrix3D();
  if (vertices.empty()) {
    throw std::runtime_error("Empty model");
  }

  double max_distance = 0.0;
  for (const auto& vertex : vertices) {
    double distance =
        sqrt(pow(vertex[0], 2) + pow(vertex[1], 2) + pow(vertex[2], 2));
    if (distance > max_distance) {
      max_distance = distance;
    }
  }

  if (max_distance > 0.0) {
    double scale = scale_factor / max_distance;
    for (auto& vertex :
         const_cast<std::vector<std::vector<double>>&>(vertices)) {
      vertex[0] *= scale;
      vertex[1] *= scale;
      vertex[2] *= scale;
    }
  }
}

void Model::ClearData() {
  matrix_3d.clear();
  polygons.clear();
  count_of_vertices = 0;
  count_of_facets = 0;
  rotation_x = 0.0;
  rotation_y = 0.0;
  rotation_z = 0.0;
}

}  // namespace s21

#ifndef CPP4_S21_3DVIEWER_V2_SRC_MODEL_MODEL_H
#define CPP4_S21_3DVIEWER_V2_SRC_MODEL_MODEL_H

#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace s21 {

struct Facet {
  std::vector<int> vertices;
  int count_vertices_in_facets;
};

class Model {
 public:
  Model();
  ~Model();

  void CountVerticesAndFacets(const std::string& file_path);
  void ParseModelData(const std::string& file_path);
  void RotateModel(double step, char xyz);
  void ApplyRotation();
  void MoveModel(double distance, char xyz);
  void CenterModel();
  void ScaleModelToFit(double scale_factor);
  void ClearData();

  int GetVertexCount() const { return count_of_vertices; }
  int GetFacetCount() const { return count_of_facets; }
  const std::vector<std::vector<double>>& GetMatrix3D() const {
    return matrix_3d;
  }
  void SetMatrix3D(const std::vector<std::vector<double>>& matrix) {
    matrix_3d = matrix;
  }
  const std::vector<Facet>& GetPolygons() const { return polygons; }

 private:
  void RotatePoint(std::vector<double>& point, double angle, char xyz);

  int count_of_vertices = 0;
  int count_of_facets = 0;
  std::vector<std::vector<double>> matrix_3d;
  std::vector<Facet> polygons;
  double rotation_x;
  double rotation_y;
  double rotation_z;
};

}  // namespace s21

#endif  // CPP4_S21_3DVIEWER_V2_SRC_MODEL_MODEL_H

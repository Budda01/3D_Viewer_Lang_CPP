#include <gtest/gtest.h>

#include "model/model.h"

namespace s21 {

class ModelTest : public ::testing::Test {
 protected:
  Model* model;

  void SetUp() override { model = new Model(); }

  void TearDown() override { delete model; }
};

TEST_F(ModelTest, CountVerticesAndFacets) {
  model->CountVerticesAndFacets("obj/skull.obj");
  EXPECT_EQ(model->GetVertexCount(), 6122);
  EXPECT_EQ(model->GetFacetCount(), 9537);
}

TEST_F(ModelTest, CountVerticesAndFacets2) {
  model->CountVerticesAndFacets("obj/rathalos.obj");
  EXPECT_EQ(model->GetVertexCount(), 64656);
  EXPECT_EQ(model->GetFacetCount(), 194035);
}

TEST_F(ModelTest, CountVerticesAndFacetsIncorrect) {
  EXPECT_THROW(model->CountVerticesAndFacets("obj/skulll.obj"), std::exception);
}

TEST_F(ModelTest, ParseModelDataTest) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);

  int expected_vertex_count = 8;
  int expected_facet_count = 12;

  EXPECT_EQ(model->GetVertexCount(), expected_vertex_count);
  EXPECT_EQ(model->GetFacetCount(), expected_facet_count);

  model->ParseModelData(file_path);

  const auto& vertices = model->GetMatrix3D();
  const auto& facets = model->GetPolygons();

  std::vector<std::vector<double>> expected_vertices = {
      {0.0, 0.0, 0.0},
      {1.000000, 1.000000, -1.000000},
      {1.000000, -1.000000, -1.000000},
      {1.000000, 1.000000, 1.000000},
      {1.000000, -1.000000, 1.000000},
      {-1.000000, 1.000000, -1.000000},
      {-1.000000, -1.000000, -1.000000},
      {-1.000000, 1.000000, 1.000000},
      {-1.000000, -1.000000, 1.000000}};

  ASSERT_EQ(vertices.size(), expected_vertices.size());
  for (size_t i = 1; i < vertices.size(); ++i) {
    EXPECT_EQ(vertices[i], expected_vertices[i]);
  }

  std::vector<Facet> expected_facets = {{},
                                        {{5, 3, 1}, 3},
                                        {{3, 8, 4}, 3},
                                        {{7, 6, 8}, 3},
                                        {{2, 8, 6}, 3},
                                        {{1, 4, 2}, 3},
                                        {{5, 2, 6}, 3},
                                        {{5, 7, 3}, 3},
                                        {{3, 7, 8}, 3},
                                        {{7, 5, 6}, 3},
                                        {{2, 4, 8}, 3},
                                        {{1, 3, 4}, 3},
                                        {{5, 1, 2}, 3}};

  ASSERT_EQ(facets.size(), expected_facets.size());
  for (size_t i = 1; i < facets.size(); ++i) {
    EXPECT_EQ(facets[i].vertices, expected_facets[i].vertices);
    EXPECT_EQ(facets[i].count_vertices_in_facets,
              expected_facets[i].count_vertices_in_facets);
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, ParseModelDataTest2) {
  std::string file_path = "obj/skull.obj";

  model->CountVerticesAndFacets(file_path);

  int expected_vertex_count = 6122;
  int expected_facet_count = 9537;

  EXPECT_EQ(model->GetVertexCount(), expected_vertex_count);
  EXPECT_EQ(model->GetFacetCount(), expected_facet_count);

  model->ParseModelData(file_path);
  EXPECT_NO_THROW(model->ParseModelData(file_path));

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, ParseModelDataIncorrect) {
  EXPECT_THROW(model->ParseModelData("obj/skulll.obj"), std::exception);
}

TEST_F(ModelTest, ParseModelDataIncorrect2) {
  EXPECT_THROW(model->ParseModelData("obj/invalid_vertex_index.obj"),
               std::runtime_error);
}

TEST_F(ModelTest, ParseModelDataIncorrect3) {
  EXPECT_THROW(model->ParseModelData("obj/less_than_3_coordinates.obj"),
               std::runtime_error);
}

TEST_F(ModelTest, ParseModelDataIncorrect5) {
  std::string file_path = "obj/invalid_vertex_index.obj";

  model->CountVerticesAndFacets(file_path);

  int expected_vertex_count = 4;
  int expected_facet_count = 4;

  EXPECT_EQ(model->GetVertexCount(), expected_vertex_count);
  EXPECT_EQ(model->GetFacetCount(), expected_facet_count);

  EXPECT_THROW(model->ParseModelData(file_path), std::runtime_error);

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, RotateModelTestX) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_x = {
      {0.0, 0.0, 0.0},
      {1.000000, 1.000000, 1.000000},
      {1.000000, 1.000000, -1.000000},
      {1.000000, -1.000000, 1.000000},
      {1.000000, -1.000000, -1.000000},
      {-1.000000, 1.000000, 1.000000},
      {-1.000000, 1.000000, -1.000000},
      {-1.000000, -1.000000, 1.000000},
      {-1.000000, -1.000000, -1.000000}};

  model->RotateModel(M_PI / 2, 'x');
  model->ApplyRotation();

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_x.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_x[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, RotateModelTestY) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_y = {
      {0.0, 0.0, 0.0},
      {-1.000000, 1.000000, -1.000000},
      {-1.000000, -1.000000, -1.000000},
      {1.000000, 1.000000, -1.000000},
      {1.000000, -1.000000, -1.000000},
      {-1.000000, 1.000000, 1.000000},
      {-1.000000, -1.000000, 1.000000},
      {1.000000, 1.000000, 1.000000},
      {1.000000, -1.000000, 1.000000}};

  model->RotateModel(M_PI / 2, 'y');
  model->ApplyRotation();

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_y.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_y[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, RotateModelTestZ) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_z = {
      {0.0, 0.0, 0.0},
      {-1.000000, 1.000000, -1.000000},
      {1.000000, 1.000000, -1.000000},
      {-1.000000, 1.000000, 1.000000},
      {1.000000, 1.000000, 1.000000},
      {-1.000000, -1.000000, -1.000000},
      {1.000000, -1.000000, -1.000000},
      {-1.000000, -1.000000, 1.000000},
      {1.000000, -1.000000, 1.000000}};

  model->RotateModel(M_PI / 2, 'z');
  model->ApplyRotation();

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_z.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_z[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, RotateModelTestXX) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_x = {
      {0.0, 0.0, 0.0},
      {1.000000, -1.000000, -1.000000},
      {1.000000, -1.000000, 1.000000},
      {1.000000, 1.000000, -1.000000},
      {1.000000, 1.000000, 1.000000},
      {-1.000000, -1.000000, -1.000000},
      {-1.000000, -1.000000, 1.000000},
      {-1.000000, 1.000000, -1.000000},
      {-1.000000, 1.000000, 1.000000}};

  model->RotateModel(M_PI / 2, 'X');
  model->ApplyRotation();

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_x.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_x[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, RotateModelTestYY) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_y = {
      {0.0, 0.0, 0.0},
      {1.000000, 1.000000, 1.000000},
      {1.000000, -1.000000, 1.000000},
      {-1.000000, 1.000000, 1.000000},
      {-1.000000, -1.000000, 1.000000},
      {1.000000, 1.000000, -1.000000},
      {1.000000, -1.000000, -1.000000},
      {-1.000000, 1.000000, -1.000000},
      {-1.000000, -1.000000, -1.000000}};

  model->RotateModel(M_PI / 2, 'Y');
  model->ApplyRotation();

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_y.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_y[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, RotateModelTestZZ) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_z = {
      {0.0, 0.0, 0.0},
      {1.000000, -1.000000, -1.000000},
      {-1.000000, -1.000000, -1.000000},
      {1.000000, -1.000000, 1.000000},
      {-1.000000, -1.000000, 1.000000},
      {1.000000, 1.000000, -1.000000},
      {-1.000000, 1.000000, -1.000000},
      {1.000000, 1.000000, 1.000000},
      {-1.000000, 1.000000, 1.000000}};

  model->RotateModel(M_PI / 2, 'Z');
  model->ApplyRotation();

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_z.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_z[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, RotateModelTestIncorrect) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_z = {
      {0.0, 0.0, 0.0},
      {1.000000, 1.000000, -1.000000},
      {1.000000, -1.000000, -1.000000},
      {1.000000, 1.000000, 1.000000},
      {1.000000, -1.000000, 1.000000},
      {-1.000000, 1.000000, -1.000000},
      {-1.000000, -1.000000, -1.000000},
      {-1.000000, 1.000000, 1.000000},
      {-1.000000, -1.000000, 1.000000}};

  model->RotateModel(M_PI / 2, 't');
  model->ApplyRotation();

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_z.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_z[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, MoveModelTestX) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_x = {
      {0.0, 0.0, 0.0},
      {3.000000, 1.000000, -1.000000},
      {3.000000, -1.000000, -1.000000},
      {3.000000, 1.000000, 1.000000},
      {3.000000, -1.000000, 1.000000},
      {1.000000, 1.000000, -1.000000},
      {1.000000, -1.000000, -1.000000},
      {1.000000, 1.000000, 1.000000},
      {1.000000, -1.000000, 1.000000}};

  model->MoveModel(2.0, 'x');

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_x.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_x[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, MoveModelTestY) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_y = {
      {0.0, 0.0, 0.0},
      {1.000000, 3.000000, -1.000000},
      {1.000000, 1.000000, -1.000000},
      {1.000000, 3.000000, 1.000000},
      {1.000000, 1.000000, 1.000000},
      {-1.000000, 3.000000, -1.000000},
      {-1.000000, 1.000000, -1.000000},
      {-1.000000, 3.000000, 1.000000},
      {-1.000000, 1.000000, 1.000000}};

  model->MoveModel(2.0, 'y');

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_y.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_y[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, MoveModelTestZ) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_z = {
      {0.0, 0.0, 0.0},
      {1.000000, 1.000000, 1.000000},
      {1.000000, -1.000000, 1.000000},
      {1.000000, 1.000000, 3.000000},
      {1.000000, -1.000000, 3.000000},
      {-1.000000, 1.000000, 1.000000},
      {-1.000000, -1.000000, 1.000000},
      {-1.000000, 1.000000, 3.000000},
      {-1.000000, -1.000000, 3.000000}};

  model->MoveModel(2.0, 'z');

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_z.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_z[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, MoveModelTestXX) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_x = {
      {0.0, 0.0, 0.0},
      {-1.000000, 1.000000, -1.000000},
      {-1.000000, -1.000000, -1.000000},
      {-1.000000, 1.000000, 1.000000},
      {-1.000000, -1.000000, 1.000000},
      {-3.000000, 1.000000, -1.000000},
      {-3.000000, -1.000000, -1.000000},
      {-3.000000, 1.000000, 1.000000},
      {-3.000000, -1.000000, 1.000000}};

  model->MoveModel(2.0, 'X');

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_x.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_x[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, MoveModelTestYY) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_y = {
      {0.0, 0.0, 0.0},
      {1.000000, -1.000000, -1.000000},
      {1.000000, -3.000000, -1.000000},
      {1.000000, -1.000000, 1.000000},
      {1.000000, -3.000000, 1.000000},
      {-1.000000, -1.000000, -1.000000},
      {-1.000000, -3.000000, -1.000000},
      {-1.000000, -1.000000, 1.000000},
      {-1.000000, -3.000000, 1.000000}};

  model->MoveModel(2.0, 'Y');

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_y.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_y[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, MoveModelTestZZ) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_z = {
      {0.0, 0.0, 0.0},
      {1.000000, 1.000000, -3.000000},
      {1.000000, -1.000000, -3.000000},
      {1.000000, 1.000000, -1.000000},
      {1.000000, -1.000000, -1.000000},
      {-1.000000, 1.000000, -3.000000},
      {-1.000000, -1.000000, -3.000000},
      {-1.000000, 1.000000, -1.000000},
      {-1.000000, -1.000000, -1.000000}};

  model->MoveModel(2.0, 'Z');

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_z.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_z[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, MoveModelTestIncorrect) {
  std::string file_path = "obj/cube.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_vertices_z = {
      {0.0, 0.0, 0.0},
      {1.000000, 1.000000, -1.000000},
      {1.000000, -1.000000, -1.000000},
      {1.000000, 1.000000, 1.000000},
      {1.000000, -1.000000, 1.000000},
      {-1.000000, 1.000000, -1.000000},
      {-1.000000, -1.000000, -1.000000},
      {-1.000000, 1.000000, 1.000000},
      {-1.000000, -1.000000, 1.000000}};

  model->MoveModel(2.0, 't');

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_vertices_z.size());

  double tolerance = 1e-5;
  for (size_t i = 1; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_vertices_z[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, CenterModelTest) {
  std::string file_path = "obj/pyramid.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  std::vector<std::vector<double>> expected_centered_vertices = {
      {0, 0, 0},          {2.12, 4.12, 2.4},   {-2.12, -4.12, -5.6},
      {2.12, -4.12, 2.4}, {-2.12, 4.12, -5.6}, {0, 0, 6.4},
  };

  model->CenterModel();

  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_centered_vertices.size());

  double tolerance = 1e-5;
  for (size_t i = 0; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_centered_vertices[i][j], tolerance);
    }
  }

  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, ScaleModelToFitTest) {
  std::string file_path = "obj/pyramid.obj";

  model->CountVerticesAndFacets(file_path);
  model->ParseModelData(file_path);

  double scale_factor = 10.0;

  std::vector<std::vector<double>> expected_scaled_vertices = {
      {0, 0, 0},        {2.65, 5.15, 5},   {-2.65, -5.15, -5},
      {2.65, -5.15, 5}, {-2.65, 5.15, -5}, {0, 0, 10}};

  double max_distance = 0.0;
  for (const auto& vertex : expected_scaled_vertices) {
    double distance =
        sqrt(pow(vertex[0], 2) + pow(vertex[1], 2) + pow(vertex[2], 2));
    if (distance > max_distance) {
      max_distance = distance;
    }
  }

  double scale = scale_factor / max_distance;

  for (auto& vertex : expected_scaled_vertices) {
    vertex[0] *= scale;
    vertex[1] *= scale;
    vertex[2] *= scale;
  }

  model->ScaleModelToFit(scale_factor);
  const auto& vertices = model->GetMatrix3D();
  ASSERT_EQ(vertices.size(), expected_scaled_vertices.size());

  double tolerance = 1e-5;
  for (size_t i = 0; i < vertices.size(); ++i) {
    for (size_t j = 0; j < 3; ++j) {
      EXPECT_NEAR(vertices[i][j], expected_scaled_vertices[i][j], tolerance);
    }
  }
  model->ClearData();
  EXPECT_EQ(model->GetVertexCount(), 0);
  EXPECT_EQ(model->GetFacetCount(), 0);
}

TEST_F(ModelTest, CenterModelEmpty) {
  model->ClearData();
  EXPECT_THROW(model->CenterModel(), std::runtime_error);
}

TEST_F(ModelTest, ScaleModelToFitEmpty) {
  model->ClearData();
  EXPECT_THROW(model->ScaleModelToFit(1.0), std::runtime_error);
}

}  // namespace s21

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#pragma once

#include <memory>
#include <optional>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include "pypalp/palp_types.hpp"
#include "pypalp/utils.hpp"

struct Polytope {

  // Everything that is defined in poly.c in PALP
  std::unique_ptr<CWS> CW;
  std::unique_ptr<Weight> W;
  std::unique_ptr<VertexNumList> V;
  std::unique_ptr<EqList> E;
  std::unique_ptr<EqList> DE;
  std::unique_ptr<BaHo> BH;
  std::unique_ptr<VaHo> VH;
  std::unique_ptr<PolyPointList> P;
  std::unique_ptr<PolyPointList> DP;
  std::unique_ptr<FaceInfo> FI;
  std::unique_ptr<PairMatStruct> PM;
  std::unique_ptr<PairMatStruct> DPM;
  std::unique_ptr<C5stats> C5S;

  // Cached results from computations
  bool ran_Find_Equations;
  bool ran_EL_to_PPL;
  bool ran_Sort_VL;
  bool ran_Make_VEPM;
  bool ran_Complete_Poly;
  std::optional<bool> is_ip_;
  std::optional<bool> is_reflexive_;
  std::optional<pybind11::array_t<Long>> vertices_;
  std::optional<pybind11::array_t<Long>> points_;

  Polytope(pybind11::array_t<int> const &matrix) {
    std::atexit(check_final_status);

    inFILE = std::tmpfile();
    outFILE = std::tmpfile();

    read_into_file(matrix, inFILE);

    CW = std::make_unique<CWS>();
    P = std::make_unique<PolyPointList>();

    Read_CWS_PP(CW.get(), P.get());

    std::fclose(inFILE);
    std::fclose(outFILE);
    outFILE = nullptr;

    ran_Find_Equations = false;
    ran_EL_to_PPL = false;
    ran_Sort_VL = false;
    ran_Make_VEPM = false;
    ran_Complete_Poly = false;
  }

  pybind11::array_t<Long> vertices() {
    if (vertices_) {
      return *vertices_;
    }

    std::atexit(check_final_status);
    outFILE = std::tmpfile();

    if (!V) {
      V = std::make_unique<VertexNumList>();
    }
    if (!E) {
      E = std::make_unique<EqList>();
    }
    if (!DP) {
      DP = std::make_unique<PolyPointList>();
    }

    if (!ran_Find_Equations) {
      is_ip_ = Find_Equations(P.get(), V.get(), E.get());
      ran_Find_Equations = true;
    }
    if (!ran_EL_to_PPL && *is_ip_) {
      is_reflexive_ = EL_to_PPL(E.get(), DP.get(), &P->n);
      ran_EL_to_PPL = true;
    }
    if (!ran_Sort_VL) {
      Sort_VL(V.get());
      ran_Sort_VL = true;
    }

    std::fclose(outFILE);
    outFILE = nullptr;

    ssize_t num_vert = V->nv;
    ssize_t dim = P->n;

    auto result = pybind11::array_t<Long>({num_vert, dim});

    auto buf = result.mutable_data();

    for (int i = 0; i < num_vert; i++) {
      for (int j = 0; j < dim; j++) {
        buf[i * dim + j] = P->x[V->v[i]][j];
      }
    }

    vertices_ = std::move(result);

    return *vertices_;
  }

  pybind11::array_t<Long> points() {
    if (points_) {
      return *points_;
    }

    std::atexit(check_final_status);
    outFILE = std::tmpfile();

    if (!V) {
      V = std::make_unique<VertexNumList>();
    }
    if (!E) {
      E = std::make_unique<EqList>();
    }
    if (!DP) {
      DP = std::make_unique<PolyPointList>();
    }
    if (!PM) {
      PM = std::make_unique<PairMatStruct>();
    }

    if (!ran_Find_Equations) {
      is_ip_ = Find_Equations(P.get(), V.get(), E.get());
      ran_Find_Equations = true;
    }
    if (!ran_EL_to_PPL && *is_ip_) {
      is_reflexive_ = EL_to_PPL(E.get(), DP.get(), &P->n);
      ran_EL_to_PPL = true;
    }
    if (!ran_Sort_VL) {
      Sort_VL(V.get());
      ran_Sort_VL = true;
    }
    if (!ran_Make_VEPM) {
      Make_VEPM(P.get(), V.get(), E.get(), PM->data);
      ran_Make_VEPM = true;
    }
    if (!ran_Complete_Poly) {
      Complete_Poly(PM->data, E.get(), V->nv, P.get());
      ran_Complete_Poly = true;
    }

    std::fclose(outFILE);
    outFILE = nullptr;

    ssize_t num_pts = P->np;
    ssize_t dim = P->n;

    auto result = pybind11::array_t<Long>({num_pts, dim});

    auto buf = result.mutable_data();

    for (int i = 0; i < num_pts; i++) {
      for (int j = 0; j < dim; j++) {
        buf[i * dim + j] = P->x[i][j];
      }
    }

    points_ = std::move(result);

    return *points_;
  }

  bool is_ip() {
    // This is computed in vertices()
    vertices();
    return *is_ip_;
  }

  bool is_reflexive() {
    // This is computed in vertices()
    vertices();
    // Need to also check if it is IP since is_reflexive_ is std::nullopt if
    // is_ip_ is false
    return *is_ip_ && *is_reflexive_;
  }
};

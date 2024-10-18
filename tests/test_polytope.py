from __future__ import annotations

import numpy as np

from pypalp import Polytope

def test_vertices():
    points = np.array([[1, 1], [-1, 1], [1, -1], [-1,-1]], dtype=np.int64)
    vertices = points
    sorted_vertices = sorted(tuple(pt) for pt in vertices)

    p = Polytope(points)
    computed_vertices = p.vertices()
    sorted_computed_vertices = sorted(tuple(pt) for pt in computed_vertices)

    print(f"vertices: {vertices}")
    print(f"computed_vertices: {computed_vertices}")

    assert len(computed_vertices) == 4
    assert sorted_computed_vertices == sorted_vertices

    points = np.array([[1, 1], [-1, 1], [1, -1], [-1,-1], [0, 1]], dtype=np.int64)
    vertices = points[:-1]
    sorted_vertices = sorted(tuple(pt) for pt in vertices)

    p = Polytope(points)
    computed_vertices = p.vertices()
    sorted_computed_vertices = sorted(tuple(pt) for pt in computed_vertices)

    print(f"vertices: {vertices}")
    print(f"computed_vertices: {computed_vertices}")

    assert len(computed_vertices) == 4
    assert sorted_computed_vertices == sorted_vertices

def test_points():
    vertices = np.array([[1, 1], [-1, 1], [1, -1], [-1,-1]], dtype=np.int64)
    points = np.array([[1, 1], [-1, 1], [1, -1], [-1,-1], [0, 0], [0,1], [0,-1], [1,0], [-1,0]], dtype=np.int64)
    sorted_points = sorted(tuple(pt) for pt in points)

    p = Polytope(vertices)
    computed_points = p.points()
    sorted_computed_points = sorted(tuple(pt) for pt in computed_points)

    assert len(computed_points) == 9
    assert sorted_computed_points == sorted_points

def test_ip():
    vertices = [[1,0,0,0],[0,1,0,0],[0,0,1,0],[0,0,0,1],[-1,-1,-1,-1]]

    p = Polytope(vertices)

    assert p.is_ip()

def test_reflexive():
    vertices = [[1,0,0,0],[0,1,0,0],[0,0,1,0],[0,0,0,1],[-1,-1,-1,-1]]

    p = Polytope(vertices)

    assert p.is_reflexive()

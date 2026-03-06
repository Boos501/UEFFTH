"""Tests for the vegetation module."""

import math
import pytest

from vegetation import VegetationManager, VegetationSpawner, VegetationType
from vegetation.manager import VegetationInstance


# ---------------------------------------------------------------------------
# VegetationType tests
# ---------------------------------------------------------------------------

class TestVegetationType:
    def test_default_values(self):
        vt = VegetationType(name="oak")
        assert vt.name == "oak"
        assert vt.density == 1.0
        assert vt.min_scale == 0.8
        assert vt.max_scale == 1.2
        assert vt.mesh_path is None
        assert vt.casts_shadow is True
        assert vt.collision_enabled is True

    def test_custom_values(self):
        vt = VegetationType(
            name="grass",
            density=5.0,
            min_scale=0.5,
            max_scale=1.5,
            mesh_path="/meshes/grass.fbx",
            casts_shadow=False,
            collision_enabled=False,
        )
        assert vt.density == 5.0
        assert vt.mesh_path == "/meshes/grass.fbx"

    def test_negative_density_raises(self):
        with pytest.raises(ValueError, match="density"):
            VegetationType(name="bad", density=-1.0)

    def test_zero_min_scale_raises(self):
        with pytest.raises(ValueError, match="min_scale"):
            VegetationType(name="bad", min_scale=0.0)

    def test_max_scale_less_than_min_raises(self):
        with pytest.raises(ValueError, match="max_scale"):
            VegetationType(name="bad", min_scale=1.5, max_scale=1.0)


# ---------------------------------------------------------------------------
# VegetationManager tests
# ---------------------------------------------------------------------------

class TestVegetationManager:
    def _make_instance(self, type_name: str, x: float = 0.0, y: float = 0.0) -> VegetationInstance:
        vt = VegetationType(name=type_name)
        return VegetationInstance(vegetation_type=vt, position=(x, y, 0.0))

    def test_add_and_count(self):
        mgr = VegetationManager()
        assert mgr.instance_count == 0
        mgr.add_instance(self._make_instance("tree"))
        assert mgr.instance_count == 1

    def test_get_instances_by_type(self):
        mgr = VegetationManager()
        mgr.add_instance(self._make_instance("tree"))
        mgr.add_instance(self._make_instance("bush"))
        mgr.add_instance(self._make_instance("tree"))
        trees = mgr.get_instances_by_type("tree")
        assert len(trees) == 2
        assert all(i.vegetation_type.name == "tree" for i in trees)

    def test_remove_instances_by_type(self):
        mgr = VegetationManager()
        mgr.add_instance(self._make_instance("tree"))
        mgr.add_instance(self._make_instance("bush"))
        mgr.add_instance(self._make_instance("tree"))
        removed = mgr.remove_instances_by_type("tree")
        assert removed == 2
        assert mgr.instance_count == 1

    def test_remove_nonexistent_type(self):
        mgr = VegetationManager()
        mgr.add_instance(self._make_instance("tree"))
        removed = mgr.remove_instances_by_type("ghost")
        assert removed == 0
        assert mgr.instance_count == 1

    def test_clear(self):
        mgr = VegetationManager()
        for _ in range(5):
            mgr.add_instance(self._make_instance("tree"))
        mgr.clear()
        assert mgr.instance_count == 0

    def test_get_instances_in_bounds(self):
        mgr = VegetationManager()
        mgr.add_instance(self._make_instance("tree", x=1.0, y=1.0))
        mgr.add_instance(self._make_instance("tree", x=5.0, y=5.0))
        mgr.add_instance(self._make_instance("tree", x=9.0, y=9.0))
        inside = mgr.get_instances_in_bounds(0.0, 6.0, 0.0, 6.0)
        assert len(inside) == 2

    def test_type_counts(self):
        mgr = VegetationManager()
        for _ in range(3):
            mgr.add_instance(self._make_instance("tree"))
        for _ in range(2):
            mgr.add_instance(self._make_instance("bush"))
        counts = mgr.type_counts()
        assert counts["tree"] == 3
        assert counts["bush"] == 2


# ---------------------------------------------------------------------------
# VegetationSpawner tests
# ---------------------------------------------------------------------------

class TestVegetationSpawner:
    def test_spawn_produces_instances(self):
        spawner = VegetationSpawner(width=10.0, height=10.0, seed=42)
        vt = VegetationType(name="tree", density=1.0)
        mgr = VegetationManager()
        count = spawner.spawn(vt, mgr)
        assert count > 0
        assert mgr.instance_count == count

    def test_spawn_zero_density_produces_no_instances(self):
        spawner = VegetationSpawner(width=10.0, height=10.0, seed=0)
        vt = VegetationType(name="empty", density=0.0)
        mgr = VegetationManager()
        count = spawner.spawn(vt, mgr)
        assert count == 0

    def test_higher_density_produces_more_instances(self):
        low_density_vt = VegetationType(name="sparse", density=0.5)
        high_density_vt = VegetationType(name="dense", density=4.0)

        mgr_low = VegetationManager()
        mgr_high = VegetationManager()
        spawner = VegetationSpawner(width=10.0, height=10.0, seed=1)
        spawner.spawn(low_density_vt, mgr_low)
        spawner.spawn(high_density_vt, mgr_high)

        assert mgr_high.instance_count > mgr_low.instance_count

    def test_spawn_respects_origin_offset(self):
        spawner = VegetationSpawner(width=5.0, height=5.0, seed=7)
        vt = VegetationType(name="tree", density=1.0)
        mgr = VegetationManager()
        spawner.spawn(vt, mgr, origin=(100.0, 200.0, 0.0))
        for inst in mgr.get_instances_by_type("tree"):
            assert inst.position[0] >= 100.0
            assert inst.position[1] >= 200.0

    def test_spawn_scale_within_bounds(self):
        spawner = VegetationSpawner(width=10.0, height=10.0, seed=3)
        vt = VegetationType(name="tree", min_scale=0.5, max_scale=2.0)
        mgr = VegetationManager()
        spawner.spawn(vt, mgr)
        for inst in mgr.get_instances_by_type("tree"):
            assert 0.5 <= inst.scale <= 2.0

    def test_replace_swaps_vegetation_type(self):
        spawner = VegetationSpawner(width=10.0, height=10.0, seed=5)
        old_vt = VegetationType(name="old_tree", density=1.0)
        new_vt = VegetationType(name="new_tree", density=1.0)
        mgr = VegetationManager()
        spawner.spawn(old_vt, mgr)

        assert mgr.instance_count > 0
        old_count = mgr.instance_count

        new_count = spawner.replace("old_tree", new_vt, mgr)

        assert mgr.get_instances_by_type("old_tree") == []
        assert mgr.instance_count == new_count

    def test_invalid_dimensions_raise(self):
        with pytest.raises(ValueError):
            VegetationSpawner(width=0.0, height=10.0)
        with pytest.raises(ValueError):
            VegetationSpawner(width=10.0, height=-1.0)

    def test_reproducible_with_seed(self):
        vt = VegetationType(name="tree", density=2.0)

        mgr1 = VegetationManager()
        VegetationSpawner(width=10.0, height=10.0, seed=99).spawn(vt, mgr1)

        mgr2 = VegetationManager()
        VegetationSpawner(width=10.0, height=10.0, seed=99).spawn(vt, mgr2)

        positions1 = [i.position for i in mgr1.get_instances_by_type("tree")]
        positions2 = [i.position for i in mgr2.get_instances_by_type("tree")]
        assert positions1 == positions2

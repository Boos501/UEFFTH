"""Vegetation spawner for distributing instances across a terrain area."""

import math
import random
from typing import List, Optional, Tuple

from .manager import VegetationInstance, VegetationManager
from .types import VegetationType


class VegetationSpawner:
    """Distributes vegetation instances across a rectangular area.

    Uses a simple Poisson-disc-inspired grid approach to ensure instances
    are spread according to each type's density value while avoiding
    exact regular grids.

    Args:
        width: Width of the spawn area in world units.
        height: Height of the spawn area in world units.
        seed: Optional random seed for reproducible results.
    """

    def __init__(
        self,
        width: float,
        height: float,
        seed: Optional[int] = None,
    ) -> None:
        if width <= 0 or height <= 0:
            raise ValueError("width and height must be positive")
        self._width = width
        self._height = height
        self._rng = random.Random(seed)

    # ------------------------------------------------------------------
    # Public API
    # ------------------------------------------------------------------

    def spawn(
        self,
        vegetation_type: VegetationType,
        manager: VegetationManager,
        origin: Tuple[float, float, float] = (0.0, 0.0, 0.0),
    ) -> int:
        """Spawn instances of *vegetation_type* into *manager*.

        Instances are placed on a jittered grid whose cell size is derived
        from the type's density so that denser types produce more instances.

        Args:
            vegetation_type: The type to spawn.
            manager: Destination manager that will own the new instances.
            origin: World-space offset applied to every spawned position.

        Returns:
            The number of instances that were added to *manager*.
        """
        positions = self._generate_positions(vegetation_type.density)
        count = 0
        for x, y in positions:
            scale = self._rng.uniform(
                vegetation_type.min_scale, vegetation_type.max_scale
            )
            yaw = self._rng.uniform(0.0, 360.0)
            instance = VegetationInstance(
                vegetation_type=vegetation_type,
                position=(origin[0] + x, origin[1] + y, origin[2]),
                scale=scale,
                rotation_yaw=yaw,
            )
            manager.add_instance(instance)
            count += 1
        return count

    def replace(
        self,
        old_type_name: str,
        new_type: VegetationType,
        manager: VegetationManager,
        origin: Tuple[float, float, float] = (0.0, 0.0, 0.0),
    ) -> int:
        """Replace all instances of *old_type_name* with *new_type* instances.

        Removes existing instances of the old type, then spawns fresh
        instances of the new type using this spawner's area settings.

        Args:
            old_type_name: Name of the vegetation type to replace.
            new_type: The replacement vegetation type.
            manager: The manager whose instances are being replaced.
            origin: World-space offset for newly spawned instances.

        Returns:
            The number of new instances added.
        """
        manager.remove_instances_by_type(old_type_name)
        return self.spawn(new_type, manager, origin)

    # ------------------------------------------------------------------
    # Internal helpers
    # ------------------------------------------------------------------

    def _generate_positions(self, density: float) -> List[Tuple[float, float]]:
        """Return a list of jittered-grid positions for the given density."""
        if density == 0:
            return []

        cell_size = 1.0 / math.sqrt(density)
        cols = max(1, int(self._width / cell_size))
        rows = max(1, int(self._height / cell_size))

        cell_w = self._width / cols
        cell_h = self._height / rows

        positions: List[Tuple[float, float]] = []
        for row in range(rows):
            for col in range(cols):
                jitter_x = self._rng.uniform(0.0, cell_w)
                jitter_y = self._rng.uniform(0.0, cell_h)
                x = col * cell_w + jitter_x
                y = row * cell_h + jitter_y
                positions.append((x, y))
        return positions

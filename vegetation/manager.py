"""Vegetation manager for tracking and querying vegetation instances."""

from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple

from .types import VegetationType


@dataclass
class VegetationInstance:
    """A single placed vegetation instance.

    Attributes:
        vegetation_type: The type of vegetation this instance represents.
        position: (x, y, z) world-space position.
        scale: Uniform scale factor for the instance.
        rotation_yaw: Yaw rotation in degrees around the vertical axis.
    """

    vegetation_type: VegetationType
    position: Tuple[float, float, float]
    scale: float = 1.0
    rotation_yaw: float = 0.0


class VegetationManager:
    """Manages a collection of vegetation instances.

    Provides methods to add, remove, and query instances by type or
    spatial bounds.
    """

    def __init__(self) -> None:
        self._instances: List[VegetationInstance] = []

    # ------------------------------------------------------------------
    # Mutation helpers
    # ------------------------------------------------------------------

    def add_instance(self, instance: VegetationInstance) -> None:
        """Register a new vegetation instance."""
        self._instances.append(instance)

    def remove_instances_by_type(self, type_name: str) -> int:
        """Remove all instances of the given type.

        Returns:
            The number of instances removed.
        """
        before = len(self._instances)
        self._instances = [
            inst for inst in self._instances if inst.vegetation_type.name != type_name
        ]
        return before - len(self._instances)

    def clear(self) -> None:
        """Remove all vegetation instances."""
        self._instances.clear()

    # ------------------------------------------------------------------
    # Query helpers
    # ------------------------------------------------------------------

    def get_instances_by_type(self, type_name: str) -> List[VegetationInstance]:
        """Return all instances that match the given type name."""
        return [
            inst for inst in self._instances if inst.vegetation_type.name == type_name
        ]

    def get_instances_in_bounds(
        self,
        x_min: float,
        x_max: float,
        y_min: float,
        y_max: float,
    ) -> List[VegetationInstance]:
        """Return all instances whose XY position falls within the given bounds."""
        return [
            inst
            for inst in self._instances
            if x_min <= inst.position[0] <= x_max and y_min <= inst.position[1] <= y_max
        ]

    @property
    def instance_count(self) -> int:
        """Total number of managed instances."""
        return len(self._instances)

    def type_counts(self) -> Dict[str, int]:
        """Return a mapping of type name -> instance count."""
        counts: Dict[str, int] = {}
        for inst in self._instances:
            counts[inst.vegetation_type.name] = (
                counts.get(inst.vegetation_type.name, 0) + 1
            )
        return counts

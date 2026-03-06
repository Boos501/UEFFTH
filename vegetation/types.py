"""Vegetation type definitions."""

from dataclasses import dataclass, field
from typing import Optional


@dataclass
class VegetationType:
    """Defines the properties of a vegetation type (e.g. tree, bush, grass).

    Attributes:
        name: Unique identifier for this vegetation type.
        density: Expected number of instances per unit area (default 1.0).
        min_scale: Minimum scale factor applied to instances (default 0.8).
        max_scale: Maximum scale factor applied to instances (default 1.2).
        mesh_path: Optional path to the mesh asset used for rendering.
        casts_shadow: Whether instances cast shadows (default True).
        collision_enabled: Whether instances have collision (default True).
    """

    name: str
    density: float = 1.0
    min_scale: float = 0.8
    max_scale: float = 1.2
    mesh_path: Optional[str] = None
    casts_shadow: bool = True
    collision_enabled: bool = True

    def __post_init__(self) -> None:
        if self.density < 0:
            raise ValueError("density must be non-negative")
        if self.min_scale <= 0:
            raise ValueError("min_scale must be positive")
        if self.max_scale < self.min_scale:
            raise ValueError("max_scale must be >= min_scale")

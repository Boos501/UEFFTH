"""
Vegetation module - replacement for extended vegetation functionality.

Provides tools for defining, spawning, and managing vegetation instances
across a terrain or scene.
"""

from .types import VegetationType
from .manager import VegetationManager
from .spawner import VegetationSpawner

__all__ = ["VegetationType", "VegetationManager", "VegetationSpawner"]

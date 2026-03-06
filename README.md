# UEFFTH

Replacement for the extended vegetation feature — provides a lightweight, pure-Python system for defining, spawning, and managing vegetation instances across a terrain or scene.

## Modules

### `vegetation.VegetationType`
Describes a single category of vegetation (e.g. tree, bush, grass):

| Field | Type | Default | Description |
|---|---|---|---|
| `name` | `str` | — | Unique identifier |
| `density` | `float` | `1.0` | Instances per unit area |
| `min_scale` | `float` | `0.8` | Minimum scale factor |
| `max_scale` | `float` | `1.2` | Maximum scale factor |
| `mesh_path` | `str \| None` | `None` | Path to mesh asset |
| `casts_shadow` | `bool` | `True` | Whether instances cast shadows |
| `collision_enabled` | `bool` | `True` | Whether instances have collision |

### `vegetation.VegetationManager`
Tracks all active vegetation instances and supports spatial and type-based queries.

```python
from vegetation import VegetationManager, VegetationType
from vegetation.manager import VegetationInstance

mgr = VegetationManager()
vt  = VegetationType(name="oak", density=2.0)
mgr.add_instance(VegetationInstance(vegetation_type=vt, position=(10.0, 5.0, 0.0)))
print(mgr.instance_count)          # 1
print(mgr.type_counts())           # {'oak': 1}
```

### `vegetation.VegetationSpawner`
Distributes instances across a rectangular area using a jittered grid algorithm derived from each type's density.

```python
from vegetation import VegetationManager, VegetationSpawner, VegetationType

spawner = VegetationSpawner(width=100.0, height=100.0, seed=42)
mgr = VegetationManager()

tree = VegetationType(name="pine", density=0.5)
spawner.spawn(tree, mgr)
print(mgr.instance_count)   # ~50 trees spread across the 100×100 area

# Replace all "pine" instances with "oak"
oak = VegetationType(name="oak", density=0.5)
spawner.replace("pine", oak, mgr)
```

## Running the tests

```bash
python -m pytest tests/
```
import sys
import os
import json
from pathlib import Path
from attrs import define
from tqdm import tqdm


sys.path.append(os.path.abspath("/home/kama/Projects/iq_rings/build/"))
from native_rings_engine import Exercise as NativeExercise




@define
class DetailState:
    row: int
    column: int
    rotation: int
    side: bool

    @classmethod
    def from_json_dict(cls, d: dict):
        return cls(
            row=d["position"][0], column=d["position"][-1],
            rotation=d["rotation"], side=d["isFrontSide"]
        )
    
    def __hash__(self) -> int:
        return hash(str(self))

    def __str__(self) -> str:
        return f"{self.row}_{self.column}_{self.rotation}_{self.side}"


class Exercise:
    def __init__(self, config_path: str | Path, pb = None) -> None:
        with open(config_path) as f:
            cfg_str = f.read()
            cfg = json.loads(cfg_str)
        self._native_exercise = NativeExercise(cfg_str)
        self.size = len(cfg["details"])
        self.free_details: set[int] = set(range(self.size))
        self.rows = cfg["field"]["rows"]
        self.columns = cfg["field"]["columns"]
        self.cells: set[tuple[int, int]] = {
            (y, x) for y in range(self.rows) for x in range(self.columns)
        }
        self.pb: tqdm = pb


    def insert(self, detail: int, state: DetailState) -> bool:
        fits = self._native_exercise.tryInsert(
            detail, state.row, state.column, state.rotation, state.side
        )
        if fits:
            self.free_details.remove(detail)
        self.pb.update(1)
        return fits
    
    def remove(self, detail: int) -> None:
        if detail in self.free_details:
            return
        self.free_details.add(detail)
        self._native_exercise.removeDetail(detail)
    
    def is_detail_connected(self, detail: int) -> bool:
        return self._native_exercise.isDetailConnected(detail)

    def is_packed(self) -> bool:
        return self._native_exercise.allActive()
    
    def is_connected(self) -> bool:
        return self._native_exercise.isConnected()
    
    @property
    def occupied_cells(self) -> set[tuple[int, int]]:
        return self._native_exercise.getOccupiedCells()
    
    @property
    def vacant_cells(self) -> set[tuple[int, int]]:
        return self._native_exercise.getVacantCells()
    
    @property
    def free_cells(self) -> set[tuple[int, int]]:
        return self.cells - self.occupied_cells
    


    
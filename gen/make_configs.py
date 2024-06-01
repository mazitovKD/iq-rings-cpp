from copy import deepcopy
import json
from pathlib import Path


def make_complited_exercise(src_path: str | Path, dst_path: str | Path):
    src_path = Path(src_path)
    with open(src_path, "r") as fp:
        exercise_cfg = json.load(fp)
    cfg: dict = deepcopy(exercise_cfg)
    details_dir = src_path.parent / cfg.pop("detailsDir")
    for d in cfg["details"]:
        d: dict
        d["state"] = {
            key: d.pop(key)
            for key in ("position", "rotation", "isFrontSide") 
        }
        with open(details_dir / (d["name"] + ".json"), "r") as fp:
            d["form"] = json.load(fp)
    
    with open(dst_path, "w") as fp:
        json.dump(
            cfg, fp, indent=4, 
        )


if __name__ == "__main__":
    make_complited_exercise(
        "/home/kama/Projects/iq_rings/configs/exercises/junior25.json",
        "/home/kama/Projects/iq_rings/configs/complited_exercises/junior25.json"
    )

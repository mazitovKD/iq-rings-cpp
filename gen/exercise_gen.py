from typing import Generator, Any
import pickle
import uuid
import json
from pathlib import Path
from attrs import define
from tqdm import tqdm


from gen.native_engine import Exercise, DetailState

CONFIG_PATH = "/home/kama/Projects/iq_rings/configs/complited_exercises/junior25.json"
SAVE_PATH = Path("/home/kama/Projects/data/")


    
def iter_states(cells: set[tuple[int, int]]) -> Generator[DetailState, Any, None]:
    for row, column in cells:
        for rotation in range(6):
            for side in (True, False):
                 yield DetailState(row, column, rotation, side)



def search_connected_packing(
    ex: Exercise, detail: int, state: DetailState, arrangement: list[dict[str, int | str]]
):
    fits = ex.insert(detail, state)
    if not fits:
        return
    connected = ex.is_detail_connected(detail)
    if not connected:
        # print()
        ex.remove(detail)
        return
    arrangement.append({"d": detail, "s": str(state)})
    if ex.is_packed():
        with open(SAVE_PATH / f"{uuid.uuid1()}.json", "w") as fp:
            json.dump(arrangement, fp, indent=2)
    else:
        for d in ex.free_details:
            for cell in ex.free_cells:
                for r in range(6):
                    for side in (True, False):
                        search_connected_packing(ex, d, DetailState(*cell, r, side), arrangement)

    arrangement.pop()
    ex.remove(detail)


def set_arrangement(ex: Exercise, arrangement: tuple[DetailState], start_det: int):
    for d in range(start_det, len(arrangement)):
        ex.insert(d, arrangement[d])


def reset_arrangement(ex: Exercise, start_det: int):
    for d in range(start_det, ex.size):
        ex.remove(d)


def arrangement_traversal(ex: Exercise, start_arrangement: tuple[DetailState], save_dir: Path):
    set_arrangement(ex, start_arrangement, 0)

    start_det = len(start_arrangement)
    for curr_det in range(start_det, ex.size):
        prev_arrag = curr_arrang
        curr_arrang = set()
        for states in tqdm(prev_arrag, f"insert {curr_det}"):
            set_arrangement(ex, states, start_det)

            for state in iter_states(ex.free_cells):
                if ex.is_fits(curr_det, state):
                    curr_arrang.add(states + (state, ))
            reset_arrangement(ex, start_det)
        with open(save_dir / f"fs_{curr_det}.pickle", "wb") as f:
            pickle.dump(list(curr_arrang), f)
    
    connected = []
    for states in tqdm(curr_arrang, "check connected"):
        for d, s in enumerate(states):
            ex.insert(d, s)
        if ex.is_connected():
            connected.append(states)
        for d, s in enumerate(states):
                ex.remove(d)
    
    with open("arrangements_and_connected.pickle", "wb") as f:
        pickle.dump(dict(
            arrangements=curr_arrang, connected=connected
        ), f)


def main():
    ex = Exercise(CONFIG_PATH) 
    arrangement_traversal(ex)
    # with open("arrangements.pickle", "wb") as f:
    #     d = pickle.load(f)
    #     print(type(d), len(d))
    # with tqdm(total=36**11 * 8**10 * 3 ** 11 * 5**8) as pb:
    #     ex = Exercise(CONFIG_PATH, pb) 
    #     arrangement = []
    #     search_connected_packing(ex, 0, DetailState(1, 5, 4, False), arrangement)


main()

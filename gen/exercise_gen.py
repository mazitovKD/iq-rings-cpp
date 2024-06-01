import uuid
import json
from pathlib import Path
from tqdm import tqdm


from gen.native_engine import Exercise, DetailState

CONFIG_PATH = "/home/kama/Projects/iq_rings/configs/complited_exercises/junior25.json"
SAVE_PATH = Path("/home/kama/Projects/iq_rings/configs/solutions/")
# class Field:
#     def __init__(self, rows: int, columns: int, disabled: list[tuple[int]]) -> None:
#         self.rows = rows
#         self.columns = columns
#         self.disabled = disabled
#         self.details = 

    
def generate_states(rows: int, columns: int) -> list[DetailState]:
    return [
        DetailState(row, column, rotation, side)
        for side in (True, False)
        for rotation in range(6)
        for row in range(rows)
        for column in range(columns) 
    ]



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
    if len(ex.free_details) < 1:
            print("AAAA") 
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


def main():
    with tqdm(total=36**11 * 8**10 * 3 ** 11 * 5**8) as pb:
        ex = Exercise(CONFIG_PATH, pb) 
        arrangement = []
        search_connected_packing(ex, 0, DetailState(1, 5, 4, False), arrangement)


main()

from collections import defaultdict
from subprocess import PIPE, Popen
from typing import Literal

import numpy as np
import numpy.typing as npt
from numpy import double

__all__ = ["newton_at_one_point", "newton_in_rectangle", "root_label", "root_dict", "iter_dict"]


def newton_at_one_point(x: double, y: double) -> npt.NDArray[double]:
    """
    呼叫 C++ 程式，取得牛頓法每次迭代的計算結果
    """

    process = Popen(["./newton_cli"], stdin=PIPE, stdout=PIPE)
    cpp_input = f"one {x} {y}"
    stdout, stderr = process.communicate(input=cpp_input.encode())
    ret = stdout.decode().strip()

    results = np.fromstring(ret, double, sep=" ").reshape(-1, 3)
    return results


def newton_in_rectangle(
    xmin: double,
    ymin: double,
    xmax: double,
    ymax: double,
    step: double,
    *,
    cmd: Literal["root", "iter"] = "root",
) -> npt.NDArray[double]:
    """
    呼叫 C++ 程式，取得牛頓法在區域內每個點的計算結果
    只回傳收斂到的根 (root) 或 迭代次數 (iter)
    """

    process = Popen(["./newton_cli"], stdin=PIPE, stdout=PIPE)
    cpp_input = f"rect {cmd} {xmin} {xmax} {ymin} {ymax} {step}"
    stdout, stderr = process.communicate(input=cpp_input.encode())
    ret = stdout.decode().strip()

    results = np.fromstring(ret, double, sep=" ").reshape(-1, 4 if cmd == "root" else 3)
    results = np.round(results, decimals=6, out=results)
    return results


def root_label(x, y) -> str:
    if x == 0.0:
        x = "\\pm 0.0"
    elif x == np.inf:
        x = "\\infty"
    if y == 0.0:
        y = "\\pm 0.0"
    elif y == np.inf:
        y = "\\infty"
    return f"$({x}, {y})$"


def root_dict(root: npt.NDArray[double]) -> dict:
    d = defaultdict(list)

    for x0, y0, x, y in root:
        d[root_label(x, y)].append([x0, y0])

    for key in d.keys():
        d[key] = np.array(d[key])

    return dict(d)


def iter_dict(iter: npt.NDArray[double]) -> dict:
    d = defaultdict(list)

    for x0, y0, iters in iter:
        d[f"{iters}"].append([x0, y0])

    for key in d.keys():
        d[key] = np.array(d[key])

    return dict(d)

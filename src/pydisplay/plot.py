import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import BoundaryNorm, ListedColormap
from matplotlib.figure import Figure
from matplotlib.ticker import MultipleLocator
from mpl_toolkits.axisartist import Axes
from numpy import double

from .func import funcLower, funcUpper
from .newton import newton_at_one_point, newton_in_rectangle, root_label

__all__ = [
    "plot_fg",
    "plot_convergent_path",
    "plot_divergent_path",
    "plot_one_point_animation",
    "plot_roots_area",
    "plot_iter_cnt",
]


def plot_fg(ax: Axes):
    """
    畫出兩個方程式 (f, g) 的圖形
    """

    x1 = np.arange(-3, 3.01, 0.01)
    ax.plot(x1, funcUpper(1, 0, 1, 0, 0, -9)(x1), "r-", label="$f(x, y) \\equiv x^2 + y^2 - 9 = 0$")
    ax.plot(x1, funcLower(1, 0, 1, 0, 0, -9)(x1), "r-")

    x2 = np.arange(-10, 10.01, 0.01)
    ax.plot(x2, funcUpper(1, -3, 1, 0, 0, -9)(x2), "b-", label="$g(x, y) \\equiv x^2 - 3xy + y^2 - 9 = 0$")
    ax.plot(x2, funcLower(1, -3, 1, 0, 0, -9)(x2), "b-")

    ax.legend()


def plot_convergent_path(ax: Axes, x: double, y: double, tag: str = "x"):
    """
    繪製牛頓法在 (x, y) 點的收斂路徑
    """
    results = newton_at_one_point(x, y)
    x_vals, y_vals, _ = results.T
    ax.plot(x_vals, y_vals, "o-", label=f"${tag}_0({x_vals[0]}, {y_vals[0]})$ 的收斂路徑")

    for i, (px, py, _) in enumerate(results):
        if i == 0:
            ax.text(px + 0.1, py, f"${tag}_{i} = ({px}, {py})$", fontsize=12, color="black", ha="left")
        else:
            ax.text(px + 0.1, py, f"${tag}_{i}$", fontsize=12, color="black", ha="left")
    ax.legend()


def plot_divergent_path(ax: Axes, x: double, y: double):
    """
    繪製牛頓法在 (x, y) 點的發散路徑
    """
    results = newton_at_one_point(x, y)
    x_vals, y_vals, _ = results.T
    ax.plot(x_vals, y_vals, "o-", label=f"$({x_vals[0]}, {y_vals[0]})$ 的發散路徑")

    for i, (px, py, _) in enumerate(results):
        if i == 0:
            ax.text(px, py + 0.1, f"$x_{{{i}}} = ({px}, {py})$", fontsize=12, color="black", ha="left")
        else:
            ax.text(px, py + 0.1, f"$x_{{{i}}}$", fontsize=12, color="black", ha="center")
    ax.legend()


def plot_one_point_animation(fig: Figure, x: double, y: double):
    """
    繪製牛頓法在 (x, y) 點的收斂動畫
    """
    ax = fig.gca()  # 取得 fig 當前的 ax

    results = newton_at_one_point(x, y)

    # show results

    x, y = [], []
    (points,) = ax.plot([], [], "o-", color="gray", alpha=0.5)
    (line,) = ax.plot([], [], "go-")
    (err_txt,) = ax.plot([], [], " ", label="$e_0$")
    txt = ax.text(0, 0 + 0.2, "", fontsize=12, color="black", ha="center")

    def update(frame):
        # 加入新點
        px, py, err = results[frame]
        x.append(px)
        y.append(py)
        print(repr(frame), f"({px}, {py}), err={err}")

        # 舊的塗灰色
        points.set_data(x, y)

        # 新的塗綠色
        line.set_data(x[-2:], y[-2:])
        txt.set_position((px, py + 0.2))
        txt.set_text(f"$x_{{{frame}}} \\approx ({px}, {py})$")

        # 顯示誤差值
        err_txt.set_label(f"$e_{{{frame}}}$ = {err}")
        ax.legend()

    ani = animation.FuncAnimation(fig, update, frames=len(results), interval=500, repeat=False)
    ani.save("images/converge_animate.gif", writer="pillow", fps=2)


def plot_roots_area(fig: Figure):
    """
    繪製牛頓法在區域內每個點收斂到的根
    """
    ax = fig.gca()  # 取得 fig 當前的 ax

    mapping = {
        "$(\\infty, \\infty)$": 0,
        "$(3.0, \\pm 0.0)$": 1,
        "$(\\pm 0.0, 3.0)$": 2,
        "$(-3.0, \\pm 0.0)$": 3,
        "$(\\pm 0.0, -3.0)$": 4,
    }

    results = newton_in_rectangle(-6, -6, 6, 6, 0.05)
    x0, y0, x, y = results.T
    roots = np.array([mapping[root_label(xi, yi)] for xi, yi in zip(x, y)])

    # 一些顏色設定
    tab10 = list(plt.get_cmap("tab10").colors)
    cmap = ListedColormap(["black"] + tab10[:7] + tab10[8:])
    levels = np.arange(0, len(mapping) + 1) - 0.5
    norm = BoundaryNorm(levels, cmap.N)

    # 畫收斂到的根的區域
    Z = roots.reshape(np.sqrt(x0.shape[0]).astype(int), -1)
    Z = Z.T  # 轉置，讓 x, y 軸對應正確
    ax.imshow(Z, extent=(-6, 6, -6, 6), origin="lower", cmap=cmap, norm=norm, alpha=0.5)

    # 畫格子點
    results = newton_in_rectangle(-5, -5, 5, 5, 1)
    x0, y0, x, y = results.T
    roots = np.array([mapping[root_label(xi, yi)] for xi, yi in zip(x, y)])

    scat = ax.scatter(x0, y0, c=roots, cmap=cmap, norm=norm)
    scat.zorder = 999

    # 顏色說明
    cbar = fig.colorbar(scat, ax=ax, label="Converged roots")
    cbar.set_ticks(np.arange(len(mapping)))
    cbar.set_ticklabels(list(mapping.keys()))

    fig.savefig("images/converge_roots.png", dpi=300)


def plot_iter_cnt(fig: Figure):
    """
    繪製牛頓法在區域內每個點收斂所需的迭代次數
    """
    ax = fig.gca()  # 取得 fig 當前的 ax

    results = newton_in_rectangle(-6, -6, 6, 6, 0.05, cmd="iter")
    x0, y0, cnt = results.T

    # 一些顏色設定
    # tab10 = list(plt.get_cmap("tab10").colors)
    # cmap = ListedColormap(["black"] + tab10[:7] + tab10[8:])
    cmap = plt.get_cmap("terrain")
    levels = np.arange(cnt.min(), cnt.max() + 2) - 0.5
    norm = BoundaryNorm(levels, cmap.N)

    # 畫迭代次數的區域
    Z = cnt.reshape(np.sqrt(x0.shape[0]).astype(int), -1)
    Z = Z.T  # 轉置，讓 x, y 軸對應正確

    im = ax.imshow(Z, extent=(-6, 6, -6, 6), origin="lower", cmap=cmap, norm=norm)  # , alpha=0.5)

    # # 畫格子點
    # results = newton_in_rectangle(-5, -5, 5, 5, 1, cmd="iter")
    # x0, y0, cnt = results.T

    # scat = ax.scatter(x0, y0, c=cnt, cmap=cmap, norm=norm)
    # scat.zorder = 999

    # 顏色說明
    # cbar = fig.colorbar(scat, ax=ax, label="Number of iterations")
    cbar = fig.colorbar(im, ax=ax, label="Number of iterations")
    cbar.ax.yaxis.set_major_locator(MultipleLocator(1))

    fig.savefig("images/converge_iterations_count.png", dpi=300)

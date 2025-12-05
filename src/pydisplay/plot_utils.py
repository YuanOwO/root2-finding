import matplotlib.pyplot as plt
from matplotlib.figure import Figure
from matplotlib.ticker import AutoLocator, AutoMinorLocator, MaxNLocator, MultipleLocator
from mpl_toolkits.axisartist import Axes, AxesZero

__all__ = ["init_plot", "set_bbox"]

plt.rcParams["font.family"] = "Noto Sans TC"


def set_bbox(ax: AxesZero, bbox: tuple[float, float, float, float]):
    """
    設定顯示範圍 (xmin, ymin, xmax, ymax)
    """
    ax.set_xlim(bbox[0], bbox[2])
    ax.set_ylim(bbox[1], bbox[3])

    # major and minor ticks
    if bbox[2] - bbox[1] >= 5 and bbox[3] - bbox[1] >= 5:
        ax.xaxis.set_major_locator(MultipleLocator(2))
        ax.yaxis.set_major_locator(MultipleLocator(2))
        ax.xaxis.set_minor_locator(MultipleLocator(1))
        ax.yaxis.set_minor_locator(MultipleLocator(1))
    elif bbox[2] - bbox[1] >= 2 and bbox[3] - bbox[1] >= 2:
        ax.xaxis.set_major_locator(MultipleLocator(0.5))
        ax.yaxis.set_major_locator(MultipleLocator(0.5))
        ax.xaxis.set_minor_locator(MultipleLocator(0.1))
        ax.yaxis.set_minor_locator(MultipleLocator(0.1))
    # else:
    #     ax.xaxis.set_major_locator(AutoLocator())
    #     ax.yaxis.set_major_locator(AutoLocator())
    #     ax.xaxis.set_minor_locator(AutoMinorLocator())
    #     ax.yaxis.set_minor_locator(AutoMinorLocator())

    ax.grid(which="major", linestyle="-")

    ax.grid(which="minor", alpha=0.5)

    # visable = []

    # if bbox[0] > 0:  # 左邊界大於 y 軸
    #     visable.append("left")
    # elif bbox[2] < 0:  # 右邊界小於 y 軸
    #     visable.append("right")
    # else:  # y 軸在範圍內
    #     visable.append("yzero")

    # if bbox[1] > 0:  # 下邊界大於 x 軸
    #     visable.append("bottom")
    # elif bbox[3] < 0:  # 上邊界小於 x 軸
    #     visable.append("top")
    # else:  # x 軸在範圍內
    #     visable.append("xzero")

    # for direction in ["xzero", "yzero", "left", "right", "bottom", "top"]:
    #     ax.axis[direction].set_visible(direction in visable)


def init_plot() -> tuple[Figure, Axes]:
    fig = plt.figure(figsize=(10, 8), dpi=300, tight_layout=True)
    # ax = fig.add_subplot(axes_class=AxesZero)
    ax = fig.add_subplot()

    # for direction in ["xzero", "yzero", "left", "right", "bottom", "top"]:
    #     # adds arrows at the ends of each axis
    #     ax.axis[direction].set_axisline_style("-|>")

    # 顯示範圍
    set_bbox(ax, (-5, -5, 5, 5))  # (xmin, ymin, xmax, ymax)

    # 設定 x 軸和 y 軸的比例相同
    ax.set_aspect("equal", adjustable="box")

    return fig, ax

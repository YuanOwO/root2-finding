from random import random

from pydisplay import (
    init_plot,
    plot_convergent_path,
    plot_divergent_path,
    plot_fg,
    plot_iter_cnt,
    plot_one_point_animation,
    plot_roots_area,
    set_bbox,
)

# Init figure and draw functions

fig, ax = init_plot()

set_bbox(ax, (-6, -6, 6, 6))  # 設邊界
plot_fg(ax)  # 畫 f, g


__PNO = 2  # 題號

if __PNO == 2:  # 畫第二題的圖
    # [2, 1] 會收斂
    plot_convergent_path(ax, 2, 1)

    x, y = -2, 2  # [-2, 2] 無法收斂
    plot_divergent_path(ax, x, y, "y")
    # 加上 [-.5, .5] 的隨機擾動，就能收斂
    plot_convergent_path(ax, x + random() - 0.5, y + random() - 0.5, "z")

    fig.savefig("images/fg.png", dpi=300)
elif __PNO == 2.5:  # 初始點 [2, 1] 的收斂動畫
    plot_one_point_animation(2, 1)
elif __PNO == 3:  # 畫第三題的圖 (迭代次數分佈)
    plot_iter_cnt(fig, ax)
elif __PNO == 3.5:  # 畫第三題的圖 (根的分佈區域)
    plot_roots_area(fig, ax)
else:
    print("Hello world!")

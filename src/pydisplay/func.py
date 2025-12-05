from numpy import double, sqrt

__all__ = ["funcUpper", "funcLower"]


def funcUpper(a: double, b: double, c: double, d: double, e: double, f: double) -> callable:
    """
    輸入一個二元二次方程式的係數，將其轉換成 y 對 x 的函數。
    輸出其圖形的上半部的函數

    ax^2 + bxy + cy^2 + dx + ey + f = 0

    """

    def func(x: double) -> double:
        A = c
        B = b * x + e
        C = a * x**2 + d * x + f

        DELTA = B**2 - 4 * A * C

        return (-B + sqrt(DELTA)) / (2 * A)

    return func


def funcLower(a: double, b: double, c: double, d: double, e: double, f: double) -> callable:
    """
    輸入一個二元二次方程式的係數，將其轉換成 y 對 x 的函數。
    輸出其圖形的下半部的函數

    ax^2 + bxy + cy^2 + dx + ey + f = 0
    """

    def func(x: double) -> double:
        A = c
        B = b * x + e
        C = a * x**2 + d * x + f

        return (-B - sqrt(B**2 - 4 * A * C)) / (2 * A)

    return func

"""

Reference data for testing our implementation of the De Boor algorithm.

"""

from scipy.interpolate import BSpline
import numpy as np
import argparse


def parse_args():
    """
    parse_args parses the command line arguments.

    :return: the parsed arguments
    :rtype: argparse.Namespace
    """

    parser = argparse.ArgumentParser(
        description="Generate reference data for the De Boor algorithm."
    )
    parser.add_argument(
        "--knots", type=float, nargs="+", help="The knots vector.", required=True
    )
    parser.add_argument(
        "--control_points",
        type=float,
        nargs="+",
        help="The control points vector.",
        required=True,
    )
    parser.add_argument(
        "--degree", type=int, help="The degree of the spline.", required=True
    )
    parser.add_argument(
        "--type",
        type=str,
        help="The type of the spline (i.e., open | clamped | periodic).",
        default="open",
    )
    parser.add_argument(
        "--extrapolate",
        type=str,
        help="The type of extrapolation to use (i.e., none | constant | periodic).",
        default="none",
    )
    parser.add_argument(
        "--plot", action="store_true", help="Whether to plot the reference data or not."
    )
    parser.add_argument(
        "--print",
        action="store_true",
        help="Whether to print the reference data or not.",
    )
    return parser.parse_args()


def check_args(args: argparse.Namespace) -> None:
    """
    check_args checks the validity of the arguments.

    :param args: the parsed arguments
    :type args: argparse.Namespace
    """

    if not isinstance(args.knots, list) and not isinstance(args.control_points, list):
        raise ValueError("The knots and control points must be lists.")
    if not isinstance(args.degree, int):
        raise ValueError("The degree must be an integer.")
    if args.type not in ["open", "clamped", "periodic"]:
        raise ValueError("The spline type must be open, clamped, or periodic.")
    if args.extrapolate not in ["none", "constant", "periodic"]:
        raise ValueError("The extrapolation type must be none, constant, or periodic.")
    if not isinstance(args.plot, bool) and not isinstance(args.print, bool):
        raise ValueError("The plot and print flags must be boolean.")


def ref_open(knots: list, control_points: list, degree: int, extrapolate: str) -> tuple:
    """
    ref_open generates reference data for the open spline case.

    :param knots: the knots vector
    :type knots: list
    :param control_points: the control points vector
    :type control_points: list
    :param degree: the degree of the spline
    :type degree: int
    :param extrapolate: the type of extrapolation to use
    :type extrapolate: str
    :return: x_values, y_values
    :rtype: tuple(list, list)
    """

    if extrapolate == "none":
        extrapolate = False
    else:
        raise NotImplementedError(
            "Other types of extrapolation for open splines are not implemented yet."
        )

    spline = BSpline(knots, control_points, degree, extrapolate=extrapolate)

    x_values = np.arange(knots[degree], knots[-degree - 1], 0.1)
    y_values = spline(x_values)

    return x_values, y_values


def ref_clamped(
    knots: list, control_points: list, degree: int, extrapolate: str
) -> tuple:
    """
    ref_clamped generates reference data for the clamped spline case.

    :param knots: the knots vector
    :type knots: list
    :param control_points: the control points vector
    :type control_points: list
    :param degree: the degree of the spline
    :type degree: int
    :param extrapolate: the type of extrapolation to use
    :type extrapolate: str
    :return: x_values, y_values
    :rtype: tuple(list, list)
    """

    if extrapolate == "none":
        extrapolate = False
    else:
        raise NotImplementedError(
            "Other types of extrapolation for clamped splines are not implemented yet."
        )

    pad_left = knots[0:1] * degree
    pad_right = knots[-1:] * degree
    knots = pad_left + knots + pad_right

    spline = BSpline(knots, control_points, degree, extrapolate=extrapolate)

    x_values = np.arange(knots[degree], knots[-degree - 1], 0.1)
    y_values = spline(x_values)

    return x_values, y_values


def ref_periodic(
    knots: list, control_points: list, degree: int, extrapolate: str
) -> tuple:
    """
    ref_periodic generates reference data for the periodic spline case.

    :param knots: the knots vector
    :type knots: list
    :param control_points: the control points vector
    :type control_points: list
    :param degree: the degree of the spline
    :type degree: int
    :param extrapolate: the type of extrapolation to use
    :type extrapolate: str
    :return: x_values, y_values
    :rtype: tuple(list, list)
    """

    if extrapolate == "none":
        extrapolate = False
    elif extrapolate == "periodic":
        extrapolate = "periodic"
    else:
        raise NotImplementedError(
            "Other types of extrapolation for periodic splines are not implemented yet."
        )

    period = knots[-1] - knots[0]
    pad_left = [knot - period for knot in knots[-degree - 1 : -1]]
    pad_right = [knot + period for knot in knots[1 : degree + 1]]
    knots = pad_left + knots + pad_right

    control_points_right = control_points[:degree]
    control_points = control_points + control_points_right

    spline = BSpline(knots, control_points, degree, extrapolate=extrapolate)

    if extrapolate == "periodic":
        x_values = np.arange(
            knots[degree] - 3 * period, knots[-degree - 1] + 3 * period, 0.1
        )
    else:
        x_values = np.arange(knots[degree], knots[-degree - 1], 0.1)
    y_values = spline(x_values)

    return x_values, y_values


def main():
    """
    main is the entry point of the reference module.
    """

    args = parse_args()
    check_args(args)

    if args.type == "open":
        x_values, y_values = ref_open(
            args.knots, args.control_points, args.degree, args.extrapolate
        )
    elif args.type == "clamped":
        x_values, y_values = ref_clamped(
            args.knots, args.control_points, args.degree, args.extrapolate
        )
    elif args.type == "periodic":
        x_values, y_values = ref_periodic(
            args.knots, args.control_points, args.degree, args.extrapolate
        )
    else:
        raise NotImplementedError("The requested b-spline type is not implemented.")

    if args.plot:
        import matplotlib.pyplot as plt

        plt.figure()
        plt.scatter(x_values, np.nan_to_num(y_values))
        plt.grid()
        plt.show()

    if args.print:
        print(f"knots = {{{', '.join([str(k) for k in args.knots])}}};")
        print(
            f"control_points = {{{', '.join([str(c) for c in args.control_points])}}};"
        )
        print(
            f"std::vector<double> x_values{{{', '.join([str(x) for x in x_values])}}};"
        )
        print(
            f"std::vector<double> y_values{{{', '.join([str(y) for y in y_values])}}};"
        )


if __name__ == "__main__":
    main()

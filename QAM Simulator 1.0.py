# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import os
import math

def generate_qam_constellation(M, exclude_points):
    """
    Generates the QAM constellation points.
    M: The size of the QAM constellation.
    exclude_points: The number of points to exclude from the corners.
    """
    side_len = int(math.sqrt(M))

    # Adjust side length for specific cases to ensure correct constellation shape
    if M == 32:
        side_len_x, side_len_y = 6, 6
    elif M == 128:
        side_len_x, side_len_y = 12, 12
    elif M == 512:
        side_len_x, side_len_y = 24, 24
    elif M == 2048:
        side_len_x, side_len_y = 46, 46
    else:
        side_len_x, side_len_y = side_len, side_len

    # Generate all possible points within the grid
    points = []
    for x in range(-side_len_x + 1, side_len_x, 2):
        for y in range(-side_len_y + 1, side_len_y, 2):
            points.append((x, y))
    points = np.array(points)

    # Remove any point at the origin (0, 0)
    points = points[np.any(points != 0, axis=1)]

    excluded_points = []
    if exclude_points > 0:
        square_size = int(math.sqrt(exclude_points // 4))

        for i in range(square_size):
            for j in range(square_size):
                excluded_points.extend([
                    (side_len_x - 1 - 2 * i, side_len_y - 1 - 2 * j),
                    (-side_len_x + 1 + 2 * i, side_len_y - 1 - 2 * j),
                    (side_len_x - 1 - 2 * i, -side_len_y + 1 + 2 * j),
                    (-side_len_x + 1 + 2 * i, -side_len_y + 1 + 2 * j)
                ])
        excluded_points = np.array(excluded_points)

        # Remove excluded points
        mask = np.ones(len(points), dtype=bool)
        for excluded_point in excluded_points:
            mask &= ~np.all(points == excluded_point, axis=1)
        points = points[mask]

    return points, excluded_points


def plot_qam_constellation(M):
    exclusions = {32: 4, 128: 16, 512: 64, 2048: 196}
    exclude_points = exclusions.get(M, 0)

    points, excluded_points = generate_qam_constellation(M, exclude_points)

    energy = np.sqrt(points[:, 0] ** 2 + points[:, 1] ** 2)
    phase = np.arctan2(points[:, 1], points[:, 0])
    phase = (phase + 2 * np.pi) % (2 * np.pi)
    phase_pi = phase / np.pi

    sort_indices = np.argsort(phase)
    points = points[sort_indices]
    energy = energy[sort_indices]
    phase_pi = phase_pi[sort_indices]

    # Constellation Plot
    plt.figure(figsize=(8, 6))
    plt.scatter(points[:, 0], points[:, 1], color='blue', label='Included Symbols')
    if len(excluded_points) > 0:
        plt.scatter(excluded_points[:, 0], excluded_points[:, 1], color='red', marker='x', label='Excluded Symbols')
    plt.grid(True)
    plt.title("{}-QAM Constellation Diagram".format(M))
    plt.xlabel('In-Phase (I)')
    plt.ylabel('Quadrature (Q)')
    plt.gca().set_aspect('equal', adjustable='box')
    plt.legend()

    max_val = int(np.max(np.abs(points))) + 2
    ticks = range(-max_val, max_val + 1, 2)
    plt.xticks([t for t in ticks if t != 0])
    plt.yticks([t for t in ticks if t != 0])

    plt.axhline(0, color='black', linewidth=1.2)
    plt.axvline(0, color='black', linewidth=1.2)

    desktop_path = os.path.join(os.path.expanduser("~"), 'Desktop')
    pdf_filename = os.path.join(desktop_path, '{}-QAM_constellation.pdf'.format(M))
    with PdfPages(pdf_filename) as pdf:
        pdf.savefig()
    plt.close()

    # Table Plot
    symbol_numbers = range(1, len(points) + 1)
    chunk_size = 20
    num_chunks = int(math.ceil(len(points) / float(chunk_size)))

    table_pdf_filename = os.path.join(desktop_path, '{}-QAM_table.pdf'.format(M))
    with PdfPages(table_pdf_filename) as pdf:
        for i in range(num_chunks):
            start_idx = i * chunk_size
            end_idx = min((i + 1) * chunk_size, len(points))
            fig_table = plt.figure(figsize=(8, 6))
            ax_table = fig_table.add_subplot(1, 1, 1)
            ax_table.axis('off')

            table_data = []
            for num, (q, i), e, p in zip(symbol_numbers[start_idx:end_idx], points[start_idx:end_idx], energy[start_idx:end_idx], phase_pi[start_idx:end_idx]):
                table_data.append([num, q, i, "{:.2f}".format(e), "{:.2f}pi".format(p)])

            table = ax_table.table(cellText=table_data,
                                   colLabels=["Symbol", "Q", "I", "Energy", "Phase (rad)"],
                                   loc="center", cellLoc="center", colWidths=[0.2, 0.2, 0.2, 0.2, 0.2])
            table.auto_set_font_size(False)
            table.set_fontsize(10)
            table.scale(1.2, 1.2)

            pdf.savefig(fig_table)
            plt.close(fig_table)


M = int(raw_input("Enter the size of QAM (4, 16, 32, 64, 128, 512, 1024, 2048, 4096): "))
valid_sizes = [4, 16, 32, 64, 128, 512, 1024, 2048, 4096]
if M in valid_sizes:
    print("Generating QAM constellation and table. Exporting to PDF...")
    plot_qam_constellation(M)
    print("PDF files generated successfully to your Desktop!")
else:
    print("Invalid QAM size. Please choose from: {}".format(valid_sizes))

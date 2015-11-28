#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright © 2015 Martin Ueding <dev@martin-ueding.de>

import argparse

import numpy as np
import matplotlib.pyplot as pl
import pandas as pd
import scipy.integrate

def lorentz_peak(x, mean, width, area, offset):
    return area / np.pi * width/2 / ((x - mean)**2 + (width/2)**2) + offset

def load_lines():
    filename = 'Daten/generiesb.dat'
    data = pd.read_csv(filename, header=0, delim_whitespace=True)

    return data

def get_peaks():
    return np.loadtxt('Daten/Langzeit_Peaks.dat')

def main():
    options = _parse_args()

    messwerte = np.loadtxt('_build/langzeit.tsv')

    messwerte = messwerte[messwerte[:, 0] > 200]

    print(messwerte.shape)

    peaks = get_peaks()

    lines = load_lines()

    pad = 2

    elements = list(set(lines['Nuclide']))
    print(elements)

    fig = pl.figure()
    ax = fig.add_subplot(1, 1, 1)

    results = {}

    m = 0

    for element in elements:
        selection = lines['Nuclide'] == element
        selected = lines[selection]

        x = messwerte[:, 0]
        y = np.zeros(x.shape)

        for id, energy, delta_energy, sigma_val, sigma_err, nuclide in selected.itertuples():
            y += lorentz_peak(x, energy, 1.5, sigma_val, 0)


        self_integral = scipy.integrate.simps(y, x)
        overlap = y * messwerte[:, 1]
        overlap_integral = scipy.integrate.simps(overlap, x)

        matchness = overlap_integral / self_integral

        if matchness >= +0.0005:
            ax.plot(x, y / self_integral, label=nuclide)

        results[element] = matchness

        m = max(m, np.max(y))



    ax.legend(loc='best')

    m2 = np.max(messwerte[:, 1])

    print('m', m)
    print('m2', m2)
    scale = ax.get_ylim()[1] / m2
    print('Scale', scale)

    ax.plot(x, messwerte[:, 1] * scale, alpha=0.3, color='black')

    ax.grid(True)
    ax.margins(.05)
    fig.tight_layout()
    fig.show()
    input()
    fig.savefig('test.pdf')

    results_sorted = sorted([(matchness, element) for element, matchness in results.items()])

    for matchness, element in results_sorted:
        print('{:5s} {:+.10f}'.format(element, matchness))



    for peak in peaks[:2]:
        print(peak)

        selection = (lines['E(gamma)'] < (peak + pad)) & (lines['E(gamma)'] > (peak - pad))
        selected = lines[selection]

        sorted_ = selected.sort_values(by='P', ascending=False)

        print(sorted_)

        break

        for id, iso, z, energy, i1, i2, i3 in selected.itertuples():
            print(iso, energy)

            selection2 = lines['Nuclide'] == iso
            selected2 = lines[selection2]

            print(selected2)

        print()


def _parse_args():
    '''
    Parses the command line arguments.

    :return: Namespace with arguments.
    :rtype: Namespace
    '''
    parser = argparse.ArgumentParser(description='')
    options = parser.parse_args()

    return options

if __name__ == '__main__':
    main()

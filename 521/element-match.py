#!/usr/bin/python3
# -*- coding: utf-8 -*-

# Copyright © 2015 Martin Ueding <dev@martin-ueding.de>

import argparse

import numpy as np
import matplotlib.pyplot as pl
import pandas as pd

def lorentz_peak(x, mean, width, area, offset):
    return area / np.pi * width/2 / ((x - mean)**2 + (width/2)**2) + offset

def load_lines():
    filename = 'Daten/nglist_a.dat'
    data = pd.read_csv(filename, header=0, delim_whitespace=True)

    return data

def get_peaks():
    return np.loadtxt('Daten/Langzeit_Peaks.dat')

def main():
    options = _parse_args()

    peaks = get_peaks()

    lines = load_lines()

    pad = 2

    elements = list(set(lines['Iso']))
    print(elements)

    fig = pl.figure()
    ax = fig.add_subplot(1, 1, 1)

    for element in elements:
        selection = lines['Iso'] == element
        selected = lines[selection]

        x = np.linspace(0, 1500, 10000)
        y = np.zeros(x.shape)

        for id, iso, z, energy, delta_energy, sigma_val, sigma_err in selected.itertuples():
            y += lorentz_peak(x, energy, 7, sigma_val, 0)

        ax.plot(x, y, label=iso)



    ax.legend(loc='best')
    fig.savefig('test.pdf')



    for peak in peaks[:2]:
        print(peak)

        selection = (lines['E(gamma)'] < (peak + pad)) & (lines['E(gamma)'] > (peak - pad))
        selected = lines[selection]

        sorted_ = selected.sort_values(by='Sigma', ascending=False)

        print(sorted_)

        break

        for id, iso, z, energy, i1, i2, i3 in selected.itertuples():
            print(iso, energy)

            selection2 = lines['Iso'] == iso
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

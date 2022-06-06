
from __future__ import absolute_import, division, print_function, unicode_literals

import sys
# import os
from os import path
from multiprocessing import Process, Manager
# import io
# from io import StringIO
# import collections as coll

import numpy as np


def convertUStringToDistinctIntsDict(mat, convertDicts, counts):
    # Converts matrix of unicode strings into distinct integers.
    #
    # Inputs:
    #     mat (np.array): array of unicode strings to convert
    #     convertDicts (list): dictionary for each column
    #     counts (list): number of different categories in each column
    #
    # Outputs:
    #     out (np.array): array of output integers
    #     convertDicts (list): dictionary for each column
    #     counts (list): number of different categories in each column

    # check if convertDicts and counts match correct length of mat
    if len(convertDicts) != mat.shape[1] or len(counts) != mat.shape[1]:
        print("Length of convertDicts or counts does not match input shape")
        print("Generating convertDicts and counts...")

        convertDicts = [{} for _ in range(mat.shape[1])]
        counts = [0 for _ in range(mat.shape[1])]

    # initialize output
    out = np.zeros(mat.shape)

    for j in range(mat.shape[1]):
        for i in range(mat.shape[0]):
            # add to convertDict and increment count
            if mat[i, j] not in convertDicts[j]:
                convertDicts[j][mat[i, j]] = counts[j]
                counts[j] += 1
            out[i, j] = convertDicts[j][mat[i, j]]

    return out, convertDicts, counts


def convertUStringToDistinctIntsUnique(mat, mat_uni, counts):
    # mat is an array of 0,...,# samples, with each being 26 categorical features

    # check if mat_unique and counts match correct length of mat
    if len(mat_uni) != mat.shape[1] or len(counts) != mat.shape[1]:
        print("Length of mat_unique or counts does not match input shape")
        print("Generating mat_unique and counts...")

        mat_uni = [np.array([]) for _ in range(mat.shape[1])]
        counts = [0 for _ in range(mat.shape[1])]

    # initialize output
    out = np.zeros(mat.shape)
    ind_map = [np.array([]) for _ in range(mat.shape[1])]

    # find out and assign unique ids to features
    for j in range(mat.shape[1]):
        m = mat_uni[j].size
        mat_concat = np.concatenate((mat_uni[j], mat[:, j]))
        mat_uni[j], ind_map[j] = np.unique(mat_concat, return_inverse=True)
        out[:, j] = ind_map[j][m:]
        counts[j] = mat_uni[j].size

    return out, mat_uni, counts


import pyautogui
import time


# Indicates the range of lengths of words (not inclusive)
LOWER_BOUND = 3
UPPER_BOUND = 8


# Returns true if (i, j) is in a square matrix of size dimension
def in_matrix(i, j, dimension):
     return (0 <= i < dimension) and (0 <= j < dimension)


# Adds a string of index to keep track of position and
# so that every element is unique
def make_unique(matrix):
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            matrix[i][j] += str(i) + str(j)


def getPath(matrix, i, j, words, current=[]):
    # Check if position valid
    global UPPER_BOUND
    if len(current) > UPPER_BOUND:
        # If word is too long, probably doesn't exist
        return
    if not in_matrix(i, j, len(matrix)):
        return
    if matrix[i][j] in current:
        return
    direction = ((0, 1), (1, 1), (1,0), (-1,0), (0,-1), (-1, -1), (-1, 1), (1, -1))
    for x, y in direction:
        current.append(matrix[i][j])
        words.add(''.join(current))
        getPath(matrix, i + x, j + y, words, current)
        current.pop()


def getWord(string):
    new = ""
    i = 0
    while i < len(string):
        new += string[i]
        i += 3
    return new


def getNums(string):
    i = 0
    array = list()
    while i < len(string):
        array.append(string[i+ 1: i+ 3])
        i += 3
    return array


def clean(array):
    return [getWord(i) for i in array]


def getAllWords(matrix):
    words = set()
    make_unique(matrix)
    
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            getPath(matrix, i, j, words)
    global LOWER_BOUND
    seen = set()
    array = list()
    for i in words:
        actualWord = getWord(i)
        if actualWord in seen:
            continue
        if len(actualWord) > LOWER_BOUND:
            seen.add(actualWord)
            array.append([actualWord, getNums(i)])

    return array


if __name__ == "__main__":
    # Position of buttons
    positions = {
            '00': (109, 362),
            '01': (182, 361),
            '02': (264, 355),
            '03': (340, 358),
            '10': (101, 428),
            '11': (178, 434),
            '12': (257, 437),
            '13': (331, 430),
            '20': (115, 511),
            '21': (187, 516),
            '22': (254, 519),
            '23': (333, 524),
            '30': (105, 594),
            '31': (178, 595),
            '32': (254, 595),
            '33': (343, 595)
    }
    # Load set of all english words
    with open("dict.txt", "r") as f:
        text = {line.strip() for line in f}
    # Get input matrix from user
    input_matrix = list()
    i = 0
    while i < 4:
        input_matrix.append(list(input()))
        i += 1 
    real_words = [i for i in getAllWords(input_matrix) if i[0] in text]
    for i in real_words:
        print(positions[i[1][0]])
        pyautogui.moveTo(positions[i[1][0]])
        pyautogui.mouseDown()
        for p in i[1][1:]:
            pyautogui.moveTo(positions[p])
        pyautogui.mouseUp()
        time.sleep(0.1)

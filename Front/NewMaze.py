# -*- coding: utf-8 -*-
import ctypes
import math
from OnLib import *


def NewGen(mz, size_maze):
    testpp.PGen(mz, size_maze, size_maze)
    VertArray = ctypes.cast(testpp.PVert(mz), ctypes.POINTER(ctypes.c_short))
    HorizArray = ctypes.cast(testpp.PHoriz(mz), ctypes.POINTER(ctypes.c_short))
    Vmaze = []
    Hmaze = []
    for i in range(size_maze):
        list_bufV = []
        list_bufH = []
        for j in range(size_maze):
            list_bufV.append(VertArray[j + i * size_maze])
            list_bufH.append(HorizArray[j + i * size_maze])
        Vmaze.append(list_bufV)
        Hmaze.append(list_bufH)
    return Vmaze, Hmaze


def PaintMaze(size_maze, Vmaze, Hmaze, wall):
    for i in range(size_maze):
        for j in range(size_maze):
            if Vmaze[i][j] == 1:
                pygame.draw.rect(
                    sf, GREY, (a_sh * (j + 1), wall + i * a_sh, wall, a_sh))
            if Hmaze[i][j] == 1:
                pygame.draw.rect(sf, GREY, (a_sh * j,
                                            wall + (i + 1) * a_sh, a_sh + wall, wall))


def PaintPath():
    CoordFind_array = (ctypes.c_short * len(CoordFind))(*CoordFind)
    FindArray = ctypes.cast(testpp.PFind(fp, mz, CoordFind_array),
                            ctypes.POINTER(ctypes.c_short))
    FA = []
    for i in range(1, FindArray[0]):
        FA.append(FindArray[i])
    pairs = [(FA[i + 1] * a_sh + a_sh // 2, FA[i] * a_sh + a_sh // 2)
             for i in range(0, len(FA), 2)]
    for i in range(len(pairs) - 1):
        pygame.draw.line(
            sf, GREEN, pairs[i], pairs[i + 1], 3)


testpp = ctypes.CDLL('libmaz.so')
testpp.MazeGenNew.restype = ctypes.c_void_p
testpp.PGen.argtypes = [ctypes.c_void_p, ctypes.c_short, ctypes.c_short]
testpp.PVert.restype = ctypes.POINTER(ctypes.c_short)
testpp.PVert.argtypes = [ctypes.c_void_p]
testpp.PHoriz.restype = ctypes.POINTER(ctypes.c_short)
testpp.PHoriz.argtypes = [ctypes.c_void_p]
testpp.MazeGenDel.argtypes = [ctypes.c_void_p]
testpp.FindNew.restype = ctypes.c_void_p
testpp.FindDel.argtypes = [ctypes.c_void_p]
testpp.PFind.restype = ctypes.POINTER(ctypes.c_short)
testpp.PFind.argtypes = [ctypes.c_void_p,
                         ctypes.c_void_p, ctypes.POINTER(ctypes.c_short)]


size_maze = 15

mz = testpp.MazeGenNew()
fp = testpp.FindNew()
Vmaze, Hmaze = NewGen(mz, size_maze)

a_sh = math.floor(996/size_maze)
sf = pygame.display.set_mode((1300, 1000))

sf.fill(DARK_GREY)
wall = 2
button_gen = ButtonWithOutline(
    sf, (1028, 100, 250, 60), (GREY, LIGHT_GREY, BLACK), 'ГЕНЕРАЦИЯ')
imput_size = TextField(sf, (1113, 25, 80, 60), (GREY, BLUE, BLACK), (1, 2))
imput_size.SetText('15')
x1 = TextField(sf, (1065, 200, 80, 60), (GREY, BLUE, BLACK), (1, 2))
y1 = TextField(sf, (1165, 200, 80, 60), (GREY, BLUE, BLACK), (1, 2))
x2 = TextField(sf, (1065, 280, 80, 60), (GREY, BLUE, BLACK), (1, 2))
y2 = TextField(sf, (1165, 280, 80, 60), (GREY, BLUE, BLACK), (1, 2))
x1.SetText('0')
y1.SetText('0')
x2.SetText(str(size_maze - 1))
y2.SetText(str(size_maze - 1))
paint_path = ButtonWithOutline(
    sf, (1028, 380, 250, 60), (GREY, LIGHT_GREY, BLACK), 'ПУТЬ')
PaintMaze(size_maze, Vmaze, Hmaze, wall)
pygame.draw.rect(sf, GREY, (0, 0, a_sh * size_maze +
                            wall, a_sh * size_maze + wall * 2), 2)
running = True
while running:
    pygame.display.flip()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            running = False
            testpp.MazeGenDel(mz)
            testpp.FindDel(fp)
            break
        if button_gen.MouseTracking(event):
            try:
                size_maze = int(imput_size.GetText())
            except ValueError:
                size_maze = 5
            a_sh = math.floor(996/size_maze)
            Vmaze, Hmaze = NewGen(mz, size_maze)
            pygame.draw.rect(sf, DARK_GREY, (0, 0, 1005, 1000))
            pygame.draw.rect(sf, GREY, (0, 0, a_sh * size_maze +
                             wall, a_sh * size_maze + wall * 2), 2)
            PaintMaze(size_maze, Vmaze, Hmaze, wall)
        imput_size.MouseTracking(event)
        x1.MouseTracking(event)
        y1.MouseTracking(event)
        x2.MouseTracking(event)
        y2.MouseTracking(event)
        if paint_path.MouseTracking(event):
            pygame.draw.rect(sf, DARK_GREY, (0, 0, 1005, 1000))
            pygame.draw.rect(sf, GREY, (0, 0, a_sh * size_maze +
                             wall, a_sh * size_maze + wall * 2), 2)
            PaintMaze(size_maze, Vmaze, Hmaze, wall)
            CoordFind = [int(x1.GetText()), int(y1.GetText()),
                         int(x2.GetText()), int(y2.GetText())]
            if all(0 <= element < size_maze for element in CoordFind):
                PaintPath()

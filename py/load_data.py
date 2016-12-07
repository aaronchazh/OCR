import subprocess
import cv2
import string

import numpy as np

PATH = './GoodImg/Bmp/Sample0'
HEIGTH = 24
WIDTH = 24

def load():
	classes = list(range(0,10))
	classes += list(string.ascii_uppercase)
	classes += list(string.ascii_lowercase)
	X = []
	y = []
	for i in range(1, 63):
		num = ""
		if (i <= 9):
			num += "0" + str(i)
		else:
			num += str(i)
		img_path = PATH + num
		cmd = "ls " + img_path
		process = subprocess.Popen(cmd, stdout=subprocess.PIPE, shell=True)
		out, err = process.communicate()
		output = str(out).replace('\'','').replace('b','')
		images = output.split('\\n')
		for img in images:
			if img == "":
				break
			img_path_full = img_path + "/" + img
			read = cv2.imread(img_path_full, 0)
			try:
				rsz = cv2.resize(read, (HEIGTH, WIDTH))
			except:
				print(img_path_full)
				print(read.shape())
			X.append(rsz)
			y.append(classes[i - 1])
	return X, y		
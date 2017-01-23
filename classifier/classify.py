#!/usr/bin/python3

import subprocess
import cv2
import string
import pickle
import sys

import numpy as np

from sklearn.svm import SVC
from pathlib import Path

PATH = './GoodImg/Bmp/Sample0'
HEIGTH = 24
WIDTH = 24

def load():
	classes = list(range(0,10))
	classes += list(string.ascii_uppercase)
	classes += list(string.ascii_lowercase)
	Xlist = []
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
			Xlist.append(rsz)
			y.append(classes[i - 1])
	X = np.array(Xlist)
	return X, y

def convertTo2D(img):
	nx, ny = img.shape
	return img.reshape((1, nx*ny))

def train():
	X, y = load()
	nsamples, nx, ny = X.shape
	train = X.reshape((nsamples, nx*ny))
	clf = SVC()
	print("training model")
	clf.fit(train, y)
	out = open('classify.pickle','wb')
	pickle.dump(clf, out)

def predict(img):
	class_file = Path("classify.pickle")
	if class_file.is_file() == False:
		train()
	read = open('classify.pickle','rb')
	clf = pickle.load(read)

	Mat = cv2.resize(img, (HEIGTH, WIDTH))
	ex = convertTo2D(Mat)
	return clf.predict(ex)

def main():
	img_path = str(sys.argv[1])
	img = cv2.imread(img_path, 0)
	parse = str(predict(img)).replace('[','')
	parse2 = parse.replace(']','')
	parse3 = parse2.replace('\'','')
	print(parse3)

if __name__ == "__main__":
	main()
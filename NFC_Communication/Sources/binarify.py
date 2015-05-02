#!/usr/bin/python

import sys, getopt
import re, struct


def parse_int(str):
	if re.match('^(0[xX][a-fA-F\d]{1,2}|[a-fA-F\d]{1,2}[hH])$', str) != None:
		return int(str, 16)
	elif re.match('^(\d{1,2}|[012]\d{2})$', str) != None:
		return int(str, 10)
	elif re.match('^(0?[bB])?[01]{1,8}$', str) != None:
		return int(str, 2)
	else:
		raise ValueError

def main(argv):
	byteOrder = "<"

	try:
		opts, args = getopt.getopt(argv,"ho:", ["help", "order="])

		for opt, arg in opts:
			if opt in ('-h', '--help'):
				raise getopt.GetoptError('')
			if opt in ('-o', '--order'):
				if arg in ('be', 'BE'):
					byteOrder = ">"
				elif arg in ('le', 'LE'):
					byteOrder = "<"
				else:
					raise getopt.GetoptError('')

	except getopt.GetoptError:
		print ('\nbinarify.py [-o={be|le}] <inputfile (ASCII Decimal/Binary/Hex)> <outputfile (Binary)>')
		return 2

	with open (argv[-2], "r") as fin:
		values = re.sub('[^a-fA-FxXbB\d,]+', '', fin.read()).split(',')

		if len(values) > 0:
			with open(argv[-1], "wb") as fout:
				for value in values:
					fout.write(struct.pack(byteOrder + 'B', parse_int(value)))
		else:
			return -1

	return 0

if __name__ == "__main__":
   sys.exit(main(sys.argv[1:]))

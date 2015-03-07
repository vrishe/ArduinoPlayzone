#!/usr/bin/python

import sys, getopt
import re, struct, binascii

def main(argv):
	scale = 255
	cutoff = 255
	invert = False
	offset = 0

	try:
		opts, args = getopt.getopt(argv,"his:c:o:", ["help", "invert", "scale=", "cutoff=", "offset="])

		for opt, arg in opts:
			if opt in ('-h', '--help'):
				raise getopt.GetoptError('')
			elif opt in ('-i', '--invert'):
				invert = True
			elif opt in ('-s', '--scale'):
				scale = int(arg);

				if not (0 <= scale and scale <= 255):
					print ('\nBad scale value: %0d\n' % scale)
					raise getopt.GetoptError('')
			elif opt in ('-c', '--cutoff'):
				cutoff = int(arg)

				if not (0 <= cutoff and cutoff <= 255):
					print ('\nBad cutoff value: %0d\n' % cutoff)
					raise getopt.GetoptError('')
			elif opt in ('-o', '--offset'):
				offset = min(max(int(arg), -128), 127)

	except getopt.GetoptError:
		print ('\nequalize.py [-h (--help), -i, -t <value1>, -c <value1>, -o <value2>] <inputfile> <outputfile>')
		print ('\n -i (--invert) - set this flag to invert the output sample magnitudes (255 - Vs)')
		print ('\n -t (--scale)-a scale value, the resulting samples are limited with')
		print ('\n -c (--cutoff) - a cutoff value, intermediate sample values will be limited with')
		print ('\n -o (--offset) - shift the resulting sample magnitude by <value2>')
		print ('\n    All the <value1> values lie within range [0..255]')
		print ('\n    All the <value2> values lie within range [-128..127]')
		return 2

	with open (argv[-2], "r") as fin:
		data = fin.read()
		match = re.search(r"\{\r?\n([a-fx\d\s,]+)\r?\n\}", data)

		if match != None:
			values = match.group(1).replace('\n\t', '').split(',')

			amplitudes = []
			amplitudeMax = -128

			# Shift to zero point first:
			for i in range(len(values)):
				amplitude = struct.unpack('b', binascii.unhexlify(values[i].replace('0x','').strip()))[0]

				if invert:
					amplitude = -amplitude

				amplitudes.append(amplitude)
				if amplitude > amplitudeMax:
					amplitudeMax = amplitude

			with open(argv[-1], "w") as fout:
				fout.write(data[:match.start()] + "{\n")

				accumulator = ''
				countWrites = 0;
				amplitudeMax = float(amplitudeMax)
				for i in range(len(amplitudes)):
					if (countWrites % 16) == 0:
						accumulator += '\t'
					accumulator += '0x%02x, ' % min(round(scale * amplitudes[i] / float(amplitudeMax)) + offset, cutoff);

					countWrites = countWrites + 1
					if (countWrites % 16) == 0:
						accumulator += '\n'

						if countWrites % 800 == 0:
							fout.write(accumulator);
							accumulator = ''

				if len(accumulator) > 0:
					fout.write(accumulator + "\n")

				fout.write("}" + data[match.end():])



if __name__ == "__main__":
   sys.exit(main(sys.argv[1:]))

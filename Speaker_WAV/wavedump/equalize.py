#!/usr/bin/python

import sys, getopt
import re, struct, binascii

def main(argv):
	threshold = 255

	try:
		opts, args = getopt.getopt(argv,"ht:", ["help", "threshold="])

		for opt, arg in opts:
			if opt in ('-h', '--help'):
				raise getopt.GetoptError('')
			elif opt in ('-t', '--threshold'):
				threshold = int(arg);

				if not (0 <= threshold and threshold < 255):
					print ('\nBad threshold value: %0d\n' % threshold)
					raise getopt.GetoptError('')

	except getopt.GetoptError:
		print ('\nequalize.py [-h (--help), -t (--threshold=)<value>] <inputfile> <outputfile>')
		print ('\n\twhere <value> lies within [0..255)')
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
				amplitude = min(struct.unpack('b', binascii.unhexlify(values[i].replace('0x','').strip()))[0] + 128, threshold)

				amplitudes.append(amplitude)
				if amplitude > amplitudeMax:
					amplitudeMax = amplitude

			# Normalize volume if needed
			if 0 < amplitudeMax and amplitudeMax < 255:
				amplitudeMax = float(amplitudeMax)
				for i in range(len(amplitudes)):
					amplitudes[i] = round(255 * amplitudes[i] / amplitudeMax);

			with open(argv[-1], "w") as fout:
				fout.write(data[:match.start()] + "{\n")

				accumulator = ''
				countWrites = 0;
				for i in range(len(amplitudes)):
					if (countWrites % 16) == 0:
						accumulator += '\t'
					accumulator += '0x%02x, ' % amplitudes[i]

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

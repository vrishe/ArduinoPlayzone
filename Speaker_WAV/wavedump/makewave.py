#!/usr/bin/python

import sys
import re, struct, binascii

def main(argv):
	with open (argv[-2], "r") as fin:
		data = fin.read()
		matchSampleRate = re.search(r"SAMPLE_RATE\s(\d+)", data)
		matchData = re.search(r"\{\r?\n([a-fx\d\s,]+)\r?\n\}", data)


		if not (matchSampleRate == None or matchData == None):
			sampleRate = int(matchSampleRate.group(1).strip())
			values = matchData.group(1).replace('\n\t,', '').strip().split(',')

			with open (argv[-1], "wb") as fout:
				fout.write(bytes('RIFF', 'ASCII'))
				fout.write(struct.pack('<I', len(values) + 36))
				fout.write(bytes('WAVE', 'ASCII'))
				fout.write(bytes('fmt ', 'ASCII'))
				fout.write(struct.pack('<I', 16))
				fout.write(struct.pack('<H', 1))
				fout.write(struct.pack('<H', 1))
				fout.write(struct.pack('<I', sampleRate))
				fout.write(struct.pack('<I', sampleRate))
				fout.write(struct.pack('<H', 1))
				fout.write(struct.pack('<H', 8))
				fout.write(bytes('data', 'ASCII'))
				fout.write(struct.pack('<I', len(values)))

				# Shift to zero point first:
				for i in range(len(values)):
					fout.write(binascii.unhexlify(values[i].replace('0x','').strip()))


if __name__ == "__main__":
   sys.exit(main(sys.argv[1:]))

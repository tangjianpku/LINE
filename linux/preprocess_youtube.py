import sys
import os

fi = open(sys.argv[1], 'r')
fo = open(sys.argv[2], 'w')
for line in fi:
	items = line.strip().split()
	fo.write('{}\t{}\t1\n'.format(items[0], items[1]))
	fo.write('{}\t{}\t1\n'.format(items[1], items[0]))
fi.close()
fo.close()

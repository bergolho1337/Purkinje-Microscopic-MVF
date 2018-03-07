def diam (d0,K):
	d = d0
	DELTA = 0.7
	print d
	for i in range(K):
		d = d*DELTA
		print d

diam(30,3)

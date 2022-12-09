all:
	g++ -Wall RedBlackTree.cpp RedBlackTreeTests.cpp -o rbtests
	g++ -Wall RedBlackTree.cpp SpeedTest.cpp -o speed-test

rbt-tests:
	./rbtests

speedtest:
	./speed-test
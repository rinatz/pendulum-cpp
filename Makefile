.PHONY: build
build:
	@g++ -std=c++11 -g -O0 -Ilibs/include -I. -D_GLIBCXX_USE_CXX11_ABI=0 -Llibs/lib \
		test/*.cpp -o pendulum_test -lgmock -lgtest -lcctz -ldl -lpthread

.PHONY: test
test:
	@./pendulum_test

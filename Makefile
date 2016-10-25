all:
	go get ./...

test:
	go test

run:
	./connect.sh &
	go run main.go

dev: test run

.PHONY: all,test,run,dev

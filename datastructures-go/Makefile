.PHONY: test test-coverage coverage-html bench clean

test:
	go test -v ./...

test-coverage:
	go test -coverprofile=coverage.out ./...
	go tool cover -func=coverage.out

coverage-html:
	go test -coverprofile=coverage.out ./...
	go tool cover -html=coverage.out -o coverage.html
	open coverage.html

bench:
	go test -bench=. -benchmem ./...

clean:
	rm -f coverage.out coverage.html

package main

import (
	"fmt"
	"os"
	"os/exec"
)

func main() {
	fmt.Println("Running tests and generating coverage report...")
	
	cmd := exec.Command("go", "test", "-coverprofile=coverage.out", "./...")
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	
	if err := cmd.Run(); err != nil {
		fmt.Printf("Error running tests: %v\n", err)
		os.Exit(1)
	}
	
	cmd = exec.Command("go", "tool", "cover", "-html=coverage.out", "-o", "coverage.html")
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	
	if err := cmd.Run(); err != nil {
		fmt.Printf("Error generating HTML coverage: %v\n", err)
		os.Exit(1)
	}
	
	fmt.Println("Coverage report generated: coverage.html")
	fmt.Println("Opening coverage report in browser...")
	
	cmd = exec.Command("open", "coverage.html")
	if err := cmd.Run(); err != nil {
		fmt.Printf("Error opening coverage report: %v\n", err)
		fmt.Println("You can open it manually by running: open coverage.html")
	}
}

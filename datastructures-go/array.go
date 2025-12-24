package datastructures

import (
	"encoding/binary"
	"fmt"
	"io"
	"strings"
)

type Array struct {
	Name     string
	data     []string
	capacity int
}

func NewArray(name string, initialCapacity ...int) *Array {
	capacity := 8
	if len(initialCapacity) > 0 && initialCapacity[0] > 0 {
		capacity = initialCapacity[0]
	}
	return &Array{
		Name:     name,
		data:     make([]string, 0, capacity),
		capacity: capacity,
	}
}

func (a *Array) ensureCapacity() {
	if len(a.data) < cap(a.data) {
		return
	}
	newCapacity := cap(a.data) * 2
	if newCapacity == 0 {
		newCapacity = 1
	}
	newData := make([]string, len(a.data), newCapacity)
	copy(newData, a.data)
	a.data = newData
}

func (a *Array) PushBack(value string) {
	a.ensureCapacity()
	a.data = append(a.data, value)
}

func (a *Array) Insert(index int, value string) error {
	if index < 0 || index > len(a.data) {
		return fmt.Errorf("index out of bounds")
	}
	a.ensureCapacity()
	a.data = append(a.data, "")
	copy(a.data[index+1:], a.data[index:])
	a.data[index] = value
	return nil
}

func (a *Array) Get(index int) (string, error) {
	if index < 0 || index >= len(a.data) {
		return "", fmt.Errorf("index out of bounds")
	}
	return a.data[index], nil
}

func (a *Array) Remove(index int) error {
	if index < 0 || index >= len(a.data) {
		return fmt.Errorf("index out of bounds")
	}
	a.data = append(a.data[:index], a.data[index+1:]...)
	return nil
}

func (a *Array) Replace(index int, value string) error {
	if index < 0 || index >= len(a.data) {
		return fmt.Errorf("index out of bounds")
	}
	a.data[index] = value
	return nil
}

func (a *Array) Length() int {
	return len(a.data)
}

func (a *Array) Read() string {
	return strings.Join(a.data, " ")
}

func (a *Array) SerializeBinary(w io.Writer) error {
	length := uint32(len(a.data))
	if err := binary.Write(w, binary.LittleEndian, length); err != nil {
		return err
	}
	for _, s := range a.data {
		length := uint32(len(s))
		if err := binary.Write(w, binary.LittleEndian, length); err != nil {
			return err
		}
		if _, err := w.Write([]byte(s)); err != nil {
			return err
		}
	}
	return nil
}

func (a *Array) DeserializeBinary(r io.Reader) error {
	var length uint32
	if err := binary.Read(r, binary.LittleEndian, &length); err != nil {
		return err
	}
	
	a.data = make([]string, 0, length)
	for i := uint32(0); i < length; i++ {
		var strLen uint32
		if err := binary.Read(r, binary.LittleEndian, &strLen); err != nil {
			return err
		}
		
		buf := make([]byte, strLen)
		if _, err := io.ReadFull(r, buf); err != nil {
			return err
		}
		a.data = append(a.data, string(buf))
	}
	return nil
}

func (a *Array) SerializeText(w io.Writer) error {
	if _, err := fmt.Fprintf(w, "%d\n", len(a.data)); err != nil {
		return err
	}
	for _, s := range a.data {
		if _, err := fmt.Fprintf(w, "%s\n", s); err != nil {
			return err
		}
	}
	return nil
}

func (a *Array) DeserializeText(r io.Reader) error {
	var length int
	if _, err := fmt.Fscanf(r, "%d\n", &length); err != nil {
		return err
	}
	
	a.data = make([]string, 0, length)
	for i := 0; i < length; i++ {
		var s string
		if _, err := fmt.Fscanf(r, "%s\n", &s); err != nil {
			return err
		}
		a.data = append(a.data, s)
	}
	return nil
}

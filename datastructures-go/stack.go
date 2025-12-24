package datastructures

import (
	"encoding/binary"
	"fmt"
	"io"
	"strings"
)

type stackNode struct {
	data string
	next *stackNode
}

type Stack struct {
	Name string
	top  *stackNode
}

func NewStack(name string) *Stack {
	return &Stack{Name: name}
}

func (s *Stack) clear() {
	s.top = nil
}

func (s *Stack) Push(data string) {
	node := &stackNode{data: data}
	node.next = s.top
	s.top = node
}

func (s *Stack) Pop() string {
	if s.top == nil {
		return ""
	}
	data := s.top.data
	s.top = s.top.next
	return data
}

func (s *Stack) Peek() string {
	if s.top == nil {
		return ""
	}
	return s.top.data
}

func (s *Stack) IsEmpty() bool {
	return s.top == nil
}

func (s *Stack) Read() string {
	var values []string
	current := s.top
	for current != nil {
		values = append(values, current.data)
		current = current.next
	}
	return strings.Join(values, " ")
}

func (s *Stack) SerializeBinary(w io.Writer) error {
	var values []string
	current := s.top
	for current != nil {
		values = append(values, current.data)
		current = current.next
	}
	
	length := uint32(len(values))
	if err := binary.Write(w, binary.LittleEndian, length); err != nil {
		return err
	}
	
	for _, v := range values {
		strLen := uint32(len(v))
		if err := binary.Write(w, binary.LittleEndian, strLen); err != nil {
			return err
		}
		if _, err := w.Write([]byte(v)); err != nil {
			return err
		}
	}
	return nil
}

func (s *Stack) DeserializeBinary(r io.Reader) error {
	s.clear()
	
	var length uint32
	if err := binary.Read(r, binary.LittleEndian, &length); err != nil {
		return err
	}
	
	temp := make([]string, length)
	for i := 0; i < int(length); i++ {
		var strLen uint32
		if err := binary.Read(r, binary.LittleEndian, &strLen); err != nil {
			return err
		}
		
		buf := make([]byte, strLen)
		if _, err := io.ReadFull(r, buf); err != nil {
			return err
		}
		temp[i] = string(buf)
	}
	
	for i := len(temp) - 1; i >= 0; i-- {
		s.Push(temp[i])
	}
	return nil
}

func (s *Stack) SerializeText(w io.Writer) error {
	var values []string
	current := s.top
	for current != nil {
		values = append(values, current.data)
		current = current.next
	}
	
	if _, err := fmt.Fprintf(w, "%d\n", len(values)); err != nil {
		return err
	}
	
	for _, v := range values {
		if _, err := fmt.Fprintf(w, "%s\n", v); err != nil {
			return err
		}
	}
	return nil
}

func (s *Stack) DeserializeText(r io.Reader) error {
	s.clear()
	
	var length int
	if _, err := fmt.Fscanf(r, "%d\n", &length); err != nil {
		return err
	}
	
	temp := make([]string, length)
	for i := 0; i < length; i++ {
		var data string
		if _, err := fmt.Fscanf(r, "%s\n", &data); err != nil {
			return err
		}
		temp[i] = data
	}
	
	for i := len(temp) - 1; i >= 0; i-- {
		s.Push(temp[i])
	}
	return nil
}

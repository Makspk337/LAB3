package datastructures

import (
	"encoding/binary"
	"fmt"
	"io"
	"strings"
)

type queueNode struct {
	data string
	next *queueNode
}

type Queue struct {
	Name string
	head *queueNode
	tail *queueNode
}

func NewQueue(name string) *Queue {
	return &Queue{Name: name}
}

func (q *Queue) clear() {
	q.head = nil
	q.tail = nil
}

func (q *Queue) Push(data string) {
	node := &queueNode{data: data}
	if q.tail == nil {
		q.head = node
		q.tail = node
		return
	}
	q.tail.next = node
	q.tail = node
}

func (q *Queue) Pop() string {
	if q.head == nil {
		return ""
	}
	data := q.head.data
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil
	}
	return data
}

func (q *Queue) Peek() string {
	if q.head == nil {
		return ""
	}
	return q.head.data
}

func (q *Queue) IsEmpty() bool {
	return q.head == nil
}

func (q *Queue) Read() string {
	var values []string
	current := q.head
	for current != nil {
		values = append(values, current.data)
		current = current.next
	}
	return strings.Join(values, " ")
}

func (q *Queue) SerializeBinary(w io.Writer) error {
	var values []string
	current := q.head
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

func (q *Queue) DeserializeBinary(r io.Reader) error {
	q.clear()
	
	var length uint32
	if err := binary.Read(r, binary.LittleEndian, &length); err != nil {
		return err
	}
	
	for i := uint32(0); i < length; i++ {
		var strLen uint32
		if err := binary.Read(r, binary.LittleEndian, &strLen); err != nil {
			return err
		}
		
		buf := make([]byte, strLen)
		if _, err := io.ReadFull(r, buf); err != nil {
			return err
		}
		q.Push(string(buf))
	}
	return nil
}

func (q *Queue) SerializeText(w io.Writer) error {
	var values []string
	current := q.head
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

func (q *Queue) DeserializeText(r io.Reader) error {
	q.clear()
	
	var length int
	if _, err := fmt.Fscanf(r, "%d\n", &length); err != nil {
		return err
	}
	
	for i := 0; i < length; i++ {
		var s string
		if _, err := fmt.Fscanf(r, "%s\n", &s); err != nil {
			return err
		}
		q.Push(s)
	}
	return nil
}

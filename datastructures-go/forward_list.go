package datastructures

import (
	"encoding/binary"
	"fmt"
	"io"
	"strings"
)

type forwardListNode struct {
	value string
	next  *forwardListNode
}

type ForwardList struct {
	Name string
	head *forwardListNode
}

func NewForwardList(name string) *ForwardList {
	return &ForwardList{Name: name}
}

func (fl *ForwardList) clear() {
	fl.head = nil
}

func (fl *ForwardList) PushHead(value string) {
	node := &forwardListNode{value: value}
	node.next = fl.head
	fl.head = node
}

func (fl *ForwardList) PushTail(value string) {
	node := &forwardListNode{value: value}
	if fl.head == nil {
		fl.head = node
		return
	}
	
	current := fl.head
	for current.next != nil {
		current = current.next
	}
	current.next = node
}

func (fl *ForwardList) PushBefore(before, value string) {
	if fl.head == nil {
		return
	}
	
	if fl.head.value == before {
		fl.PushHead(value)
		return
	}
	
	prev := fl.head
	current := fl.head.next
	for current != nil {
		if current.value == before {
			node := &forwardListNode{value: value}
			prev.next = node
			node.next = current
			return
		}
		prev = current
		current = current.next
	}
}

func (fl *ForwardList) PushAfter(after, value string) {
	current := fl.head
	for current != nil {
		if current.value == after {
			node := &forwardListNode{value: value}
			node.next = current.next
			current.next = node
			return
		}
		current = current.next
	}
}

func (fl *ForwardList) DeleteHead() {
	if fl.head == nil {
		return
	}
	fl.head = fl.head.next
}

func (fl *ForwardList) DeleteTail() {
	if fl.head == nil {
		return
	}
	
	if fl.head.next == nil {
		fl.head = nil
		return
	}
	
	prev := fl.head
	current := fl.head.next
	for current.next != nil {
		prev = current
		current = current.next
	}
	prev.next = nil
}

func (fl *ForwardList) DeleteBefore(value string) {
	if fl.head == nil || fl.head.next == nil {
		return
	}
	
	if fl.head.next.value == value {
		fl.DeleteHead()
		return
	}
	
	prevPrev := fl.head
	prev := fl.head.next
	current := prev.next
	
	for current != nil {
		if current.value == value {
			prevPrev.next = current
			return
		}
		prevPrev = prev
		prev = current
		current = current.next
	}
}

func (fl *ForwardList) DeleteAfter(value string) {
	current := fl.head
	for current != nil && current.next != nil {
		if current.value == value {
			current.next = current.next.next
			return
		}
		current = current.next
	}
}

func (fl *ForwardList) DeleteValue(value string) {
	if fl.head == nil {
		return
	}
	
	if fl.head.value == value {
		fl.DeleteHead()
		return
	}
	
	prev := fl.head
	current := fl.head.next
	
	for current != nil {
		if current.value == value {
			prev.next = current.next
			return
		}
		prev = current
		current = current.next
	}
}

func (fl *ForwardList) Find(value string) bool {
	current := fl.head
	for current != nil {
		if current.value == value {
			return true
		}
		current = current.next
	}
	return false
}

func (fl *ForwardList) Read() string {
	var values []string
	current := fl.head
	for current != nil {
		values = append(values, current.value)
		current = current.next
	}
	return strings.Join(values, " ")
}

func (fl *ForwardList) ReadReverse() string {
	var values []string
	current := fl.head
	for current != nil {
		values = append(values, current.value)
		current = current.next
	}
	
	for i, j := 0, len(values)-1; i < j; i, j = i+1, j-1 {
		values[i], values[j] = values[j], values[i]
	}
	return strings.Join(values, " ")
}

func (fl *ForwardList) SerializeBinary(w io.Writer) error {
	var values []string
	current := fl.head
	for current != nil {
		values = append(values, current.value)
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

func (fl *ForwardList) DeserializeBinary(r io.Reader) error {
	fl.clear()
	
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
		fl.PushTail(string(buf))
	}
	return nil
}

func (fl *ForwardList) SerializeText(w io.Writer) error {
	var values []string
	current := fl.head
	for current != nil {
		values = append(values, current.value)
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

func (fl *ForwardList) DeserializeText(r io.Reader) error {
	fl.clear()
	
	var length int
	if _, err := fmt.Fscanf(r, "%d\n", &length); err != nil {
		return err
	}
	
	for i := 0; i < length; i++ {
		var s string
		if _, err := fmt.Fscanf(r, "%s\n", &s); err != nil {
			return err
		}
		fl.PushTail(s)
	}
	return nil
}

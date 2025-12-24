package datastructures

import (
	"encoding/binary"
	"fmt"
	"io"
	"strings"
)

type doublyListNode struct {
	value string
	prev  *doublyListNode
	next  *doublyListNode
}

type DoublyList struct {
	Name string
	head *doublyListNode
	tail *doublyListNode
}

func NewDoublyList(name string) *DoublyList {
	return &DoublyList{Name: name}
}

func (dl *DoublyList) clear() {
	dl.head = nil
	dl.tail = nil
}

func (dl *DoublyList) PushHead(value string) {
	node := &doublyListNode{value: value}
	node.next = dl.head
	if dl.head != nil {
		dl.head.prev = node
	} else {
		dl.tail = node
	}
	dl.head = node
}

func (dl *DoublyList) PushTail(value string) {
	node := &doublyListNode{value: value}
	node.prev = dl.tail
	if dl.tail != nil {
		dl.tail.next = node
	} else {
		dl.head = node
	}
	dl.tail = node
}

func (dl *DoublyList) PushBefore(before, value string) {
	current := dl.head
	for current != nil {
		if current.value == before {
			if current == dl.head {
				dl.PushHead(value)
				return
			}
			node := &doublyListNode{value: value}
			node.prev = current.prev
			node.next = current
			current.prev.next = node
			current.prev = node
			return
		}
		current = current.next
	}
}

func (dl *DoublyList) PushAfter(after, value string) {
	current := dl.head
	for current != nil {
		if current.value == after {
			if current == dl.tail {
				dl.PushTail(value)
				return
			}
			node := &doublyListNode{value: value}
			node.next = current.next
			node.prev = current
			current.next.prev = node
			current.next = node
			return
		}
		current = current.next
	}
}

func (dl *DoublyList) DeleteHead() {
	if dl.head == nil {
		return
	}
	dl.head = dl.head.next
	if dl.head != nil {
		dl.head.prev = nil
	} else {
		dl.tail = nil
	}
}

func (dl *DoublyList) DeleteTail() {
	if dl.tail == nil {
		return
	}
	dl.tail = dl.tail.prev
	if dl.tail != nil {
		dl.tail.next = nil
	} else {
		dl.head = nil
	}
}

func (dl *DoublyList) DeleteBefore(value string) {
	current := dl.head
	for current != nil {
		if current.value == value {
			target := current.prev
			if target == nil {
				return
			}
			if target == dl.head {
				dl.DeleteHead()
				return
			}
			target.prev.next = current
			current.prev = target.prev
			return
		}
		current = current.next
	}
}

func (dl *DoublyList) DeleteAfter(value string) {
	current := dl.head
	for current != nil {
		if current.value == value {
			target := current.next
			if target == nil {
				return
			}
			if target == dl.tail {
				dl.DeleteTail()
				return
			}
			current.next = target.next
			target.next.prev = current
			return
		}
		current = current.next
	}
}

func (dl *DoublyList) DeleteValue(value string) {
	current := dl.head
	for current != nil {
		if current.value == value {
			if current == dl.head {
				dl.DeleteHead()
				return
			}
			if current == dl.tail {
				dl.DeleteTail()
				return
			}
			current.prev.next = current.next
			current.next.prev = current.prev
			return
		}
		current = current.next
	}
}

func (dl *DoublyList) Find(value string) bool {
	current := dl.head
	for current != nil {
		if current.value == value {
			return true
		}
		current = current.next
	}
	return false
}

func (dl *DoublyList) Read() string {
	var values []string
	current := dl.head
	for current != nil {
		values = append(values, current.value)
		current = current.next
	}
	return strings.Join(values, " ")
}

func (dl *DoublyList) ReadReverse() string {
	var values []string
	current := dl.tail
	for current != nil {
		values = append(values, current.value)
		current = current.prev
	}
	return strings.Join(values, " ")
}

func (dl *DoublyList) SerializeBinary(w io.Writer) error {
	var values []string
	current := dl.head
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

func (dl *DoublyList) DeserializeBinary(r io.Reader) error {
	dl.clear()
	
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
		dl.PushTail(string(buf))
	}
	return nil
}

func (dl *DoublyList) SerializeText(w io.Writer) error {
	var values []string
	current := dl.head
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

func (dl *DoublyList) DeserializeText(r io.Reader) error {
	dl.clear()
	
	var length int
	if _, err := fmt.Fscanf(r, "%d\n", &length); err != nil {
		return err
	}
	
	for i := 0; i < length; i++ {
		var s string
		if _, err := fmt.Fscanf(r, "%s\n", &s); err != nil {
			return err
		}
		dl.PushTail(s)
	}
	return nil
}

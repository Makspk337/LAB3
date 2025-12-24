package datastructures

import (
	"encoding/binary"
	"fmt"
	"io"
	"strconv"
	"strings"
)

type treeNode struct {
	value int
	left  *treeNode
	right *treeNode
}

type Tree struct {
	Name string
	root *treeNode
}

func NewTree(name string) *Tree {
	return &Tree{Name: name}
}

func (t *Tree) destroy(node *treeNode) {
	if node == nil {
		return
	}
	t.destroy(node.left)
	t.destroy(node.right)
}

func (t *Tree) insertBST(node **treeNode, value int) {
	if *node == nil {
		*node = &treeNode{value: value}
		return
	}
	
	if value < (*node).value {
		t.insertBST(&(*node).left, value)
	} else if value > (*node).value {
		t.insertBST(&(*node).right, value)
	}
}

func (t *Tree) findBST(node *treeNode, value int) bool {
	if node == nil {
		return false
	}
	
	if value == node.value {
		return true
	}
	
	if value < node.value {
		return t.findBST(node.left, value)
	}
	
	return t.findBST(node.right, value)
}

func (t *Tree) preorder(node *treeNode, out *strings.Builder) {
	if node == nil {
		return
	}
	
	if out.Len() > 0 {
		out.WriteByte(' ')
	}
	out.WriteString(strconv.Itoa(node.value))
	
	t.preorder(node.left, out)
	t.preorder(node.right, out)
}

func (t *Tree) isFullImpl(node *treeNode) bool {
	if node == nil {
		return true
	}
	
	if node.left == nil && node.right == nil {
		return true
	}
	
	if node.left != nil && node.right != nil {
		return t.isFullImpl(node.left) && t.isFullImpl(node.right)
	}
	
	return false
}

func (t *Tree) Insert(value int) {
	t.insertBST(&t.root, value)
}

func (t *Tree) Find(value int) bool {
	return t.findBST(t.root, value)
}

func (t *Tree) Read() string {
	var builder strings.Builder
	t.preorder(t.root, &builder)
	return builder.String()
}

func (t *Tree) IsFull() bool {
	return t.isFullImpl(t.root)
}

func (t *Tree) serializeBinNode(w io.Writer, node *treeNode) error {
	present := uint8(0)
	if node != nil {
		present = 1
	}
	
	if err := binary.Write(w, binary.LittleEndian, present); err != nil {
		return err
	}
	
	if node == nil {
		return nil
	}
	
	if err := binary.Write(w, binary.LittleEndian, int32(node.value)); err != nil {
		return err
	}
	
	if err := t.serializeBinNode(w, node.left); err != nil {
		return err
	}
	
	if err := t.serializeBinNode(w, node.right); err != nil {
		return err
	}
	
	return nil
}

func (t *Tree) deserializeBinNode(r io.Reader) (*treeNode, error) {
	var present uint8
	if err := binary.Read(r, binary.LittleEndian, &present); err != nil {
		return nil, err
	}
	
	if present == 0 {
		return nil, nil
	}
	
	var value int32
	if err := binary.Read(r, binary.LittleEndian, &value); err != nil {
		return nil, err
	}
	
	node := &treeNode{value: int(value)}
	
	left, err := t.deserializeBinNode(r)
	if err != nil {
		return nil, err
	}
	node.left = left
	
	right, err := t.deserializeBinNode(r)
	if err != nil {
		return nil, err
	}
	node.right = right
	
	return node, nil
}

func (t *Tree) SerializeBinary(w io.Writer) error {
	return t.serializeBinNode(w, t.root)
}

func (t *Tree) DeserializeBinary(r io.Reader) error {
	t.destroy(t.root)
	
	node, err := t.deserializeBinNode(r)
	if err != nil {
		return err
	}
	
	t.root = node
	return nil
}

func (t *Tree) serializeTextNode(w io.Writer, node *treeNode) error {
	if node == nil {
		_, err := fmt.Fprint(w, "# ")
		return err
	}
	
	_, err := fmt.Fprintf(w, "%d ", node.value)
	if err != nil {
		return err
	}
	
	if err := t.serializeTextNode(w, node.left); err != nil {
		return err
	}
	
	if err := t.serializeTextNode(w, node.right); err != nil {
		return err
	}
	
	return nil
}

func (t *Tree) deserializeTextNode(r io.Reader) (*treeNode, error) {
	var tok string
	_, err := fmt.Fscan(r, &tok)
	if err != nil {
		return nil, err
	}
	
	if tok == "#" {
		return nil, nil
	}
	
	value, err := strconv.Atoi(tok)
	if err != nil {
		return nil, err
	}
	
	node := &treeNode{value: value}
	
	left, err := t.deserializeTextNode(r)
	if err != nil {
		return nil, err
	}
	node.left = left
	
	right, err := t.deserializeTextNode(r)
	if err != nil {
		return nil, err
	}
	node.right = right
	
	return node, nil
}

func (t *Tree) SerializeText(w io.Writer) error {
	return t.serializeTextNode(w, t.root)
}

func (t *Tree) DeserializeText(r io.Reader) error {
	t.destroy(t.root)
	
	node, err := t.deserializeTextNode(r)
	if err != nil {
		return err
	}
	
	t.root = node
	return nil
}

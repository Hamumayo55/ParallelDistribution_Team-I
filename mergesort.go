package main

import (
	"fmt"
	"math/rand"
	"time"
)

func Merge(ldata []int, rdata []int) (result []int) {
	left, right := 0, 0
	result = make([]int, len(ldata)+len(rdata))

	for i := 0; i < len(result); i++ {
		if left >= len(ldata) {
			result[i] = rdata[right]
			right++
		} else if right >= len(rdata) {
			result[i] = ldata[left]
			left++
		} else if ldata[left] < rdata[right] {
			result[i] = ldata[left]
			left++
		} else {
			result[i] = rdata[right]
			right++
		}
	}
	return
}

func MergeSort(val []int) []int {

	if len(val) == 1 {
		return val
	}

	mid := len(val) / 2
	leftData := MergeSort(val[:mid])
	rightData := MergeSort(val[mid:])

	return Merge(leftData, rightData)
}

func main() {

	t := time.Now().Unix()
	s := rand.NewSource(t)
	r := rand.New(s)

	N := 100
	val := make([]int, N)
	for i := 0; i < N; i++ {
		val[i] = r.Intn(N)
	}
	start := time.Now()
	MergeSort(val)
	end := time.Now()

	fmt.Printf("%f\n", (end.Sub(start)).Seconds())
}

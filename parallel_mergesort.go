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

func MergeSort(data []int, r chan []int) {
	if len(data) == 1 {
		r <- data
		return
	}

	leftChan := make(chan []int)
	rightChan := make(chan []int)
	middle := len(data) / 2

	go MergeSort(data[:middle], leftChan)
	go MergeSort(data[middle:], rightChan)

	ldata := <-leftChan
	rdata := <-rightChan

	close(leftChan)
	close(rightChan)
	r <- Merge(ldata, rdata)

	return
}

func main() {
	t := time.Now().Unix()
	s_a := rand.NewSource(t)
	r_a := rand.New(s_a)

	N := 1000
	s := make([]int, N)
	for i := 0; i < N; i++ {
		s[i] = r_a.Intn(N)
	}

	result := make(chan []int)

	start := time.Now()
	go MergeSort(s, result)
	close(result)
	end := time.Now()

	fmt.Printf("%f秒\n", (end.Sub(start)).Seconds())
}

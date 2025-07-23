package main

import (
	"fmt"
	"math/rand"
	"net/http"
	"strconv"
)
 // не даю гарантий что сервер работает, но на момент ласт коммита вроде как работал
const (
	SeedConst   = 583485834
	XorConstant = 0xDC9A
)

type customRandomSource struct {
	seed int64
}

func (s *customRandomSource) Int63() int64 {
	s.seed = s.seed*6364136222846791005 + 1
	return s.seed
}

func (s *customRandomSource) Seed(seed int64) {
	s.seed = seed
}

func newConsistentRandom(seed int64) *rand.Rand {
	return rand.New(&customRandomSource{seed: seed})
}

func decryptHandler(w http.ResponseWriter, r *http.Request) {
	valueStr := r.URL.Query().Get("value")
	seedStr := r.URL.Query().Get("seed")
	rtdscStr := r.URL.Query().Get("rtdsc")

	address, err := strconv.ParseInt(valueStr, 10, 64)
	if err != nil {
		return
	}

	initialSeed, err := strconv.ParseInt(seedStr, 10, 64)
	if err != nil {
		return
	}

	rtdsc, err := strconv.ParseInt(rtdscStr, 10, 64)
	if err != nil {
		return
	}

	randomSource := customRandomSource{seed: SeedConst}
	seedAsLong := initialSeed ^ (XorConstant ^ randomSource.Int63())

	random := newConsistentRandom(seedAsLong)
	key := random.Int63()
	address ^= key

	for i := 0; i < 6; i++ {
		ohShit := random.Int63()
		address ^= ohShit
		if i == 5 {
			address ^= rtdsc
		}
	}

	fmt.Println("Server seed:", seedAsLong, " | Result:", address)

	w.WriteHeader(http.StatusOK)
	_, _ = w.Write([]byte(strconv.FormatInt(address, 10)))
}

func main() {
	http.HandleFunc("/decrypt", decryptHandler)
	err := http.ListenAndServe(":6555", nil)
	if err != nil {
		fmt.Println("Error starting server:", err)
	}
}

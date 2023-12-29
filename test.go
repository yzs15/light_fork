package main

import (
    "fmt"
    "net/http"
)

func helloWorld(w http.ResponseWriter, r *http.Request) {
    fmt.Fprintf(w, "Hello, World!")
}

func main() {
    http.HandleFunc("/", helloWorld)
    fmt.Println("Server is starting...")
    http.ListenAndServe(":8080", nil)
}

package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"
)

func main() {
	db := InitDB()
	defer db.Close()
	CreateTable(db)
	InitData(db)

	reader := bufio.NewReader(os.Stdin)
	fmt.Println("Simple SQL REPL")
	fmt.Println("---------------------")

	for {
		fmt.Println("Choose an option:")
		fmt.Println("1. Insert record")
		fmt.Println("2. Query Mode")
		fmt.Println("3. SQL Mode")
		fmt.Println("4. Exit")
		fmt.Print("Enter choice: ")
		choice, _ := reader.ReadString('\n')
		choice = strings.TrimSpace(choice)

		switch choice {
		case "1":
			for {
				fmt.Print("Enter text to insert (type 'exit' to go back): ")
				text, _ := reader.ReadString('\n')
				text = strings.TrimSpace(text)
				if len(text) == 0 {
					continue
				}
				if strings.ToLower(text) == "exit" {
					break
				}
				InsertRecord(db, 0, text)
			}
		case "2":
			for {
				fmt.Print("Enter Query Mode (type 'exit' to go back): ")
				query, _ := reader.ReadString('\n')
				query = strings.TrimSpace(query)
				if len(query) == 0 {
					continue
				}
				if strings.ToLower(query) == "exit" {
					break
				}

				clause := ParseClause(query)
				sql := "select biz_id, simple_highlight(t1, 1, '[', ']') from t1 where text match ('" + clause + "')"
				log.Println(sql)
				Query(db, sql)
			}
		case "3":
			for {
				fmt.Print("Enter SQL Mode (type 'exit' to go back): ")
				query, _ := reader.ReadString('\n')
				query = strings.TrimSpace(query)
				if len(query) == 0 {
					continue
				}
				if strings.ToLower(query) == "exit" {
					break
				}
				log.Println(query)
				Query(db, query)
			}
		case "4":
			fmt.Println("Exiting...")
			return
		default:
			fmt.Println("Invalid choice, please try again.")
		}
	}
}

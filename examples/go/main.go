package main

import (
	"bufio"
	"database/sql"
	"fmt"
	"github.com/chrwhy/open-pinyin/dict"
	"github.com/chrwhy/open-pinyin/parser"
	"github.com/chrwhy/open-pinyin/util"
	"github.com/mattn/go-sqlite3"
	"log"
	"os"
	"strings"
)

func CreateTable(db *sql.DB) {
	createTableSQL := `CREATE VIRTUAL TABLE t1 USING fts5(id, text, tokenize = 'simple');`

	_, err := db.Exec(createTableSQL)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("数据表创建成功")

}

func InsertRecord(db *sql.DB, records []string) {
	insertSQL := `INSERT INTO t1(id, text) VALUES (?, ?)`
	for i, record := range records {
		_, err := db.Exec(insertSQL, i, record)
		if err != nil {
			log.Fatal(err)
		}
	}
}

func Init() *sql.DB {
	sql.Register("sqlite3_simple",
		&sqlite3.SQLiteDriver{
			Extensions: []string{
				"./libsimple-osx-x64/libsimple",
				//"/Users/stephen/Lab/github/simple/examples/go/libsimple-osx-x64/libsimple",
			},
		})

	db, err := sql.Open("sqlite3_simple", ":memory:")
	if err != nil {
		log.Fatal("open error: ", err)
	}

	err = db.Ping()
	if err != nil {
		log.Fatal("ping error: ", err)
	}
	return db
}

func TestPinyinFunction(db *sql.DB) {
	rows, err := db.Query(`select simple_query('pinyin')`)

	if err != nil {
		log.Fatal("query error: ", err)
	}

	defer rows.Close()
	for rows.Next() {
		var query string
		rows.Scan(&query)
		fmt.Printf("%s\n", query)
	}
}

func Query(db *sql.DB, sql string) {
	//rows, err := db.Query(`select '    ', simple_highlight(t1, 0, '[', ']') from t1 where x match ('` + clause + `')`)
	rows, err := db.Query(sql)
	if err != nil {
		log.Println(err)
		return
		log.Fatal("query error: ", err)
	}
	defer rows.Close()
	for rows.Next() {
		var col1 int
		var col2 string
		rows.Scan(&col1, &col2)
		fmt.Printf("ID: %d, text: %s\n", col1, col2)
	}
}

func ToPinyinClause(input string) string {
	pinyinGroups := parser.Parse(input)
	initPinyin := parser.ParseInitial(input)
	if len(initPinyin) > 0 {
		pinyinGroups = append(pinyinGroups, initPinyin)
	}
	clause := ""
	for i, pinyinGroup := range pinyinGroups {
		for j, _ := range pinyinGroup {
			if _, ok := dict.SUB_PINYIN[pinyinGroup[j]]; ok {
				pinyinGroup[j] = pinyinGroup[j] + "v"
			}
		}
		clause += util.Concat(pinyinGroup, "+")
		if len(pinyinGroups) > 1 && i != len(pinyinGroups)-1 {
			clause += " OR "
		}
	}
	return clause
}

func main() {
	db := Init()
	defer db.Close()
	TestPinyinFunction(db)
	CreateTable(db)

	records := []string{
		"周杰伦 Jay Chou:最美的不是下雨天，是曾与你躲过雨的屋檐",
		"中转箱",
		"李安",
		"西安",
		"周星驰",
		"李荟",
		"刘慧子",
		"chrwhy @ gmail.com",
	}

	InsertRecord(db, records)
	pinyinGroups := parser.Parse("zhoujielun")
	clause := ""
	for i, pinyinGroup := range pinyinGroups {
		clause += util.Concat(pinyinGroup, "+")
		if len(pinyinGroups) > 1 && i != len(pinyinGroups)-1 {
			clause += " OR "
		}
	}

	log.Println(clause)

	sql := `select id, simple_highlight(t1, 1, '[', ']') from t1 where text match ('` + clause + `')`
	Query(db, sql)

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
				fmt.Print("Enter record to insert (type 'exit' to go back): ")
				record, _ := reader.ReadString('\n')
				record = strings.TrimSpace(record)
				if strings.ToLower(record) == "exit" {
					break
				}
				InsertRecord(db, []string{record})
			}
		case "2":
			for {
				fmt.Print("Enter Query Mode (type 'exit' to go back): ")
				query, _ := reader.ReadString('\n')
				query = strings.TrimSpace(query)
				if strings.ToLower(query) == "exit" {
					break
				}
				inputClause := ToPinyinClause(query)
				log.Println(inputClause)
				inputClause = `select id, simple_highlight(t1, 1, '[', ']') from t1 where text match ('` + inputClause + " OR " + query + `')`
				log.Println(inputClause)
				Query(db, inputClause)
			}
		case "3":
			for {

				fmt.Print("Enter SQL Mode (type 'exit' to go back): ")
				query, _ := reader.ReadString('\n')
				query = strings.TrimSpace(query)
				if strings.ToLower(query) == "exit" {
					break
				}
				if len(query) == 0 {
					continue
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

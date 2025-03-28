package main

import (
	"database/sql"
	"github.com/mattn/go-sqlite3"
	"log"
	"math/rand/v2"
	"time"
)

func InitDB() *sql.DB {
	sql.Register("sqlite3_simple",
		&sqlite3.SQLiteDriver{
			Extensions: []string{
				"./libsimple-osx-x64/libsimple",
			},
		})

	db, err := sql.Open("sqlite3_simple", ":memory:")
	//db, err := sql.Open("sqlite3_simple", "example.db")
	if err != nil {
		log.Fatalf("open error: %v", err)
	}

	err = db.Ping()
	if err != nil {
		log.Fatal("ping error: ", err)
	}
	return db
}

func CreateTable(db *sql.DB) {
	createTableSQL := `CREATE VIRTUAL TABLE t1 USING fts5(biz_id, text, tokenize = 'simple');`
	_, err := db.Exec(createTableSQL)
	if err != nil {
		log.Fatal(err)
	}
	log.Println("Table created successfully")
}

func InsertRecord(db *sql.DB, bizId int, text string) {
	if bizId <= 0 {
		bizId = rand.Int()
	}
	insertSQL := `INSERT INTO t1(biz_id, text) VALUES (?, ?)`
	_, err := db.Exec(insertSQL, bizId, text)
	if err != nil {
		log.Fatal(err)
	}
	//log.Println("Records inserted successfully")
}

func Query(db *sql.DB, sql string) {
	t0 := time.Now()
	rows, err := db.Query(sql)
	log.Println("Query cost: ", time.Now().Sub(t0))
	if err != nil {
		log.Printf("query error: %v with sql: %s", err, sql)
		return
	}
	defer rows.Close()
	i := 0
	for rows.Next() {
		var bizId int
		var text string
		rows.Scan(&bizId, &text)
		log.Printf("Biz Id: %d text: %s\n", bizId, text)
		i++
	}
	log.Printf("Total : %d\n", i)
}

module github.com/wangfenjin/simple/examples/go

go 1.23.0

toolchain go1.24.1

require (
	github.com/chrwhy/open-pinyin v1.0.0
	github.com/mattn/go-sqlite3 v1.14.12
	golang.org/x/text v0.23.0
)

replace github.com/chrwhy/open-pinyin => ../../../open-pinyin

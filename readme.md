# Project Title
*
Log ingestor and query interface

## Table of Contents
**
1. Introduction 
2. Requirements
3. Installing Dependencies
4. Query Interface
5. Getting Started

# Introduction
*
To develop a log ingestor system that can efficiently handle vast volumes of log data, and offer a simple interface for querying this data using full-text search or specific field filters.
The logs are ingested (in the log ingestor) over HTTP, on port`3000`.

This project includes a simple log ingestor system and a query interface implemented in C++ using SQLite3.

### Requirements
**
The requirements for the log ingestor and the query interface are specified below.

1. Log Ingestor:
- Develop a mechanism to ingest logs in the provided format.
- Ensure scalability to handle high volumes of logs efficiently.
- Mitigate potential bottlenecks such as I/O operations, database write speeds, etc.
- Make sure that the logs are ingested via an HTTP server, which runs on port 3000 by default. 

2. Query Interface:
- Created a user interface (Web UI) for full-text search across logs.
- Included filters based on:
- level
- message
- resourceId
- timestamp
- traceId
- spanId
- commit
- metadata.parentResourceId

### Installing Dependencies
***
- [cpp-httplib](https://github.com/yhirose/cpp-httplib): C++ HTTP client/server library
- [json] (github.com/nlohmann/json): Handles JSON data in the log ingestor.
- [sqlite3.h] (SQLite Download Page):  Interacts with an SQLite database.

## Query Interface
***
The query interface allows users to perform searches on the ingested log data stored in the SQLite database: `index.html`

### Getting Started
***
NOTE: Make sure to have SQLite3 installed on your system.

1. IDE used - VScode
2. Clone the repository: `git clone "link"`
2. Build the log ingestor: `g++ log_ingestor.cpp -o log_ingestor -lsqlite3 -std=c++11 -pthread`
3. Run the log ingestor: `./log_ingestor`
Enter the search term when prompted, and the query interface will display the results.

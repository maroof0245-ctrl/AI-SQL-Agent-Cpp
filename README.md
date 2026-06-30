# AI-Driven Natural Language to SQL Executor

An autonomous C++ system agent that leverages a frontier LLM API (Gemini) to translate standard English text prompts into syntactically precise MySQL queries, featuring dynamic local database execution and structured real-time data grid rendering.

---

## 🚀 Key Architectural Features
* **Natural Language Parsing:** Integrates REST endpoints to pipe user constraints to an LLM engine for zero-shot text-to-SQL translation.
* **Deterministic Output Isolation:** Implements rigid contextual system instructions ensuring the AI bypasses conversational fluff and delivers purely executable query strings.
* **Modern DB Pipeline:** Built using the asynchronous **MySQL X DevAPI** (`mysqlx`) rather than legacy procedural drivers, working within an automated system sandbox (`ai_sandbox`).
* **Dynamic Grid Engine:** Parses query metadata fields on-the-fly to construct beautifully aligned ASCII terminal table layouts complete with precise column headers.

---

## 🛠️ Technical Stack
* **Language:** Modern C++ (Compiled via MSVC / CMake)
* **HTTP Transport:** `cpr` (C++ Requests library built over libcurl)
* **Serialization:** `nlohmann/json` for strict API data contract mapping
* **Database Driver:** MySQL Connector/C++ (X DevAPI core)

---

## 📋 Sample Pipeline Workflow

### 1. English Input Prompt
```text
What would you like your database to do?
> Show me all columns from the students table
```
### 2. Autonomous Core Pipeline Handling
```text
[INFO] Contacting Gemini AI Engine...
-----------------------------------------
💻 GENERATED MYSQL QUERY:
-----------------------------------------
SELECT * FROM students;
-----------------------------------------
[INFO] Sending query to local MySQL...
[SUCCESS] Query processed cleanly!
```
### 3. Dynamic Metadata Render Display
```text
[DATABASE RECORD DATASET]
+----------------+----------------+----------------+
| id             | name           | grade          |
+----------------+----------------+----------------+
| 1              | Alice          | A              |
| 2              | Bob            | B              |
| 3              | Charlie        | A              |
+----------------+----------------+----------------+
```

## 🔧 Local Workspace Setup
* **Ensure your local environment is configured with the following dependencies linked in your build path:**
### 1. MySQL Server 8.0+ running locally on port `33060`.
### 2. A valid Gemini API Key.

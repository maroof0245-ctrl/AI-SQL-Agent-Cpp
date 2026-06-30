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

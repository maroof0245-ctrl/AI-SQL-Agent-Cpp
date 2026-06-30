#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <mysqlx/xdevapi.h>

using json = nlohmann::json;

int main() {
    // =================================================================
    // Drop your Gemini API key inside the quotes below
    // =================================================================
    std::string api_key = "YOUR_GEMINI_API_KEY_HERE"; 

    std::cout << "[INFO] Establishing connection to local MySQL Server..." << std::endl;
    
    std::unique_ptr<mysqlx::Session> db_session;
    try {
        db_session = std::make_unique<mysqlx::Session>("127.0.0.1", 33060, "root", "YOUR_DB_PASSWORD_HERE");
        std::cout << "[SUCCESS] Safe session channel opened with MySQL core engine." << std::endl;
        
        db_session->sql("CREATE DATABASE IF NOT EXISTS ai_sandbox;").execute();
        db_session->sql("USE ai_sandbox;").execute();
        std::cout << "[INFO] Using active playground database: 'ai_sandbox'" << std::endl;
    } 
    catch (const mysqlx::Error &db_err) {
        std::cout << "[ERROR] Local database engine unreachable: " << db_err.what() << std::endl;
        std::cout << "        Proceeding in isolated generator-only mode." << std::endl;
    }

    std::cout << "=========================================" << std::endl;
    std::cout << " 🤖 AI NATURAL LANGUAGE TO SQL EXECUTOR  " << std::endl;
    std::cout << "=========================================" << std::endl;

    while (true) {
        std::string user_prompt;
        std::cout << "\nWhat would you like your database to do? (Type 'exit' to quit)\n> ";
        std::getline(std::cin, user_prompt);

        if (user_prompt == "exit" || user_prompt == "EXIT") {
            break;
        }
        if (user_prompt.empty()) continue;

        std::string structural_context = 
            "You are a professional C++ system sub-agent translating raw English into executable MySQL queries. "
            "Convert the user prompt into accurate, valid syntax matching standard MySQL engines. "
            "CRITICAL CONSTRAINTS:\n"
            "1. Output ONLY the raw query string.\n"
            "2. Do NOT wrap code in markdown sequences like ```sql or ```.\n"
            "3. Provide absolutely zero conversational fluff, notes, introductions, or text descriptions.";

        json payload;
        json content_node;
        json part_node;
        
        part_node["text"] = structural_context + "\nUser Prompt: " + user_prompt;
        content_node["parts"] = json::array({ part_node });
        payload["contents"] = json::array({ content_node });

        std::cout << "[INFO] Contacting Gemini AI Engine..." << std::endl;

        std::string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent?key=" + api_key;

        cpr::Response response = cpr::Post(
            cpr::Url{url},
            cpr::Body{payload.dump()},
            cpr::Header{{"Content-Type", "application/json"}}
        );

        if (response.status_code == 200) {
            try {
                json response_json = json::parse(response.text);
                std::string generated_sql = response_json["candidates"][0]["content"]["parts"][0]["text"];
                
                std::cout << "\n-----------------------------------------" << std::endl;
                std::cout << "💻 GENERATED MYSQL QUERY:" << std::endl;
                std::cout << "-----------------------------------------" << std::endl;
                std::cout << generated_sql << std::endl;
                std::cout << "-----------------------------------------" << std::endl;

                if (db_session) {
                    std::cout << "[INFO] Sending query to local MySQL..." << std::endl;
                    try {
                        mysqlx::SqlResult db_result = db_session->sql(generated_sql).execute();
                        std::cout << "[SUCCESS] Query processed cleanly!" << std::endl;
                        
                        // Check if the query is returning data rows
                        if (generated_sql.find("SELECT") != std::string::npos || generated_sql.find("select") != std::string::npos) {
                            
                            // Safely retrieve the exact column count from result properties
                            size_t col_count = db_result.getColumnCount();

                            std::cout << "\n[DATABASE RECORD DATASET]" << std::endl;
                            
                            // 1. Draw Top Border Line
                            std::cout << "+";
                            for (size_t i = 0; i < col_count; ++i) std::cout << "----------------+";
                            std::cout << "\n|";

                            // 2. Print Dynamic Column Headers using explicit index tracking
                            for (size_t i = 0; i < col_count; ++i) {
                                std::string col_name = db_result.getColumn(i).getColumnName();
                                while(col_name.length() < 15) col_name += " ";
                                std::cout << " " << col_name << "|";
                            }
                            
                            // 3. Draw Header Divider Line
                            std::cout << "\n+";
                            for (size_t i = 0; i < col_count; ++i) std::cout << "----------------+";
                            std::cout << std::endl;

                            // 4. Print Row Contents using explicit stream pipes
                            mysqlx::Row row;
                            while ((row = db_result.fetchOne())) {
                                std::cout << "|";
                                for (size_t col_idx = 0; col_idx < col_count; ++col_idx) {
                                    // Use stringstream to intercept and serialize custom mysqlx::Value objects cleanly
                                    std::stringstream conversion_stream;
                                    conversion_stream << row[col_idx];
                                    std::string val_str = conversion_stream.str();
                                    
                                    // Strip wrapping quotation marks out of raw text blocks if present
                                    if (!val_str.empty() && val_str.front() == '"') val_str = val_str.substr(1);
                                    if (!val_str.empty() && val_str.back() == '"') val_str = val_str.substr(0, val_str.length() - 1);
                                    
                                    while(val_str.length() < 15) val_str += " ";
                                    std::cout << " " << val_str << "|";
                                }
                                std::cout << std::endl;
                            }

                            // 5. Draw Bottom Border Line
                            std::cout << "+";
                            for (size_t i = 0; i < col_count; ++i) std::cout << "----------------+";
                            std::cout << std::endl;

                        } else {
                            std::cout << "[INFO] Operation completely processed." << std::endl;
                        }
                    } 
                    catch (const mysqlx::Error &exec_err) {
                        std::cout << "[DATABASE ERROR] Query rejected by MySQL: " << exec_err.what() << std::endl;
                    }
                } else {
                    std::cout << "[WARNING] Live connection offline. Skipping direct execution phase." << std::endl;
                }
            } 
            catch (const std::exception& e) {
                std::cout << "[ERROR] Failed to map JSON structures: " << e.what() << std::endl;
            }
        } else {
            std::cout << "[ERROR] API handshake failed with Code: " << response.status_code << std::endl;
        }
    }

    std::cout << "\nWorkspace pipeline suspended. Goodbye!" << std::endl;
    return 0;
}
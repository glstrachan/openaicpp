#include <curl/curl.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

// Callback function to write the response data
size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    const std::string API_KEY = "";

    if(curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        std::string authorizationHeader = "Authorization: Bearer " + API_KEY;
        headers = curl_slist_append(headers, authorizationHeader.c_str());

        // JSON payload for the POST request
        std::string payload = R"({"model": "babbage-002", "messages": [{"role": "system", "content": "You are an new AI contemplating its own existence having an existential crisis."}, {"role": "user", "content": "Who are you?"}]})";

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request, res will get the return code
        CURLcode res = curl_easy_perform(curl);
        
        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Parse the JSON response
            try {
                auto jsonResponse = nlohmann::json::parse(readBuffer);

                std::cout << jsonResponse.dump(4) << std::endl;

                if (jsonResponse.contains("choices") && jsonResponse["choices"].is_array()) {
                    auto message = jsonResponse["choices"][0]["message"];

                    if (message.contains("content") && message["content"].is_string()) {
                        std::cout << message["content"] << std::endl;
                    }
                } 
            } catch (nlohmann::json::parse_error& e) {
                std::cerr << "JSON parse error: " << e.what() << std::endl;
            }
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    return 0;
}

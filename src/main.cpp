#include <cstdlib>
#include <iostream>
#include <string>
#include <cassert>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void HandleToolCalls(const json &result)
{
    // return if tool_calls doesn't exist or if it's empty
    if (!result["choices"][0]["message"].contains("tool_calls")
        || result["choices"][0]["message"]["tool_calls"].empty())
        return;

    // extract tool_calls as json array
    json toolCalls = result["choices"][0]["message"]["tool_calls"];
    assert(toolCalls.is_array() && "tool_calls must be a JSON array");

    // parse to get function name and arguments
    for (const auto &call : toolCalls)
    {
        std::string sFuncName = call["function"]["name"].get<std::string>();
        json funcArgs = json::parse(call["function"]["arguments"].get<std::string>()); // Explicit conversion to std::string
        std::string sFilePath = funcArgs["file_path"].get<std::string>();              // Added .get<std::string>() for consistency

        // 1. Read tool
        if (sFuncName == "myRead")
        {
            // Read the file
            std::ifstream file(sFilePath);
            if (file.is_open())
            {
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                file.close();
                // print content
                std::cout << content << std::endl;
            }
            else
            {
                std::cerr << "Error: Could not open file " << sFilePath << std::endl;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    // declare prompt entered by the user
    std::string prompt;

    // loop that iterates through the command line arguments and initializes prompt
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "-p" && i + 1 < argc)
        {
            prompt = argv[++i];
        }
    }

    // null check for prompt
    if (prompt.empty())
    {
        std::cerr << "Prompt must not be empty" << std::endl;
        return 1;
    }

    const char *api_key_env = std::getenv("OPENROUTER_API_KEY");
    const char *base_url_env = std::getenv("OPENROUTER_BASE_URL");

    std::string api_key = api_key_env ? api_key_env : "";
    std::string base_url = base_url_env ? base_url_env : "https://openrouter.ai/api/v1";

    if (api_key.empty())
    {
        std::cerr << "OPENROUTER_API_KEY is not set" << std::endl;
        return 1;
    }

    // define a tool that can allow the model to read content
    json read_tool = {
        {"type", "function"},
        {"function", {{"name", "myRead"}, {"description", "Read and return the contents of a file"}, {"parameters", {{"type", "object"}, {"properties", {{"file_path", {{"type", "string"}, {"description", "The path to the file to read"}}}}}, {"required", json::array({"file_path"})}}}}}};

    // main request as a json object
    json request_body = {
        {"model", "anthropic/claude-haiku-4.5"},
        {"messages", json::array({{{"role", "user"}, {"content", prompt}}})},
        {"tools", json::array({read_tool})}};

    // create the http request and use cpr to send a POST request
    cpr::Response response = cpr::Post(
        cpr::Url{base_url + "/chat/completions"},
        cpr::Header{
            {"Authorization", "Bearer " + api_key},
            {"Content-Type", "application/json"}},
        cpr::Body{request_body.dump()});

    // error checking
    if (response.status_code != 200)
    {
        std::cerr << "HTTP error: " << response.status_code << std::endl;
        return 1;
    }

    // e.g. this is what response looks like:
    /*
    {
        "choices" : [
            {
                "index" : 0,
                "message" : {
                    "role" : "assistant",
                    "content" : null,
                    "tool_calls" : [
                        {
                            "id" : "call_abc123",
                            "type" : "function",
                            "function" : {
                                "name" : "myRead",
                                "arguments" : "{\"file_path\": \"/path/to/file.txt\"}"
                            }
                        }
                    ]
                },
                "finish_reason" : "tool_calls"
            }
        ]
    }
    */

    // parse the response body (a json string) into result (a json object).
    json result = json::parse(response.text);

    // check that the result has a non-empty "choices" array.
    if (!result.contains("choices") || result["choices"].empty())
    {
        std::cerr << "No choices in response" << std::endl;
        return 1;
    }

    // check the result for tools and execute tool call
    HandleToolCalls(result);

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here!" << std::endl;

    std::cout << result["choices"][0]["message"]["content"].get<std::string>();

    return 0;
}

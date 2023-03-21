#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "pugixml.hpp"
#include <iostream>
#include <string>

using namespace rapidjson;

void jsonToXml(const Value& json, pugi::xml_node& xml);

int main() {
    // Sample JSON string
    const char* jsonString = "{\"name\":\"John\",\"age\":30,\"city\":\"New York\"}";

    // Parse JSON string into a RapidJSON Document
    Document document;
    document.Parse(jsonString);

    // Create an empty pugixml document
    pugi::xml_document xmlDocument;

    // Convert JSON to XML
    jsonToXml(document, xmlDocument);

    // Print XML
    xmlDocument.save(std::cout, "  ");

    return 0;
}

void jsonToXml(const Value& json, pugi::xml_node& xml) {
    // Traverse the JSON object
    for (Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr) {
        // If the value is an object, create a new XML element
        if (itr->value.IsObject()) {
            pugi::xml_node child = xml.append_child(itr->name.GetString());

            // Recursively convert the child JSON object to XML
            jsonToXml(itr->value, child);
        }
        // If the value is an array, create a new XML element for each element in the array
        else if (itr->value.IsArray()) {
            for (Value::ConstValueIterator arrayItr = itr->value.Begin(); arrayItr != itr->value.End(); ++arrayItr) {
                pugi::xml_node child = xml.append_child(itr->name.GetString());

                // Recursively convert the child JSON object to XML
                jsonToXml(*arrayItr, child);
            }
        }
        // If the value is a string, create a new XML element with the string value
        else if (itr->value.IsString()) {
            xml.append_child(itr->name.GetString()).text().set(itr->value.GetString

#include "FoolWindow.h"
#include "../d3d_backend/ImGuiD3D11.h"
#include "../include/httplib.h"
#include "../imgui/imgui.h"

#define RAPIDJSON_NOMEMBERITERATORCLASS 1
#include "../include/rapidjson/rapidjson.h"
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"

#include <string>

int FoolWindow::render()
{
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    if (show_window) {
        ImGui::Begin("fool", &show_window);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        //std::cout << "cursor pos: " << pos.x << ", " << pos.y << std::endl;

        ImDrawList* list = ImGui::GetWindowDrawList();
        list->AddTriangleFilled(pos, ImVec2(pos.x + 100, pos.y), ImVec2(pos.x + 100, pos.y + 100), IM_COL32(255, 0, 0, 255));

        //ImVec2 lineP1 = { 150.0f, 100.0f };
        //ImVec2 lineP2 = { 150.0f, 300.0f };
        //static float thickness = 4.0f;
        //static float arrowWidth = 12.0f;
        //static float arrowHeight = 18.0f;
        //static float lineWidth = 4.0f;

        //list->PathLineTo({ lineP1.x - thickness, lineP1.y + arrowHeight }); // P1
        //list->PathLineTo({ lineP1.x - arrowWidth, lineP1.y + arrowHeight }); // P2
        //list->PathLineTo({ lineP1.x, lineP1.y }); // P3
        //list->PathLineTo({ lineP1.x + arrowWidth, lineP1.y + arrowHeight }); // P4
        //list->PathLineTo({ lineP1.x + thickness, lineP1.y + arrowHeight }); // P5
        //list->PathLineTo({ lineP2.x + thickness, lineP2.y }); // P6
        //list->PathLineTo({ lineP2.x - thickness, lineP2.y }); // P7
        //list->PathFillConvex(IM_COL32(255, 255, 0, 255));

        //list->PathLineTo({ lineP1.x - thickness, lineP1.y + arrowHeight }); // P1
        //list->PathLineTo({ lineP1.x - arrowWidth, lineP1.y + arrowHeight }); // P2
        //list->PathLineTo({ lineP1.x, lineP1.y }); // P3
        //list->PathLineTo({ lineP1.x + arrowWidth, lineP1.y + arrowHeight }); // P4
        //list->PathLineTo({ lineP1.x + thickness, lineP1.y + arrowHeight }); // P5
        //list->PathLineTo({ lineP2.x + thickness, lineP2.y }); // P6
        //list->PathLineTo({ lineP2.x - thickness, lineP2.y }); // P7
        //list->PathStroke(IM_COL32(0, 0, 0, 255), true, lineWidth);


        if (ImGui::Button(u8"开始记录")) {
            //task = std::thread([]() { system("python python/scripts/record.py"); }); 
            //task.detach();
            
            // HTTP
            httplib::Client cli("http://localhost:8383");

            auto res = cli.Get("/start_record");
            std::cout << "start_record: " << res->status;
            std::cout << "start_record: body" << res->body;


        }

        if (ImGui::Button(u8"停止记录")) {
            httplib::Client cli("http://localhost:8383");

            auto res = cli.Get("/stop_record");
            std::cout << "stop_record: " << res->status;
            std::cout << "stop_record: body" << res->body;


            if (res->status == 200) {
                table_ready = true;
                parse_table(res->body);
            }
            else {
                table_ready = false;
            }
        }


        ImGui::SliderInt("Sample count", &display_count, 1, 400);


        static int lines = 7;
        ImGui::SliderInt("Lines", &lines, 1, 15);

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
        ImVec2 scrolling_child_size = ImVec2(0, ImGui::GetFrameHeightWithSpacing() * lines + 30);
        ImGui::BeginChild("scrolling", scrolling_child_size, true, ImGuiWindowFlags_HorizontalScrollbar);
        for (int line = 0; line < lines; line++)
        {
            // Display random stuff. For the sake of this trivial demo we are using basic Button() + SameLine()
            // If you want to create your own time line for a real application you may be better off manipulating
            // the cursor position yourself, aka using SetCursorPos/SetCursorScreenPos to position the widgets
            // yourself. You may also want to use the lower-level ImDrawList API.
            int num_buttons = 10 + ((line & 1) ? line * 9 : line * 3);
            for (int n = 0; n < num_buttons; n++)
            {
                if (n > 0) ImGui::SameLine();
                ImGui::PushID(n + line * 1000);
                //char num_buf[16];
                //sprintf(num_buf, "%d", n);
                std::string num_buf = std::to_string(n);
                const char* label = (!(n % 15)) ? "FizzBuzz" : (!(n % 3)) ? "Fizz" : (!(n % 5)) ? "Buzz" : num_buf.c_str();
                float hue = n * 0.05f;
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f));
                auto pos = ImGui::GetCursorPos();
                //std::cout << "line： " << line << ", button: " << n << ", pos: " << pos.x << ", " << pos.y << std::endl;
                //ImGui::SetCursorPos(); 
                //ImGui::Button(label, ImVec2(40.0f + sinf((float)(line + n)) * 20.0f, 0.0f));
                ImGui::Button(label, ImVec2(0.0f, 10.0f));
                ImGui::PopStyleColor(3);
                ImGui::PopID();
            }
        }
        float scroll_x = ImGui::GetScrollX();
        float scroll_max_x = ImGui::GetScrollMaxX();
        ImGui::EndChild();
        ImGui::PopStyleVar(2);


        render_timeline();
        render_table();

        ImGui::End();
    }


    return 0;
}


int FoolWindow::render_timeline() {
    static int offset_x = 0;
    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
    ImGui::DragInt("Offset X", &offset_x, 1.0f, -1000, 1000);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (float)offset_x);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 100));
    ImGui::BeginChild("Red", ImVec2(200, 100), true, ImGuiWindowFlags_None);
    for (int n = 0; n < 50; n++)
        ImGui::Text("Some test %d", n);
    ImGui::EndChild();
    bool child_is_hovered = ImGui::IsItemHovered();
    ImVec2 child_rect_min = ImGui::GetItemRectMin();
    ImVec2 child_rect_max = ImGui::GetItemRectMax();
    ImGui::PopStyleColor();

    return 0;
}

int FoolWindow::parse_table(std::string& content)
{
    rapidjson::Document json_doc;
    json_doc.Parse(content.c_str());
    if (json_doc.HasParseError()) {
        std::cout << "parse error" << std::endl;
        return -1;
    }

    // 3. Stringify the DOM
    //rapidjson::StringBuffer buffer;
    //rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    //json_doc.Accept(writer);
    //std::cout << "json serialize: " << buffer.GetString() << std::endl;

    if (json_doc.HasMember("data") && json_doc["data"].IsArray()) {
        rapidjson::Value& data = json_doc["data"];

        if (data.Size() > 0) {
            rapidjson::Value& record = data[0];

            table_header.clear();
            for (rapidjson::Value::ConstMemberIterator it = record.MemberBegin(); it != record.MemberEnd(); ++it) {
                table_header.push_back(it->name.GetString());
            }
        }

        std::cout << "data size: " << data.Size() << ", header size: " << table_header.size() << std::endl;


        table_content.clear();
        for (int i = 0; i < data.Size(); i++) {
            rapidjson::Value& record = data[i];
            if (record.IsObject()) {
                std::vector<std::string> row;
                for (auto& col_name : table_header) {
                    row.push_back(record[col_name.c_str()].GetString());
                }

                table_content.emplace_back(row);
            }
        }
    }
}

int FoolWindow::render_table() {
    if (!table_ready) return -1;

    static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;
    int num_cols = table_header.size();
    if (num_cols <= 0 || num_cols > 64) return -1;

    std::cout << "num cols: " << num_cols << std::endl;

    if (ImGui::BeginTable("table1", num_cols, flags))
    {
        //if (display_headers)
        {
            for (auto& col : table_header) {
                ImGui::TableSetupColumn(col.c_str());
            }
            ImGui::TableHeadersRow();
        }

        for (int row = 0; row < table_content.size(); row++)
        {
            std::cout << "row size : " << table_content[row].size() << std::endl;
            ImGui::TableNextRow();
            for (int column = 0; column < table_content[row].size(); column++)
            {
                //ImGui::TableNextColumn();
                ImGui::TableSetColumnIndex(column);
                ImGui::Text("%s", table_content[row][column].c_str());
            }
        }
        ImGui::EndTable();
    }

    return 0;
}
